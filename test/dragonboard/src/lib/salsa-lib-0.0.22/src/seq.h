#ifndef __ALSA_SEQ_H
#define __ALSA_SEQ_H

#define SND_SEQ_OPEN_OUTPUT	1	/**< open for output (write) */
#define SND_SEQ_OPEN_INPUT	2	/**< open for input (read) */
#define SND_SEQ_OPEN_DUPLEX	(SND_SEQ_OPEN_OUTPUT|SND_SEQ_OPEN_INPUT)	/**< open for both input and output (read/write) */

#define SND_SEQ_NONBLOCK	0x0001	/**< non-blocking mode (flag to open mode) */

/** sequencer handle type */
typedef enum _snd_seq_type {
	SND_SEQ_TYPE_HW,		/**< hardware */
	SND_SEQ_TYPE_SHM,		/**< shared memory (NYI) */
	SND_SEQ_TYPE_INET		/**< network (NYI) */
} snd_seq_type_t;

/** special client (port) ids */
#define SND_SEQ_ADDRESS_UNKNOWN		253	/**< unknown source */
#define SND_SEQ_ADDRESS_SUBSCRIBERS	254	/**< send event to all subscribed ports */
#define SND_SEQ_ADDRESS_BROADCAST	255	/**< send event to all queues/clients/ports/channels */

/** known client numbers */
#define SND_SEQ_CLIENT_SYSTEM		0	/**< system client */

/*
 */
static inline __SALSA_NOT_IMPLEMENTED
int snd_seq_open(snd_seq_t **handle, const char *name, int streams, int mode)
{
	return -ENXIO;
}

static inline __SALSA_NOT_IMPLEMENTED
int snd_seq_open_lconf(snd_seq_t **handle, const char *name, int streams,
		       int mode, snd_config_t *lconf)
{
	return -ENXIO;
}
static inline
const char *snd_seq_name(snd_seq_t *seq)
{
	return NULL;
}
static inline
snd_seq_type_t snd_seq_type(snd_seq_t *seq)
{
	return SND_SEQ_TYPE_HW;
}
static inline
int snd_seq_close(snd_seq_t *handle)
{
	return 0;
}
static inline
int snd_seq_poll_descriptors_count(snd_seq_t *handle, short events)
{
	return 0;
}
static inline
int snd_seq_poll_descriptors(snd_seq_t *handle, struct pollfd *pfds,
			     unsigned int space, short events)
{
	return -ENXIO;
}
static inline
int snd_seq_poll_descriptors_revents(snd_seq_t *seq, struct pollfd *pfds,
				     unsigned int nfds, unsigned short *revents)
{
	return -ENXIO;
}
static inline
int snd_seq_nonblock(snd_seq_t *handle, int nonblock)
{
	return -ENXIO;
}
static inline
int snd_seq_client_id(snd_seq_t *handle)
{
	return -ENXIO;
}

static inline
size_t snd_seq_get_output_buffer_size(snd_seq_t *handle)
{
	return 0;
}
static inline
size_t snd_seq_get_input_buffer_size(snd_seq_t *handle)
{
	return 0;
}
static inline
int snd_seq_set_output_buffer_size(snd_seq_t *handle, size_t size)
{
	return -ENXIO;
}
static inline
int snd_seq_set_input_buffer_size(snd_seq_t *handle, size_t size)
{
	return -ENXIO;
}

/** system information container */
typedef struct _snd_seq_system_info snd_seq_system_info_t;

static inline
size_t snd_seq_system_info_sizeof(void)
{
	return 0;
}
/** allocate a #snd_seq_system_info_t container on stack */
#define snd_seq_system_info_alloca(ptr)
static inline
int snd_seq_system_info_malloc(snd_seq_system_info_t **ptr)
{
	return -ENXIO;
}
static inline
void snd_seq_system_info_free(snd_seq_system_info_t *ptr)
{
}
static inline
void snd_seq_system_info_copy(snd_seq_system_info_t *dst,
			      const snd_seq_system_info_t *src)
{
}

static inline
int snd_seq_system_info_get_queues(const snd_seq_system_info_t *info)
{
	return 0;
}
static inline
int snd_seq_system_info_get_clients(const snd_seq_system_info_t *info)
{
	return 0;
}
static inline
int snd_seq_system_info_get_ports(const snd_seq_system_info_t *info)
{
	return 0;
}
static inline
int snd_seq_system_info_get_channels(const snd_seq_system_info_t *info)
{
	return 0;
}
static inline
int snd_seq_system_info_get_cur_clients(const snd_seq_system_info_t *info)
{
	return 0;
}
static inline
int snd_seq_system_info_get_cur_queues(const snd_seq_system_info_t *info)
{
	return 0;
}

static inline
int snd_seq_system_info(snd_seq_t *handle, snd_seq_system_info_t *info)
{
	return -ENXIO;
}

/** client information container */
typedef struct _snd_seq_client_info snd_seq_client_info_t;

/** client types */
typedef enum snd_seq_client_type {
	SND_SEQ_USER_CLIENT     = 1,	/**< user client */
	SND_SEQ_KERNEL_CLIENT   = 2	/**< kernel client */
} snd_seq_client_type_t;

static inline
size_t snd_seq_client_info_sizeof(void)
{
	return 0;
}
/** allocate a #snd_seq_client_info_t container on stack */
#define snd_seq_client_info_alloca(ptr)

