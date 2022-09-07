#include <sys/ioctl.h>
#include <errno.h>
#include "ion_alloc.h"
#include "ionAllocList.h"
#include <ion_mem_alloc.h>

#define CONFIG_LOG_LEVEL    OPTION_LOG_LEVEL_DETAIL
#define LOG_TAG "ionAlloc"
#define DEBUG_ION_REF			0
#define ION_ALLOC_ALIGN			SZ_4k
#define DEV_NAME				"/dev/ion"
#define CEDAR_DEV_NAME				"/dev/cedar_dev"
#define ION_IOC_SUNXI_POOL_INFO 10
#define UNUSA_PARAM(param) (void)param

#define loge(fmt, arg...) fprintf(stderr, fmt "\n", ##arg)
#define logw(fmt, arg...)
#define logd(fmt, arg...)
#define logv(fmt, arg...)

#define CONF_KERNEL_IOMMU (1)

#if DEBUG_ION_REF==1
    int cdx_use_mem = 0;
    typedef struct ION_BUF_NODE_TEST {
        unsigned int addr;
        int size;
    } ion_buf_node_test;

    #define ION_BUF_LEN  50
    ion_buf_node_test ion_buf_nodes_test[ION_BUF_LEN];
#endif

struct sunxi_pool_info {
        unsigned int total; /*unit kb*/
        unsigned int free_kb;  /* size kb */
        unsigned int free_mb;  /* size mb */
};

typedef struct BUFFER_NODE {
    struct aw_mem_list_head i_list;
    unsigned long phy; /*phisical address*/
    unsigned long vir; /*virtual address*/
    unsigned int size; /*buffer size*/
    unsigned int tee;
    unsigned long user_virt;
    ion_fd_data_t fd_data;
} buffer_node;

typedef struct ION_ALLOC_CONTEXT {
    int fd;
    int cedar_fd;
    struct aw_mem_list_head list; /* buffer list */
    int ref_cnt; /* reference count */
} ion_alloc_context;

struct user_iommu_param {
    int	fd;
    unsigned int iommu_addr;
};

ion_alloc_context *g_ion_alloc_context = NULL;
pthread_mutex_t g_ion_mutex_alloc = PTHREAD_MUTEX_INITIALIZER;

/*funciton begin*/
int sunxi_ion_alloc_open()
{
	logd("begin ion_alloc_open\n");
    pthread_mutex_lock(&g_ion_mutex_alloc);
    if (g_ion_alloc_context != NULL) {
		logv("ion allocator has already been created\n");
        goto SUCCEED_OUT;
    }

    g_ion_alloc_context = (ion_alloc_context*)malloc(sizeof(ion_alloc_context));
	if (g_ion_alloc_context == NULL) {
		loge("create ion allocator failed, out of memory\n");
		goto ERROR_OUT;
    } else {
		logv("pid:%d, g_alloc_context = %p\n", getpid(), g_ion_alloc_context);
    }
    memset((void*)g_ion_alloc_context, 0, sizeof(ion_alloc_context));
	/* Readonly should be enough. */
	g_ion_alloc_context->fd = open(DEV_NAME, O_RDONLY, 0);
	if (g_ion_alloc_context->fd <= 0) {
	loge("open %s failed\n", DEV_NAME);
		goto ERROR_OUT;
	}

#if defined(CONF_KERNEL_IOMMU)
    g_ion_alloc_context->cedar_fd = open(CEDAR_DEV_NAME, O_RDONLY, 0);
    if (g_ion_alloc_context->cedar_fd <= 0) {
	loge("open %s failed\n", CEDAR_DEV_NAME);
		goto ERROR_OUT;
    }
#endif

#if DEBUG_ION_REF==1
    cdx_use_mem = 0;
    memset(&ion_buf_nodes_test, sizeof(ion_buf_nodes_test), 0);
    logd("ion_open, cdx_use_mem=[%dByte].", cdx_use_mem);
    ion_alloc_get_total_size();
#endif
    AW_MEM_INIT_LIST_HEAD(&g_ion_alloc_context->list);

SUCCEED_OUT:
    g_ion_alloc_context->ref_cnt++;
    pthread_mutex_unlock(&g_ion_mutex_alloc);
    return 0;

ERROR_OUT:
    if (g_ion_alloc_context != NULL
        && g_ion_alloc_context->fd > 0) {
        close(g_ion_alloc_context->fd);
        g_ion_alloc_context->fd = 0;
    }

#if defined(CONF_KERNEL_IOMMU)
    if (g_ion_alloc_context != NULL
        && g_ion_alloc_context->cedar_fd > 0) {
        close(g_ion_alloc_context->cedar_fd);
        g_ion_alloc_context->cedar_fd = 0;
    }
#endif

    if (g_ion_alloc_context != NULL) {
        free(g_ion_alloc_context);
        g_ion_alloc_context = NULL;
    }

    pthread_mutex_unlock(&g_ion_mutex_alloc);
    return -1;
}

