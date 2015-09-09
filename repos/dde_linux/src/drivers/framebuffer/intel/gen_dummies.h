bool access_ok(int access, void *addr, size_t size)
{
	TRACE_AND_STOP;
	return -1;
}

int acpi_video_register(void)
{
	TRACE_AND_STOP;
	return -1;
}

void acpi_video_unregister(void)
{
	TRACE_AND_STOP;
}

void add_wait_queue(wait_queue_head_t *, wait_queue_t *)
{
	TRACE_AND_STOP;
}

void assert_spin_locked(spinlock_t *lock)
{
	TRACE_AND_STOP;
}

void atomic_set_mask(unsigned int mask, atomic_t *v)
{
	TRACE_AND_STOP;
}

bool cancel_delayed_work_sync(struct delayed_work *work)
{
	TRACE_AND_STOP;
	return -1;
}

bool cancel_work_sync(struct work_struct *work)
{
	TRACE_AND_STOP;
	return -1;
}

void console_lock(void)
{
	TRACE_AND_STOP;
}

int console_trylock(void)
{
	TRACE_AND_STOP;
	return -1;
}

void console_unlock(void)
{
	TRACE_AND_STOP;
}

size_t copy_from_user(void *to, void const *from, size_t len)
{
	TRACE_AND_STOP;
	return -1;
}

size_t copy_to_user(void *dst, void const *src, size_t len)
{
	TRACE_AND_STOP;
	return -1;
}

void cpu_relax(void)
{
	TRACE_AND_STOP;
}

void destroy_timer_on_stack(struct timer_list *timer)
{
	TRACE_AND_STOP;
}

void destroy_workqueue(struct workqueue_struct *wq)
{
	TRACE_AND_STOP;
}

int dma_map_sg_attrs(struct device *dev, struct scatterlist *sg, int nents, enum dma_data_direction dir, struct dma_attrs *attrs)
{
	TRACE_AND_STOP;
	return -1;
}

int dma_set_coherent_mask(struct device *dev, u64 mask)
{
	TRACE_AND_STOP;
	return -1;
}

void dma_unmap_sg_attrs(struct device *dev, struct scatterlist *sg, int nents, enum dma_data_direction dir, struct dma_attrs *attrs)
{
	TRACE_AND_STOP;
}

void drm_calc_timestamping_constants(struct drm_crtc *crtc, const struct drm_display_mode *mode)
{
	TRACE_AND_STOP;
}

int drm_calc_vbltimestamp_from_scanoutpos(struct drm_device *dev, int crtc, int *max_error, struct timeval *vblank_time, unsigned flags, const struct drm_crtc *refcrtc, const struct drm_display_mode *mode)
{
	TRACE_AND_STOP;
	return -1;
}

void drm_clflush_pages(struct page *pages[], unsigned long num_pages)
{
	TRACE_AND_STOP;
}

void drm_clflush_sg(struct sg_table *st)
{
	TRACE_AND_STOP;
}

void drm_clflush_virt_range(char *addr, unsigned long length)
{
	TRACE_AND_STOP;
}

bool drm_dp_channel_eq_ok(const u8 link_status[DP_LINK_STATUS_SIZE], int lane_count)
{
	TRACE_AND_STOP;
	return -1;
}

bool drm_dp_clock_recovery_ok(const u8 link_status[DP_LINK_STATUS_SIZE], int lane_count)
{
	TRACE_AND_STOP;
	return -1;
}

u8 drm_dp_get_adjust_request_pre_emphasis(const u8 link_status[DP_LINK_STATUS_SIZE], int lane)
{
	TRACE_AND_STOP;
	return -1;
}

u8 drm_dp_get_adjust_request_voltage(const u8 link_status[DP_LINK_STATUS_SIZE], int lane)
{
	TRACE_AND_STOP;
	return -1;
}

void drm_dp_link_train_channel_eq_delay(const u8 dpcd[DP_RECEIVER_CAP_SIZE])
{
	TRACE_AND_STOP;
}

void drm_dp_link_train_clock_recovery_delay(const u8 dpcd[DP_RECEIVER_CAP_SIZE])
{
	TRACE_AND_STOP;
}

int drm_gem_create_mmap_offset(struct drm_gem_object *obj)
{
	TRACE_AND_STOP;
	return -1;
}

int drm_gem_dumb_destroy(struct drm_file *file, struct drm_device *dev, uint32_t handle)
{
	TRACE_AND_STOP;
	return -1;
}

void drm_gem_free_mmap_offset(struct drm_gem_object *obj)
{
	TRACE_AND_STOP;
}

int drm_gem_handle_create(struct drm_file *file_priv, struct drm_gem_object *obj, u32 *handlep)
{
	TRACE_AND_STOP;
	return -1;
}

int drm_gem_mmap(struct file *filp, struct vm_area_struct *vma)
{
	TRACE_AND_STOP;
	return -1;
}