static inline
int snd_seq_client_info_malloc(snd_seq_client_info_t **ptr)
{
	return -ENXIO;
}
static inline
void snd_seq_client_info_free(snd_seq_client_info_t *ptr)
{
}
static inline
void snd_seq_client_info_copy(snd_seq_client_info_t *dst,
			      const snd_seq_client_info_t *src)
{
}

static inline
int snd_seq_client_info_get_client(const snd_seq_client_info_t *info)
{
	return -ENXIO;
}
static inline
snd_seq_client_type_t snd_seq_client_info_get_type(const snd_seq_client_info_t *info)
{
	return SND_SEQ_USER_CLIENT;
}
static inline
const char *snd_seq_client_info_get_name(snd_seq_client_info_t *info)
{
	return NULL;
}
static inline
int snd_seq_client_info_get_broadcast_filter(const snd_seq_client_info_t *info)
{
	return 0;
}
static inline
int snd_seq_client_info_get_error_bounce(const snd_seq_client_info_t *info)
{
	return 0;
}
static inline
const unsigned char *snd_seq_client_info_get_event_filter(const snd_seq_client_info_t *info)
{
	return NULL;
}
static inline
int snd_seq_client_info_get_num_ports(const snd_seq_client_info_t *info)
{
	return 0;
}
static inline
int snd_seq_client_info_get_event_lost(const snd_seq_client_info_t *info)
{
	return 0;
}

static inline
void snd_seq_client_info_set_client(snd_seq_client_info_t *info, int client)
{
}
static inline
void snd_seq_client_info_set_name(snd_seq_client_info_t *info, const char *name)
{
}
static inline
void snd_seq_client_info_set_broadcast_filter(snd_seq_client_info_t *info, int val)
{
}
static inline
void snd_seq_client_info_set_error_bounce(snd_seq_client_info_t *info, int val)
{
}
static inline
void snd_seq_client_info_set_event_filter(snd_seq_client_info_t *info, unsigned char *filter)
{
}

static inline
int snd_seq_get_client_info(snd_seq_t *handle, snd_seq_client_info_t *info)
{
	return -ENXIO;
}
static inline
int snd_seq_get_any_client_info(snd_seq_t *handle, int client, snd_seq_client_info_t *info)
{
	return -ENXIO;
}
static inline
int snd_seq_set_client_info(snd_seq_t *handle, snd_seq_client_info_t *info)
{
	return -ENXIO;
}
static inline
int snd_seq_query_next_client(snd_seq_t *handle, snd_seq_client_info_t *info)
{
	return -ENXIO;
}

/*
 */

/** client pool information container */
typedef struct _snd_seq_client_pool snd_seq_client_pool_t;

static inline
size_t snd_seq_client_pool_sizeof(void)
{
	return 0;
}
/** allocate a #snd_seq_client_pool_t container on stack */
#define snd_seq_client_pool_alloca(ptr)

static inline
int snd_seq_client_pool_malloc(snd_seq_client_pool_t **ptr)
{
	return -ENXIO;
}
static inline
void snd_seq_client_pool_free(snd_seq_client_pool_t *ptr)
{
}
static inline
void snd_seq_client_pool_copy(snd_seq_client_pool_t *dst,
			      const snd_seq_client_pool_t *src)
{
}

static inline
int snd_seq_client_pool_get_client(const snd_seq_client_pool_t *info)
{
	return -ENXIO;
}
static inline
size_t snd_seq_client_pool_get_output_pool(const snd_seq_client_pool_t *info)
{
	return 0;
}
static inline
size_t snd_seq_client_pool_get_input_pool(const snd_seq_client_pool_t *info)
{
	return 0;
}
static inline
size_t snd_seq_client_pool_get_output_room(const snd_seq_client_pool_t *info)
{
	return 0;
}
static inline
size_t snd_seq_client_pool_get_output_free(const snd_seq_client_pool_t *info)
{
	return 0;
}
static inline
size_t snd_seq_client_pool_get_input_free(const snd_seq_client_pool_t *info)
{
	return 0;
}
static inline
void snd_seq_client_pool_set_output_pool(snd_seq_client_pool_t *info,
					 size_t size)
{
}
static inline
void snd_seq_client_pool_set_input_pool(snd_seq_client_pool_t *info, size_t size)
{
}
static inline
void snd_seq_client_pool_set_output_room(snd_seq_client_pool_t *info, size_t size)
{
}

static inline
int snd_seq_get_client_pool(snd_seq_t *handle, snd_seq_client_pool_t *info)
{
	return -ENXIO;
}
static inline
int snd_seq_set_client_pool(snd_seq_t *handle, snd_seq_client_pool_t *info)
{
	return -ENXIO;
}


/** port information container */
typedef struct _snd_seq_port_info snd_seq_port_info_t;

/** known port numbers */
#define SND_SEQ_PORT_SYSTEM_TIMER	0	/**< system timer port */
#define SND_SEQ_PORT_SYSTEM_ANNOUNCE	1	/**< system announce port */

/** port capabilities (32 bits) */
#define SND_SEQ_PORT_CAP_READ		(1<<0)	/**< readable from this port */
#define SND_SEQ_PORT_CAP_WRITE		(1<<1)	/**< writable to this port */

