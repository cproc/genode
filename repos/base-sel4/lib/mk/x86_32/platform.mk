#
# Create prerequisites for building Genode for seL4
#
# Prior building Genode programs for seL4, the kernel bindings needed are
# symlinked to the build directory.
#

#
# Execute the rules in this file only at the second build stage when we know
# about the complete build settings, e.g., the 'CROSS_DEV_PREFIX'.
#
ifeq ($(called_from_lib_mk),yes)

LIBSEL4_DIR := $(call select_from_ports,sel4)/src/kernel/sel4/libsel4

#
# Make seL4 kernel API headers available to the Genode build system
#
# We have to create symbolic links of the content of seL4's 'include/sel4' and
# 'include_arch/<arch>/sel4' directories into our local build directory.
#

SEL4_ARCH_INCLUDES := objecttype.h types.h bootinfo.h constants.h functions.h \
                      pfIPC.h syscalls.h exIPC.h

SEL4_INCLUDES := objecttype.h types.h bootinfo.h errors.h constants.h \
                 messages.h sel4.h benchmark.h types.bf macros.h \
                 types_gen.h syscall.h

SEL4_INCLUDE_SYMLINKS += $(addprefix $(BUILD_BASE_DIR)/include/sel4/,     $(SEL4_INCLUDES))
SEL4_INCLUDE_SYMLINKS += $(addprefix $(BUILD_BASE_DIR)/include/sel4/arch/,$(SEL4_ARCH_INCLUDES))

SEL4_INCLUDE_DIRS = $(sort $(dir $(SEL4_INCLUDE_SYMLINKS)))

all: $(SEL4_INCLUDE_SYMLINKS)

$(SEL4_INCLUDE_SYMLINKS): $(SEL4_INCLUDE_DIRS)

$(SEL4_INCLUDE_DIRS):
	$(VERBOSE)mkdir -p $@

#
# Plain symlinks to existing headers
#
$(BUILD_BASE_DIR)/include/sel4/arch/%.h: $(LIBSEL4_DIR)/arch_include/ia32/sel4/arch/%.h
	$(VERBOSE)ln -sf $< $@

$(BUILD_BASE_DIR)/include/sel4/%.h: $(LIBSEL4_DIR)/include/sel4/%.h
	$(VERBOSE)ln -sf $< $@

#
# Generated headers
#
$(BUILD_BASE_DIR)/include/sel4/types_gen.h: $(LIBSEL4_DIR)/include/sel4/types.bf
	$(MSG_CONVERT)$(notdir $@)
	$(VERBOSE)python $(LIBSEL4_DIR)/tools/bitfield_gen.py \
	                 --environment libsel4 "$<" $@

$(BUILD_BASE_DIR)/include/sel4/syscall.h: $(LIBSEL4_DIR)/include/api/syscall.xml
	$(MSG_CONVERT)$(notdir $@)
	$(VERBOSE)python $(LIBSEL4_DIR)/tools/syscall_header_gen.py \
	                 --xml $< --libsel4_header $@


endif