int drm_gem_object_init(struct drm_device *dev, struct drm_gem_object *obj, size_t size)
{
	TRACE_AND_STOP;
	return -1;
}

struct drm_gem_object *drm_gem_object_lookup(struct drm_device *dev, struct drm_file *filp, u32 handle)
{
	TRACE_AND_STOP;
	return NULL;
}

void drm_gem_object_reference(struct drm_gem_object *obj)
{
	TRACE_AND_STOP;
}

void drm_gem_object_release(struct drm_gem_object *obj)
{
	TRACE_AND_STOP;
}

void drm_gem_object_unreference(struct drm_gem_object *obj)
{
	TRACE_AND_STOP;
}

void drm_gem_object_unreference_unlocked(struct drm_gem_object *obj)
{
	TRACE_AND_STOP;
}

int drm_gem_prime_fd_to_handle(struct drm_device *dev, struct drm_file *file_priv, int prime_fd, uint32_t *handle)
{
	TRACE_AND_STOP;
	return -1;
}

int drm_gem_prime_handle_to_fd(struct drm_device *dev, struct drm_file *file_priv, uint32_t handle, uint32_t flags, int *prime_fd)
{
	TRACE_AND_STOP;
	return -1;
}

void drm_gem_vm_close(struct vm_area_struct *vma)
{
	TRACE_AND_STOP;
}

void drm_gem_vm_open(struct vm_area_struct *vma)
{
	TRACE_AND_STOP;
}

struct drm_local_map *drm_getsarea(struct drm_device *dev)
{
	TRACE_AND_STOP;
	return NULL;
}

bool drm_handle_vblank(struct drm_device *dev, int crtc)
{
	TRACE_AND_STOP;
	return -1;
}

bool drm_helper_hpd_irq_event(struct drm_device *dev)
{
	TRACE_AND_STOP;
	return -1;
}

int drm_helper_mode_fill_fb_struct(struct drm_framebuffer *fb, struct drm_mode_fb_cmd2 *mode_cmd)
{
	TRACE_AND_STOP;
	return -1;
}

void drm_helper_move_panel_connectors_to_head(struct drm_device *)
{
	TRACE_AND_STOP;
}

int drm_helper_probe_single_connector_modes(struct drm_connector *connector, uint32_t maxX, uint32_t maxY)
{
	TRACE_AND_STOP;
	return -1;
}

long drm_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	TRACE_AND_STOP;
	return -1;
}

int drm_irq_uninstall(struct drm_device *dev)
{
	TRACE_AND_STOP;
	return -1;
}

void drm_kms_helper_hotplug_event(struct drm_device *dev)
{
	TRACE_AND_STOP;
}

void drm_kms_helper_poll_disable(struct drm_device *dev)
{
	TRACE_AND_STOP;
}

void drm_kms_helper_poll_enable(struct drm_device *dev)
{
	TRACE_AND_STOP;
}

void drm_kms_helper_poll_fini(struct drm_device *dev)
{
	TRACE_AND_STOP;
}

void drm_kms_helper_poll_init(struct drm_device *dev)
{
	TRACE_AND_STOP;
}

void drm_mode_copy(struct drm_display_mode *dst, const struct drm_display_mode *src)
{
	TRACE_AND_STOP;
}

void drm_mode_debug_printmodeline(const struct drm_display_mode *mode)
{
	TRACE_AND_STOP;
}

struct drm_display_mode *drm_mode_duplicate(struct drm_device *dev, const struct drm_display_mode *mode)
{
	TRACE_AND_STOP;
	return NULL;
}

bool drm_mode_equal(const struct drm_display_mode *mode1, const struct drm_display_mode *mode2)
{
	TRACE_AND_STOP;
	return -1;
}

void drm_mode_set_crtcinfo(struct drm_display_mode *p, int adjust_flags)
{
	TRACE_AND_STOP;
}

int drm_noop(struct drm_device *dev, void *data, struct drm_file *file_priv)
{
	TRACE_AND_STOP;
	return -1;
}

int drm_open(struct inode *inode, struct file *filp)
{
	TRACE_AND_STOP;
	return -1;
}

drm_dma_handle_t *drm_pci_alloc(struct drm_device *dev, size_t size, size_t align)
{
	TRACE_AND_STOP;
	return NULL;
}

void drm_pci_free(struct drm_device *dev, drm_dma_handle_t * dmah)
{
	TRACE_AND_STOP;
}

unsigned int drm_poll(struct file *filp, struct poll_table_struct *wait)
{
	TRACE_AND_STOP;
	return -1;
}

void drm_prime_gem_destroy(struct drm_gem_object *obj, struct sg_table *sg)
{
	TRACE_AND_STOP;
}

void drm_put_dev(struct drm_device *dev)
{
	TRACE_AND_STOP;
}

ssize_t drm_read(struct file *filp, char __user *buffer, size_t count, loff_t *offset)
{
	TRACE_AND_STOP;
	return -1;
}

