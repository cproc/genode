include ports/mesa10.inc

MESA_TGZ     = $(MESA).tar.gz
MESA_URL     = ftp://ftp.freedesktop.org/pub/mesa/10.1/$(MESA_TGZ)

#
# Interface to top-level prepare Makefile
#
# Register Mesa port as lower case to be consistent with the
# other libraries.
#
PORTS += mesa-$(MESA_VERSION)

MESA_INCLUDE_SYMLINKS = $(addprefix include/,EGL GL GLES GLES2 KHR)

#prepare-mesa10: $(CONTRIB_DIR)/$(MESA_DIR) tool/mesa/glsl $(MESA_INCLUDE_SYMLINKS)
prepare-mesa10: $(CONTRIB_DIR)/$(MESA_DIR) $(MESA_INCLUDE_SYMLINKS)

$(CONTRIB_DIR)/$(MESA_DIR): clean-mesa10

#
# Port-specific local rules
#
$(DOWNLOAD_DIR)/$(MESA_TGZ):
	$(VERBOSE)wget -c -P $(DOWNLOAD_DIR) $(MESA_URL) && touch $@

$(CONTRIB_DIR)/$(MESA_DIR): $(DOWNLOAD_DIR)/$(MESA_TGZ)
	$(VERBOSE)tar xfz $< -C $(CONTRIB_DIR)
	$(VERBOSE)patch -N -p1 -d $(CONTRIB_DIR)/$(MESA_DIR) < src/lib/glapi/glapi.patch
	$(VERBOSE)patch -N -p1 -d $(CONTRIB_DIR)/$(MESA_DIR) < src/lib/glesv2/glesv2.patch
	$(VERBOSE)patch -N -p1 -d $(CONTRIB_DIR)/$(MESA_DIR) < src/lib/egl/egl.patch
	$(VERBOSE)touch $@

tool/mesa/glsl:
	$(VERBOSE)make -C tool/mesa

$(MESA_INCLUDE_SYMLINKS):
	$(VERBOSE)ln -sf $(realpath $(CONTRIB_DIR)/$(MESA_DIR)/$@) $@ && touch $@

clean_tool_mesa:
	$(VERBOSE)make -C tool/mesa clean

clean_mesa_include_symlinks:
	$(VERBOSE)rm -f $(MESA_INCLUDE_SYMLINKS)

clean-mesa10: clean_tool_mesa clean_mesa_include_symlinks
	$(VERBOSE)rm -rf $(CONTRIB_DIR)/$(MESA_DIR)
