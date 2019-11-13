#include <lx_emul.h>
#include <lx_emul_c.h>

#include <drm/drm_device.h>
#include <linux/component.h>
#include <soc/imx8/sc/types.h>


#if 0
/***************************
 ** drivers/base/devres.c **
 ***************************/

void devres_add(struct device *dev, void *res)
{
	TRACE_AND_STOP;
}

void *devres_alloc(dr_release_t release, size_t size, gfp_t gfp)
{
	TRACE_AND_STOP;
	return NULL;
}

void devres_close_group(struct device *dev, void *id)
{
	TRACE_AND_STOP;
}

void * devres_open_group(struct device *dev, void *id, gfp_t gfp)
{
	TRACE_AND_STOP;
	return NULL;
}

int devres_release_group(struct device *dev, void *id)
{
	TRACE_AND_STOP;
	return -1;
}

void devres_remove_group(struct device *dev, void *id)
{
	TRACE_AND_STOP;
}
#endif

/********************************
 ** drivers/base/dma-mapping.c **
 ********************************/

void dmam_free_coherent(struct device *dev, size_t size, void *vaddr,
			dma_addr_t dma_handle)
{
	TRACE_AND_STOP;
}


/*****************************
 ** drivers/base/platform.c **
 *****************************/

int platform_device_put(struct platform_device *pdev)
{
	TRACE_AND_STOP;
	return 0;
}

void platform_device_unregister(struct platform_device *pdev)
{
	TRACE_AND_STOP;
}


/***********************
 ** drivers/clk/clk.c **
 ***********************/

struct clk *clk_get_parent(struct clk *clk)
{
	TRACE_AND_STOP;
	return NULL;
}

unsigned long clk_get_rate(struct clk *clk)
{
	TRACE_AND_STOP;
	return 0;
}

bool clk_is_match(const struct clk *p, const struct clk *q)
{
	TRACE_AND_STOP;
	return false;
}

int clk_set_parent(struct clk *clk, struct clk *parent)
{
	TRACE_AND_STOP;
	return -1;
}

int clk_set_rate(struct clk *clk, unsigned long rate)
{
	TRACE_AND_STOP;
	return -1;
}


/*******************************
 ** drivers/gpu/drm/drm_drv.c **
 *******************************/
#if 0
struct drm_device *drm_dev_alloc(struct drm_driver *driver,
				 struct device *parent)
{
	TRACE_AND_STOP;
	return NULL;
}
#endif
void drm_dev_unref(struct drm_device *dev)
{
	TRACE_AND_STOP;
}

void drm_dev_unregister(struct drm_device *dev)
{
	TRACE_AND_STOP;
}


/*****************************************
 ** drivers/gpu/drm/drm_fb_cma_helper.c **
 *****************************************/

struct drm_framebuffer *drm_fb_cma_create(struct drm_device *dev,
                                          struct drm_file *file_priv,
                                          const struct drm_mode_fb_cmd2 *mode_cmd)
{
	TRACE_AND_STOP;
	return NULL;
}

int drm_fb_cma_prepare_fb(struct drm_plane *plane,
                          struct drm_plane_state *state)
{
	TRACE_AND_STOP;
	return -1;
}

struct drm_gem_cma_object *drm_fb_cma_get_gem_obj(struct drm_framebuffer *fb,
                                                  unsigned int plane)
{
	TRACE_AND_STOP;
	return NULL;
}

void drm_fbdev_cma_hotplug_event(struct drm_fbdev_cma *fbdev_cma)
{
	TRACE_AND_STOP;
}


/*********************************************
 ** drivers/gpu/drm/imx/hdp/imx-hdp-audio.c **
 *********************************************/

void imx_hdp_register_audio_driver(struct device *dev)
{
	TRACE_AND_STOP;
}


/***********************
 ** drivers/of/base.c **
 ***********************/

bool of_device_is_available(const struct device_node *device)
{
	return device;
}


/**************************
 ** drivers/of/dynamic.c **
 **************************/

struct device_node *of_node_get(struct device_node *node)
{
	TRACE;
	return node;
}

void of_node_put(struct device_node *node)
{
	TRACE;
}


/***************************
 ** drivers/of/property.c **
 ***************************/

int of_property_read_string(const struct device_node *np, const char *propname,
                            const char **out_string)
{
	TRACE_AND_STOP;
	return -1;
}

int of_property_read_u32(const struct device_node *np, const char *propname, u32 *out_value)
{
	TRACE_AND_STOP;
	return -1;
}


/***********************************
 ** drivers/soc/imx/sc/main/ipc.c **
 ***********************************/

int sc_ipc_getMuID(uint32_t *mu_id)
{
	TRACE_AND_STOP;
	return -1;
}

void sc_ipc_close(sc_ipc_t handle)
{
	TRACE_AND_STOP;
}

sc_err_t sc_ipc_open(sc_ipc_t *handle, uint32_t id)
{
	TRACE_AND_STOP;
	return -1;
}


/********************************************
 ** drivers/soc/imx/sc/svc/misc/rpc_clnt.c **
 ********************************************/

sc_err_t sc_misc_set_control(sc_ipc_t ipc, sc_rsrc_t resource,
                             sc_ctrl_t ctrl, uint32_t val)
{
	TRACE_AND_STOP;
	return -1;
}


/********************************
 ** drivers/soc/imx/soc-imx8.c **
 ********************************/

bool check_hdcp_enabled(void)
{
	TRACE_AND_STOP;
	return false;
}

bool cpu_is_imx8mq(void)
{
	TRACE_AND_STOP;
	return false;
}

bool cpu_is_imx8qm(void)
{
	TRACE_AND_STOP;
	return false;
}