int drm_release(struct inode *inode, struct file *filp)
{
	TRACE_AND_STOP;
	return -1;
}

void drm_send_vblank_event(struct drm_device *dev, int crtc, struct drm_pending_vblank_event *e)
{
	TRACE_AND_STOP;
}

int drm_sysfs_connector_add(struct drm_connector *connector)
{
	TRACE_AND_STOP;
	return -1;
}

void drm_sysfs_connector_remove(struct drm_connector *connector)
{
	TRACE_AND_STOP;
}

void drm_vblank_cleanup(struct drm_device *dev)
{
	TRACE_AND_STOP;
}

int drm_vblank_get(struct drm_device *dev, int crtc)
{
	TRACE_AND_STOP;
	return -1;
}

void drm_vblank_off(struct drm_device *dev, int crtc)
{
	TRACE_AND_STOP;
}

void drm_vblank_post_modeset(struct drm_device *dev, int crtc)
{
	TRACE_AND_STOP;
}

void drm_vblank_pre_modeset(struct drm_device *dev, int crtc)
{
	TRACE_AND_STOP;
}

void drm_vblank_put(struct drm_device *dev, int crtc)
{
	TRACE_AND_STOP;
}

bool drm_vma_node_has_offset(struct drm_vma_offset_node *node)
{
	TRACE_AND_STOP;
	return -1;
}

__u64 drm_vma_node_offset_addr(struct drm_vma_offset_node *node)
{
	TRACE_AND_STOP;
	return -1;
}

void drm_vma_node_unmap(struct drm_vma_offset_node *node, struct address_space *file_mapping)
{
	TRACE_AND_STOP;
}

struct intel_hdmi *enc_to_intel_hdmi(struct drm_encoder *encoder)
{
	TRACE_AND_STOP;
	return NULL;
}

int fault_in_multipages_readable(const char __user *uaddr, int size)
{
	TRACE_AND_STOP;
	return -1;
}

int fault_in_multipages_writeable(char __user *uaddr, int size)
{
	TRACE_AND_STOP;
	return -1;
}

struct inode *file_inode(struct file *f)
{
	TRACE_AND_STOP;
	return NULL;
}

void flush_scheduled_work(void)
{
	TRACE_AND_STOP;
}

void flush_workqueue(struct workqueue_struct *wq)
{
	TRACE_AND_STOP;
}

void __free_pages(struct page *page, unsigned int order)
{
	TRACE_AND_STOP;
}

void free_pages(unsigned long addr, unsigned int order)
{
	TRACE_AND_STOP;
}

void gen6_rps_boost(struct drm_i915_private *dev_priv)
{
	TRACE_AND_STOP;
}

void gen6_rps_idle(struct drm_i915_private *dev_priv)
{
	TRACE_AND_STOP;
}

void gen6_set_rps(struct drm_device *dev, u8 val)
{
	TRACE_AND_STOP;
}

void gen6_update_ring_freq(struct drm_device *dev)
{
	TRACE_AND_STOP;
}

unsigned long __get_free_pages(gfp_t gfp_mask, unsigned int order)
{
	TRACE_AND_STOP;
	return -1;
}

void getrawmonotonic(struct timespec *ts)
{
	TRACE_AND_STOP;
}

unsigned long get_seconds(void)
{
	TRACE_AND_STOP;
	return -1;
}

void i915_capture_error_state(struct drm_device *dev)
{
	TRACE_AND_STOP;
}

void i915_destroy_error_state(struct drm_device *dev)
{
	TRACE_AND_STOP;
}

void i915_gem_context_close(struct drm_device *dev, struct drm_file *file)
{
	TRACE_AND_STOP;
}

int i915_gem_context_create_ioctl(struct drm_device *dev, void *data, struct drm_file *file)
{
	TRACE_AND_STOP;
	return -1;
}

int i915_gem_context_destroy_ioctl(struct drm_device *dev, void *data, struct drm_file *file)
{
	TRACE_AND_STOP;
	return -1;
}

void i915_gem_context_fini(struct drm_device *dev)
{
	TRACE_AND_STOP;
}

void i915_gem_context_free(struct kref *ctx_ref)
{
	TRACE_AND_STOP;
}

int __must_check i915_gem_context_init(struct drm_device *dev)
{
	TRACE_AND_STOP;
	return -1;
}

int i915_gem_evict_everything(struct drm_device *dev)
{
	TRACE_AND_STOP;
	return -1;
}

int __must_check i915_gem_evict_something(struct drm_device *dev, struct i915_address_space *vm, int min_size, unsigned alignment, unsigned cache_level, bool mappable, bool nonblock)
{
	TRACE_AND_STOP;
	return -1;
}

void i915_gem_object_do_bit_17_swizzle(struct drm_i915_gem_object *obj)
{
	TRACE_AND_STOP;
}

void i915_gem_object_save_bit_17_swizzle(struct drm_i915_gem_object *obj)
{
	TRACE_AND_STOP;
}