void sunxi_ion_alloc_close()
{
    struct aw_mem_list_head * pos, *q;

    logv("ion_alloc_close\n");

    pthread_mutex_lock(&g_ion_mutex_alloc);
    if (--g_ion_alloc_context->ref_cnt <= 0) {
        logv("pid: %d, release g_ion_alloc_context = %p\n", getpid(), g_ion_alloc_context);
        aw_mem_list_for_each_safe(pos, q, &g_ion_alloc_context->list) {
            buffer_node * tmp;
            tmp = aw_mem_list_entry(pos, buffer_node, i_list);
            logv("ion_alloc_close del item phy= 0x%lx vir= 0x%lx, size= %d\n", \
                tmp->phy, tmp->vir, tmp->size);
            aw_mem_list_del(pos);
            free(tmp);
        }
#if DEBUG_ION_REF==1
        logd("ion_close, cdx_use_mem=[%d MB]", cdx_use_mem/1024/1024);
        ion_alloc_get_total_size();
#endif
        close(g_ion_alloc_context->fd);
        g_ion_alloc_context->fd = 0;

#if defined(CONF_KERNEL_IOMMU)
	close(g_ion_alloc_context->cedar_fd);
        g_ion_alloc_context->cedar_fd = 0;
#endif

        free(g_ion_alloc_context);
        g_ion_alloc_context = NULL;
    } else {
        logv("ref cnt: %d > 0, do not free\n", g_ion_alloc_context->ref_cnt);
    }
    pthread_mutex_unlock(&g_ion_mutex_alloc);
#if DEBUG_ION_REF==1
    int i = 0;
    int counter = 0;
    for (i = 0; i < ION_BUF_LEN; i++) {
        if (ion_buf_nodes_test[i].addr != 0 || ion_buf_nodes_test[i].size != 0) {
            loge("ion mem leak????  addr->[0x%x], leak size->[%dByte]", \
                ion_buf_nodes_test[i].addr, ion_buf_nodes_test[i].size);
            counter ++;
        }
    }

    if (counter != 0) {
        loge("my god, have [%d]blocks ion mem leak.!!!!", counter);
    } else {
        logd("well done, no ion mem leak.");
    }
#endif
    return ;
}