/*************************
 ** kernel/irq/devres.c **
 *************************/

int devm_request_threaded_irq(struct device *dev, unsigned int irq,
			      irq_handler_t handler, irq_handler_t thread_fn,
			      unsigned long irqflags, const char *devname,
			      void *dev_id)
{
	TRACE_AND_STOP;
	return -1;
}


/*************************
 ** kernel/irq/manage.c **
 *************************/

int disable_irq_nosync(unsigned int irq)
{
	TRACE_AND_STOP;
	return 0;
}


/******************
 ** lib/string.c **
 ******************/

char *strstr(const char * a0, const char * a1)
{
	TRACE_AND_STOP;
	return NULL;
}


/*****************
 ** linux/clk.h **
 *****************/

void clk_disable_unprepare(struct clk *clk)
{
	TRACE_AND_STOP;
}

int clk_prepare_enable(struct clk *clk)
{
	TRACE;
	return 0;
}


/************************
 ** linux/completion.h **
 ************************/

void reinit_completion(struct completion *x)
{
	TRACE_AND_STOP;
}


/*******************
 ** linux/ctype.h **
 *******************/

int isprint(int v)
{
	TRACE_AND_STOP;
	return -1;
}


/*****************
 ** linux/irq.h **
 *****************/

void irq_set_status_flags(unsigned int irq, unsigned long set)
{
	TRACE_AND_STOP;
}


/************************
 ** linux/pm_runtime.h **
 ************************/

int pm_runtime_get_sync(struct device *dev)
{
	TRACE_AND_STOP;
	return 0;
}

int pm_runtime_put_sync(struct device *dev)
{
	TRACE_AND_STOP;
	return 0;
}


/**********************
 ** linux/spinlock.h **
 **********************/

void assert_spin_locked(spinlock_t *lock)
{
	TRACE_AND_STOP;
}


/*************************
 ** linux/timekeeping.h **
 *************************/

ktime_t ktime_get_real(void)
{
	TRACE_AND_STOP;
	return -1;
}


/*************************
 ** linux/dma-mapping.h **
 *************************/

void dma_free_wc(struct device *dev, size_t size,
                 void *cpu_addr, dma_addr_t dma_addr)
{
	TRACE_AND_STOP;
}

int
dma_get_sgtable_attrs(struct device *dev, struct sg_table *sgt, void *cpu_addr,
                      dma_addr_t dma_addr, size_t size,
                      unsigned long attrs)
{
	TRACE_AND_STOP;
	return -1;
}

int dma_mmap_wc(struct device *dev,
                struct vm_area_struct *vma,
                void *cpu_addr, dma_addr_t dma_addr,
                size_t size)
{
	TRACE_AND_STOP;
	return -1;
}


/************************
 ** linux/pm-runtime.h **
 ************************/

void pm_runtime_enable(struct device *dev)
{
	TRACE;
}

void pm_runtime_disable(struct device *dev)
{
	TRACE_AND_STOP;
}


/******************
 ** linux/wait.h **
 ******************/

void wait_event_interruptible_locked(wait_queue_head_t *wq, bool condition)
{
	TRACE_AND_STOP;
}


#if 0
#include <drm/drmP.h>
#include <drm/drm_dp_mst_helper.h>
#include <drm/drm_rect.h>
#include <i915/i915_drv.h>


bool access_ok(int access, void *addr, size_t size)
{
	TRACE_AND_STOP;
	return -1;
}

int acpi_device_uevent_modalias(struct device *dev, struct kobj_uevent_env *ev)
{
	TRACE_AND_STOP;
	return -1;
}