struct dma_buf *i915_gem_prime_export(struct drm_device *dev, struct drm_gem_object *gem_obj, int flags)
{
	TRACE_AND_STOP;
	return NULL;
}

struct drm_gem_object *i915_gem_prime_import(struct drm_device *dev, struct dma_buf *dma_buf)
{
	TRACE_AND_STOP;
	return NULL;
}

void i915_get_extra_instdone(struct drm_device *dev, uint32_t *instdone)
{
	TRACE_AND_STOP;
}

int i915_restore_state(struct drm_device *dev)
{
	TRACE_AND_STOP;
	return -1;
}

int i915_save_state(struct drm_device *dev)
{
	TRACE_AND_STOP;
	return -1;
}

void i915_setup_sysfs(struct drm_device *dev_priv)
{
	TRACE_AND_STOP;
}

int i915_switch_context(struct intel_ring_buffer *ring, struct drm_file *file, int to_id)
{
	TRACE_AND_STOP;
	return -1;
}

void i915_teardown_sysfs(struct drm_device *dev_priv)
{
	TRACE_AND_STOP;
}

void i915_update_gfx_val(struct drm_i915_private *dev_priv)
{
	TRACE_AND_STOP;
}

void ilk_wm_get_hw_state(struct drm_device *dev)
{
	TRACE_AND_STOP;
}

void intel_attach_broadcast_rgb_property(struct drm_connector *connector)
{
	TRACE_AND_STOP;
}

void intel_attach_force_audio_property(struct drm_connector *connector)
{
	TRACE_AND_STOP;
}

void intel_aux_display_runtime_get(struct drm_i915_private *dev_priv)
{
	TRACE_AND_STOP;
}

void intel_aux_display_runtime_put(struct drm_i915_private *dev_priv)
{
	TRACE_AND_STOP;
}

void intel_cleanup_overlay(struct drm_device *dev)
{
	TRACE_AND_STOP;
}

void intel_cleanup_ring_buffer(struct intel_ring_buffer *ring)
{
	TRACE_AND_STOP;
}

int intel_connector_update_modes(struct drm_connector *connector, struct edid *edid)
{
	TRACE_AND_STOP;
	return -1;
}

void intel_crt_init(struct drm_device *dev)
{
	TRACE_AND_STOP;
}

int intel_ddc_get_modes(struct drm_connector *c, struct i2c_adapter *adapter)
{
	TRACE_AND_STOP;
	return -1;
}

void intel_disable_fbc(struct drm_device *dev)
{
	TRACE_AND_STOP;
}

bool intel_display_power_enabled(struct drm_device *dev, enum intel_display_power_domain domain)
{
	TRACE_AND_STOP;
	return -1;
}

void intel_display_power_get(struct drm_device *dev, enum intel_display_power_domain domain)
{
	TRACE_AND_STOP;
}

void intel_display_power_put(struct drm_device *dev, enum intel_display_power_domain domain)
{
	TRACE_AND_STOP;
}

bool intel_dsi_init(struct drm_device *dev)
{
	TRACE_AND_STOP;
	return -1;
}

void intel_dvo_init(struct drm_device *dev)
{
	TRACE_AND_STOP;
}

void intel_enable_gt_powersave(struct drm_device *dev)
{
	TRACE_AND_STOP;
}

void intel_fini_runtime_pm(struct drm_i915_private *dev_priv)
{
	TRACE_AND_STOP;
}

void intel_gpu_ips_init(struct drm_i915_private *dev_priv)
{
	TRACE_AND_STOP;
}

void intel_gpu_ips_teardown(void)
{
	TRACE_AND_STOP;
}

bool intel_hdmi_compute_config(struct intel_encoder *encoder, struct intel_crtc_config *pipe_config)
{
	TRACE_AND_STOP;
	return -1;
}

void intel_hdmi_init(struct drm_device *dev, int hdmi_reg, enum port port)
{
	TRACE_AND_STOP;
}

void intel_hdmi_init_connector(struct intel_digital_port *intel_dig_port, struct intel_connector *intel_connector)
{
	TRACE_AND_STOP;
}

int intel_init_blt_ring_buffer(struct drm_device *dev)
{
	TRACE_AND_STOP;
	return -1;
}

int intel_init_bsd_ring_buffer(struct drm_device *dev)
{
	TRACE_AND_STOP;
	return -1;
}

void intel_init_clock_gating(struct drm_device *dev)
{
	TRACE_AND_STOP;
}

int intel_init_render_ring_buffer(struct drm_device *dev)
{
	TRACE_AND_STOP;
	return -1;
}

void intel_init_runtime_pm(struct drm_i915_private *dev_priv)
{
	TRACE_AND_STOP;
}

int intel_init_vebox_ring_buffer(struct drm_device *dev)
{
	TRACE_AND_STOP;
	return -1;
}