/* return virtual address: 0 failed */
void* sunxi_ion_alloc_palloc(int size)
{
    aw_ion_allocation_info_t alloc_data;
    ion_fd_data_t fd_data;
    struct ion_handle_data handle_data;
    struct aw_ion_custom_info custom_data;
    sunxi_phys_data   phys_data;
    struct user_iommu_param iommu_param;

    int rest_size = 0;
    unsigned long addr_phy = 0;
    unsigned long addr_vir = 0;
    buffer_node * alloc_buffer = NULL;
    int ret = 0;
    (void)custom_data;
    (void)phys_data;
    (void)rest_size;

    pthread_mutex_lock(&g_ion_mutex_alloc);

    if (g_ion_alloc_context == NULL) {
		loge("call ion_alloc_open\n");
        goto ALLOC_OUT;
    }

    if (size <= 0) {
        loge("can not alloc size 0\n");
        goto ALLOC_OUT;
    }

    alloc_data.aw_len = (size_t)size;
    alloc_data.aw_align = ION_ALLOC_ALIGN ;

#if defined(CONF_KERNEL_IOMMU)
    alloc_data.aw_heap_id_mask = AW_ION_SYSTEM_HEAP_MASK | AW_ION_CARVEOUT_HEAP_MASK;
#else
    alloc_data.aw_heap_id_mask = AW_ION_DMA_HEAP_MASK | AW_ION_CARVEOUT_HEAP_MASK;
#endif
    alloc_data.flags = AW_ION_CACHED_FLAG | AW_ION_CACHED_NEEDS_SYNC_FLAG;

    ret = ioctl(g_ion_alloc_context->fd, AW_MEM_ION_IOC_ALLOC, &alloc_data);
    if (ret) {
        loge("ION_IOC_ALLOC error\n");
        goto ALLOC_OUT;
    }

    /* get dmabuf fd */
    fd_data.handle = alloc_data.handle;
    ret = ioctl(g_ion_alloc_context->fd, AW_MEM_ION_IOC_MAP, &fd_data);
    if (ret) {
        loge("ION_IOC_MAP err, ret %d, dmabuf fd 0x%08x\n",
				ret, (unsigned int)fd_data.aw_fd);
        goto ALLOC_OUT;
    }

    /* mmap to user */
    addr_vir = (unsigned long)mmap(NULL, alloc_data.aw_len, \
        PROT_READ|PROT_WRITE, MAP_SHARED, fd_data.aw_fd, 0);

    if ((unsigned long)MAP_FAILED == addr_vir) {
        loge("mmap err, ret %u\n", (unsigned int)addr_vir);
        addr_vir = 0;
        goto ALLOC_OUT;
    }

    /* get phy address */
#if defined(CONF_KERNEL_IOMMU)
    memset(&iommu_param, 0, sizeof(iommu_param));
    iommu_param.fd = fd_data.aw_fd;
    ret = ioctl(g_ion_alloc_context->cedar_fd, AW_MEM_ENGINE_REQ, 0);
    if (ret) {
        loge("ENGINE_REQ err, ret %d\n", ret);
        addr_phy = 0;
        addr_vir = 0;
        goto ALLOC_OUT;
    }
    ret = ioctl(g_ion_alloc_context->cedar_fd, AW_MEM_GET_IOMMU_ADDR, &iommu_param);
    if (ret) {
        loge("GET_IOMMU_ADDR err, ret %d\n", ret);
        addr_phy = 0;
        addr_vir = 0;
        goto ALLOC_OUT;
    }
    addr_phy = iommu_param.iommu_addr;
#else
    memset(&phys_data, 0, sizeof(phys_data));
    phys_data.handle = alloc_data.handle;
    phys_data.size = size;
    custom_data.aw_cmd = ION_IOC_SUNXI_PHYS_ADDR;
    custom_data.aw_arg = (unsigned long)&phys_data;
    ret = ioctl(g_ion_alloc_context->fd, AW_MEM_ION_IOC_CUSTOM, &custom_data);
    if (ret) {
        loge("ION_IOC_CUSTOM err, ret %d\n", ret);
        addr_phy = 0;
        addr_vir = 0;
        goto ALLOC_OUT;
    }
    addr_phy = phys_data.phys_addr;
#endif

    alloc_buffer = (buffer_node *)malloc(sizeof(buffer_node));
    if (alloc_buffer == NULL) {
        loge("malloc buffer node failed");

        /* unmmap */
        ret = munmap((void*)addr_vir, alloc_data.aw_len);
        if (ret) {
            loge("munmap err, ret %d\n", ret);
        }

        /* close dmabuf fd */
        close(fd_data.aw_fd);

        /* free buffer */
        handle_data.handle = alloc_data.handle;
        ret = ioctl(g_ion_alloc_context->fd, AW_MEM_ION_IOC_FREE, &handle_data);
        if(ret)
            loge("ION_IOC_FREE err, ret %d\n", ret);

        addr_phy = 0;
        addr_vir = 0; /* value of MAP_FAILED is -1, should return 0*/

        goto ALLOC_OUT;
    }
    alloc_buffer->phy = addr_phy;
    alloc_buffer->vir = addr_vir;
    alloc_buffer->user_virt = addr_vir;
    alloc_buffer->size = size;
    alloc_buffer->fd_data.handle = fd_data.handle;
    alloc_buffer->fd_data.aw_fd = fd_data.aw_fd;

    aw_mem_list_add_tail(&alloc_buffer->i_list, &g_ion_alloc_context->list);

#if DEBUG_ION_REF==1
    cdx_use_mem += size;
    int i = 0;
    for (i = 0; i < ION_BUF_LEN; i++) {
        if (ion_buf_nodes_test[i].addr == 0
				&& ion_buf_nodes_test[i].size == 0) {
            ion_buf_nodes_test[i].addr = addr_vir;
            ion_buf_nodes_test[i].size = size;
            break;
        }
    }

    if (i>= ION_BUF_LEN) {
        loge("error, ion buf len is large than [%d]", ION_BUF_LEN);
    }
#endif

ALLOC_OUT:
    pthread_mutex_unlock(&g_ion_mutex_alloc);
    return (void*)addr_vir;
}

