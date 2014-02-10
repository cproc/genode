include ports/mesa-demos.inc

MESA_DEMOS_TBZ2    = $(MESA_DEMOS).tar.bz2
MESA_DEMOS_URL     = ftp://ftp.freedesktop.org/pub/mesa/demos/$(MESA_DEMOS_VERSION)/$(MESA_DEMOS_TBZ2)

#
# Interface to top-level prepare Makefile
#
PORTS += $(MESA_DEMOS)

prepare-mesa-demos: $(CONTRIB_DIR)/$(MESA_DEMOS) include/eglut/eglut.h

$(CONTRIB_DIR)/$(MESA_DEMOS): clean-mesa-demos

#
# Port-specific local rules
#
$(DOWNLOAD_DIR)/$(MESA_DEMOS_TBZ2):
	$(VERBOSE)wget -c -P $(DOWNLOAD_DIR) $(MESA_DEMOS_URL) && touch $@

$(CONTRIB_DIR)/$(MESA_DEMOS): $(DOWNLOAD_DIR)/$(MESA_DEMOS_TBZ2)
	$(VERBOSE)tar xfj $< -C $(CONTRIB_DIR)
	$(VERBOSE)patch -N -p1 -d $(CONTRIB_DIR)/$(MESA_DEMOS) < src/app/eglgears/eglgears.patch
	$(VERBOSE)patch -N -p1 -d $(CONTRIB_DIR)/$(MESA_DEMOS) < src/app/es1gears/es1gears.patch
	$(VERBOSE)patch -N -p1 -d $(CONTRIB_DIR)/$(MESA_DEMOS) < src/app/es2gears/es2gears.patch
	$(VERBOSE)touch $@

#
# Install eglut header
#
include/eglut/eglut.h:
	$(VERBOSE)mkdir -p $(dir $@)
	$(VERBOSE)ln -sf ../../$(CONTRIB_DIR)/$(MESA_DEMOS)/src/egl/eglut/eglut.h $@

clean-mesa-demos:
	$(VERBOSE)rm -rf include/eglut
	$(VERBOSE)rm -rf $(CONTRIB_DIR)/$(MESA_DEMOS)
