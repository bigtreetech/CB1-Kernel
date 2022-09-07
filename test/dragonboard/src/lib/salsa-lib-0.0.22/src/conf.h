/*
 * Dummy snd_conf_* definitions
 */

#ifndef __ALSA_CONF_H
#define __ALSA_CONF_H

/** Configuration node type. */
typedef enum _snd_config_type {
	/** Integer number. */
        SND_CONFIG_TYPE_INTEGER,
	/** 64 bit Integer number. */
        SND_CONFIG_TYPE_INTEGER64,
	/** Real number. */
        SND_CONFIG_TYPE_REAL,
	/** Character string. */
        SND_CONFIG_TYPE_STRING,
        /** Pointer (runtime only, cannot be saved). */
        SND_CONFIG_TYPE_POINTER,
	/** Compound node. */
	SND_CONFIG_TYPE_COMPOUND = 1024
} snd_config_type_t;

typedef struct _snd_config_iterator *snd_config_iterator_t;
typedef struct _snd_config_update snd_config_update_t;

extern snd_config_t *snd_config;

static inline __SALSA_NOT_IMPLEMENTED
int snd_config_top(snd_config_t **config)
{
	return -ENXIO;
}

static inline __SALSA_NOT_IMPLEMENTED
int snd_config_load(snd_config_t *config, snd_input_t *in)
{
	return -ENXIO;
}

static inline __SALSA_NOT_IMPLEMENTED
int snd_config_load_override(snd_config_t *config, snd_input_t *in)
{
	return -ENXIO;
}

static inline __SALSA_NOT_IMPLEMENTED
int snd_config_save(snd_config_t *config, snd_output_t *out)
{
	return -ENXIO;
}

static inline
int snd_config_update(void)
{
	/* an invalid address, but just mark to be non-NULL */
	snd_config = (snd_config_t*)1;
	return 0;
}

static inline
int snd_config_update_r(snd_config_t **top, snd_config_update_t **update,
			const char *path)
{
	return 0;
}

static inline
int snd_config_update_free(snd_config_update_t *update)
{
	return 0;
}

#if SALSA_HAS_DUMMY_CONF
static inline
int snd_config_update_free_global(void)
{
	snd_config = NULL;
	return 0;
}
#endif

static inline __SALSA_NOT_IMPLEMENTED
int snd_config_search(snd_config_t *config, const char *key,
		      snd_config_t **result)
{
	return -ENXIO;
}

static inline __SALSA_NOT_IMPLEMENTED
int snd_config_searchv(snd_config_t *config,
		       snd_config_t **result, ...)
{
	return -ENXIO;
}

static inline __SALSA_NOT_IMPLEMENTED
int snd_config_search_definition(snd_config_t *config,
				 const char *base, const char *key,
				 snd_config_t **result)
{
	return -ENXIO;
}

static inline __SALSA_NOT_IMPLEMENTED
int snd_config_expand(snd_config_t *config, snd_config_t *root,
		      const char *args, snd_config_t *private_data,
		      snd_config_t **result)
{
	return -ENXIO;
}

static inline __SALSA_NOT_IMPLEMENTED
int snd_config_evaluate(snd_config_t *config, snd_config_t *root,
			snd_config_t *private_data, snd_config_t **result)
{
	return -ENXIO;
}

static inline __SALSA_NOT_IMPLEMENTED
int snd_config_add(snd_config_t *config, snd_config_t *leaf)
{
	return -ENXIO;
}

static inline __SALSA_NOT_IMPLEMENTED
int snd_config_delete(snd_config_t *config)
{
	return -ENXIO;
}

static inline __SALSA_NOT_IMPLEMENTED
int snd_config_delete_compound_members(const snd_config_t *config)
{
	return -ENXIO;
}

static inline __SALSA_NOT_IMPLEMENTED
int snd_config_copy(snd_config_t **dst, snd_config_t *src)
{
	return -ENXIO;
}


static inline
int snd_config_make(snd_config_t **config, const char *key,
		    snd_config_type_t type)
{
	return -ENXIO;
}

static inline
int snd_config_make_integer(snd_config_t **config, const char *key)
{
	return -ENXIO;
}

static inline
int snd_config_make_integer64(snd_config_t **config, const char *key)
{
	return -ENXIO;
}

static inline
int snd_config_make_real(snd_config_t **config, const char *key)
{
	return -ENXIO;
}

static inline
int snd_config_make_string(snd_config_t **config, const char *key)
{
	return -ENXIO;
}

static inline
int snd_config_make_pointer(snd_config_t **config, const char *key)
{
	return -ENXIO;
}

static inline
int snd_config_make_compound(snd_config_t **config, const char *key, int join)
{
	return -ENXIO;
}


