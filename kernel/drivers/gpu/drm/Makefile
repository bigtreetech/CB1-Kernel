# SPDX-License-Identifier: GPL-2.0

# Makefile for the drm device driver.  This driver provides support for the
# Direct Rendering Infrastructure (DRI) in XFree86 4.1.0 and higher.

drm-y       :=	drm_aperture.o drm_auth.o drm_cache.o \
		drm_file.o drm_gem.o drm_ioctl.o drm_irq.o \
		drm_drv.o \
		drm_sysfs.o drm_hashtab.o drm_mm.o \
		drm_crtc.o drm_fourcc.o drm_modes.o drm_edid.o drm_displayid.o \
		drm_encoder_slave.o \
		drm_trace_points.o drm_prime.o \
		drm_rect.o drm_vma_manager.o drm_flip_work.o \
		drm_modeset_lock.o drm_atomic.o drm_bridge.o \
		drm_framebuffer.o drm_connector.o drm_blend.o \
		drm_encoder.o drm_mode_object.o drm_property.o \
		drm_plane.o drm_color_mgmt.o drm_print.o \
		drm_dumb_buffers.o drm_mode_config.o drm_vblank.o \
		drm_syncobj.o drm_lease.o drm_writeback.o drm_client.o \
		drm_client_modeset.o drm_atomic_uapi.o drm_hdcp.o \
		drm_managed.o drm_vblank_work.o

drm-$(CONFIG_COMPAT) += drm_ioc32.o
drm-$(CONFIG_DRM_GEM_CMA_HELPER) += drm_gem_cma_helper.o
drm-$(CONFIG_DRM_PANEL) += drm_panel.o
drm-$(CONFIG_OF) += drm_of.o
drm-$(CONFIG_DEBUG_FS) += drm_debugfs.o drm_debugfs_crc.o
drm-$(CONFIG_DRM_LOAD_EDID_FIRMWARE) += drm_edid_load.o

drm_kms_helper-y := drm_bridge_connector.o drm_crtc_helper.o \
		drm_dsc.o drm_probe_helper.o \
		drm_plane_helper.o drm_atomic_helper.o \
		drm_kms_helper_common.o \
		drm_simple_kms_helper.o drm_modeset_helper.o \
		drm_scdc_helper.o drm_gem_atomic_helper.o \
		drm_gem_framebuffer_helper.o \
		drm_atomic_state_helper.o drm_damage_helper.o \
		drm_format_helper.o drm_self_refresh_helper.o


drm_kms_helper-$(CONFIG_DRM_FBDEV_EMULATION) += drm_fb_helper.o
drm_kms_helper-$(CONFIG_DRM_KMS_CMA_HELPER) += drm_fb_cma_helper.o
drm-$(CONFIG_DRM_GEM_SHMEM_HELPER) += drm_gem_shmem_helper.o

obj-$(CONFIG_DRM_KMS_HELPER) += drm_kms_helper.o
obj-$(CONFIG_DRM_VGEM)	+= vgem/
obj-$(CONFIG_DRM_VKMS)	+= vkms/

obj-$(CONFIG_DRM)	+= drm.o

obj-$(CONFIG_DRM_PANEL_ORIENTATION_QUIRKS) += drm_panel_orientation_quirks.o
obj-$(CONFIG_DRM_SCHED)	+= scheduler/

obj-$(CONFIG_DRM_SUN4I) += sun4i/

obj-y			+= bridge/
obj-$(CONFIG_DRM_PANFROST) += panfrost/
