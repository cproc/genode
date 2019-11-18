/*
 * \brief  Linux emulation C helper functions
 * \author Stefan Kalkowski
 * \date   2016-03-22
 */

/*
 * Copyright (C) 2016-2017 Genode Labs GmbH
 *
 * This file is distributed under the terms of the GNU General Public License
 * version 2.
 */


/* Genode includes */
#include <lx_emul.h>
#include <lx_emul_c.h>

#if 0
#include <../drivers/gpu/drm/i915/i915_drv.h>
#include <../drivers/gpu/drm/i915/intel_drv.h>
#include <drm/drm_atomic_helper.h>

extern struct drm_framebuffer *
lx_c_intel_framebuffer_create(struct drm_device *dev,
                         struct drm_mode_fb_cmd2 *mode_cmd,
                         struct drm_i915_gem_object *obj);
#endif

#include <drm/drm_encoder.h>
#include <drm/drm_gem_cma_helper.h>
#include <drm/drm_modeset_helper.h>

void lx_c_allocate_framebuffer(struct drm_device * dev,
                               struct lx_c_fb_config *c)
{
	/* from drm_fbdev_cma_create() */

	struct drm_gem_cma_object *obj;

	c->pitch = roundup(c->width * c->bpp, 64);
	c->size  = roundup(c->pitch * c->height, PAGE_SIZE);

lx_printf("c->width: %d, c->height: %d, c->bpp: %d, c->pitch: %d, c->size: %d\n", c->width, c->height, c->bpp, c->pitch, c->size);

	obj = drm_gem_cma_create(dev, c->size);

	if (obj == NULL)
		return;

	c->addr = obj->vaddr;

	/* from drm_gem_fb_alloc() */

	struct drm_framebuffer *fb = kzalloc(sizeof(*fb), GFP_KERNEL);

	if (fb == NULL)
		goto err;

	struct drm_mode_fb_cmd2 mode_cmd = { 0 };

	mode_cmd.width = c->width;
	mode_cmd.height = c->height;
	mode_cmd.pitches[0] = c->pitch;
	mode_cmd.pixel_format = /*DRM_FORMAT_RGB565*/DRM_FORMAT_RGBA8888;

	drm_helper_mode_fill_fb_struct(dev, fb, &mode_cmd);

	fb->obj[0] = &obj->base;

	if (drm_framebuffer_init(dev, fb, NULL) != 0) {
		kfree(fb);
		goto err;
	}

	c->lx_fb = fb;

	memset_io(c->addr, 0, c->size);

	return;

err:
	drm_gem_object_put_unlocked(&obj->base); /* as in drm_gem_cma_create() */
	return;

#if 0
	struct drm_i915_private *dev_priv = dev->dev_private; /* intelfb_create() */
	struct drm_mode_fb_cmd2 * r;                          /* intelfb_alloc()  */
	struct drm_i915_gem_object * obj = NULL;              /* intelfb_alloc()  */

	mutex_lock(&dev->struct_mutex);                       /* intelfb_create() */

	/* for linear buffers the pitch needs to be 64 byte aligned */
	c->pitch = roundup(c->width * c->bpp, 64);            /* intelfb_alloc() */
	c->size  = roundup(c->pitch * c->height, PAGE_SIZE);  /* intelfb_alloc() */

	obj = i915_gem_object_create_stolen(dev_priv, c->size); /* intelfb_alloc() */

	if (obj == NULL)
		obj = i915_gem_object_create(dev_priv, c->size);    /* intelfb_alloc() */

	if (obj == NULL) goto out2;

	r = (struct drm_mode_fb_cmd2*) kzalloc(sizeof(struct drm_mode_fb_cmd2), 0); /* intelfb_alloc() */
	if (!r) goto err2;                                                          /* intelfb_alloc() */ 

	r->width        = c->width;                   /* intelfb_alloc() */
	r->height       = c->height;                  /* intelfb_alloc() */
	r->pixel_format = DRM_FORMAT_RGB565;          /* intelfb_alloc() */
	r->pitches[0]   = c->pitch;                   /* intelfb_alloc() */
	c->lx_fb = intel_framebuffer_create(obj, r);  /* intelfb_alloc() */
	if (IS_ERR(c->lx_fb)) goto err2;              /* intelfb_alloc() */

	/* XXX rotation info missing */
	struct i915_vma * vma = intel_pin_and_fence_fb_obj(c->lx_fb, DRM_MODE_ROTATE_0); /* intelfb_create() */
	if (IS_ERR(vma))                                                                 /* intelfb_create() */
		goto err1;                                                                   /* intelfb_create() */

	c->addr = ioremap_wc(dev_priv->ggtt.gmadr.start + i915_ggtt_offset(vma),         /* intelfb_create() */
	                     c->size);                                                   /* intelfb_create() */