int intel_overlay_switch_off(struct intel_overlay *overlay)
{
	TRACE_AND_STOP;
	return -1;
}

void intel_plane_disable(struct drm_plane *plane)
{
	TRACE_AND_STOP;
}

void intel_plane_restore(struct drm_plane *plane)
{
	TRACE_AND_STOP;
}

void intel_power_domains_remove(struct drm_device *dev)
{
	TRACE_AND_STOP;
}

int intel_render_ring_init_dri(struct drm_device *dev, u64 start, u32 size)
{
	TRACE_AND_STOP;
	return -1;
}

void __intel_ring_advance(struct intel_ring_buffer *ring)
{
	TRACE_AND_STOP;
}

int __must_check intel_ring_begin(struct intel_ring_buffer *ring, int n)
{
	TRACE_AND_STOP;
	return -1;
}

int __must_check intel_ring_cacheline_align(struct intel_ring_buffer *ring)
{
	TRACE_AND_STOP;
	return -1;
}

int intel_ring_flush_all_caches(struct intel_ring_buffer *ring)
{
	TRACE_AND_STOP;
	return -1;
}

u32 intel_ring_get_active_head(struct intel_ring_buffer *ring)
{
	TRACE_AND_STOP;
	return -1;
}

int __must_check intel_ring_idle(struct intel_ring_buffer *ring)
{
	TRACE_AND_STOP;
	return -1;
}

void intel_ring_init_seqno(struct intel_ring_buffer *ring, u32 seqno)
{
	TRACE_AND_STOP;
}

void intel_ring_setup_status_page(struct intel_ring_buffer *ring)
{
	TRACE_AND_STOP;
}

void intel_runtime_pm_get(struct drm_i915_private *dev_priv)
{
	TRACE_AND_STOP;
}

void intel_runtime_pm_put(struct drm_i915_private *dev_priv)
{
	TRACE_AND_STOP;
}

u32 intel_sbi_read(struct drm_i915_private *dev_priv, u16 reg, enum intel_sbi_destination destination)
{
	TRACE_AND_STOP;
	return -1;
}

void intel_sbi_write(struct drm_i915_private *dev_priv, u16 reg, u32 value, enum intel_sbi_destination destination)
{
	TRACE_AND_STOP;
}

bool intel_sdvo_init(struct drm_device *dev, uint32_t sdvo_reg, bool is_sdvob)
{
	TRACE_AND_STOP;
	return -1;
}

void intel_setup_overlay(struct drm_device *dev)
{
	TRACE_AND_STOP;
}

int intel_sprite_get_colorkey(struct drm_device *dev, void *data, struct drm_file *file_priv)
{
	TRACE_AND_STOP;
	return -1;
}

int intel_sprite_set_colorkey(struct drm_device *dev, void *data, struct drm_file *file_priv)
{
	TRACE_AND_STOP;
	return -1;
}

void intel_suspend_hw(struct drm_device *dev)
{
	TRACE_AND_STOP;
}

void intel_tv_init(struct drm_device *dev)
{
	TRACE_AND_STOP;
}

void intel_update_fbc(struct drm_device *dev)
{
	TRACE_AND_STOP;
}

void intel_update_watermarks(struct drm_crtc *crtc)
{
	TRACE_AND_STOP;
}

void io_mapping_free(struct io_mapping *mapping)
{
	TRACE_AND_STOP;
}

void *io_mapping_map_atomic_wc(struct io_mapping *mapping, unsigned long offset)
{
	TRACE_AND_STOP;
	return NULL;
}

void io_mapping_unmap_atomic(void *vaddr)
{
	TRACE_AND_STOP;
}

void io_schedule(void)
{
	TRACE_AND_STOP;
}

void  iounmap(volatile void *addr)
{
	TRACE_AND_STOP;
}

bool ironlake_set_drps(struct drm_device *dev, u8 val)
{
	TRACE_AND_STOP;
	return -1;
}

void ironlake_teardown_rc6(struct drm_device *dev)
{
	TRACE_AND_STOP;
}

void kmem_cache_destroy(struct kmem_cache *)
{
	TRACE_AND_STOP;
}

void *kmem_cache_zalloc(struct kmem_cache *k, gfp_t flags)
{
	TRACE_AND_STOP;
	return NULL;
}

int kobject_uevent_env(struct kobject *kobj, enum kobject_action action, char *envp[])
{
	TRACE_AND_STOP;
	return -1;
}

void kref_get(struct kref *kref)
{
	TRACE_AND_STOP;
}

int  kref_put(struct kref *kref, void (*release) (struct kref *kref))
{
	TRACE_AND_STOP;
	return -1;
}

gfp_t mapping_gfp_mask(struct address_space * mapping)
{
	TRACE_AND_STOP;
	return -1;
}

void mapping_set_gfp_mask(struct address_space *m, gfp_t mask)
{
	TRACE_AND_STOP;
}

void mark_page_accessed(struct page *)
{
	TRACE_AND_STOP;
}