#define SND_SEQ_PORT_CAP_SYNC_READ	(1<<2)	/**< allow read subscriptions */
#define SND_SEQ_PORT_CAP_SYNC_WRITE	(1<<3)	/**< allow write subscriptions */

#define SND_SEQ_PORT_CAP_DUPLEX		(1<<4)	/**< allow read/write duplex */

#define SND_SEQ_PORT_CAP_SUBS_READ	(1<<5)	/**< allow read subscription */
#define SND_SEQ_PORT_CAP_SUBS_WRITE	(1<<6)	/**< allow write subscription */
#define SND_SEQ_PORT_CAP_NO_EXPORT	(1<<7)	/**< routing not allowed */

/* port type */
/** Messages sent from/to this port have device-specific semantics. */
#define SND_SEQ_PORT_TYPE_SPECIFIC	(1<<0)
/** This port understands MIDI messages. */
#define SND_SEQ_PORT_TYPE_MIDI_GENERIC	(1<<1)
/** This port is compatible with the General MIDI specification. */
#define SND_SEQ_PORT_TYPE_MIDI_GM	(1<<2)
/** This port is compatible with the Roland GS standard. */
#define SND_SEQ_PORT_TYPE_MIDI_GS	(1<<3)
/** This port is compatible with the Yamaha XG specification. */
#define SND_SEQ_PORT_TYPE_MIDI_XG	(1<<4)
/** This port is compatible with the Roland MT-32. */
#define SND_SEQ_PORT_TYPE_MIDI_MT32	(1<<5)
/** This port is compatible with the General MIDI 2 specification. */
#define SND_SEQ_PORT_TYPE_MIDI_GM2	(1<<6)
/** This port understands SND_SEQ_EVENT_SAMPLE_xxx messages
    (these are not MIDI messages). */
#define SND_SEQ_PORT_TYPE_SYNTH		(1<<10)
/** Instruments can be downloaded to this port
    (with SND_SEQ_EVENT_INSTR_xxx messages sent directly). */
#define SND_SEQ_PORT_TYPE_DIRECT_SAMPLE (1<<11)
/** Instruments can be downloaded to this port
    (with SND_SEQ_EVENT_INSTR_xxx messages sent directly or through a queue). */
#define SND_SEQ_PORT_TYPE_SAMPLE	(1<<12)
/** This port is implemented in hardware. */
#define SND_SEQ_PORT_TYPE_HARDWARE	(1<<16)
/** This port is implemented in software. */
#define SND_SEQ_PORT_TYPE_SOFTWARE	(1<<17)
/** Messages sent to this port will generate sounds. */
#define SND_SEQ_PORT_TYPE_SYNTHESIZER	(1<<18)
/** This port may connect to other devices
    (whose characteristics are not known). */
#define SND_SEQ_PORT_TYPE_PORT		(1<<19)
/** This port belongs to an application, such as a sequencer or editor. */
#define SND_SEQ_PORT_TYPE_APPLICATION	(1<<20)


static inline
size_t snd_seq_port_info_sizeof(void)
{
	return 0;
}
/** allocate a #snd_seq_port_info_t container on stack */
#define snd_seq_port_info_alloca(ptr)

static inline
int snd_seq_port_info_malloc(snd_seq_port_info_t **ptr)
{
	return -ENXIO;
}
static inline
void snd_seq_port_info_free(snd_seq_port_info_t *ptr)
{
}
static inline
void snd_seq_port_info_copy(snd_seq_port_info_t *dst,
			    const snd_seq_port_info_t *src)
{
}

static inline
int snd_seq_port_info_get_client(const snd_seq_port_info_t *info)
{
	return -ENXIO;
}
static inline
int snd_seq_port_info_get_port(const snd_seq_port_info_t *info)
{
	return -ENXIO;
}
static inline
const snd_seq_addr_t *snd_seq_port_info_get_addr(const snd_seq_port_info_t *info)
{
	return NULL;
}
static inline
const char *snd_seq_port_info_get_name(const snd_seq_port_info_t *info)
{
	return NULL;
}
static inline
unsigned int snd_seq_port_info_get_capability(const snd_seq_port_info_t *info)
{
	return 0;
}
static inline
unsigned int snd_seq_port_info_get_type(const snd_seq_port_info_t *info)
{
	return 0;
}
static inline
int snd_seq_port_info_get_midi_channels(const snd_seq_port_info_t *info)
{
	return 0;
}
static inline
int snd_seq_port_info_get_midi_voices(const snd_seq_port_info_t *info)
{
	return 0;
}
static inline
int snd_seq_port_info_get_synth_voices(const snd_seq_port_info_t *info)
{
	return 0;
}
static inline
int snd_seq_port_info_get_read_use(const snd_seq_port_info_t *info)
{
	return 0;
}
static inline
int snd_seq_port_info_get_write_use(const snd_seq_port_info_t *info)
{
	return 0;
}
static inline
int snd_seq_port_info_get_port_specified(const snd_seq_port_info_t *info)
{
	return 0;
}
static inline
int snd_seq_port_info_get_timestamping(const snd_seq_port_info_t *info)
{
	return 0;
}
static inline
int snd_seq_port_info_get_timestamp_real(const snd_seq_port_info_t *info)
{
	return 0;
}
static inline
int snd_seq_port_info_get_timestamp_queue(const snd_seq_port_info_t *info)
{
	return 0;
}