void sunxi_ion_alloc_pfree(void * pbuf)
{
    int flag = 0;
    unsigned long addr_vir = (unsigned long)pbuf;
    buffer_node * tmp;
    int ret;
    struct ion_handle_data handle_data;

    if (NULL == pbuf) {
        loge("can not free NULL buffer\n");
        return ;
    }

    pthread_mutex_lock(&g_ion_mutex_alloc);

    if (g_ion_alloc_context == NULL) {
        loge("call ion_alloc_open before ion_alloc_alloc\n");
        return ;
    }

    aw_mem_list_for_each_entry(tmp, &g_ion_alloc_context->list, i_list)
    {
        if (tmp->vir == addr_vir) {
            logv("ion_alloc_free item phy =0x%lx vir =0x%lx, size =%d\n", \
                tmp->phy, tmp->vir, tmp->size);
            /*unmap user space*/
            if (munmap((void *)(tmp->user_virt), tmp->size) < 0) {
                loge("munmap 0x%p, size: %u failed\n", (void*)addr_vir, tmp->size);
            }

#if defined(CONF_KERNEL_IOMMU)
	   ret = ioctl(g_ion_alloc_context->cedar_fd, AW_MEM_ENGINE_REL, 0);
	   if (ret)
              logv("ENGINE_REL failed\n");

	   struct user_iommu_param iommu_param;
           memset(&iommu_param, 0, sizeof(iommu_param));
           iommu_param.fd = tmp->fd_data.aw_fd;
	   ret = ioctl(g_ion_alloc_context->cedar_fd, AW_MEM_FREE_IOMMU_ADDR, &iommu_param);
           if (ret)
              logv("FREE_IOMMU_ADDR err, ret %d\n", ret);
#endif

            /*close dma buffer fd*/
            close(tmp->fd_data.aw_fd);
            /* free buffer */
            handle_data.handle = tmp->fd_data.handle;

            ret = ioctl(g_ion_alloc_context->fd, AW_MEM_ION_IOC_FREE, &handle_data);
            if (ret)
                logv("TON_IOC_FREE failed\n");

            aw_mem_list_del(&tmp->i_list);
            free(tmp);

            flag = 1;

#if DEBUG_ION_REF==1
            int i = 0;
            for ( i = 0; i < ION_BUF_LEN; i++) {
                if (ion_buf_nodes_test[i].addr == addr_vir && ion_buf_nodes_test[i].size > 0) {
                    cdx_use_mem -= ion_buf_nodes_test[i].size;
                    logd("--cdx_use_mem = [%d MB], reduce size->[%d B]",\
                        cdx_use_mem/1024/1024, ion_buf_nodes_test[i].size);
                    ion_buf_nodes_test[i].addr = 0;
                    ion_buf_nodes_test[i].size = 0;

                    break;
                }
            }

            if (i >= ION_BUF_LEN) {
                loge("error, ion buf len is large than [%d]", ION_BUF_LEN);
            }
#endif
            break;
        }
    }

    if (0 == flag) {
        loge("ion_alloc_free failed, do not find virtual address: 0x%lx\n", addr_vir);
    }

    pthread_mutex_unlock(&g_ion_mutex_alloc);
    return ;
}