int memcmp(const void *, const void *, size_t)
{
	TRACE_AND_STOP;
	return -1;
}

void  *memset_io(void *s, int c, size_t n)
{
	TRACE_AND_STOP;
	return NULL;
}

loff_t noop_llseek(struct file *file, loff_t offset, int whence)
{
	TRACE_AND_STOP;
	return -1;
}

dma_addr_t page_to_pfn(struct page *page)
{
	TRACE_AND_STOP;
	return -1;
}

resource_size_t pcibios_align_resource(void *, const struct resource *, resource_size_t, resource_size_t)
{
	TRACE_AND_STOP;
	return -1;
}

void pci_disable_device(struct pci_dev *dev)
{
	TRACE_AND_STOP;
}

void pci_disable_msi(struct pci_dev *dev)
{
	TRACE_AND_STOP;
}

int pci_dma_mapping_error(struct pci_dev *pdev, dma_addr_t dma_addr)
{
	TRACE_AND_STOP;
	return -1;
}

int pci_enable_device(struct pci_dev *dev)
{
	TRACE_AND_STOP;
	return -1;
}

struct pci_dev *pci_get_device(unsigned int vendor, unsigned int device, struct pci_dev *from)
{
	TRACE_AND_STOP;
	return NULL;
}

void *pci_get_drvdata(struct pci_dev *pdev)
{
	TRACE_AND_STOP;
	return NULL;
}

void pci_iounmap(struct pci_dev *dev, void __iomem *p)
{
	TRACE_AND_STOP;
}

dma_addr_t pci_map_page(struct pci_dev *hwdev, struct page *page, unsigned long offset, size_t size, int direction)
{
	TRACE_AND_STOP;
	return -1;
}

int pci_save_state(struct pci_dev *dev)
{
	TRACE_AND_STOP;
	return -1;
}

int pci_set_power_state(struct pci_dev *dev, pci_power_t state)
{
	TRACE_AND_STOP;
	return -1;
}

void pci_unmap_page(struct pci_dev *hwdev, dma_addr_t dma_address, size_t size, int direction)
{
	TRACE_AND_STOP;
}

void pm_qos_remove_request(struct pm_qos_request *req)
{
	TRACE_AND_STOP;
}

void pm_qos_update_request(struct pm_qos_request *req, s32 new_value)
{
	TRACE_AND_STOP;
}

void put_page(struct page *page)
{
	TRACE_AND_STOP;
}

bool queue_delayed_work(struct workqueue_struct *, struct delayed_work *, unsigned long)
{
	TRACE_AND_STOP;
	return -1;
}

bool queue_work(struct workqueue_struct *wq, struct work_struct *work)
{
	TRACE_AND_STOP;
	return -1;
}

int release_resource(struct resource *r)
{
	TRACE_AND_STOP;
	return -1;
}

void remove_wait_queue(wait_queue_head_t *, wait_queue_t *)
{
	TRACE_AND_STOP;
}

int request_resource(struct resource *root, struct resource *)
{
	TRACE_AND_STOP;
	return -1;
}

unsigned long round_jiffies_up(unsigned long j)
{
	TRACE_AND_STOP;
	return -1;
}

unsigned long round_jiffies_up_relative(unsigned long j)
{
	TRACE_AND_STOP;
	return -1;
}

int sandybridge_pcode_write(struct drm_i915_private *dev_priv, u8 mbox, u32 val)
{
	TRACE_AND_STOP;
	return -1;
}

int schedule_delayed_work(struct delayed_work *work, unsigned long delay)
{
	TRACE_AND_STOP;
	return -1;
}

int schedule_work(struct work_struct *work)
{
	TRACE_AND_STOP;
	return -1;
}

void __set_current_state(int state)
{
	TRACE_AND_STOP;
}

void set_normalized_timespec(struct timespec *ts, time_t sec, s64 nsec)
{
	TRACE_AND_STOP;
}

int set_page_dirty(struct page *page)
{
	TRACE_AND_STOP;
	return -1;
}

int set_pages_wb(struct page *page, int numpages)
{
	TRACE_AND_STOP;
	return -1;
}

int sg_alloc_table(struct sg_table *, unsigned int, gfp_t)
{
	TRACE_AND_STOP;
	return -1;
}

void sg_free_table(struct sg_table *)
{
	TRACE_AND_STOP;
}

void sg_mark_end(struct scatterlist *sg)
{
	TRACE_AND_STOP;
}

struct scatterlist *sg_next(struct scatterlist *)
{
	TRACE_AND_STOP;
	return NULL;
}

dma_addr_t sg_page_iter_dma_address(struct sg_page_iter *piter)
{
	TRACE_AND_STOP;
	return -1;
}

bool __sg_page_iter_next(struct sg_page_iter *piter)
{
	TRACE_AND_STOP;
	return -1;
}

struct page *sg_page_iter_page(struct sg_page_iter *piter)
{
	TRACE_AND_STOP;
	return NULL;
}