static inline
void snd_seq_port_info_set_client(snd_seq_port_info_t *info, int client)
{
}
static inline
void snd_seq_port_info_set_port(snd_seq_port_info_t *info, int port)
{

}
static inline
void snd_seq_port_info_set_addr(snd_seq_port_info_t *info,
				const snd_seq_addr_t *addr)
{
}
static inline
void snd_seq_port_info_set_name(snd_seq_port_info_t *info, const char *name)
{
}
static inline
void snd_seq_port_info_set_capability(snd_seq_port_info_t *info,
				      unsigned int capability)
{
}
static inline
void snd_seq_port_info_set_type(snd_seq_port_info_t *info, unsigned int type)
{
}
static inline
void snd_seq_port_info_set_midi_channels(snd_seq_port_info_t *info,
					 int channels)
{
}
static inline
void snd_seq_port_info_set_midi_voices(snd_seq_port_info_t *info, int voices)
{
}
static inline
void snd_seq_port_info_set_synth_voices(snd_seq_port_info_t *info, int voices)
{
}
static inline
void snd_seq_port_info_set_port_specified(snd_seq_port_info_t *info, int val)
{
}
static inline
void snd_seq_port_info_set_timestamping(snd_seq_port_info_t *info, int enable)
{
}
static inline
void snd_seq_port_info_set_timestamp_real(snd_seq_port_info_t *info,
					  int realtime)
{
}
static inline
void snd_seq_port_info_set_timestamp_queue(snd_seq_port_info_t *info, int queue)
{
}

static inline
int snd_seq_create_port(snd_seq_t *handle, snd_seq_port_info_t *info)
{
	return -ENXIO;
}
static inline
int snd_seq_delete_port(snd_seq_t *handle, int port)
{
	return -ENXIO;
}
static inline
int snd_seq_get_port_info(snd_seq_t *handle, int port,
			  snd_seq_port_info_t *info)
{
	return -ENXIO;
}
static inline
int snd_seq_get_any_port_info(snd_seq_t *handle, int client, int port,
			      snd_seq_port_info_t *info)
{
	return -ENXIO;
}
static inline
int snd_seq_set_port_info(snd_seq_t *handle, int port,
			  snd_seq_port_info_t *info)
{
	return -ENXIO;
}
static inline
int snd_seq_query_next_port(snd_seq_t *handle, snd_seq_port_info_t *info)
{
	return -ENXIO;
}

/** port subscription container */
typedef struct _snd_seq_port_subscribe snd_seq_port_subscribe_t;

static inline
size_t snd_seq_port_subscribe_sizeof(void)
{
	return 0;
}
/** allocate a #snd_seq_port_subscribe_t container on stack */
#define snd_seq_port_subscribe_alloca(ptr)

static inline
int snd_seq_port_subscribe_malloc(snd_seq_port_subscribe_t **ptr)
{
	return -ENXIO;
}
static inline
void snd_seq_port_subscribe_free(snd_seq_port_subscribe_t *ptr)
{
}
static inline
void snd_seq_port_subscribe_copy(snd_seq_port_subscribe_t *dst,
				 const snd_seq_port_subscribe_t *src)
{
}

static inline
const snd_seq_addr_t *snd_seq_port_subscribe_get_sender(const snd_seq_port_subscribe_t *info)
{
	return NULL;
}
static inline
const snd_seq_addr_t *snd_seq_port_subscribe_get_dest(const snd_seq_port_subscribe_t *info)
{
	return NULL;
}
static inline
int snd_seq_port_subscribe_get_queue(const snd_seq_port_subscribe_t *info)
{
	return 0;
}
static inline
int snd_seq_port_subscribe_get_exclusive(const snd_seq_port_subscribe_t *info)
{
	return 0;
}
static inline
int snd_seq_port_subscribe_get_time_update(const snd_seq_port_subscribe_t *info)
{
	return 0;
}
static inline
int snd_seq_port_subscribe_get_time_real(const snd_seq_port_subscribe_t *info)
{
	return 0;
}

static inline
void snd_seq_port_subscribe_set_sender(snd_seq_port_subscribe_t *info,
				       const snd_seq_addr_t *addr)
{
}
static inline
void snd_seq_port_subscribe_set_dest(snd_seq_port_subscribe_t *info,
				     const snd_seq_addr_t *addr)
{
}
static inline
void snd_seq_port_subscribe_set_queue(snd_seq_port_subscribe_t *info, int q)
{
}
static inline
void snd_seq_port_subscribe_set_exclusive(snd_seq_port_subscribe_t *info,
					  int val)
{
}
static inline
void snd_seq_port_subscribe_set_time_update(snd_seq_port_subscribe_t *info,
					    int val)
{
}
static inline
void snd_seq_port_subscribe_set_time_real(snd_seq_port_subscribe_t *info,
					  int val)
{
}

static inline
int snd_seq_get_port_subscription(snd_seq_t *handle,
				  snd_seq_port_subscribe_t *sub)
{
	return -ENXIO;
}
static inline
int snd_seq_subscribe_port(snd_seq_t *handle, snd_seq_port_subscribe_t *sub)
{
	return -ENXIO;
}
static inline
int snd_seq_unsubscribe_port(snd_seq_t *handle, snd_seq_port_subscribe_t *sub)
{
	return -ENXIO;
}

/*
 */

/** subscription query container */
typedef struct _snd_seq_query_subscribe snd_seq_query_subscribe_t;