void* sunxi_ion_alloc_vir2phy_cpu(void * pbuf)
{
    int flag = 0;
    unsigned long addr_vir = (unsigned long)pbuf;
    unsigned long addr_phy = 0;
    buffer_node * tmp;

    if (0 == pbuf) {
        logv("can not vir2phy NULL buffer\n");
        return 0;
    }

    pthread_mutex_lock(&g_ion_mutex_alloc);
    aw_mem_list_for_each_entry(tmp, &g_ion_alloc_context->list, i_list)
    {
        if (addr_vir >= tmp->vir
            && addr_vir < tmp->vir + tmp->size) {
            addr_phy = tmp->phy + addr_vir - tmp->vir;
            logv("ion_alloc_vir2phy phy= 0x%08x vir= 0x%08x\n", addr_phy, addr_vir);
            flag = 1;
            break;
        }
    }
    if (0 == flag)
        loge("ion_alloc_vir2phy failed,no virtual address: 0x%lx\n", addr_vir);
    pthread_mutex_unlock(&g_ion_mutex_alloc);

    return (void*)addr_phy;
}

void* sunxi_ion_alloc_phy2vir_cpu(void * pbuf)
{
    int flag = 0;
    unsigned long addr_vir = 0;
    unsigned long addr_phy = (unsigned long)pbuf;
    buffer_node * tmp;

    if (0 == pbuf) {
        loge("can not phy2vir NULL buffer\n");
        return 0;
    }

    pthread_mutex_lock(&g_ion_mutex_alloc);
    aw_mem_list_for_each_entry(tmp, &g_ion_alloc_context->list, i_list)
    {
        if (addr_phy >= tmp->phy
            && addr_phy < tmp->phy + tmp->size) {
            addr_vir = tmp->vir + addr_phy - tmp->phy;
            flag = 1;
            break;
        }
    }

    if (0 == flag)
        loge("%s failed,can not find phy adr:0x%lx\n",
		__FUNCTION__, addr_phy);

    pthread_mutex_unlock(&g_ion_mutex_alloc);

    return (void*)addr_vir;
}


int sunxi_ion_alloc_get_bufferFd(void * pbuf)
{
    int flag = 0;
    unsigned long addr_vir = (unsigned long)pbuf;
    int buffer_fd = 0;
    buffer_node * tmp;

    if (0 == pbuf) {
        loge("can not vir2phy NULL buffer\n");
        return 0;
    }

    pthread_mutex_lock(&g_ion_mutex_alloc);
    aw_mem_list_for_each_entry(tmp, &g_ion_alloc_context->list, i_list)
    {
        if (addr_vir >= tmp->vir
            && addr_vir < tmp->vir + tmp->size) {

            buffer_fd = tmp->fd_data.aw_fd;
            logv("ion_alloc_get_bufferFd, fd = 0x%08x vir= 0x%08x\n", buffer_fd, addr_vir);
            flag = 1;
            break;
        }
    }
    if (0 == flag)
        loge("ion_alloc_vir2phy failed,no virtual address: 0x%lx\n", addr_vir);
    pthread_mutex_unlock(&g_ion_mutex_alloc);

    logv("*** get_bufferfd: %d, flag = %d\n",buffer_fd, flag);
    return buffer_fd;
}


#if defined(CONF_KERNEL_VERSION_3_10) || defined(CONF_KERNEL_VERSION_4_4) || defined(CONF_KERNEL_VERSION_4_9)
void sunxi_ion_alloc_flush_cache(void* startAddr, int size)
{
    sunxi_cache_range range;
    int ret;

    /* clean and invalid user cache */
    range.start = (unsigned long)startAddr;
    range.end = (unsigned long)startAddr + size;

    ret = ioctl(g_ion_alloc_context->fd, ION_IOC_SUNXI_FLUSH_RANGE, &range);
    if (ret)
        loge("ION_IOC_SUNXI_FLUSH_RANGE failed\n");

    return;
}
#else
void sunxi_ion_alloc_flush_cache(void* startAddr, int size)
{
    sunxi_cache_range range;
    struct aw_ion_custom_info custom_data;
    int ret;

    /* clean and invalid user cache */
    range.start = (unsigned long)startAddr;
    range.end = (unsigned long)startAddr + size;

    custom_data.aw_cmd = ION_IOC_SUNXI_FLUSH_RANGE;
    custom_data.aw_arg = (unsigned long)&range;

    ret = ioctl(g_ion_alloc_context->fd, AW_MEM_ION_IOC_CUSTOM, &custom_data);
    if (ret)
        loge("ION_IOC_CUSTOM failed\n");

    return;
}
#endif

