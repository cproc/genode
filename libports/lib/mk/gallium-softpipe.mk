include $(REP_DIR)/lib/mk/mesa10.inc

# sources and compiler options extracted from the Linux build log for libsoftpipe.a

SRC_C = sp_fs_exec.c \
        sp_clear.c \
        sp_fence.c \
        sp_flush.c \
        sp_query.c \
        sp_context.c \
        sp_draw_arrays.c \
        sp_prim_vbuf.c \
        sp_quad_pipe.c \
        sp_quad_stipple.c \
        sp_quad_depth_test.c \
        sp_quad_fs.c \
        sp_quad_blend.c \
        sp_screen.c \
        sp_setup.c \
        sp_state_blend.c \
        sp_state_clip.c \
        sp_state_derived.c \
        sp_state_sampler.c \
        sp_state_shader.c \
        sp_state_so.c \
        sp_state_rasterizer.c \
        sp_state_surface.c \
        sp_state_vertex.c \
        sp_texture.c \
        sp_tex_sample.c \
        sp_tex_tile_cache.c \
        sp_tile_cache.c \
        sp_surface.c \

INC_DIR += $(REP_DIR)/contrib/$(MESA_DIR)/include \
           $(REP_DIR)/contrib/$(MESA_DIR)/src/gallium/auxiliary \
           $(REP_DIR)/contrib/$(MESA_DIR)/src/gallium/include \

LIBS = libc

vpath %.c $(REP_DIR)/contrib/$(MESA_DIR)/src/gallium/drivers/softpipe