/** type of query subscription */
typedef enum {
	SND_SEQ_QUERY_SUBS_READ,	/**< query read subscriptions */
	SND_SEQ_QUERY_SUBS_WRITE	/**< query write subscriptions */
} snd_seq_query_subs_type_t;

static inline
size_t snd_seq_query_subscribe_sizeof(void)
{
	return 0;
}
/** allocate a #snd_seq_query_subscribe_t container on stack */
#define snd_seq_query_subscribe_alloca(ptr)

static inline
int snd_seq_query_subscribe_malloc(snd_seq_query_subscribe_t **ptr)
{
	return -ENXIO;
}
static inline
void snd_seq_query_subscribe_free(snd_seq_query_subscribe_t *ptr)
{
}
static inline
void snd_seq_query_subscribe_copy(snd_seq_query_subscribe_t *dst,
				  const snd_seq_query_subscribe_t *src)
{
}

static inline
int snd_seq_query_subscribe_get_client(const snd_seq_query_subscribe_t *info)
{
	return 0;
}
static inline
int snd_seq_query_subscribe_get_port(const snd_seq_query_subscribe_t *info)
{
	return 0;
}
static inline
const snd_seq_addr_t *snd_seq_query_subscribe_get_root(const snd_seq_query_subscribe_t *info)
{
	return NULL;
}
static inline
snd_seq_query_subs_type_t snd_seq_query_subscribe_get_type(const snd_seq_query_subscribe_t *info)
{
	return SND_SEQ_QUERY_SUBS_READ;
}
static inline
int snd_seq_query_subscribe_get_index(const snd_seq_query_subscribe_t *info)
{
	return 0;
}
static inline
int snd_seq_query_subscribe_get_num_subs(const snd_seq_query_subscribe_t *info)
{
	return 0;
}
static inline
const snd_seq_addr_t *snd_seq_query_subscribe_get_addr(const snd_seq_query_subscribe_t *info)
{
	return NULL;
}
static inline
int snd_seq_query_subscribe_get_queue(const snd_seq_query_subscribe_t *info)
{
	return 0;
}
static inline
int snd_seq_query_subscribe_get_exclusive(const snd_seq_query_subscribe_t *info)
{
	return 0;
}
static inline
int snd_seq_query_subscribe_get_time_update(const snd_seq_query_subscribe_t *info)
{
	return 0;
}
static inline
int snd_seq_query_subscribe_get_time_real(const snd_seq_query_subscribe_t *info)
{
	return 0;
}

static inline
void snd_seq_query_subscribe_set_client(snd_seq_query_subscribe_t *info,
					int client)
{
}
static inline
void snd_seq_query_subscribe_set_port(snd_seq_query_subscribe_t *info, int port)
{
}
static inline
void snd_seq_query_subscribe_set_root(snd_seq_query_subscribe_t *info,
				      const snd_seq_addr_t *addr)
{
}
static inline
void snd_seq_query_subscribe_set_type(snd_seq_query_subscribe_t *info,
				      snd_seq_query_subs_type_t type)
{
}
static inline
void snd_seq_query_subscribe_set_index(snd_seq_query_subscribe_t *info,
				       int _index)
{
}

static inline
int snd_seq_query_port_subscribers(snd_seq_t *seq,
				   snd_seq_query_subscribe_t * subs)
{
	return -ENXIO;
}

/** queue information container */
typedef struct _snd_seq_queue_info snd_seq_queue_info_t;
/** queue status container */
typedef struct _snd_seq_queue_status snd_seq_queue_status_t;
/** queue tempo container */
typedef struct _snd_seq_queue_tempo snd_seq_queue_tempo_t;
/** queue timer information container */
typedef struct _snd_seq_queue_timer snd_seq_queue_timer_t;

/** special queue ids */
#define SND_SEQ_QUEUE_DIRECT		253	/**< direct dispatch */

static inline
size_t snd_seq_queue_info_sizeof(void)
{
	return 0;
}
/** allocate a #snd_seq_queue_info_t container on stack */
#define snd_seq_queue_info_alloca(ptr)

static inline
int snd_seq_queue_info_malloc(snd_seq_queue_info_t **ptr)
{
	return -ENXIO;
}
static inline
void snd_seq_queue_info_free(snd_seq_queue_info_t *ptr)
{
}
static inline
void snd_seq_queue_info_copy(snd_seq_queue_info_t *dst,
			     const snd_seq_queue_info_t *src)
{
}

static inline
int snd_seq_queue_info_get_queue(const snd_seq_queue_info_t *info)
{
	return 0;
}
static inline
const char *snd_seq_queue_info_get_name(const snd_seq_queue_info_t *info)
{
	return NULL;
}
static inline
int snd_seq_queue_info_get_owner(const snd_seq_queue_info_t *info)
{
	return 0;
}
static inline
int snd_seq_queue_info_get_locked(const snd_seq_queue_info_t *info)
{
	return 0;
}
static inline
unsigned int snd_seq_queue_info_get_flags(const snd_seq_queue_info_t *info)
{
	return 0;
}

static inline
void snd_seq_queue_info_set_name(snd_seq_queue_info_t *info, const char *name)
{
}
static inline
void snd_seq_queue_info_set_owner(snd_seq_queue_info_t *info, int owner)
{
}
static inline
void snd_seq_queue_info_set_locked(snd_seq_queue_info_t *info, int locked)
{
}
static inline
void snd_seq_queue_info_set_flags(snd_seq_queue_info_t *info, unsigned int flags)
{
}

