#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <stdint.h>
#include <sys/termios.h>
#include <sys/ioctl.h>
#include <limits.h>
#include "hciattach.h"

/******************************************************************************
**  Constants & Macros
******************************************************************************/
#define LOG_STR "AIC Bluetooth"
#define DBG_ON   1

#define AIC_DBG(fmt, arg...)                                \
  do {                                                     \
    if (DBG_ON)                                            \
      fprintf(stderr, "%s: " fmt "\n" , LOG_STR, ##arg);   \
  } while(0)

#define AIC_ERR(fmt, arg...)                                \
  do {                                                     \
    fprintf(stderr, "%s ERROR: " fmt "\n", LOG_STR, ##arg);\
    perror(LOG_STR" ERROR reason");                        \
  } while(0)

#define AIC_DUMP(buffer, len)                               \
  fprintf(stderr, "%s: ", LOG_STR);                        \
  do {                                                     \
    for (int i = 0; i < len; i++) {                        \
      if (i && !(i % 16)) {                                \
        fprintf(stderr, "\n");                             \
        fprintf(stderr, "%s: ", LOG_STR);                  \
      }                                                    \
      fprintf(stderr, "%02x ", buffer[i]);                 \
    }                                                      \
    fprintf(stderr, "\n");                                 \
  } while (0)

#define UINT8_TO_STREAM(p, u8) \
  { *(p)++ = (uint8_t)(u8); }

#define STREAM_TO_UINT8(u8, p) \
  {                            \
    (u8) = (uint8_t)(*(p));    \
    (p) += 1;                  \
  }

#define UINT16_TO_STREAM(p, u16)    \
  {                                 \
    *(p)++ = (uint8_t)(u16);        \
    *(p)++ = (uint8_t)((u16) >> 8); \
  }

#define STREAM_TO_UINT16(u16, p)                                  \
  {                                                               \
    (u16) = ((uint16_t)(*(p)) + (((uint16_t)(*((p) + 1))) << 8)); \
    (p) += 2;                                                     \
  }

#define UINT32_TO_STREAM(p, u32)     \
  {                                  \
    *(p)++ = (uint8_t)(u32);         \
    *(p)++ = (uint8_t)((u32) >> 8);  \
    *(p)++ = (uint8_t)((u32) >> 16); \
    *(p)++ = (uint8_t)((u32) >> 24); \
  }

#define RESPONSE_LENGTH                 100
#define HCI_CMD_MAX_LEN                 258
#define HCI_EVT_CMD_CMPL_OPCODE         3
#define HCI_PACKET_TYPE_COMMAND         1
#define HCI_CMD_PREAMBLE_SIZE           3

#define BT_HC_HDR_SIZE                  (sizeof(HC_BT_HDR))
#define BT_VND_OP_RESULT_SUCCESS        0
#define BT_VND_OP_RESULT_FAIL           1
#define MSG_STACK_TO_HC_HCI_CMD         0x2000

#define HCI_RESET                       0x0C03
#define HCI_VSC_WRITE_BD_ADDR           0xFC70
#define HCI_VSC_WR_AON_PARAM_CMD        0xFC4D
#define HCI_VSC_SET_LP_LEVEL_CMD        0xFC50
#define HCI_VSC_SET_PWR_CTRL_SLAVE_CMD  0xFC51
#define HCI_VSC_SET_CPU_POWER_OFF_CMD   0xFC52

#define HCI_VSC_WR_AON_PARAM_SIZE       104
#define AON_BT_PWR_DLY1                 (1 + 5 + 1)
#define AON_BT_PWR_DLY2                 (10 + 48 + 5 + 1)
#define AON_BT_PWR_DLY3                 (10 + 48 + 8 + 5 + 1)
#define AON_BT_PWR_DLY_AON              (10 + 48 + 8 + 5)

#define HCI_VSC_SET_LP_LEVEL_SIZE       1
#define HCI_VSC_SET_PWR_CTRL_SLAVE_SIZE 1
#define HCI_VSC_SET_CPU_POWER_OFF_SIZE  1

typedef void (*hci_cback)(void *);

typedef struct {
	uint16_t event;
	uint16_t len;
	uint16_t offset;
	uint16_t layer_specific;
	uint8_t data[];
} HC_BT_HDR;

typedef struct
{
	/// Em save start address
	uint32_t em_save_start_addr;
	/// Em save end address
	uint32_t em_save_end_addr;
	/// Minimum time that allow power off(in hs)
	int32_t aon_min_power_off_duration;
	/// Maximum aon params
	uint16_t aon_max_nb_params;
	/// RF config const time on cpus side (in hus)
	int16_t aon_rf_config_time_cpus;
	/// RF config const time on aon side (in hus)
	int16_t aon_rf_config_time_aon;
	/// Maximun active acl link supported by aon
	uint16_t aon_max_nb_active_acl;
	/// Maximun ble activity supported by aon
	uint16_t aon_ble_activity_max;
	/// Maximum bt rxdesc field supported by aon
	uint16_t aon_max_bt_rxdesc_field;
	/// Maximum bt rxdesc field supported by aon
	uint16_t aon_max_ble_rxdesc_field;
	/// Maximum regs supported by aon
	uint16_t aon_max_nb_regs;
	/// Maximum length of ke_env supported by aon
	uint16_t aon_max_ke_env_len;
	/// Maximum elements of sch_arb_env supported by aon
	uint16_t aon_max_nb_sc_arb_elt;
	/// Maximum elements of sch_plan_env supported by aon
	uint16_t aon_max_nb_sch_plan_elt;
	/// Maximum elements of sch_alarm_elt_env supported by aon
	uint16_t aon_max_nb_sch_alarm_elt;
	/// Minimum advertising interval in slots(625 us) supported by aon
	uint32_t aon_min_ble_adv_intv;
	/// Minimum connection inverval in 2-sltos(1.25ms) supported by aon
	uint32_t aon_min_ble_con_intv;
	/// Extra sleep duration for cpus(in hs), may be negative
	int32_t aon_extra_sleep_duration_cpus;
	/// Extra sleep duration for aon(in hs), may be negative
	int32_t aon_extra_sleep_duration_aon;
	/// Minimum time that allow host to power off(in us)
	int32_t aon_min_power_off_duration_cpup;
	/// aon debug level for cpus
	uint32_t aon_debug_level;
	/// aon debug level for aon
	uint32_t aon_debug_level_aon;
	/// Power on delay of bt core on when cpus_sys alive on cpus side(in lp cycles)
	uint16_t aon_bt_pwr_on_dly1;
	/// Power on delay of bt core on when cpus_sys clk gate on cpus side(in lp cycles)
	uint16_t aon_bt_pwr_on_dly2;
	/// Power on delay of bt core on when cpus_sys power off on cpus side(in lp cycles)
	uint16_t aon_bt_pwr_on_dly3;
	/// Power on delay of bt core on on aon side(in lp cycles)
	uint16_t aon_bt_pwr_on_dly_aon;
	/// Time to cancle sch arbiter elements in advance when switching to cpus(in hus)
	uint16_t aon_sch_arb_cancel_in_advance_time;
	/// Duration of sleep and wake-up algorithm (depends on CPU speed) expressed in half us on cpus side
	/// should also contain deep_sleep_on rising edge to fimecnt halt (max 4 lp cycles) and finecnt resume to dm_slp_irq(0.5 lp cycles)
	uint16_t aon_sleep_algo_dur_cpus;
	/// Duration of sleep and wake-up algorithm (depends on CPU speed) expressed in half us on aon side
	/// should also contain deep_sleep_on rising edge to fimecnt halt (max 4 lp cycles) and finecnt resume to dm_slp_irq(0.5 lp cycles)
	uint16_t aon_sleep_algo_dur_aon;
	/// Threshold that treat fractional part of restore time (in hus) as 1 hs on cpus side
	uint16_t aon_restore_time_ceil_cpus;
	/// Threshold that treat fractional part of restore time (in hus) as 1 hs on aon side
	uint16_t aon_restore_time_ceil_aon;
	/// Minmum time that allow deep sleep on cpus side (in hs)
	uint16_t aon_min_sleep_duration_cpus;
	/// Minmum time that allow deep sleep on aon side (in hs)
	uint16_t aon_min_sleep_duration_aon;
	/// Difference of resore time an save time on cpus side (in hus)
	int16_t aon_restore_save_time_diff_cpus;
	/// Difference of resore time an save time on aon side (in hus)
	int16_t aon_restore_save_time_diff_aon;
	/// Difference of restore time on aon side and save time on cpus side (in hus)
	int16_t aon_restore_save_time_diff_cpus_aon;
	/// Minimum time that allow clock gate (in hs)
	int32_t aon_min_clock_gate_duration;
	/// Minimum time that allow host to clock gate (in hus)
	int32_t aon_min_clock_gate_duration_cpup;
	// Maximum rf & md regs supported by aon
	uint16_t aon_max_nb_rf_mdm_regs;
} bt_drv_wr_aon_param;

enum {
	BT_LP_LEVEL_ACTIVE            = 0x00,//BT CORE active, CPUSYS active, VCORE active
	BT_LP_LEVEL_CLOCK_GATE1       = 0x01,//BT CORE clock gate, CPUSYS active, VCORE active
	BT_LP_LEVEL_CLOCK_GATE2       = 0x02,//BT CORE clock gate, CPUSYS clock gate, VCORE active
	BT_LP_LEVEL_CLOCK_GATE3       = 0x03,//BT CORE clock gate, CPUSYS clock gate, VCORE clock gate
	BT_LP_LEVEL_POWER_OFF1        = 0x04,//BT CORE power off, CPUSYS active, VCORE active
	BT_LP_LEVEL_POWER_OFF2        = 0x05,//BT CORE power off, CPUSYS clock gate, VCORE active
	BT_LP_LEVEL_POWER_OFF3        = 0x06,//BT CORE power off, CPUSYS power off, VCORE active
	BT_LP_LEVEL_HIBERNATE         = 0x07,//BT CORE power off, CPUSYS power off, VCORE active
	BT_LP_LEVEL_MAX               = 0x08,//
};

static uint8_t local_bdaddr[6]={0x10, 0x11, 0x12, 0x13, 0x14, 0x15};

static bt_drv_wr_aon_param wr_aon_param = {
	0x18D700, 0x18F700, 64, 40, 400, 400, 3, 2,
	3, 2, 40, 512, 20, 21, 20, 32,
	8, -1, 0, 20000, 0x113, 0x20067302, AON_BT_PWR_DLY1, AON_BT_PWR_DLY2,
	AON_BT_PWR_DLY3, AON_BT_PWR_DLY_AON, 32, 512, 420, 100, 100, 8,
	24, 40, 140, 0, 64, 20000, 50
};

static uint8_t aicbt_lp_level   = BT_LP_LEVEL_CLOCK_GATE2;
static uint8_t aicbt_pwr_slave  = 1;
static uint8_t aicbt_pwr_off_en = 1;

static int s_bt_fd = -1;

static void log_bin_to_hexstr(uint8_t *bin, uint8_t binsz, const char *log_tag)
{
	AIC_DBG("%s", log_tag);
	AIC_DUMP(bin, binsz);
}

static size_t H4Protocol_Send(uint8_t type, const uint8_t* data, size_t length)
{
	struct iovec iov[] = {
		{&type, sizeof(type)},
		{(uint8_t *)data, length}};

	ssize_t ret = 0;
	do {
		ret = writev(s_bt_fd, iov, sizeof(iov) / sizeof(iov[0]));
	} while (-1 == ret && EAGAIN == errno);

	if (ret == -1) {
		AIC_ERR("%s error writing to UART (%s)", __func__, strerror(errno));
	} else if (ret < length + 1) {
		AIC_ERR("%s: %d / %d bytes written - something went wrong...", __func__, ret, length + 1);
	}

	return ret;
}

static void *bt_vendor_alloc(int size)
{
	void *p = (uint8_t *)malloc(size);
	return p;
}

static void bt_vendor_free(void *buffer)
{
	free(buffer);
}

static uint8_t bt_vendor_xmit(uint16_t opcode, void* buffer, hci_cback callback)
{
	uint8_t type = HCI_PACKET_TYPE_COMMAND;
	(void)opcode;
	HC_BT_HDR* bt_hdr = (HC_BT_HDR *)buffer;
	H4Protocol_Send(type, bt_hdr->data, bt_hdr->len);
	return BT_VND_OP_RESULT_SUCCESS;
}

static uint8_t aic_send_hci_cmd(uint16_t cmd, uint8_t *payload, uint8_t len, hci_cback cback)
{
	HC_BT_HDR   *p_buf;
	uint8_t     *p, ret;

	p_buf = (HC_BT_HDR *)bt_vendor_alloc(
		BT_HC_HDR_SIZE + HCI_CMD_PREAMBLE_SIZE + len);
	if (p_buf) {
		p_buf->event = MSG_STACK_TO_HC_HCI_CMD;
		p_buf->offset = 0;
		p_buf->layer_specific = 0;
		p_buf->len = HCI_CMD_PREAMBLE_SIZE + len;
		p = (uint8_t *)(p_buf + 1);

		UINT16_TO_STREAM(p, cmd);
		*p++ = len;
		memcpy(p, payload, len);
		log_bin_to_hexstr((uint8_t *)(p_buf + 1), HCI_CMD_PREAMBLE_SIZE + len, __FUNCTION__);
		ret = bt_vendor_xmit(cmd, p_buf, cback);
		bt_vendor_free(p_buf);
		return ret;
	}
	return BT_VND_OP_RESULT_FAIL;
}

static uint8_t hw_cfg_set_bd_addr(void *param)
{
	uint8_t *p, msg_req[HCI_CMD_MAX_LEN];
	uint8_t *addr = (uint8_t *)param;
	uint8_t i;
	p = msg_req;

	AIC_DBG("Setting local bd addr to %02X:%02X:%02X:%02X:%02X:%02X",
		addr[0], addr[1], addr[2],
		addr[3], addr[4], addr[5]);

	for (i = 0; i < 6; i++)
		UINT8_TO_STREAM(p, addr[5 - i]);

	aic_send_hci_cmd(HCI_VSC_WRITE_BD_ADDR, msg_req, (uint8_t)(p - msg_req), NULL);
	return 0;
}

static uint8_t hw_cfg_wr_aon_param(void *param)
{
	aic_send_hci_cmd(HCI_VSC_WR_AON_PARAM_CMD, (uint8_t *)param, HCI_VSC_WR_AON_PARAM_SIZE, NULL);
	return 0;
}

static uint8_t hw_cfg_set_lp_level(void *param)
{
	aic_send_hci_cmd(HCI_VSC_SET_LP_LEVEL_CMD, (uint8_t *)param, HCI_VSC_SET_LP_LEVEL_SIZE, NULL);
	return 0;
}

static uint8_t hw_cfg_set_pwr_ctrl_slave(void *param)
{
	aic_send_hci_cmd(HCI_VSC_SET_PWR_CTRL_SLAVE_CMD, (uint8_t *)param, HCI_VSC_SET_PWR_CTRL_SLAVE_SIZE, NULL);
	return 0;
}

static uint8_t hw_cfg_set_cpu_power_off_en(void *param)
{
	aic_send_hci_cmd(HCI_VSC_SET_CPU_POWER_OFF_CMD, (uint8_t *)param, HCI_VSC_SET_CPU_POWER_OFF_SIZE, NULL);
	return 0;
}

int aic_config_init(int fd, struct uart_t *u, struct termios *ti)
{
	uint8_t recv[256];
	int len = 0;

	s_bt_fd = fd;

	aic_send_hci_cmd(HCI_RESET, NULL, 0, NULL);
	len = read_hci_event(s_bt_fd, recv, RESPONSE_LENGTH);
	AIC_DBG("Received event, len: %d", len);
	AIC_DUMP(recv, len);

	hw_cfg_set_bd_addr(local_bdaddr);
	len = read_hci_event(s_bt_fd, recv, RESPONSE_LENGTH);
	AIC_DBG("Received event, len: %d", len);
	AIC_DUMP(recv, len);

	hw_cfg_wr_aon_param(&wr_aon_param);
	len = read_hci_event(s_bt_fd, recv, RESPONSE_LENGTH);
	AIC_DBG("Received event, len: %d", len);
	AIC_DUMP(recv, len);

	hw_cfg_set_lp_level(&aicbt_lp_level);
	len = read_hci_event(s_bt_fd, recv, RESPONSE_LENGTH);
	AIC_DBG("Received event, len: %d", len);
	AIC_DUMP(recv, len);

	hw_cfg_set_pwr_ctrl_slave(&aicbt_pwr_slave);
	len = read_hci_event(s_bt_fd, recv, RESPONSE_LENGTH);
	AIC_DBG("Received event, len: %d", len);
	AIC_DUMP(recv, len);

	hw_cfg_set_cpu_power_off_en(&aicbt_pwr_off_en);
	len = read_hci_event(s_bt_fd, recv, RESPONSE_LENGTH);
	AIC_DBG("Received event, len: %d", len);
	AIC_DUMP(recv, len);

	return 0;
}

int aic_config_post(int fd, struct uart_t *u, struct termios *ti)
{
	AIC_DBG("Done setting line discpline");
	return 0;
}