bool acpi_driver_match_device(struct device *dev, const struct device_driver *drv)
{
	TRACE_AND_STOP;
	return -1;
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

void acpi_video_unregister(void)
{
	TRACE_AND_STOP;
}

int add_uevent_var(struct kobj_uevent_env *env, const char *format, ...)
{
	TRACE_AND_STOP;
	return -1;
}

bool capable(int cap)
{
	TRACE_AND_STOP;
	return false;
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

void cpufreq_cpu_put(struct cpufreq_policy *policy)
{
	TRACE_AND_STOP;
}
#endif

void destroy_workqueue(struct workqueue_struct *wq)
{
	TRACE_AND_STOP;
}

#if 0
void *dev_get_drvdata(const struct device *dev)
{
	TRACE_AND_STOP;
	return NULL;
}

int device_init_wakeup(struct device *dev, bool val)
{
	TRACE_AND_STOP;
	return -1;
}


int dma_set_coherent_mask(struct device *dev, u64 mask)
{
	TRACE_AND_STOP;
	return -1;
}

void dma_unmap_page(struct device *dev, dma_addr_t dma_address, size_t size, enum dma_data_direction direction)
{
	printk("%s %llx+%x\n", __func__, dma_address, size);
	TRACE; //_AND_STOP;
}

void down_read(struct rw_semaphore *sem)
{
	TRACE_AND_STOP;
}
#endif

struct dma_buf *drm_gem_prime_export(struct drm_device *dev,
                                     struct drm_gem_object *obj,
                                     int flags)
{
	TRACE_AND_STOP;
	return NULL;
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

struct drm_gem_object *drm_gem_prime_import(struct drm_device *dev,
                                            struct dma_buf *dma_buf)
{
	TRACE_AND_STOP;
	return NULL;
}

long drm_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	TRACE_AND_STOP;
	return -1;
}
#if 0
int drm_noop(struct drm_device *dev, void *data, struct drm_file *file_priv)
{
	TRACE_AND_STOP;
	return -1;
}
#endif
int drm_open(struct inode *inode, struct file *filp)
{
	TRACE_AND_STOP;
	return -1;
}
#if 0
struct drm_dma_handle *drm_pci_alloc(struct drm_device *dev, size_t size, size_t align)
{
	TRACE_AND_STOP;
	return NULL;
}

void drm_pci_free(struct drm_device *dev, struct drm_dma_handle * dmah)
{
	TRACE_AND_STOP;
}
#endif
unsigned int drm_poll(struct file *filp, struct poll_table_struct *wait)
{
	TRACE_AND_STOP;
	return -1;
}

void drm_prime_gem_destroy(struct drm_gem_object *obj, struct sg_table *sg)
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
#if 0
struct vm_area_struct *find_vma(struct mm_struct *mm, unsigned long addr)
{
	TRACE_AND_STOP;
	return NULL;
}

void flush_scheduled_work(void)
{
	TRACE_AND_STOP;
}

void __free_pages(struct page *page, unsigned int order)
{
	if (!page)
		TRACE_AND_STOP;

	printk("%s %llx(%llx) order=%x\n", __func__, page->addr, page->paddr, order);
}

void ips_link_to_i915_driver(void)
{
	TRACE;
}

int i915_cmd_parser_get_version(struct drm_i915_private *dev_priv)
{
	TRACE_AND_STOP;
	return -1;
}

int __must_check i915_gem_evict_something(struct i915_address_space *vm,
                                          u64 min_size, u64 alignment,
                                          unsigned cache_level,
                                          u64 start, u64 end,
                                          unsigned flags)
{
	TRACE_AND_STOP;
	return -1;
}

int i915_gem_execbuffer(struct drm_device *dev, void *data, struct drm_file *file_priv)
{
	TRACE_AND_STOP;
	return -1;
}

int i915_gem_execbuffer2(struct drm_device *dev, void *data, struct drm_file *file_priv)
{
	TRACE_AND_STOP;
	return -1;
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

unsigned long i915_gem_shrink(struct drm_i915_private *dev_priv,
                              unsigned long target, unsigned long *nr_scanned,
                              unsigned flags)
{
	TRACE_AND_STOP;
	return -1;
}

unsigned long i915_gem_shrink_all(struct drm_i915_private *dev_priv)
{
	TRACE_AND_STOP;
	return -1;
}

int i915_gem_userptr_ioctl(struct drm_device *dev, void *data, struct drm_file *file)
{
	TRACE_AND_STOP;
	return -1;
}

int i915_restore_state(struct drm_i915_private *dev_priv)
{
	TRACE_AND_STOP;
	return -1;
}

int i915_save_state(struct drm_i915_private *dev_priv)
{
	TRACE_AND_STOP;
	return -1;
}

void i915_teardown_sysfs(struct drm_i915_private *dev_priv)
{
	TRACE_AND_STOP;
}

void intel_csr_load_program(struct drm_device *dev)
{
	TRACE_AND_STOP;
}

void intel_csr_ucode_fini(struct drm_device *dev)
{
	TRACE_AND_STOP;
}

void intel_dvo_init(struct drm_device *dev)
{
	TRACE_AND_STOP;
}

int intel_guc_enable_ct(struct intel_guc *guc)
{
	TRACE_AND_STOP;
	return 0;
}

void intel_guc_disable_ct(struct intel_guc *guc)
{
	TRACE_AND_STOP;
}

void i915_guc_log_register(struct drm_i915_private *dev_priv)
{
	TRACE;
}

void i915_guc_log_unregister(struct drm_i915_private *dev_priv)
{
	TRACE_AND_STOP;
}

void intel_tv_init(struct drm_device *dev)
{
	TRACE_AND_STOP;
}

int kobject_uevent_env(struct kobject *kobj, enum kobject_action action, char *envp[])
{
	TRACE_AND_STOP;
	return -1;
}

void memcpy_toio(volatile void __iomem *dst, const void *src, size_t count)
{
	TRACE_AND_STOP;
}

void ndelay(unsigned long ns)
{
	TRACE_AND_STOP;
}

bool need_resched(void)
{
	TRACE;
	return false;
}
#endif
loff_t noop_llseek(struct file *file, loff_t offset, int whence)
{
	TRACE_AND_STOP;
	return -1;
}
#if 0
u64 nsecs_to_jiffies64(u64 n)
{
	TRACE_AND_STOP;
	return -1;
}

u64 nsecs_to_jiffies(u64 n)
{
	TRACE_AND_STOP;
	return -1;
}
#endif
int of_alias_get_id(struct device_node *np, const char *stem)
{
	TRACE_AND_STOP;
	return -ENOSYS;
}
#if 0
int of_driver_match_device(struct device *dev, const struct device_driver *drv)
{
	TRACE_AND_STOP;
	return -1;
}

int of_irq_get(struct device_node *dev, int index)
{
	TRACE_AND_STOP;
	return -1;
}

int of_irq_get_byname(struct device_node *dev, const char *name)
{
	TRACE_AND_STOP;
	return -1;
}

void of_node_clear_flag(struct device_node *n, unsigned long flag)
{
	TRACE_AND_STOP;
}

resource_size_t pcibios_align_resource(void * p, const struct resource *r, resource_size_t s1, resource_size_t s2)
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

int pci_enable_device(struct pci_dev *dev)
{
	TRACE;
	return 0;
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
#endif
pgprot_t pgprot_writecombine(pgprot_t prot)
{
	TRACE_AND_STOP;
	return prot;
}
#if 0
void pm_qos_remove_request(struct pm_qos_request *req)
{
	TRACE_AND_STOP;
}
#endif
void print_hex_dump(const char *level, const char *prefix_str, int prefix_type, int rowsize, int groupsize, const void *buf, size_t len, bool ascii)
{
	TRACE_AND_STOP;
}

int PTR_ERR_OR_ZERO(__force const void *ptr)
{
	TRACE_AND_STOP;
	return -1;
}
#if 0
void put_pid(struct pid *pid)
{
	TRACE;
}

int pwm_config(struct pwm_device *pwm, int duty_ns, int period_ns)
{
	TRACE_AND_STOP;
	return -1;
}

void pwm_disable(struct pwm_device *pwm)
{
	TRACE_AND_STOP;
}

int pwm_enable(struct pwm_device *pwm)
{
	TRACE_AND_STOP;
	return -1;
}

struct pwm_device *pwm_get(struct device *dev, const char *con_id)
{
	TRACE_AND_STOP;
	return NULL;
}

unsigned int pwm_get_duty_cycle(const struct pwm_device *pwm)
{
	TRACE_AND_STOP;
	return -1;
}

void pwm_put(struct pwm_device *pwm)
{
	TRACE_AND_STOP;
}

int register_reboot_notifier(struct notifier_block *nb)
{
	TRACE_AND_STOP;
	return -1;
}

int release_resource(struct resource *r)
{
	TRACE_AND_STOP;
	return -1;
}

int request_resource(struct resource *root, struct resource *r)
{
	TRACE_AND_STOP;
	return -1;
}

void seq_printf(struct seq_file *m, const char *fmt, ...)
{
	TRACE_AND_STOP;
}

void seq_puts(struct seq_file *m, const char *s)
{
	TRACE_AND_STOP;
}

int set_page_dirty(struct page *page)
{
	TRACE;
	return -1;
}

int set_pages_wb(struct page *page, int numpages)
{
	TRACE_AND_STOP;
	return -1;
}

struct page *shmem_read_mapping_page( struct address_space *mapping, pgoff_t index)
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

int signal_pending_state(long state, struct task_struct *p)
{
	TRACE_AND_STOP;
	return -1;
}

void sysfs_remove_link(struct kobject *kobj, const char *name)
{
	TRACE_AND_STOP;
}

int unregister_reboot_notifier(struct notifier_block *nb)
{
	TRACE_AND_STOP;
	return -1;
}

void up_read(struct rw_semaphore *sem)
{
	TRACE_AND_STOP;
}

phys_addr_t virt_to_phys(volatile void *address)
{
	TRACE_AND_STOP;
	return -1;
}
#endif
pgprot_t vm_get_page_prot(unsigned long vm_flags)
{
	pgprot_t prot;
	TRACE_AND_STOP;
	return prot;
}
#if 0
unsigned long vm_mmap(struct file *f, unsigned long l1, unsigned long l2, unsigned long l3, unsigned long l4, unsigned long l5)
{
	TRACE_AND_STOP;
	return -1;
}

int wake_up_process(struct task_struct *tsk)
{
	TRACE_AND_STOP;
	return -1;
}

void yield(void)
{
	TRACE;
}

void bus_unregister(struct bus_type *bus)
{
	TRACE_AND_STOP;
}

int pci_map_sg(struct pci_dev *hwdev, struct scatterlist *sg, int nents, int direction)
{
	TRACE_AND_STOP;
	return -1;
}

void pci_unmap_sg(struct pci_dev *hwdev, struct scatterlist *sg, int nents, int direction)
{
	TRACE_AND_STOP;
}
#endif
void ww_mutex_lock_slow(struct ww_mutex *lock, struct ww_acquire_ctx *ctx)
{
	TRACE_AND_STOP;
}

int  ww_mutex_lock_slow_interruptible(struct ww_mutex *lock, struct ww_acquire_ctx *ctx)
{
	TRACE_AND_STOP;
	return -1;
}

int  ww_mutex_trylock(struct ww_mutex *lock)
{
	TRACE_AND_STOP;
	return -1;
}
#if 0
void free_irq(unsigned int irq, void *dev_id)
{
	TRACE_AND_STOP;
}

void backlight_device_unregister(struct backlight_device *bd)
{
	TRACE_AND_STOP;
}

int  ww_mutex_lock_interruptible(struct ww_mutex *lock, struct ww_acquire_ctx *ctx)
{
	TRACE_AND_STOP;
	return -1;
}

void might_sleep(void)
{
	TRACE;
}

void rcu_read_lock(void)
{
	TRACE;
}

void rcu_read_unlock(void)
{
	TRACE;
}
#endif
void might_lock(struct mutex *m)
{
	TRACE;
}
#if 0
void unmap_mapping_range(struct address_space *a, loff_t const b,
                         loff_t const c, int d)
{
	TRACE_AND_STOP;
}

void intel_audio_init(struct drm_i915_private *dev_priv)
{
	TRACE;
}

void intel_audio_deinit(struct drm_i915_private *dev_priv)
{
	TRACE_AND_STOP;
}
#endif
void write_lock(rwlock_t *l)
{
	TRACE;
}

void write_unlock(rwlock_t *l)
{
	TRACE;
}

void read_lock(rwlock_t *l)
{
	TRACE_AND_STOP;
}

void read_unlock(rwlock_t *l)
{
	TRACE_AND_STOP;
}
#if 0
struct edid *drm_load_edid_firmware(struct drm_connector *connector)
{
	TRACE;
	return NULL;
}

ktime_t ktime_get_raw(void)
{
	TRACE_AND_STOP;
	return 0;
}
#endif
void write_seqlock(seqlock_t *l)
{
	TRACE;
}

void write_sequnlock(seqlock_t *l)
{
	TRACE;
}
#if 0
void drm_dev_fini(struct drm_device *dev)
{
	TRACE_AND_STOP;
}
#endif

unsigned read_seqbegin(const seqlock_t *s)
{
	TRACE;
	return 0;
}

unsigned read_seqretry(const seqlock_t *s, unsigned x)
{
	TRACE;
	return 0;
}
#if 0
void *kvmalloc(size_t s, gfp_t g)
{
	TRACE_AND_STOP;
}

void *kvmalloc_array(size_t a, size_t b, gfp_t g)
{
	TRACE_AND_STOP;
}

int i915_perf_open_ioctl(struct drm_device *dev, void *data,
                         struct drm_file *file)
{
	TRACE_AND_STOP;
	return 0;
}

int i915_perf_add_config_ioctl(struct drm_device *dev, void *data,
                               struct drm_file *file)
{
	TRACE_AND_STOP;
	return 0;
}

int i915_perf_remove_config_ioctl(struct drm_device *dev, void *data,
                                  struct drm_file *file)
{
	TRACE_AND_STOP;
	return 0;
}

void i915_perf_init(struct drm_i915_private *dev_priv)
{
	TRACE;
}

void i915_perf_fini(struct drm_i915_private *dev_priv)
{
	TRACE_AND_STOP;
}

void i915_perf_register(struct drm_i915_private *dev_priv)
{
	TRACE;
}

void i915_perf_unregister(struct drm_i915_private *dev_priv)
{
	TRACE_AND_STOP;
}

void cond_resched(void)
{
	TRACE;
}

struct page *kmap_to_page(void *p)
{
	TRACE_AND_STOP;
	return 0;
}

void vunmap(const void *a)
{
	TRACE_AND_STOP;
}

void reservation_object_init(struct reservation_object *obj)
{
	TRACE;
}

void reservation_object_fini(struct reservation_object *obj)
{
	TRACE;
}

bool reservation_object_test_signaled_rcu(struct reservation_object *obj,
                                          bool test_all)
{
	TRACE_AND_STOP;
	return false;
}

int reservation_object_lock(struct reservation_object *obj,
                            struct ww_acquire_ctx *ctx)
{
	TRACE;
	return 0;
}

void reservation_object_unlock(struct reservation_object *obj)
{
	TRACE;
}

bool reservation_object_trylock(struct reservation_object *obj)
{
	TRACE_AND_STOP;
	return false;
}

void reservation_object_add_excl_fence(struct reservation_object *obj,
                                       struct dma_fence *fence)
{
	TRACE_AND_STOP;
}

struct dma_fence * reservation_object_get_excl_rcu(struct reservation_object *obj)
{
	TRACE;
	return obj->fence_excl;
}

int reservation_object_get_fences_rcu(struct reservation_object *obj,
                                      struct dma_fence **pfence_excl,
                                      unsigned *pshared_count,
                                      struct dma_fence ***pshared)
{
	TRACE;
	*pshared_count = 0;
	*pfence_excl = NULL;
	*pshared = NULL;
	return 0;
}

void set_current_state(int state)
{
	switch (state) {
	case TASK_INTERRUPTIBLE:
		printk("%s TASK_INTERRUPTIBLE\n", __func__);
		break;
	case TASK_RUNNING:
		printk("%s TASK_RUNNING\n", __func__);
		break;
	default:
		printk("%s unknown %d\n", __func__, state);
	}
}

void __set_current_state(int state)
{
	set_current_state(state);
}

void tasklet_enable(struct tasklet_struct *t)
{
	TRACE_AND_STOP;
}

void tasklet_disable(struct tasklet_struct *t)
{
	TRACE_AND_STOP;
}

void tasklet_kill(struct tasklet_struct *t)
{
	TRACE_AND_STOP;
}

void intel_dsi_init(struct drm_i915_private *dev_priv)
{
	TRACE_AND_STOP;
}

int intel_dsi_dcs_init_backlight_funcs(struct intel_connector *intel_connector)
{
	TRACE_AND_STOP;
	return 0;
}
#endif
void rwlock_init(rwlock_t *rw)
{
	TRACE;
}

unsigned long vma_pages(struct vm_area_struct *p)
{
	TRACE_AND_STOP;
	return 0;
}
#if 0
void pwm_apply_args(struct pwm_device *p)
{
	TRACE_AND_STOP;
}

void intel_guc_ct_init_early(struct intel_guc_ct *ct)
{
	TRACE;

	enum { CTB_OWNER_HOST = 0 };

	ct->host_channel.owner = CTB_OWNER_HOST;
}

void i915_gem_object_set_cache_coherency(struct drm_i915_gem_object *obj,
                                         unsigned int cache_level)
{
	TRACE;
}

int i915_gem_evict_for_node(struct i915_address_space *vm,
                            struct drm_mm_node *node, unsigned int flags)
{
	TRACE_AND_STOP;
	return 0;
}

void i915_gem_shrinker_cleanup(struct drm_i915_private *dev_priv)
{
	TRACE_AND_STOP;
}

void i915_gem_cleanup_userptr(struct drm_i915_private *dev_priv)
{
	TRACE_AND_STOP;
}

void intel_lpe_audio_irq_handler(struct drm_i915_private *dev_priv)
{
	TRACE_AND_STOP;
}

bool drm_scdc_set_scrambling(struct i2c_adapter *adapter, bool enable)
{
	TRACE_AND_STOP;
	return false;
}

bool drm_scdc_set_high_tmds_clock_ratio(struct i2c_adapter *adapter, bool set)
{
	TRACE_AND_STOP;
	return false;
}

void intel_engine_init_cmd_parser(struct intel_engine_cs *engine)
{
	TRACE_AND_STOP;
}


bool intel_engines_are_idle(struct drm_i915_private *dev_priv)
{
	TRACE_AND_STOP;
	return false;
}

void intel_engines_park(struct drm_i915_private *i915)
{
	TRACE_AND_STOP;
}

void intel_engine_init_global_seqno(struct intel_engine_cs *engine, u32 seqno)
{
	TRACE_AND_STOP;
}

bool intel_engine_has_kernel_context(const struct intel_engine_cs *engine)
{
	TRACE_AND_STOP;
	return false;
}

void intel_engine_cleanup_common(struct intel_engine_cs *engine)
{
	TRACE_AND_STOP;
}

void intel_engines_unpark(struct drm_i915_private *i915)
{
	TRACE_AND_STOP;
}

void intel_engines_reset_default_submission(struct drm_i915_private *i915)
{
	TRACE_AND_STOP;
}

void intel_engine_dump(struct intel_engine_cs *engine, struct drm_printer *m,
                       const char *header, ...)
{
	TRACE_AND_STOP;
}

bool intel_engine_is_idle(struct intel_engine_cs *engine)
{
	TRACE_AND_STOP;
	return false;
}

unsigned int intel_engines_has_context_isolation(struct drm_i915_private *i915)
{
	TRACE_AND_STOP;
	return 0;
}

int logical_render_ring_init(struct intel_engine_cs *engine)
{
	TRACE_AND_STOP;
	return 0;
}

int logical_xcs_ring_init(struct intel_engine_cs *engine)
{
	TRACE_AND_STOP;
	return 0;
}

void intel_logical_ring_cleanup(struct intel_engine_cs *engine)
{
	TRACE_AND_STOP;
}

void intel_lr_context_resume(struct drm_i915_private *dev_priv)
{
	TRACE_AND_STOP;
}

int intel_engines_init(struct drm_i915_private *dev_priv)
{
	TRACE;
	return 0;
}

int init_workarounds_ring(struct intel_engine_cs *engine)
{
	TRACE_AND_STOP;
	return 0;
}

int intel_ring_workarounds_emit(struct drm_i915_gem_request *req)
{
	TRACE_AND_STOP;
	return 0;
}

int intel_engine_init_common(struct intel_engine_cs *engine)
{
	TRACE;
	return 0;
}

int intel_engine_create_scratch(struct intel_engine_cs *engine, int size)
{
	TRACE_AND_STOP;
	return 0;
}

int is_vmalloc_addr(const void *x)
{
	TRACE_AND_STOP;
	return 0;
}
#endif
void drm_dev_printk(const struct device *dev, const char *level,
                    unsigned int category, const char *function_name,
                    const char *prefix, const char *format, ...)
{
	TRACE_AND_STOP;
}
#if 0
void drm_dev_unregister(struct drm_device *dev)
{
	TRACE_AND_STOP;
}

void drm_dev_put(struct drm_device *dev)
{
	TRACE_AND_STOP;
}

void init_wait_entry(struct wait_queue_entry *wq_entry, int flags)
{
	TRACE;
	wq_entry->flags = flags;
	wq_entry->private = current;
	wq_entry->func = autoremove_wake_function;
	INIT_LIST_HEAD(&wq_entry->entry);
}

void spin_lock_nested(spinlock_t *lock, int subclass)
{
	TRACE;
}

struct pid *get_task_pid(struct task_struct *t, enum pid_type p)
{
	TRACE_AND_STOP;
	return NULL;
}

int set_pages_array_wb(struct page **p, int x)
{
	TRACE_AND_STOP;
	return 0;
}

void __pagevec_release(struct pagevec *pvec)
{
	TRACE;
}

struct file *shmem_file_setup_with_mnt(struct vfsmount *mnt, const char *name,
                                       loff_t size, unsigned long flags)
{
	TRACE_AND_STOP;
	return NULL;
}

int set_pages_array_wc(struct page **p, int c)
{
	TRACE_AND_STOP;
	return 0;
}

bool static_cpu_has(long c)
{
	TRACE;

	if (c == X86_FEATURE_CLFLUSH)
		return true;

	TRACE_AND_STOP;
	return false;
}

void rcu_barrier(void)
{
	TRACE_AND_STOP;
}

int i915_gemfs_init(struct drm_i915_private *i915)
{
	TRACE;
	return 0;
}

void i915_gemfs_fini(struct drm_i915_private *i915)
{
	TRACE_AND_STOP;
}

pid_t pid_nr(struct pid *p)
{
	TRACE_AND_STOP;
	return 0;
}

unsigned int work_busy(struct work_struct *w)
{
	TRACE_AND_STOP;
	return 0;
}
#endif
void enable_irq(unsigned int irq)
{
	TRACE_AND_STOP;
}

void disable_irq(unsigned int irq)
{
	TRACE;
}
#if 0
unsigned raw_read_seqcount(const seqcount_t *s)
{
	TRACE_AND_STOP;
	return 0;
}

int remap_io_mapping(struct vm_area_struct *vma, unsigned long addr,
                     unsigned long pfn, unsigned long size,
                     struct io_mapping *iomap)
{
	TRACE_AND_STOP;
	return 0;
}
#endif
void call_rcu(struct rcu_head *head, void (*func)(struct rcu_head *))
{
	TRACE;

	func(head);
}
#if 0
int read_seqcount_retry(const seqcount_t *s, unsigned x)
{
	TRACE_AND_STOP;
	return 0;
}

void synchronize_rcu(void)
{
	TRACE_AND_STOP;
}

gfp_t mapping_gfp_mask(struct address_space * mapping)
{
	TRACE;
	return __GFP_RECLAIM;
}

int down_write_killable(struct rw_semaphore *s)
{
	TRACE_AND_STOP;
	return 0;
}

void atomic_andnot(int x, atomic_t *t)
{
	t->counter &= ~x;
	TRACE;
}

unsigned int get_random_int(void)
{
	TRACE_AND_STOP;
	return 0;
}

unsigned long get_random_long(void)
{
	TRACE_AND_STOP;
	return 0;
}

bool boot_cpu_has(long x)
{
	TRACE_AND_STOP;
	return false;
}

int pagecache_write_begin(struct file *f, struct address_space *a, loff_t o,
                          unsigned w, unsigned x, struct page **y, void **z)
{
	TRACE_AND_STOP;
	return 0;
}

int pagecache_write_end(struct file *f, struct address_space *a, loff_t o,
                        unsigned w, unsigned x, struct page *y, void *z)
{
	TRACE_AND_STOP;
	return 0;
}

int drm_fb_helper_remove_conflicting_framebuffers(struct apertures_struct *a,
                                                  const char *b, bool c)
{
	TRACE;
	return 0;
}

int drm_fb_helper_add_one_connector(struct drm_fb_helper *fb_helper,
                                    struct drm_connector *connector)
{
	TRACE_AND_STOP;
	return 0;
}

int drm_fb_helper_remove_one_connector(struct drm_fb_helper *fb_helper,
                                       struct drm_connector *connector)
{
	TRACE_AND_STOP;
	return 0;
}

void drain_workqueue(struct workqueue_struct *w)
{
	TRACE_AND_STOP;
}

unsigned __read_seqcount_begin(const seqcount_t *s)
{
	TRACE;
	return 0;
}

int __read_seqcount_retry(const seqcount_t *s, unsigned x)
{
	TRACE;
	return 1;
}

void intel_init_audio_hooks(struct drm_i915_private *dev_priv)
{
	TRACE;
}

void intel_hangcheck_init(struct drm_i915_private *dev_priv)
{
	TRACE;
}

void intel_csr_ucode_suspend(struct drm_i915_private *dev_priv)
{
	TRACE_AND_STOP;
}

void intel_csr_ucode_resume(struct drm_i915_private *dev_priv)
{
	TRACE_AND_STOP;
}

struct page * nth_page(struct page * page, int n)
{
	TRACE_AND_STOP;
}

unsigned int swiotlb_max_segment(void)
{
	TRACE;
	return 0;
}
#endif
void seqlock_init (seqlock_t *s)
{
	TRACE;
}
#if 0
struct irq_domain *irq_domain_create_linear(struct fwnode_handle *f,
                                            unsigned int x,
                                            const struct irq_domain_ops *y,
                                            void *z)
{
	TRACE_AND_STOP;
	return NULL;
}

struct cpufreq_policy *cpufreq_cpu_get(unsigned int cpu)
{
	TRACE;
	return NULL;
}

void irq_dispose_mapping(unsigned int virq)
{
	TRACE_AND_STOP;
}

void irq_domain_remove(struct irq_domain *d)
{
	TRACE_AND_STOP;
}

void i915_memcpy_init_early(struct drm_i915_private *dev_priv)
{
	TRACE;
}

void add_taint(unsigned i, enum lockdep_ok o)
{
	TRACE_AND_STOP;
}

struct timespec64 ns_to_timespec64(const s64 nsec)
{
	struct timespec64 ret = { 0, 0 };

	TRACE;

	if (!nsec)
		return ret;

	s32 rest = 0;
	ret.tv_sec = div_s64_rem(nsec, NSEC_PER_SEC, &rest);
	if (rest < 0) {
		ret.tv_sec--;
		rest += NSEC_PER_SEC;
	}
	ret.tv_nsec = rest;

	return ret;
}
#endif
pgprot_t pgprot_decrypted(pgprot_t prot)
{
	TRACE_AND_STOP;
	return prot;
}
#if 0
void dev_pm_set_driver_flags(struct device *dev, u32 x)
{
	TRACE;
}
#endif
void dma_buf_put(struct dma_buf *buf)
{
	TRACE_AND_STOP;
}
#if 0
void wake_up_bit(void *p, int x)
{
	TRACE_AND_STOP;
}

unsigned cache_line_size()
{
	TRACE_AND_STOP;
	return 0;
}

void of_node_put(struct device_node *d)
{
	TRACE_AND_STOP;
}
#endif
int ___ratelimit(struct ratelimit_state *rs, const char *func)
{
	TRACE_AND_STOP;
	return 0;
}

bool _drm_lease_held(struct drm_file *f, int x)
{
	TRACE_AND_STOP;
	return false;
}
#if 0
void i915_syncmap_init(struct i915_syncmap **root)
{
	TRACE;
}

void i915_syncmap_free(struct i915_syncmap **root)
{
	TRACE;
}

char *kstrdup(const char *s, gfp_t gfp)
{
	if (!s)
		return NULL;

	size_t const len = strlen(s);
	char * ptr = kmalloc(len + 1, gfp);
	if (ptr)
		memcpy(ptr, s, len + 1);
	return ptr;
}

bool kthread_should_park(void)
{
	TRACE_AND_STOP;
	return false;
}

bool kthread_should_stop(void)
{
	TRACE_AND_STOP;
	return false;
}

int kthread_park(struct task_struct *t)
{
	TRACE_AND_STOP;
	return 0;
}

void kthread_unpark(struct task_struct *t)
{
	TRACE_AND_STOP;
}

void kthread_parkme(void)
{
	TRACE_AND_STOP;
}

int kthread_stop(struct task_struct *k)
{
	TRACE_AND_STOP;
	return 0;
}

void pagefault_disable(void)
{
	TRACE_AND_STOP;
}

void pagefault_enable(void)
{
	TRACE_AND_STOP;
}

bool irq_work_queue(struct irq_work *work)
{
	TRACE_AND_STOP;
	return false;
}

void init_irq_work(struct irq_work *work, void (*func)(struct irq_work *))
{
	TRACE_AND_STOP;
}
#endif
long long atomic64_add_return(long long i, atomic64_t *p)
{
	TRACE;
	p->counter += i;
	return p->counter;
}
#if 0
int wake_up_state(struct task_struct *tsk, unsigned int state)
{
	TRACE_AND_STOP;
	return 0;
}

void __init_waitqueue_head(struct wait_queue_head *wq_head, const char *name, struct lock_class_key *key)
{
	TRACE_AND_STOP;
}

int set_pages_array_uc(struct page **pages, int addrinarray)
{
	TRACE_AND_STOP;
	return 0;
}

bool PageSlab(struct page *page)
{
	TRACE_AND_STOP;
	return false;
}

void clflushopt(volatile void *p)
{
	TRACE;
}

int intel_guc_submission_init(struct intel_guc *guc)
{
	TRACE_AND_STOP;
	return 0;
}

int intel_guc_submission_enable(struct intel_guc *guc)
{
	TRACE_AND_STOP;
	return 0;
}

void intel_guc_submission_disable(struct intel_guc *guc)
{
	TRACE_AND_STOP;
}

void intel_guc_submission_fini(struct intel_guc *guc)
{
	TRACE_AND_STOP;
}

void i915_gem_shrinker_register(struct drm_i915_private *i915)
{
	TRACE;
}

void i915_gem_shrinker_unregister(struct drm_i915_private *i915)
{
	TRACE_AND_STOP;
}

ktime_t ktime_add_ns(const ktime_t kt, u64 nsec)
{
	TRACE_AND_STOP;
	return 0;
}

int i915_sw_fence_await_sw_fence(struct i915_sw_fence *fence,
				 struct i915_sw_fence *after,
				 wait_queue_entry_t *wq)
{
	TRACE_AND_STOP;
	return -1;
}

int i915_sw_fence_await_sw_fence_gfp(struct i915_sw_fence *fence,
				     struct i915_sw_fence *after,
				     gfp_t gfp)
{
	TRACE_AND_STOP;
	return -1;
}

void __i915_sw_fence_init(struct i915_sw_fence *fence,
			  i915_sw_fence_notify_t fn,
			  const char *name,
			  struct lock_class_key *key)
{
	TRACE;
}

void i915_sw_fence_commit(struct i915_sw_fence *fence)
{
	TRACE;
}

int i915_sw_fence_await_dma_fence(struct i915_sw_fence *fence,
				  struct dma_fence *dma,
				  unsigned long timeout,
				  gfp_t gfp)
{
	TRACE_AND_STOP;
	return -1;
}

int i915_sw_fence_await_reservation(struct i915_sw_fence *fence,
				    struct reservation_object *resv,
				    const struct dma_fence_ops *exclude,
				    bool write,
				    unsigned long timeout,
				    gfp_t gfp)
{
	TRACE;
	return 0;
}

u32 *intel_ring_begin(struct drm_i915_gem_request *req,
                      unsigned int n)
{
	TRACE_AND_STOP;
	return 0;
}

void intel_legacy_submission_resume(struct drm_i915_private *dev_priv)
{
	TRACE_AND_STOP;
}

void intel_engine_cleanup(struct intel_engine_cs *engine)
{
	TRACE_AND_STOP;
}

int intel_ring_wait_for_space(struct intel_ring *ring, unsigned int bytes)
{
	TRACE_AND_STOP;
	return 0;
}

void intel_ring_free(struct intel_ring *ring)
{
	TRACE_AND_STOP;
}

bool intel_breadcrumbs_busy(struct intel_engine_cs *engine)
{
	TRACE_AND_STOP;
	return 0;
}

void intel_engine_enable_signaling(struct drm_i915_gem_request *request,
                                   bool wakeup)
{
	TRACE_AND_STOP;
}

void intel_engine_remove_wait(struct intel_engine_cs *engine,
                              struct intel_wait *wait)
{
	TRACE_AND_STOP;
}

bool intel_engine_add_wait(struct intel_engine_cs *engine,
                           struct intel_wait *wait)
{
	TRACE_AND_STOP;
}

void __intel_engine_disarm_breadcrumbs(struct intel_engine_cs *engine)
{
	TRACE_AND_STOP;
}

struct page *virt_to_page(const void *addr)
{
	TRACE_AND_STOP;
	return 0;
}

int set_memory_wb(unsigned long addr, int numpages)
{
	TRACE_AND_STOP;
	return -1;
}

const char *acpi_dev_name(struct acpi_device *adev)
{
	TRACE_AND_STOP;
	return 0;
}

void ClearPageReserved(struct page *page)
{
	TRACE_AND_STOP;
}

int stop_machine(cpu_stop_fn_t a, void *b, const struct cpumask *c)
{
	TRACE_AND_STOP;
	return -1;
}

int unregister_acpi_notifier(struct notifier_block *nb)
{
	TRACE_AND_STOP;
	return -1;
}

const struct acpi_device_id * i2c_acpi_match_device(const struct acpi_device_id *matches,
                                                    struct i2c_client *client)
{
	TRACE_AND_STOP;
	return 0;
}

static int i2c_acpi_notify(struct notifier_block *nb, unsigned long value, void *arg)
{
	TRACE_AND_STOP;
	return -1;
}

struct notifier_block i2c_acpi_notifier = {
	.notifier_call = i2c_acpi_notify,
};

void intel_unregister_dsm_handler(void)
{
	TRACE_AND_STOP;
}

enum acpi_backlight_type acpi_video_get_backlight_type(void)
{
	TRACE_AND_STOP;
}
#endif
