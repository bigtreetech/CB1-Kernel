#ifndef __LOCAL_H_INC
#define __LOCAL_H_INC

int _snd_dev_get_device(const char *name, int *cardp, int *devp, int *subdevp);
int _snd_ctl_hw_open(snd_ctl_t **ctlp, int card);

int _snd_pcm_mmap(snd_pcm_t *pcm);
int _snd_pcm_munmap(snd_pcm_t *pcm);

#endif /* __LOCAL_H_INC */
