LIBDRM_VERSION = 2.4.52
LIBDRM         = libdrm-$(LIBDRM_VERSION)
LIBDRM_DIR     = libdrm-$(LIBDRM_VERSION)
LIBDRM_TBZ2    = $(LIBDRM).tar.bz2
LIBDRM_URL     = http://dri.freedesktop.org/libdrm/$(LIBDRM_TBZ2)

#
# Interface to top-level prepare Makefile
#
PORTS += $(LIBDRM)

prepare-libdrm: $(CONTRIB_DIR)/$(LIBDRM_DIR) include/libdrm

$(CONTRIB_DIR)/$(LIBDRM_DIR): clean-libdrm

#
# Port-specific local rules
#
$(DOWNLOAD_DIR)/$(LIBDRM_TBZ2):
	$(VERBOSE)wget -c -P $(DOWNLOAD_DIR) $(LIBDRM_URL) && touch $@

$(CONTRIB_DIR)/$(LIBDRM_DIR): $(DOWNLOAD_DIR)/$(LIBDRM_TBZ2)
	$(VERBOSE)tar xfj $< -C $(CONTRIB_DIR) && touch $@
	$(VERBOSE)patch -N -p1 -d $(CONTRIB_DIR)/$(LIBDRM_DIR) < src/lib/libdrm/libdrm.patch

#
# Install libdrm headers
#

DRM_INCLUDES := drm.h drm_mode.h i915_drm.h

include/libdrm:
	$(VERBOSE)mkdir -p $@
	$(VERBOSE)for i in $(DRM_INCLUDES); do \
		ln -sf ../../$(CONTRIB_DIR)/$(LIBDRM_DIR)/include/drm/$$i $@; done
	$(VERBOSE)ln -sf ../../$(CONTRIB_DIR)/$(LIBDRM_DIR)/intel/intel_bufmgr.h $@

clean-libdrm:
	$(VERBOSE)rm -rf include/libdrm
	$(VERBOSE)rm -rf $(CONTRIB_DIR)/$(LIBDRM_DIR)

