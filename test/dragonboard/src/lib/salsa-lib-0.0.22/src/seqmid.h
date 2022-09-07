#ifndef __ALSA_SEQMID_H
#define __ALSA_SEQMID_H

#define snd_seq_ev_clear(ev)
#define snd_seq_ev_set_tag(ev,t)
#define snd_seq_ev_set_dest(ev,c,p)
#define snd_seq_ev_set_subs(ev)
#define snd_seq_ev_set_broadcast(ev)
#define snd_seq_ev_set_source(ev,p)
#define snd_seq_ev_set_direct(ev)
#define snd_seq_ev_schedule_tick(ev, q, relative, ttick)
#define snd_seq_ev_schedule_real(ev, q, relative, rtime)
#define snd_seq_ev_set_priority(ev, high_prior)
#define snd_seq_ev_set_fixed(ev)
#define snd_seq_ev_set_variable(ev, datalen, dataptr)
#define snd_seq_ev_set_varusr(ev, datalen, dataptr)
#define snd_seq_ev_set_queue_control(ev, typ, q, val)
#define snd_seq_ev_set_queue_start(ev, q)
#define snd_seq_ev_set_queue_stop(ev, q)
#define snd_seq_ev_set_queue_continue(ev, q)
#define snd_seq_ev_set_queue_tempo(ev, q, val)
#define snd_seq_ev_set_queue_pos_real(ev, q, rtime)
#define snd_seq_ev_set_queue_pos_tick(ev, q, ttime)

static inline
int snd_seq_control_queue(snd_seq_t *seq, int q, int type, int value,
			  snd_seq_event_t *ev)
{
	return -ENXIO;
}
#define snd_seq_start_queue(seq, q, ev) \
	snd_seq_control_queue(seq, q, SND_SEQ_EVENT_START, 0, ev)
#define snd_seq_stop_queue(seq, q, ev) \
	snd_seq_control_queue(seq, q, SND_SEQ_EVENT_STOP, 0, ev)
#define snd_seq_continue_queue(seq, q, ev) \
	snd_seq_control_queue(seq, q, SND_SEQ_EVENT_CONTINUE, 0, ev)
#define snd_seq_change_queue_tempo(seq, q, tempo, ev) \
	snd_seq_control_queue(seq, q, SND_SEQ_EVENT_TEMPO, tempo, ev)

static inline
int snd_seq_create_simple_port(snd_seq_t *seq, const char *name,
			       unsigned int caps, unsigned int type)
{
	return -ENXIO;
}
static inline
int snd_seq_delete_simple_port(snd_seq_t *seq, int port)
{
	return 0;
}

static inline
int snd_seq_connect_from(snd_seq_t *seq, int my_port, int src_client,
			 int src_port)
{
	return -ENXIO;
}
static inline
int snd_seq_connect_to(snd_seq_t *seq, int my_port, int dest_client,
		       int dest_port)
{
	return -ENXIO;
}
static inline
int snd_seq_disconnect_from(snd_seq_t *seq, int my_port, int src_client,
			    int src_port)
{
	return -ENXIO;
}
static inline
int snd_seq_disconnect_to(snd_seq_t *seq, int my_port, int dest_client,
			  int dest_port)
{
	return -ENXIO;
}

static inline
int snd_seq_set_client_name(snd_seq_t *seq, const char *name)
{
	return -ENXIO;
}
static inline
int snd_seq_set_client_event_filter(snd_seq_t *seq, int event_type)
{
	return -ENXIO;
}
static inline
int snd_seq_set_client_pool_output(snd_seq_t *seq, size_t size)
{
	return -ENXIO;
}
static inline
int snd_seq_set_client_pool_output_room(snd_seq_t *seq, size_t size)
{
	return -ENXIO;
}
static inline
int snd_seq_set_client_pool_input(snd_seq_t *seq, size_t size)
{
	return -ENXIO;
}
static inline
int snd_seq_sync_output_queue(snd_seq_t *seq)
{
	return -ENXIO;
}

static inline
int snd_seq_parse_address(snd_seq_t *seq, snd_seq_addr_t *addr, const char *str)
{
	return -ENXIO;
}

static inline
int snd_seq_reset_pool_output(snd_seq_t *seq)
{
	return -ENXIO;
}
static inline
int snd_seq_reset_pool_input(snd_seq_t *seq)
{
	return -ENXIO;
}

#define snd_seq_ev_set_note(ev, ch, key, vel, dur)
#define snd_seq_ev_set_noteon(ev, ch, key, vel)
#define snd_seq_ev_set_noteoff(ev, ch, key, vel)
#define snd_seq_ev_set_keypress(ev,ch,key,vel)
#define snd_seq_ev_set_controller(ev,ch,cc,val)
#define snd_seq_ev_set_pgmchange(ev,ch,val)
#define snd_seq_ev_set_pitchbend(ev,ch,val)
#define snd_seq_ev_set_chanpress(ev,ch,val)
#define snd_seq_ev_set_sysex(ev,datalen,dataptr)

#endif /* __ALSA_SEQMID_H */