void __sg_page_iter_start(struct sg_page_iter *piter, struct scatterlist *sglist, unsigned int nents, unsigned long pgoffset)
{
	TRACE_AND_STOP;
}

void sg_set_page(struct scatterlist *sg, struct page *page, unsigned int len, unsigned int offset)
{
	TRACE_AND_STOP;
}

struct page *shmem_read_mapping_page( struct address_space *mapping, pgoff_t index)
{
	TRACE_AND_STOP;
	return NULL;
}

struct page *shmem_read_mapping_page_gfp(struct address_space *mapping, pgoff_t index, gfp_t gfp_mask)
{
	TRACE_AND_STOP;
	return NULL;
}

void shmem_truncate_range(struct inode *inode, loff_t start, loff_t end)
{
	TRACE_AND_STOP;
}

int signal_pending(struct task_struct *p)
{
	TRACE_AND_STOP;
	return -1;
}

void spin_lock(spinlock_t *lock)
{
	TRACE_AND_STOP;
}

void spin_lock_irq(spinlock_t *lock)
{
	TRACE_AND_STOP;
}

void spin_unlock_irq(spinlock_t *lock)
{
	TRACE_AND_STOP;
}

unsigned long timespec_to_jiffies(const struct timespec *value)
{
	TRACE_AND_STOP;
	return -1;
}

s64 timespec_to_ns(const struct timespec *ts)
{
	TRACE_AND_STOP;
	return -1;
}

bool timespec_valid(const struct timespec *ts)
{
	TRACE_AND_STOP;
	return -1;
}

void unregister_shrinker(struct shrinker *)
{
	TRACE_AND_STOP;
}

void usleep_range(unsigned long min, unsigned long max)
{
	TRACE_AND_STOP;
}

void valleyview_set_rps(struct drm_device *dev, u8 val)
{
	TRACE_AND_STOP;
}

int vga_switcheroo_process_delayed_switch(void)
{
	TRACE_AND_STOP;
	return -1;
}

void vga_switcheroo_unregister_client(struct pci_dev *dev)
{
	TRACE_AND_STOP;
}

phys_addr_t virt_to_phys(volatile void *address)
{
	TRACE_AND_STOP;
	return -1;
}

u32 vlv_bunit_read(struct drm_i915_private *dev_priv, u32 reg)
{
	TRACE_AND_STOP;
	return -1;
}

void vlv_bunit_write(struct drm_i915_private *dev_priv, u32 reg, u32 val)
{
	TRACE_AND_STOP;
}

u32 vlv_cck_read(struct drm_i915_private *dev_priv, u32 reg)
{
	TRACE_AND_STOP;
	return -1;
}

void vlv_cck_write(struct drm_i915_private *dev_priv, u32 reg, u32 val)
{
	TRACE_AND_STOP;
}

u32 vlv_dpio_read(struct drm_i915_private *dev_priv, enum pipe pipe, int reg)
{
	TRACE_AND_STOP;
	return -1;
}

void vlv_dpio_write(struct drm_i915_private *dev_priv, enum pipe pipe, int reg, u32 val)
{
	TRACE_AND_STOP;
}

u32 vlv_punit_read(struct drm_i915_private *dev_priv, u8 addr)
{
	TRACE_AND_STOP;
	return -1;
}

void vlv_punit_write(struct drm_i915_private *dev_priv, u8 addr, u32 val)
{
	TRACE_AND_STOP;
}

int vm_insert_pfn(struct vm_area_struct *vma, unsigned long addr, unsigned long pfn)
{
	TRACE_AND_STOP;
	return -1;
}

unsigned long vm_mmap(struct file *, unsigned long, unsigned long, unsigned long, unsigned long, unsigned long)
{
	TRACE_AND_STOP;
	return -1;
}

int wake_up_process(struct task_struct *tsk)
{
	TRACE_AND_STOP;
	return -1;
}

void wbinvd_on_all_cpus()
{
	TRACE_AND_STOP;
}

void ida_remove(struct ida *ida, int id)
{
	TRACE_AND_STOP;
}

void idr_destroy(struct idr *idp)
{
	TRACE_AND_STOP;
}

void *idr_find(struct idr *idr, int id)
{
	TRACE_AND_STOP;
	return NULL;
}

void idr_remove(struct idr *idp, int id)
{
	TRACE_AND_STOP;
}

void kref_init(struct kref *kref)
{
	TRACE_AND_STOP;
}

int acpi_lid_notifier_unregister(struct notifier_block *nb)
{
	TRACE_AND_STOP;
	return -1;
}

int acpi_lid_open(void)
{
	TRACE_AND_STOP;
	return -1;
}

void drm_mode_set_name(struct drm_display_mode *mode)
{
	TRACE_AND_STOP;
}