static inline
int snd_seq_create_queue(snd_seq_t *seq, snd_seq_queue_info_t *info)
{
	return -ENXIO;
}
static inline
int snd_seq_alloc_named_queue(snd_seq_t *seq, const char *name)
{
	return -ENXIO;
}
static inline
int snd_seq_alloc_queue(snd_seq_t *handle)
{
	return -ENXIO;
}
static inline
int snd_seq_free_queue(snd_seq_t *handle, int q)
{
	return -ENXIO;
}
static inline
int snd_seq_get_queue_info(snd_seq_t *seq, int q, snd_seq_queue_info_t *info)
{
	return -ENXIO;
}
static inline
int snd_seq_set_queue_info(snd_seq_t *seq, int q, snd_seq_queue_info_t *info)
{
	return -ENXIO;
}
static inline
int snd_seq_query_named_queue(snd_seq_t *seq, const char *name)
{
	return -ENXIO;
}

static inline
int snd_seq_get_queue_usage(snd_seq_t *handle, int q)
{
	return -ENXIO;
}
static inline
int snd_seq_set_queue_usage(snd_seq_t *handle, int q, int used)
{
	return -ENXIO;
}

/*
 */
static inline
size_t snd_seq_queue_status_sizeof(void)
{
	return 0;
}
/** allocate a #snd_seq_queue_status_t container on stack */
#define snd_seq_queue_status_alloca(ptr)

static inline
int snd_seq_queue_status_malloc(snd_seq_queue_status_t **ptr)
{
	return -ENXIO;
}
static inline
void snd_seq_queue_status_free(snd_seq_queue_status_t *ptr)
{
}
static inline
void snd_seq_queue_status_copy(snd_seq_queue_status_t *dst,
			       const snd_seq_queue_status_t *src)
{
}

static inline
int snd_seq_queue_status_get_queue(const snd_seq_queue_status_t *info)
{
	return 0;
}
static inline
int snd_seq_queue_status_get_events(const snd_seq_queue_status_t *info)
{
	return 0;
}
static inline
snd_seq_tick_time_t snd_seq_queue_status_get_tick_time(const snd_seq_queue_status_t *info)
{
	return 0;
}
static inline
const snd_seq_real_time_t *snd_seq_queue_status_get_real_time(const snd_seq_queue_status_t *info)
{
	return NULL;
}
static inline
unsigned int snd_seq_queue_status_get_status(const snd_seq_queue_status_t *info)
{
	return 0;
}

static inline
int snd_seq_get_queue_status(snd_seq_t *handle, int q,
			     snd_seq_queue_status_t *status)
{
	return 0;
}

/*
 */
static inline
size_t snd_seq_queue_tempo_sizeof(void)
{
	return 0;
}
/** allocate a #snd_seq_queue_tempo_t container on stack */
#define snd_seq_queue_tempo_alloca(ptr)

static inline
int snd_seq_queue_tempo_malloc(snd_seq_queue_tempo_t **ptr)
{
	return -ENXIO;
}
static inline
void snd_seq_queue_tempo_free(snd_seq_queue_tempo_t *ptr)
{
}
static inline
void snd_seq_queue_tempo_copy(snd_seq_queue_tempo_t *dst,
			      const snd_seq_queue_tempo_t *src)
{
}

static inline
int snd_seq_queue_tempo_get_queue(const snd_seq_queue_tempo_t *info)
{
	return 0;
}
static inline
unsigned int snd_seq_queue_tempo_get_tempo(const snd_seq_queue_tempo_t *info)
{
	return 0;
}
static inline
int snd_seq_queue_tempo_get_ppq(const snd_seq_queue_tempo_t *info)
{
	return 0;
}
static inline
unsigned int snd_seq_queue_tempo_get_skew(const snd_seq_queue_tempo_t *info)
{
	return 0;
}
static inline
unsigned int snd_seq_queue_tempo_get_skew_base(const snd_seq_queue_tempo_t *info)
{
	return 0;
}
static inline
void snd_seq_queue_tempo_set_tempo(snd_seq_queue_tempo_t *info,
				   unsigned int tempo)
{
}
static inline
void snd_seq_queue_tempo_set_ppq(snd_seq_queue_tempo_t *info, int ppq)
{
}
static inline
void snd_seq_queue_tempo_set_skew(snd_seq_queue_tempo_t *info,
				  unsigned int skew)
{
}
static inline
void snd_seq_queue_tempo_set_skew_base(snd_seq_queue_tempo_t *info,
				       unsigned int base)
{
}

static inline
int snd_seq_get_queue_tempo(snd_seq_t *handle, int q,
			    snd_seq_queue_tempo_t *tempo)
{
	return -ENXIO;
}
static inline
int snd_seq_set_queue_tempo(snd_seq_t *handle, int q,
			    snd_seq_queue_tempo_t *tempo)
{
	return -ENXIO;
}

/*
 */

/** sequencer timer sources */
typedef enum {
	SND_SEQ_TIMER_ALSA = 0,		/* ALSA timer */
	SND_SEQ_TIMER_MIDI_CLOCK = 1,	/* Midi Clock (CLOCK event) */
	SND_SEQ_TIMER_MIDI_TICK = 2	/* Midi Timer Tick (TICK event */
} snd_seq_queue_timer_type_t;