	memset_io(c->addr, 0, c->size);

	/* intel_framebuffer_create inc ref, so dec since obj ptr is dropped now */
	i915_gem_object_put(obj);

	goto out1;

err1:
	DRM_ERROR("could not allocate framebuffer %ld", (long)vma);
	drm_framebuffer_remove(c->lx_fb);
err2:
	c->lx_fb = NULL;
	i915_gem_object_put(obj);
out1:
	kfree(r);
out2:
	mutex_unlock(&dev->struct_mutex);
#endif
}


void lx_c_set_mode(struct drm_device * dev, struct drm_connector * connector,
                   struct drm_framebuffer *fb, struct drm_display_mode *mode)
{
	struct drm_crtc        * crtc    = NULL;
	struct drm_encoder     * encoder = connector->encoder;
lx_printf("lx_c_set_mode()\n");
	if (!encoder) {
		struct drm_encoder *enc;
		list_for_each_entry(enc, &dev->mode_config.encoder_list, head) {
			unsigned i;
			for (i = 0; i < DRM_CONNECTOR_MAX_ENCODER; i++)
				if (connector->encoder_ids[i] == enc->base.id) break;

			if (i == DRM_CONNECTOR_MAX_ENCODER) continue;

			bool used = false;
			struct drm_connector *c;
			list_for_each_entry(c, &dev->mode_config.connector_list, head) {
				if (c->encoder == enc) used = true;
			}
			if (used) continue;
			encoder = enc;
			break;
		}
	}

	if (!encoder) {
		lx_printf("Found no encoder for the connector %s\n", connector->name);
		return;
	}
lx_printf("lx_c_set_mode(): check 1\n");
	unsigned used_crtc = 0;

	crtc = encoder->crtc;
	if (!crtc) {
		unsigned i = 0;
		struct drm_crtc *c;
		list_for_each_entry(c, &dev->mode_config.crtc_list, head) {
			if (!(encoder->possible_crtcs & (1 << i))) continue;
			if (c->state->enable) {
				used_crtc ++;
				continue;
			}
			crtc = c;
			break;
		}
	}

	if (!crtc) {
		if (mode)
			lx_printf("Found no crtc for the connector %s used/max %u+1/%u\n",
			          connector->name, used_crtc, dev->mode_config.num_crtc);
		return;
	}
lx_printf("lx_c_set_mode(): check 2\n");

	/*DRM_DEBUG*/lx_printf("%s%s for connector %s\n", mode ? "set mode " : "no mode",
	          mode ? mode->name : "", connector->name);

	struct drm_mode_set set;
	set.crtc = crtc;
	set.x = 0;
	set.y = 0;
	set.mode = mode;
	set.connectors = &connector;
	set.num_connectors = mode ? 1 : 0;
	set.fb = mode ? fb : 0;

	uint32_t const ref_cnt_before = drm_framebuffer_read_refcount(fb);
lx_printf("lx_c_set_mode(): check 3\n");
	int ret = drm_atomic_helper_set_config(&set, dev->mode_config.acquire_ctx);
lx_printf("lx_c_set_mode(): check 4\n");
	if (ret)
		lx_printf("Error: set config failed ret=%d refcnt before=%u after=%u\n",
		          ret, ref_cnt_before, drm_framebuffer_read_refcount(fb));
}
#if 0
void lx_c_set_driver(struct drm_device * dev, void * driver)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	ASSERT(!dev_priv->audio_component);
	dev_priv->audio_component = (struct i915_audio_component *) driver;
}
#endif

void* lx_c_get_driver(struct drm_device * dev)
{
	TRACE_AND_STOP;
#if 0
	struct drm_i915_private *dev_priv = dev->dev_private;
	return (void*) dev_priv->audio_component;
#endif
}

#if 0
void lx_c_set_brightness(struct drm_connector * const connector,
                         unsigned const bn_set, unsigned const bn_max)
{
	struct intel_connector * const c = to_intel_connector(connector);

	intel_panel_set_backlight_acpi(c->base.state, bn_set, bn_max);
}

unsigned lx_c_get_brightness(struct drm_connector * const connector, unsigned error)
{
	if (!connector)
		return error;

	struct intel_connector * const intel_c = to_intel_connector(connector);
	if (!intel_c)
		return error;

	struct intel_panel *panel = &intel_c->panel;

	if (!panel || !panel->backlight.device || !panel->backlight.device->ops ||
	    !panel->backlight.device->ops->get_brightness)
		return error;

	panel->backlight.device->connector = intel_c;
	unsigned ret = panel->backlight.device->ops->get_brightness(panel->backlight.device);
	panel->backlight.device->connector = NULL;

	return ret;
}
#endif