static inline
int snd_config_imake_integer(snd_config_t **config, const char *key,
			     const long value)
{
	return -ENXIO;
}

static inline
int snd_config_imake_integer64(snd_config_t **config, const char *key,
			       const long long value)
{
	return -ENXIO;
}

static inline
int snd_config_imake_real(snd_config_t **config, const char *key,
			  const double value)
{
	return -ENXIO;
}

static inline
int snd_config_imake_string(snd_config_t **config, const char *key,
			    const char *ascii)
{
	return -ENXIO;
}

static inline
int snd_config_imake_pointer(snd_config_t **config, const char *key,
			     const void *ptr)
{
	return -ENXIO;
}

static inline
snd_config_type_t snd_config_get_type(const snd_config_t *config)
{
	return SND_CONFIG_TYPE_INTEGER;
}

static inline
int snd_config_set_id(snd_config_t *config, const char *id)
{
	return -ENXIO;
}

static inline
int snd_config_set_integer(snd_config_t *config, long value)
{
	return -ENXIO;
}

static inline
int snd_config_set_integer64(snd_config_t *config, long long value)
{
	return -ENXIO;
}

static inline
int snd_config_set_real(snd_config_t *config, double value)
{
	return -ENXIO;
}

static inline
int snd_config_set_string(snd_config_t *config, const char *value)
{
	return -ENXIO;
}

static inline
int snd_config_set_ascii(snd_config_t *config, const char *ascii)
{
	return -ENXIO;
}

static inline
int snd_config_set_pointer(snd_config_t *config, const void *ptr)
{
	return -ENXIO;
}

static inline
int snd_config_get_id(const snd_config_t *config, const char **value)
{
	return -ENXIO;
}

static inline
int snd_config_get_integer(const snd_config_t *config, long *value)
{
	return -ENXIO;
}

static inline
int snd_config_get_integer64(const snd_config_t *config, long long *value)
{
	return -ENXIO;
}

static inline
int snd_config_get_real(const snd_config_t *config, double *value)
{
	return -ENXIO;
}

static inline
int snd_config_get_ireal(const snd_config_t *config, double *value)
{
	return -ENXIO;
}

static inline
int snd_config_get_string(const snd_config_t *config, const char **value)
{
	return -ENXIO;
}

static inline
int snd_config_get_ascii(const snd_config_t *config, char **value)
{
	return -ENXIO;
}

static inline
int snd_config_get_pointer(const snd_config_t *config, const void **value)
{
	return -ENXIO;
}

static inline
int snd_config_test_id(const snd_config_t *config, const char *id)
{
	return -ENXIO;
}

static inline
snd_config_iterator_t snd_config_iterator_first(const snd_config_t *node)
{
	return NULL;
}

static inline
snd_config_iterator_t snd_config_iterator_next(const snd_config_iterator_t iterator)
{
	return NULL;
}

static inline
snd_config_iterator_t snd_config_iterator_end(const snd_config_t *node)
{
	return NULL;
}

static inline
snd_config_t *snd_config_iterator_entry(const snd_config_iterator_t iterator)
{
	return NULL;
}


/**
 * \brief Helper macro to iterate over the children of a compound node.
 * \param pos Iterator variable for the current node.
 * \param next Iterator variable for the next node.
 * \param node Handle to the compound configuration node to iterate over.
 *
 * This macro is designed to permit the removal of the current node.
 */
#define snd_config_for_each(pos, next, node) \
	for (pos = snd_config_iterator_first(node), next = snd_config_iterator_next(pos); pos != snd_config_iterator_end(node); pos = next, next = snd_config_iterator_next(pos))

/* Misc functions */

static inline
int snd_config_get_bool_ascii(const char *ascii)
{
	return -ENXIO;
}

static inline
int snd_config_get_bool(const snd_config_t *conf)
{
	return -ENXIO;
}

static inline
int snd_config_get_ctl_iface_ascii(const char *ascii)
{
	return -ENXIO;
}

static inline
int snd_config_get_ctl_iface(const snd_config_t *conf)
{
	return -ENXIO;
}


/* Names functions */

typedef struct snd_devname snd_devname_t;

struct snd_devname {
	char *name;	/**< Device name string */
	char *comment;	/**< Comments */
	snd_devname_t *next;	/**< Next pointer */
};

static inline __SALSA_NOT_IMPLEMENTED
int snd_names_list(const char *iface, snd_devname_t **list)
{
	return -ENXIO;
}

static inline __SALSA_NOT_IMPLEMENTED
void snd_names_list_free(snd_devname_t *list)
{
}


#endif /* __ALSA_CONF_H */