static inline
size_t snd_seq_queue_timer_sizeof(void)
{
	return 0;
}
/** allocate a #snd_seq_queue_timer_t container on stack */
#define snd_seq_queue_timer_alloca(ptr)

static inline
int snd_seq_queue_timer_malloc(snd_seq_queue_timer_t **ptr)
{
	return -ENXIO;
}
static inline
void snd_seq_queue_timer_free(snd_seq_queue_timer_t *ptr)
{
}
static inline
void snd_seq_queue_timer_copy(snd_seq_queue_timer_t *dst,
			      const snd_seq_queue_timer_t *src)
{
}

static inline
int snd_seq_queue_timer_get_queue(const snd_seq_queue_timer_t *info)
{
	return 0;
}
static inline
snd_seq_queue_timer_type_t snd_seq_queue_timer_get_type(const snd_seq_queue_timer_t *info)
{
	return SND_SEQ_TIMER_ALSA;
}
static inline
const snd_timer_id_t *snd_seq_queue_timer_get_id(const snd_seq_queue_timer_t *info)
{
	return NULL;
}
static inline
unsigned int snd_seq_queue_timer_get_resolution(const snd_seq_queue_timer_t *info)
{
	return 0;
}

static inline
void snd_seq_queue_timer_set_type(snd_seq_queue_timer_t *info,
				  snd_seq_queue_timer_type_t type)
{
}
static inline
void snd_seq_queue_timer_set_id(snd_seq_queue_timer_t *info,
				const snd_timer_id_t *id)
{
}
static inline
void snd_seq_queue_timer_set_resolution(snd_seq_queue_timer_t *info,
					unsigned int resolution)
{
}

static inline
int snd_seq_get_queue_timer(snd_seq_t *handle, int q,
			    snd_seq_queue_timer_t *timer)
{
	return -ENXIO;
}
static inline
int snd_seq_set_queue_timer(snd_seq_t *handle, int q,
			    snd_seq_queue_timer_t *timer)
{
	return -ENXIO;
}


static inline
int snd_seq_free_event(snd_seq_event_t *ev)
{
	return 0;
}
static inline
ssize_t snd_seq_event_length(snd_seq_event_t *ev)
{
	return 0;
}
static inline
int snd_seq_event_output(snd_seq_t *handle, snd_seq_event_t *ev)
{
	return -ENXIO;
}
static inline
int snd_seq_event_output_buffer(snd_seq_t *handle, snd_seq_event_t *ev)
{
	return -ENXIO;
}
static inline
int snd_seq_event_output_direct(snd_seq_t *handle, snd_seq_event_t *ev)
{
	return -ENXIO;
}
static inline
int snd_seq_event_input(snd_seq_t *handle, snd_seq_event_t **ev)
{
	return -ENXIO;
}
static inline
int snd_seq_event_input_pending(snd_seq_t *seq, int fetch_sequencer)
{
	return -ENXIO;
}
static inline
int snd_seq_drain_output(snd_seq_t *handle)
{
	return -ENXIO;
}
static inline
int snd_seq_event_output_pending(snd_seq_t *seq)
{
	return -ENXIO;
}
static inline
int snd_seq_extract_output(snd_seq_t *handle, snd_seq_event_t **ev)
{
	return -ENXIO;
}
static inline
int snd_seq_drop_output(snd_seq_t *handle)
{
	return -ENXIO;
}
static inline
int snd_seq_drop_output_buffer(snd_seq_t *handle)
{
	return -ENXIO;
}
static inline
int snd_seq_drop_input(snd_seq_t *handle)
{
	return -ENXIO;
}
static inline
int snd_seq_drop_input_buffer(snd_seq_t *handle)
{
	return -ENXIO;
}

/** event removal conditionals */
typedef struct _snd_seq_remove_events snd_seq_remove_events_t;

/** Remove conditional flags */
#define SND_SEQ_REMOVE_INPUT		(1<<0)	/**< Flush input queues */
#define SND_SEQ_REMOVE_OUTPUT		(1<<1)	/**< Flush output queues */
#define SND_SEQ_REMOVE_DEST		(1<<2)	/**< Restrict by destination q:client:port */
#define SND_SEQ_REMOVE_DEST_CHANNEL	(1<<3)	/**< Restrict by channel */
#define SND_SEQ_REMOVE_TIME_BEFORE	(1<<4)	/**< Restrict to before time */
#define SND_SEQ_REMOVE_TIME_AFTER	(1<<5)	/**< Restrict to time or after */
#define SND_SEQ_REMOVE_TIME_TICK	(1<<6)	/**< Time is in ticks */
#define SND_SEQ_REMOVE_EVENT_TYPE	(1<<7)	/**< Restrict to event type */
#define SND_SEQ_REMOVE_IGNORE_OFF 	(1<<8)	/**< Do not flush off events */
#define SND_SEQ_REMOVE_TAG_MATCH 	(1<<9)	/**< Restrict to events with given tag */

static inline
size_t snd_seq_remove_events_sizeof(void)
{
	return 0;
}
/** allocate a #snd_seq_remove_events_t container on stack */
#define snd_seq_remove_events_alloca(ptr)