void sunxi_ion_flush_cache_all()
{
    UNUSA_PARAM(sunxi_ion_flush_cache_all);

    ioctl(g_ion_alloc_context->fd, ION_IOC_SUNXI_FLUSH_ALL, 0);
}

void* sunxi_ion_alloc_alloc_drm(int size)
{
    aw_ion_allocation_info_t alloc_data;
    ion_fd_data_t fd_data;
    struct ion_handle_data handle_data;
    struct aw_ion_custom_info custom_data;
    sunxi_phys_data   phys_data, tee_data;

    int rest_size = 0;
    unsigned long addr_phy = 0;
    unsigned long addr_vir = 0;
    unsigned long addr_tee = 0;
    buffer_node * alloc_buffer = NULL;
    int ret = 0;
    (void)tee_data;
    (void)rest_size;

    pthread_mutex_lock(&g_ion_mutex_alloc);

    if (g_ion_alloc_context == NULL) {
        loge("should call ion_alloc_open\n");
        goto ALLOC_OUT;
    }

    if (size <= 0) {
        logv("can not alloc size 0\n");
        goto ALLOC_OUT;
    }

    /*alloc buffer*/
    alloc_data.aw_len = size;
    alloc_data.aw_align = ION_ALLOC_ALIGN ;
    alloc_data.aw_heap_id_mask = AW_ION_SECURE_HEAP_MASK;
    alloc_data.flags = AW_ION_CACHED_FLAG | AW_ION_CACHED_NEEDS_SYNC_FLAG;
    ret = ioctl(g_ion_alloc_context->fd, AW_MEM_ION_IOC_ALLOC, &alloc_data);
    if (ret) {
		loge("ION_IOC_ALLOC error%s\n", strerror(errno));
        goto ALLOC_OUT;
    }

    /* get dmabuf fd */
    fd_data.handle = alloc_data.handle;
    ret = ioctl(g_ion_alloc_context->fd, AW_MEM_ION_IOC_MAP, &fd_data);
    if (ret) {
        loge("ION_IOC_MAP err, ret %d, dmabuf fd 0x%08x\n",
				ret, (unsigned int)fd_data.aw_fd);
        goto ALLOC_OUT;
    }


    /* mmap to user */
    addr_vir = (unsigned long)mmap(NULL, alloc_data.aw_len, \
        PROT_READ|PROT_WRITE, MAP_SHARED, fd_data.aw_fd, 0);
    if ((unsigned long)MAP_FAILED == addr_vir) {
        addr_vir = 0;
        goto ALLOC_OUT;
    }

    /* get phy address */
    memset(&phys_data, 0, sizeof(phys_data));
    phys_data.handle = alloc_data.handle;
    phys_data.size = size;
    custom_data.aw_cmd = ION_IOC_SUNXI_PHYS_ADDR;
    custom_data.aw_arg = (unsigned long)&phys_data;

    ret = ioctl(g_ion_alloc_context->fd, AW_MEM_ION_IOC_CUSTOM, &custom_data);
    if (ret) {
        loge("ION_IOC_CUSTOM err, ret %d\n", ret);
        addr_phy = 0;
        addr_vir = 0;
        goto ALLOC_OUT;
    }

    addr_phy = phys_data.phys_addr;
#if(ADJUST_ADDRESS_FOR_SECURE_OS_OPTEE)
    memset(&tee_data, 0, sizeof(tee_data));
    tee_data.handle = alloc_data.handle;
    tee_data.size = size;
    custom_data.aw_cmd = ION_IOC_SUNXI_TEE_ADDR;
    custom_data.aw_arg = (unsigned long)&tee_data;
    ret = ioctl(g_ion_alloc_context->fd, AW_MEM_ION_IOC_CUSTOM, &custom_data);
    if (ret) {
        loge("ION_IOC_CUSTOM err, ret %d\n", ret);
        addr_phy = 0;
        addr_vir = 0;
        goto ALLOC_OUT;
    }
    addr_tee = tee_data.phys_addr;
#else
    addr_tee = addr_vir;
#endif
    alloc_buffer = (buffer_node *)malloc(sizeof(buffer_node));
    if (alloc_buffer == NULL) {
        loge("malloc buffer node failed");

        /* unmmap */
        ret = munmap((void*)addr_vir, alloc_data.aw_len);
        if(ret)
            loge("munmap err, ret %d\n", ret);

        /* close dmabuf fd */
        close(fd_data.aw_fd);

        /* free buffer */
        handle_data.handle = alloc_data.handle;
        ret = ioctl(g_ion_alloc_context->fd, AW_MEM_ION_IOC_FREE, &handle_data);
        if (ret) {
            loge("ION_IOC_FREE err, ret %d\n", ret);
        }
        addr_phy = 0;
        addr_vir = 0;

        goto ALLOC_OUT;
    }

    alloc_buffer->size = size;
    alloc_buffer->phy = addr_phy;
    alloc_buffer->user_virt = addr_vir;
    alloc_buffer->vir = addr_tee;
    alloc_buffer->tee = addr_tee;
    alloc_buffer->fd_data.handle = fd_data.handle;
    alloc_buffer->fd_data.aw_fd = fd_data.aw_fd;
    aw_mem_list_add_tail(&alloc_buffer->i_list, &g_ion_alloc_context->list);

    ALLOC_OUT:

    pthread_mutex_unlock(&g_ion_mutex_alloc);

    return (void*)addr_tee;
}