int acpi_lid_notifier_register(struct notifier_block *nb)
{
	TRACE_AND_STOP;
	return -1;
}
struct drm_display_mode *drm_cvt_mode(struct drm_device *dev, int hdisplay, int vdisplay, int vrefresh, bool reduced, bool interlaced, bool margins)
{
	TRACE_AND_STOP;
	return NULL;
}

struct drm_display_mode *drm_gtf_mode(struct drm_device *dev, int hdisplay, int vdisplay, int vrefresh, bool interlaced, int margins)
{
	TRACE_AND_STOP;
	return NULL;
}

struct drm_display_mode *drm_gtf_mode_complex(struct drm_device *dev, int hdisplay, int vdisplay, int vrefresh, bool interlaced, int margins, int GTF_M, int GTF_2C, int GTF_K, int GTF_2J)
{
	TRACE_AND_STOP;
	return NULL;
}

bool drm_mode_equal_no_clocks_no_stereo(const struct drm_display_mode *mode1, const struct drm_display_mode *mode2)
{
	TRACE_AND_STOP;
	return -1;
}

int drm_mode_hsync(const struct drm_display_mode *mode)
{
	TRACE_AND_STOP;
	return -1;
}

int drm_mode_vrefresh(const struct drm_display_mode *mode)
{
	TRACE_AND_STOP;
	return -1;
}

void *memchr_inv(const void *s, int c, size_t n)
{
	TRACE_AND_STOP;
	return NULL;
}

void print_hex_dump(const char *level, const char *prefix_str, int prefix_type, int rowsize, int groupsize, const void *buf, size_t len, bool ascii)
{
	TRACE_AND_STOP;
}

int    strncmp(const char *cs, const char *ct, size_t count)
{
	TRACE_AND_STOP;
	return -1;
}

int acpi_device_uevent_modalias(struct device *, struct kobj_uevent_env *)
{
	TRACE_AND_STOP;
	return -1;
}

int acpi_dev_pm_attach(struct device *dev, bool power_on)
{
	TRACE_AND_STOP;
	return -1;
}

void acpi_dev_pm_detach(struct device *dev, bool power_off)
{
	TRACE_AND_STOP;
}

bool acpi_driver_match_device(struct device *dev, const struct device_driver *drv)
{
	TRACE_AND_STOP;
	return -1;
}

int add_uevent_var(struct kobj_uevent_env *env, const char *format, ...)
{
	TRACE_AND_STOP;
	return -1;
}

int bus_for_each_drv(struct bus_type *bus, struct device_driver *start, void *data, int (*fn)(struct device_driver *, void *))
{
	TRACE_AND_STOP;
	return -1;
}

bool device_can_wakeup(struct device *dev)
{
	TRACE_AND_STOP;
	return -1;
}

int device_for_each_child(struct device *dev, void *data, int (*fn)(struct device *dev, void *data))
{
	TRACE_AND_STOP;
	return -1;
}

int device_init_wakeup(struct device *dev, bool val)
{
	TRACE_AND_STOP;
	return -1;
}

const char *dev_name(const struct device *dev)
{
	TRACE_AND_STOP;
	return NULL;
}

void gpio_free(unsigned gpio)
{
	TRACE_AND_STOP;
}

int gpio_get_value(unsigned int gpio)
{
	TRACE_AND_STOP;
	return -1;
}

bool gpio_is_valid(int number)
{
	TRACE_AND_STOP;
	return -1;
}

int gpio_request_one(unsigned gpio, unsigned long flags, const char *label)
{
	TRACE_AND_STOP;
	return -1;
}

void gpio_set_value(unsigned int gpio, int value)
{
	TRACE_AND_STOP;
}

bool in_atomic()
{
	TRACE_AND_STOP;
	return -1;
}

bool irqs_disabled()
{
	TRACE_AND_STOP;
	return -1;
}

void ndelay(unsigned long)
{
	TRACE_AND_STOP;
}

int of_alias_get_id(struct device_node *np, const char *stem)
{
	TRACE_AND_STOP;
	return -1;
}

int of_driver_match_device(struct device *dev, const struct device_driver *drv)
{
	TRACE_AND_STOP;
	return -1;
}

void rt_mutex_lock(struct rt_mutex *lock)
{
	TRACE_AND_STOP;
}

int rt_mutex_trylock(struct rt_mutex *lock)
{
	TRACE_AND_STOP;
	return -1;
}

void rt_mutex_unlock(struct rt_mutex *lock)
{
	TRACE_AND_STOP;
}

int    strcmp(const char *s1, const char *s2)
{
	TRACE_AND_STOP;
	return -1;
}

size_t strlcpy(char *dest, const char *src, size_t size)
{
	TRACE_AND_STOP;
	return -1;
}

void up_read(struct rw_semaphore *sem)
{
	TRACE_AND_STOP;
}

void wait_for_completion(struct completion *work) 
{
	TRACE_AND_STOP;
}

void bus_unregister(struct bus_type *bus)
{
	TRACE_AND_STOP;
}