static inline
int snd_seq_remove_events_malloc(snd_seq_remove_events_t **ptr)
{
	return -ENXIO;
}
static inline
void snd_seq_remove_events_free(snd_seq_remove_events_t *ptr)
{
}
static inline
void snd_seq_remove_events_copy(snd_seq_remove_events_t *dst,
				const snd_seq_remove_events_t *src)
{
}

static inline
unsigned int snd_seq_remove_events_get_condition(const snd_seq_remove_events_t *info)
{
	return 0;
}
static inline
int snd_seq_remove_events_get_queue(const snd_seq_remove_events_t *info)
{
	return 0;
}
static inline
const snd_seq_timestamp_t *snd_seq_remove_events_get_time(const snd_seq_remove_events_t *info)
{
	return NULL;
}
static inline
const snd_seq_addr_t *snd_seq_remove_events_get_dest(const snd_seq_remove_events_t *info)
{
	return NULL;
}
static inline
int snd_seq_remove_events_get_channel(const snd_seq_remove_events_t *info)
{
	return 0;
}
static inline
int snd_seq_remove_events_get_event_type(const snd_seq_remove_events_t *info)
{
	return 0;
}
static inline
int snd_seq_remove_events_get_tag(const snd_seq_remove_events_t *info)
{
	return 0;
}

static inline
void snd_seq_remove_events_set_condition(snd_seq_remove_events_t *info,
					 unsigned int flags)
{
}
static inline
void snd_seq_remove_events_set_queue(snd_seq_remove_events_t *info, int queue)
{
}
static inline
void snd_seq_remove_events_set_time(snd_seq_remove_events_t *info,
				    const snd_seq_timestamp_t *time)
{
}
static inline
void snd_seq_remove_events_set_dest(snd_seq_remove_events_t *info,
				    const snd_seq_addr_t *addr)
{
}
static inline
void snd_seq_remove_events_set_channel(snd_seq_remove_events_t *info,
				       int channel)
{
}
static inline
void snd_seq_remove_events_set_event_type(snd_seq_remove_events_t *info,
					  int type)
{
}
static inline
void snd_seq_remove_events_set_tag(snd_seq_remove_events_t *info, int tag)
{
}

static inline
int snd_seq_remove_events(snd_seq_t *handle, snd_seq_remove_events_t *info)
{
	return -ENXIO;
}


static inline
void snd_seq_set_bit(int nr, void *array)
{
}
static inline
int snd_seq_change_bit(int nr, void *array)
{
	return 0;
}
static inline
int snd_seq_get_bit(int nr, void *array)
{
	return 0;
}



/* event type macros */
enum {
	SND_SEQ_EVFLG_RESULT,
	SND_SEQ_EVFLG_NOTE,
	SND_SEQ_EVFLG_CONTROL,
	SND_SEQ_EVFLG_QUEUE,
	SND_SEQ_EVFLG_SYSTEM,
	SND_SEQ_EVFLG_MESSAGE,
	SND_SEQ_EVFLG_CONNECTION,
	SND_SEQ_EVFLG_SAMPLE,
	SND_SEQ_EVFLG_USERS,
	SND_SEQ_EVFLG_INSTR,
	SND_SEQ_EVFLG_QUOTE,
	SND_SEQ_EVFLG_NONE,
	SND_SEQ_EVFLG_RAW,
	SND_SEQ_EVFLG_FIXED,
	SND_SEQ_EVFLG_VARIABLE,
	SND_SEQ_EVFLG_VARUSR
};

enum {
	SND_SEQ_EVFLG_NOTE_ONEARG,
	SND_SEQ_EVFLG_NOTE_TWOARG
};

enum {
	SND_SEQ_EVFLG_QUEUE_NOARG,
	SND_SEQ_EVFLG_QUEUE_TICK,
	SND_SEQ_EVFLG_QUEUE_TIME,
	SND_SEQ_EVFLG_QUEUE_VALUE
};

#define snd_seq_type_check(ev,x)	0

#define snd_seq_ev_is_result_type(ev)	0
#define snd_seq_ev_is_note_type(ev)	0
#define snd_seq_ev_is_control_type(ev)	0
#define snd_seq_ev_is_channel_type(ev)	0

#define snd_seq_ev_is_queue_type(ev)	0
#define snd_seq_ev_is_message_type(ev)	0
#define snd_seq_ev_is_subscribe_type(ev) 0
#define snd_seq_ev_is_sample_type(ev)	0
#define snd_seq_ev_is_user_type(ev)	0
#define snd_seq_ev_is_instr_type(ev)	0
#define snd_seq_ev_is_fixed_type(ev)	0
#define snd_seq_ev_is_variable_type(ev)	0
#define snd_seq_ev_is_varusr_type(ev)	0
#define snd_seq_ev_is_reserved(ev)	0

#define snd_seq_ev_is_prior(ev)		0
#define snd_seq_ev_length_type(ev)	0
#define snd_seq_ev_is_fixed(ev)		0
#define snd_seq_ev_is_variable(ev)	0
#define snd_seq_ev_is_varusr(ev)	0
#define snd_seq_ev_timestamp_type(ev)	0
#define snd_seq_ev_is_tick(ev)		0
#define snd_seq_ev_is_real(ev)		0

#define snd_seq_ev_timemode_type(ev)	0
#define snd_seq_ev_is_abstime(ev)	0
#define snd_seq_ev_is_reltime(ev)	0
#define snd_seq_ev_is_direct(ev)	0

#endif /* __ALSA_SEQ_H */
