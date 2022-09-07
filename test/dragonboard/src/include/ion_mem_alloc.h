#ifndef MEM_INTERFACE_H
#define MEM_INTERFACE_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct SunxiMemOpsS
{
    int (*open)(void);
    void (*close)(void);
    int (*total_size)(void);
    void *(*palloc)(int /*size*/);
    void  (*pfree)(void* /*mem*/);
    void (*flush_cache)(void * /*mem*/, int /*size*/);
    void *(*cpu_get_phyaddr)(void * /*viraddr*/);
    void *(*cpu_get_viraddr)(void * /*phyaddr*/);
    int (*mem_set)(void * /*s*/, int /*c*/, size_t /*n*/);
    int (*mem_cpy)(void * /*dest*/, void * /*src*/, size_t /*n*/);
    int (*mem_read)(void * /*dest */, void * /*src*/, size_t /*n*/);
    int (*mem_write)(void * /*dest*/, void * /*src*/, size_t /*n*/);
    int (*setup)(void);
    int (*shutdown)(void);
    void *(*palloc_secure)(int /*size*/);
    int (*get_bufferFd)(void * /*viraddr*/);
    int mem_actual_width;
    int mem_actual_height;
};

static inline int SunxiMemOpen(struct SunxiMemOpsS *memops)
{
    return memops->open();
}

static inline void SunxiMemClose(struct SunxiMemOpsS *memops)
{
    memops->close();
}

static inline int SunxiMemTotalSize(struct SunxiMemOpsS *memops)
{
    return memops->total_size();
}

static inline void SunxiMemSetActualSize(struct SunxiMemOpsS *memops,int width,int height)
{
    memops->mem_actual_width = width;
    memops->mem_actual_height = height;
}

static inline void SunxiMemGetActualSize(struct SunxiMemOpsS *memops,int *width,int *height)
{
    *width = memops->mem_actual_width;
    *height = memops->mem_actual_height;
}

static inline void *SunxiMemPalloc(struct SunxiMemOpsS *memops,
		int nSize)
{
    return memops->palloc(nSize);
}

static inline void SunxiMemPfree(struct SunxiMemOpsS *memops,
		void *pMem)
{
    memops->pfree(pMem);
}

static inline void SunxiMemFlushCache(struct SunxiMemOpsS *memops,
		void *pMem, int nSize)
{
    memops->flush_cache(pMem, nSize);
}

static inline void SunxiMemSet(struct SunxiMemOpsS *memops,
		void *pMem, int nValue, int nSize)
{
   memops->mem_set(pMem, nValue, nSize);
}

static inline void SunxiMemCopy(struct SunxiMemOpsS *memops,
		void *pMemDst, void *pMemSrc, int nSize)
{
    memops->mem_cpy(pMemDst, pMemSrc, nSize);
}

static inline int SunxiMemRead(struct SunxiMemOpsS *memops,
		void *pMemDst, void *pMemSrc, int nSize)
{
    memops->mem_read(pMemDst, pMemSrc, nSize);
    return 0;
}

static inline int SunxiMemWrite(struct SunxiMemOpsS *memops,
		void *pMemDst, void *pMemSrc, int nSize)
{
    (void)memops; /*not use memops */
    memops->mem_write(pMemDst, pMemSrc, nSize);
    return 0;
}

static inline void *SunxiMemGetPhysicAddressCpu(struct SunxiMemOpsS *memops,
		void *virt)
{
    return memops->cpu_get_phyaddr(virt);
}

static inline void *SunxiMemGetVirtualAddressCpu(struct SunxiMemOpsS *memops,
		void *phy)
{
    return memops->cpu_get_viraddr(phy);
}

static inline int SunxiMemSetup(struct SunxiMemOpsS *memops)
{
    return memops->setup();
}

static inline int SunxiMemShutdown(struct SunxiMemOpsS *memops)
{
    return memops->shutdown();
}

static inline void *SunxiMemPallocSecure(struct SunxiMemOpsS *memops, int nSize)
{
    return memops->palloc_secure(nSize);
}

static inline int SunxiMemGetBufferFd(struct SunxiMemOpsS *memops,
		void *virt)
{
    return memops->get_bufferFd(virt);
}


struct SunxiMemOpsS* GetMemAdapterOpsS(void);

#ifdef __cplusplus
}
#endif

#endif
