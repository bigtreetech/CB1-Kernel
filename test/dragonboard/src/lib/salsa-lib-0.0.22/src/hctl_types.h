#ifndef __ALSA_HCTL_TYPES_H
#define __ALSA_HCTL_TYPES_H

/** HCTL element handle */
typedef struct _snd_hctl_elem snd_hctl_elem_t;

/** HCTL handle */
typedef struct _snd_hctl snd_hctl_t;

/**
 * \brief Compare function for sorting HCTL elements
 * \param e1 First element
 * \param e2 Second element
 * \return -1 if e1 < e2, 0 if e1 == e2, 1 if e1 > e2
 */
typedef int (*snd_hctl_compare_t)(const snd_hctl_elem_t *e1,
				  const snd_hctl_elem_t *e2);
int snd_hctl_compare_fast(const snd_hctl_elem_t *c1,
			  const snd_hctl_elem_t *c2);
/**
 * \brief HCTL callback function
 * \param hctl HCTL handle
 * \param mask event mask
 * \param elem related HCTL element (if any)
 * \return 0 on success otherwise a negative error code
 */
typedef int (*snd_hctl_callback_t)(snd_hctl_t *hctl,
				   unsigned int mask,
				   snd_hctl_elem_t *elem);
/**
 * \brief HCTL element callback function
 * \param elem HCTL element
 * \param mask event mask
 * \return 0 on success otherwise a negative error code
 */
typedef int (*snd_hctl_elem_callback_t)(snd_hctl_elem_t *elem,
					unsigned int mask);

#endif /* __ALSA_HCTL_TYPES_H */