struct sunxi_pool_info binfo = {
	.total	 = 0,
	.free_kb  = 0,
	.free_mb = 0,
};

/*return total meminfo with MB */
int sunxi_ion_alloc_get_total_size()
{
    int ret = 0;
    int ion_fd = open(DEV_NAME, O_WRONLY);
	struct aw_ion_custom_info cdata;

    if (ion_fd < 0) {
        loge("open ion dev failed, cannot get ion mem.");
        goto err;
    }

    cdata.aw_cmd = ION_IOC_SUNXI_POOL_INFO;
    cdata.aw_arg = (unsigned long)&binfo;
    ret = ioctl(ion_fd,AW_MEM_ION_IOC_CUSTOM, &cdata);
    if (ret < 0) {
        loge("Failed to ioctl ion device, errno:%s\n", strerror(errno));
        goto err;
    }

    logd(" ion dev get free pool [%d MB], total [%d MB]\n",
			binfo.free_mb, binfo.total / 1024);
    ret = binfo.total;
err:
    if (ion_fd > 0)
        close(ion_fd);

	return ret;
}

int sunxi_ion_alloc_memset(void* buf, int value, size_t n)
{
    memset(buf, value, n);
    return -1;
}

int sunxi_ion_alloc_copy(void* dst, void* src, size_t n)
{
    memcpy(dst, src, n);
    return -1;
}

int sunxi_ion_alloc_read(void* dst, void* src, size_t n)
{
    memcpy(dst, src, n);
    return -1;
}

int sunxi_ion_alloc_write(void* dst, void* src, size_t n)
{
    memcpy(dst, src, n);
    return -1;
}

int sunxi_ion_alloc_setup()
{
    return -1;
}

int sunxi_ion_alloc_shutdown()
{
    return -1;
}

struct SunxiMemOpsS _allocionMemOpsS =
{
    open:				sunxi_ion_alloc_open,
    close:				sunxi_ion_alloc_close,
    total_size:			sunxi_ion_alloc_get_total_size,
    palloc:			sunxi_ion_alloc_palloc,
    pfree:				sunxi_ion_alloc_pfree,
    flush_cache:		sunxi_ion_alloc_flush_cache,
    cpu_get_phyaddr:	sunxi_ion_alloc_vir2phy_cpu,
    cpu_get_viraddr:	sunxi_ion_alloc_phy2vir_cpu,
    mem_set:			sunxi_ion_alloc_memset,
    mem_cpy:			sunxi_ion_alloc_copy,
    mem_read:			sunxi_ion_alloc_read,
    mem_write:			sunxi_ion_alloc_write,
    setup:				sunxi_ion_alloc_setup,
    shutdown:			sunxi_ion_alloc_shutdown,
    palloc_secure:		sunxi_ion_alloc_alloc_drm,
    get_bufferFd:       sunxi_ion_alloc_get_bufferFd

};

struct SunxiMemOpsS* GetMemAdapterOpsS()
{
	logd("*** get __GetIonMemOpsS ***");

	return &_allocionMemOpsS;
}
