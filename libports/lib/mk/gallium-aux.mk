include $(REP_DIR)/lib/mk/mesa10.inc

# sources and compiler options extracted from the Linux build log for libgallium.a

SRC_C = cso_cache/cso_cache.c \
        cso_cache/cso_context.c \
        cso_cache/cso_hash.c \
        draw/draw_context.c \
        draw/draw_fs.c \
        draw/draw_gs.c \
        draw/draw_pipe.c \
        draw/draw_pipe_aaline.c \
        draw/draw_pipe_aapoint.c \
        draw/draw_pipe_clip.c \
        draw/draw_pipe_cull.c \
        draw/draw_pipe_flatshade.c \
        draw/draw_pipe_offset.c \
        draw/draw_pipe_pstipple.c \
        draw/draw_pipe_stipple.c \
        draw/draw_pipe_twoside.c \
        draw/draw_pipe_unfilled.c \
        draw/draw_pipe_util.c \
        draw/draw_pipe_validate.c \
        draw/draw_pipe_vbuf.c \
        draw/draw_pipe_wide_line.c \
        draw/draw_pipe_wide_point.c \
        draw/draw_prim_assembler.c \
        draw/draw_pt.c \
        draw/draw_pt_emit.c \
        draw/draw_pt_fetch.c \
        draw/draw_pt_fetch_emit.c \
        draw/draw_pt_fetch_shade_emit.c \
        draw/draw_pt_fetch_shade_pipeline.c \
        draw/draw_pt_post_vs.c \
        draw/draw_pt_so_emit.c \
        draw/draw_pt_util.c \
        draw/draw_pt_vsplit.c \
        draw/draw_vertex.c \
        draw/draw_vs.c \
        draw/draw_vs_exec.c \
        draw/draw_vs_variant.c \
        hud/font.c \
        hud/hud_context.c \
        hud/hud_cpu.c \
        hud/hud_fps.c \
        hud/hud_driver_query.c \
        indices/u_primconvert.c \
        os/os_misc.c \
        os/os_process.c \
        os/os_time.c \
        pipebuffer/pb_buffer_fenced.c \
        pipebuffer/pb_buffer_malloc.c \
        pipebuffer/pb_bufmgr_alt.c \
        pipebuffer/pb_bufmgr_cache.c \
        pipebuffer/pb_bufmgr_debug.c \
        pipebuffer/pb_bufmgr_mm.c \
        pipebuffer/pb_bufmgr_ondemand.c \
        pipebuffer/pb_bufmgr_pool.c \
        pipebuffer/pb_bufmgr_slab.c \
        pipebuffer/pb_validate.c \
        postprocess/pp_celshade.c \
        postprocess/pp_colors.c \
        postprocess/pp_init.c \
        postprocess/pp_mlaa.c \
        postprocess/pp_run.c \
        postprocess/pp_program.c \
        rbug/rbug_connection.c \
        rbug/rbug_context.c \
        rbug/rbug_core.c \
        rbug/rbug_demarshal.c \
        rbug/rbug_texture.c \
        rbug/rbug_shader.c \
        rtasm/rtasm_cpu.c \
        rtasm/rtasm_execmem.c \
        rtasm/rtasm_x86sse.c \
        tgsi/tgsi_build.c \
        tgsi/tgsi_dump.c \
        tgsi/tgsi_exec.c \
        tgsi/tgsi_info.c \
        tgsi/tgsi_iterate.c \
        tgsi/tgsi_parse.c \
        tgsi/tgsi_sanity.c \
        tgsi/tgsi_scan.c \
        tgsi/tgsi_strings.c \
        tgsi/tgsi_text.c \
        tgsi/tgsi_transform.c \
        tgsi/tgsi_ureg.c \
        tgsi/tgsi_util.c \
        translate/translate.c \
        translate/translate_cache.c \
        translate/translate_generic.c \
        translate/translate_sse.c \
        util/u_debug.c \
        util/u_debug_describe.c \
        util/u_debug_flush.c \
        util/u_debug_memory.c \
        util/u_debug_refcnt.c \
        util/u_debug_stack.c \
        util/u_debug_symbol.c \
        util/u_dump_defines.c \
        util/u_dump_state.c \
        util/u_bitmask.c \
        util/u_blit.c \
        util/u_blitter.c \
        util/u_cache.c \
        util/u_caps.c \
        util/u_cpu_detect.c \
        util/u_dl.c \
        util/u_draw.c \
        util/u_draw_quad.c \
        util/u_format.c \
        util/u_format_other.c \
        util/u_format_latc.c \
        util/u_format_s3tc.c \
        util/u_format_rgtc.c \
        util/u_format_etc.c \
        util/u_format_tests.c \
        util/u_format_yuv.c \
        util/u_format_zs.c \
        util/u_framebuffer.c \
        util/u_gen_mipmap.c \
        util/u_handle_table.c \
        util/u_hash.c \
        util/u_hash_table.c \
        util/u_helpers.c \
        util/u_index_modify.c \
        util/u_keymap.c \
        util/u_linear.c \
        util/u_linkage.c \
        util/u_network.c \
        util/u_math.c \
        util/u_mm.c \
        util/u_pstipple.c \
        util/u_ringbuffer.c \
        util/u_sampler.c \
        util/u_simple_shaders.c \
        util/u_slab.c \
        util/u_snprintf.c \
        util/u_staging.c \
        util/u_suballoc.c \
        util/u_surface.c \
        util/u_surfaces.c \
        util/u_texture.c \
        util/u_tile.c \
        util/u_transfer.c \
        util/u_resource.c \
        util/u_upload_mgr.c \
        util/u_vbuf.c \
        vl/vl_csc.c \
        vl/vl_compositor.c \
        vl/vl_matrix_filter.c \
        vl/vl_median_filter.c \
        vl/vl_decoder.c \
        vl/vl_mpeg12_decoder.c \
        vl/vl_mpeg12_bitstream.c \
        vl/vl_zscan.c \
        vl/vl_idct.c \
        vl/vl_mc.c \
        vl/vl_vertex_buffers.c \
        vl/vl_video_buffer.c \
        indices/u_indices_gen.c \
        indices/u_unfilled_gen.c \
        util/u_format_srgb.c \
        util/u_format_table.c \

PYTHON2 := $(VERBOSE)$(lastword $(shell which python2 python2.{4,5,6,7,8}))

indices/u_%_gen.c: $(REP_DIR)/contrib/$(MESA_DIR)/src/gallium/auxiliary/indices/u_%_gen.py
	$(MSG_CONVERT)$@
	$(PYTHON2) $< > $@

util/u_format_%.c: $(REP_DIR)/contrib/$(MESA_DIR)/src/gallium/auxiliary/util/u_format_%.py
	$(MSG_CONVERT)$@
	$(PYTHON2) $< > $@

# -fvisibility=hidden -g -O2 -Wall -std=c99 -Werror=implicit-function-declaration -Werror=missing-prototypes -fno-strict-aliasing -fno-builtin-memcmp

INC_DIR += $(REP_DIR)/contrib/$(MESA_DIR)/include \
           $(REP_DIR)/contrib/$(MESA_DIR)/src/gallium/auxiliary \
           $(REP_DIR)/contrib/$(MESA_DIR)/src/gallium/auxiliary/util \
           $(REP_DIR)/contrib/$(MESA_DIR)/src/gallium/include \
 
LIBS = libc

vpath %.c $(REP_DIR)/contrib/$(MESA_DIR)/src/gallium/auxiliary

