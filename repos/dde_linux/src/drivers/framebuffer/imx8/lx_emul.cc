/*
 * \brief  Emulation of Linux kernel interfaces
 * \author Norman Feske
 * \author Stefan Kalkowski
 * \date   2015-08-19
 */

/*
 * Copyright (C) 2015-2017 Genode Labs GmbH
 *
 * This file is distributed under the terms of the GNU General Public License
 * version 2.
 */

/* Genode includes */
#include <base/attached_io_mem_dataspace.h>

/* local includes */
#include <component.h>

#include <lx_emul.h>
#include <lx_emul_c.h>

/* DRM-specific includes */
#include <lx_emul/extern_c_begin.h>
#include <drm/drmP.h>
#include <drm/drm_fb_cma_helper.h>
#include <drm/drm_gem_cma_helper.h>
#include <drm/drm_gem_framebuffer_helper.h>
#include <drm/drm_of.h>
#include "drm_crtc_internal.h"
#include "drm_internal.h"
#include <linux/component.h>
#include <lx_emul/extern_c_end.h>

#include <lx_kit/scheduler.h> /* dependency of lx_emul/impl/completion.h */

#include <lx_emul/impl/completion.h>
#include <lx_emul/impl/delay.h>
#include <lx_emul/impl/gfp.h>
#include <lx_emul/impl/kernel.h>
#include <lx_emul/impl/mutex.h>
#include <lx_emul/impl/sched.h>
#include <lx_emul/impl/slab.h>
#include <lx_emul/impl/spinlock.h>
#include <lx_emul/impl/timer.h>
#include <lx_emul/impl/wait.h> /* dependency of lx_emul/impl/work.h */
#include <lx_emul/impl/work.h>

#include <lx_kit/irq.h>
#include <lx_kit/malloc.h>


/********************************
 ** drivers/base/dma-mapping.c **
 ********************************/

void *dmam_alloc_coherent(struct device *dev, size_t size, dma_addr_t *dma_handle, gfp_t gfp)
{
	dma_addr_t dma_addr;
	void *addr;
	if (size > 2048) {
		addr = Lx::Malloc::dma().alloc_large(size);
		dma_addr = (dma_addr_t) Lx::Malloc::dma().phys_addr(addr);
	} else
		addr = Lx::Malloc::dma().alloc(size, 12, &dma_addr);

	*dma_handle = dma_addr;
	return addr;
}


/*****************************
 ** drivers/base/platform.c **
 *****************************/

int platform_get_irq(struct platform_device *dev, unsigned int num)
{
	struct resource *r = platform_get_resource(dev, IORESOURCE_IRQ, 0);
	return r ? r->start : -1;
}

int platform_get_irq_byname(struct platform_device *dev, const char *name)
{
	lx_printf("platform_get_irq_byname(): %s\n", name);

	struct resource *r = platform_get_resource_byname(dev, IORESOURCE_IRQ, name);
	return r ? r->start : -1;
}

struct resource *platform_get_resource(struct platform_device *dev,
                                       unsigned int type, unsigned int num)
{
	unsigned i;

	for (i = 0; i < dev->num_resources; i++) {
		struct resource *r = &dev->resource[i];

		if ((type & r->flags) && num-- == 0)
			return r;
	}

	return NULL;
}

struct resource *platform_get_resource_byname(struct platform_device *dev,
                                              unsigned int type,
                                              const char *name)
{
	unsigned i;

	for (i = 0; i < dev->num_resources; i++) {
		struct resource *r = &dev->resource[i];

	if (type == r->flags && !Genode::strcmp(r->name, name))
		return r;
	}

	return NULL;
}


static int platform_match(struct device *dev, struct device_driver *drv)
{
Genode::log("platform_match()");
	if (!dev->name) {
		Genode::log("platform_match(): !dev->name");
		return 0;
	}

	lx_printf("MATCH %s %s\n", dev->name, drv->name);
	return (Genode::strcmp(dev->name, drv->name) == 0);
}


#define to_platform_driver(drv) (container_of((drv), struct platform_driver, \
                                 driver))

static int platform_drv_probe(struct device *_dev)
{
	struct platform_driver *drv = to_platform_driver(_dev->driver);
	struct platform_device *dev = to_platform_device(_dev);
Genode::log("platform_drv_probe()");
	return drv->probe(dev);
}


struct bus_type platform_bus_type = {
	.name  = "platform",
};


int platform_driver_register(struct platform_driver * drv)
{
	lx_printf("platform_driver_register: %s\n", drv->driver.name);

	/* init platform_bus_type */
	platform_bus_type.match = platform_match;
	platform_bus_type.probe = platform_drv_probe;

	drv->driver.bus = &platform_bus_type;
	if (drv->probe)
		drv->driver.probe = platform_drv_probe;

	printk("Register: %s\n", drv->driver.name);
	return driver_register(&drv->driver);
}


int platform_device_add(struct platform_device *pdev)
{
	return platform_device_register(pdev);
}


int platform_device_add_data(struct platform_device *pdev, const void *data,
                             size_t size)
{
	void *d = NULL;

	if (data && !(d = kmemdup(data, size, GFP_KERNEL)))
		return -ENOMEM;

	kfree(pdev->dev.platform_data);
	pdev->dev.platform_data = d;

	return 0;
}


int platform_device_register(struct platform_device *pdev)
{
lx_printf("platform_device_register(): %s\n", pdev->name);
	pdev->dev.bus  = &platform_bus_type;
	pdev->dev.name = pdev->name;
	/*Set parent to ourselfs */
	if (!pdev->dev.parent)
		pdev->dev.parent = &pdev->dev;
	device_add(&pdev->dev);

	return 0;
}


struct platform_device *platform_device_alloc(const char *name, int id)
{
	platform_device *pdev = (platform_device *)kzalloc(sizeof(struct platform_device), GFP_KERNEL);

	if (!pdev)
		return 0;

	int len    = strlen(name);
	pdev->name = (char *)kzalloc(len + 1, GFP_KERNEL);

	if (!pdev->name) {
		kfree(pdev);
		return 0;
	}

	memcpy(pdev->name, name, len);
	pdev->name[len] = 0;
	pdev->id = id;
	pdev->dev.dma_mask = (u64*)kzalloc(sizeof(u64),  GFP_KERNEL);

	spin_lock_init(&pdev->dev.devres_lock);
	INIT_LIST_HEAD(&pdev->dev.devres_head);

	return pdev;
}


/***********************
 ** drivers/clk/clk.c **
 ***********************/

unsigned long clk_get_rate(struct clk * clk)
{
	if (!clk) return 0;
	return clk->rate;
}

int clk_set_rate(struct clk *clk, unsigned long rate)
{
	Genode::warning(__func__, "() not implemented");
	if (!clk) return -1;
	clk->rate = rate;
	return 0;
}


/******************************
 ** drivers/clk/clk-devres.c **
 ******************************/

struct clk *devm_clk_get(struct device *dev, const char *id)
{
	/* numbers from running Linux system */

	static struct clk clocks[] {
		{ "apb", 133333334 },
		{ "axi", 800000000 },
		{ "ipg", 133333334 },
		{ "pix", 27000000 },
		{ "rtrm", 400000000 },
		{ "dtrc", 25000000 },
	};

	for (unsigned i = 0; i < (sizeof(clocks) / sizeof(struct clk)); i++)
		if (Genode::strcmp(clocks[i].name, id) == 0)
			return &clocks[i];

	Genode::warning("MISSING CLOCK: ", id);
	return nullptr;
}


/*******************************
 ** drivers/gpu/drm/drm_drv.c **
 *******************************/

void drm_dev_printk(const struct device *dev, const char *level,
                    unsigned int category, const char *function_name,
                    const char *prefix, const char *format, ...)
{
	struct va_format vaf;
	va_list args;

	if (category && !(drm_debug & category))
		return;

	va_start(args, format);
	vaf.fmt = format;
	vaf.va = &args;

	if (dev)
		dev_printk(level, dev, "[drm:%s]%s %pV", function_name, prefix,
			   &vaf);
	else
		printk("%s" "[drm:%s]%s %pV", level, function_name, prefix, &vaf);

	va_end(args);
}

/*****************************************
 ** drivers/gpu/drm/drm_fb_cma_helper.c **
 *****************************************/

struct drm_gem_cma_object *drm_fb_cma_get_gem_obj(struct drm_framebuffer *fb,
						  unsigned int plane)
{
	struct drm_gem_object *gem;

	gem = drm_gem_fb_get_obj(fb, plane);
	if (!gem)
		return NULL;

	return to_drm_gem_cma_obj(gem);
}

int drm_fb_cma_prepare_fb(struct drm_plane *plane,
                          struct drm_plane_state *state)
{
	lx_printf("drm_fb_cma_prepare_fb(): %p\n", __builtin_return_address(0));
	return drm_gem_fb_prepare_fb(plane, state);
}


/*********************************************
 ** drivers/gpu/drm/imx/hdp/imx-hdp-audio.c **
 *********************************************/

void imx_hdp_register_audio_driver(struct device *dev)
{
	/* not supported */
}


/***********************
 ** drivers/of/base.c **
 ***********************/

static struct device_node root_device_node {
	.name = "",
	.full_name = "",
};

static struct device_node hdmi_device_node {
	.name = "hdmi",
	.full_name = "hdmi",
	.parent = &root_device_node
};

static struct device_node hdmi_endpoint_device_node {
	.name = "hdmi-endpoint",
	.full_name = "hdmi-endpoint",
};

static struct device_node endpoint_device_node {
	.name = "endpoint",
	.full_name = "endpoint",
};

static struct device_node port_device_node {
	.name = "port",
	.full_name = "port"
};

int of_device_is_compatible(const struct device_node *device,
                            const char *compat)
{
	lx_printf("of_device_is_compatible(): %s\n", compat);

	if (!device)
		return false;

	if (Genode::strcmp(compat, "nxp,imx8mq-dcss") == 0) {
		lx_printf("of_device_is_compatible(): true\n");
		return true;
	}

	return false;
}

struct device_node *of_get_next_child(const struct device_node *node,
                                      struct device_node *prev)
{
	lx_printf("of_get_next_child(): node: %p, prev: %p\n", node, prev);

	if (node)
		lx_printf("of_get_next_child(): node name: %s\n", node->name);

	if (prev)
		lx_printf("of_get_next_child(): prev name: %s\n", prev->name);

	if (Genode::strcmp(node->name, "port", strlen(node->name)) == 0) {
		if (!prev) {
			lx_printf("of_get_next_child(): returning hdmi_endpoint_device_node\n");
			return &hdmi_endpoint_device_node;
		}
		return NULL;
	}

	Genode::error("of_get_next_child(): unhandled node");

	return NULL;
}

struct device_node *of_get_parent(const struct device_node *node)
{
	lx_printf("of_get_parent(): %s\n", node->name);

	static device_node dcss_device_node { "dcss", "dcss" };

	if (!node)
		return NULL;

	if (Genode::strcmp(node->name, "port", strlen("port")) == 0)
		return &dcss_device_node;
	
	Genode::error("of_get_parent(): unhandled node");

	return NULL;
}

const void *of_get_property(const struct device_node *node, const char *name, int *lenp)
{
lx_printf("of_get_property(): %s\n", name);
	for (property * p = node ? node->properties : nullptr; p; p = p->next)
		if (Genode::strcmp(name, p->name) == 0) return p->value;

	if (DEBUG_DRIVER) Genode::warning("OF property ", name, " not found");
	return nullptr;
}

struct device_node *of_parse_phandle(const struct device_node *np, const char *phandle_name, int index)
{
	lx_printf("of_parse_phandle(): %s, %s, %d\n", np->name, phandle_name, index);

	/* device node information from fsl-imx8mq.dtsi */

	static device_node dcss_device_node {
		.name = "dcss",
		.full_name = "dcss",
	};

	static device_node port_device_node {
		.name = "port",
		.full_name = "port",
		.parent = &dcss_device_node
	};

	if ((Genode::strcmp(phandle_name, "ports", strlen(phandle_name)) == 0) &&
	    (index == 0))
		return &port_device_node;

	Genode::warning("of_parse_phandle(): unhandled phandle or index");

	return NULL;
}


/*************************
 ** drivers/of/device.c **
 *************************/

extern struct dcss_devtype dcss_type_imx8m; 

const void *of_device_get_match_data(const struct device *dev)
{
	lx_printf("of_device_get_match_data(): %s\n", dev->name);

	if (Genode::strcmp(dev->name, "dcss-core", strlen(dev->name)) == 0)
		return &dcss_type_imx8m;

	return NULL;
}

const struct of_device_id *of_match_device(const struct of_device_id *matches,
                                           const struct device *dev)
{
	lx_printf("of_match_device(): dev: %s\n", dev->name);

	const char * compatible = (const char*) of_get_property(dev->of_node, "compatible", 0);

	lx_printf("of_match_device(): dev: %s\n", dev->name);

	for (; matches && matches->compatible[0]; matches++) {
		lx_printf("of_match_device(): comparing match: %s\n", matches->compatible);
		if (Genode::strcmp(matches->compatible, compatible) == 0)
			return matches;
	}
	return nullptr;
}


/***************************
 ** drivers/of/property.c **
 ***************************/

struct device_node *of_graph_get_next_endpoint(const struct device_node *parent,
                                               struct device_node *prev)
{
	lx_printf("of_graph_get_next_endpoint(): parent: %s, prev: %s\n",
	          parent->name, prev ? prev->name : "");

	if (Genode::strcmp(parent->name, "hdmi", strlen(parent->name)) == 0) {

		if (!prev)
			return &endpoint_device_node;

		return nullptr;
	}

	Genode::error(__func__, "(): unhandled parent");

	return nullptr;
}

struct device_node *of_graph_get_port_by_id(struct device_node *parent, u32 id)
{
lx_printf("of_graph_get_port_by_id(): parent: %s, id: %u\n", parent->name, id);

	if ((Genode::strcmp(parent->name, "dcss", strlen(parent->name)) == 0) &&
	    (id == 0))
		return &port_device_node;

	Genode::error("of_graph_get_port_by_id(): unhandled parent or id\n");

	return NULL;
}

struct device_node *of_graph_get_remote_port(const struct device_node *node)
{
	lx_printf("of_graph_get_remote_port(): %s\n", node->name);

	if (Genode::strcmp(node->name, "endpoint", strlen(node->name)) == 0)
		return &port_device_node;

	Genode::error("of_graph_get_remote_port(): unhandled node\n");

	return NULL;
}

struct device_node *of_graph_get_remote_port_parent(const struct device_node *node)
{
	lx_printf("of_graph_get_remote_port_parent(): %s\n", node->name);

	if (Genode::strcmp(node->name, "hdmi-endpoint") == 0)
		return &hdmi_device_node;

	Genode::error("of_graph_get_remote_port_parent(): unhandled node");

	return NULL;
}


/********************************
 ** drivers/soc/imx/soc-imx8.c **
 ********************************/

bool check_hdcp_enabled(void)
{
	return false;
}

bool cpu_is_imx8mq(void)
{
	return true;
}


/***********************
 ** kernel/irq/chip.c **
 ***********************/

static struct irq_chip *irqsteer_chip = nullptr;

static struct irq_desc irqsteer_irq_desc;

static irqreturn_t irqsteer_irq_handler(int irq, void *data)
{
	irqsteer_irq_desc.handle_irq(&irqsteer_irq_desc);
	return IRQ_HANDLED;
}

void irq_set_chained_handler_and_data(unsigned int irq,
                                      irq_flow_handler_t handle,
                                      void *data)
{
	Genode::log("*** irq_set_chained_handler_and_data(): ", irq);

	irqsteer_irq_desc.irq_common_data.handler_data = data;
	irqsteer_irq_desc.irq_data.chip = irqsteer_chip;
	irqsteer_irq_desc.handle_irq = handle;

	Lx::Irq::irq().request_irq(Platform::Device::create(Lx_kit::env().env(), irq),
	                           irq, irqsteer_irq_handler, nullptr, nullptr);
}


/*************************
 ** kernel/irq/devres.c **
 *************************/

int devm_request_threaded_irq(struct device *dev, unsigned int irq,
			      irq_handler_t handler, irq_handler_t thread_fn,
			      unsigned long irqflags, const char *devname,
			      void *dev_id)
{
Genode::log("devm_request_threaded_irq(): ", irq, ", handler: ", (void*)handler, ", thread_fn: ", (void*)thread_fn);

	/* ignore irqsteer IRQs for now */

	if (irq > 31)
		Lx::Irq::irq().request_irq(Platform::Device::create(Lx_kit::env().env(), irq),
		                           irq, handler, dev_id, thread_fn);

	return 0;
}


/**************************
 ** kernel/irq/irqdesc.c **
 **************************/

static irq_handler_t irqsteer_handler[32];
static void *irqsteer_dev_id[32];

int generic_handle_irq(unsigned int irq)
{
	/* only irqsteer irqs (< 32) are expected */

	if (irq > 31) {
		Genode::error(__func__, "(): unexpected irq ", irq);
		Genode::sleep_forever();
	}

	if (!irqsteer_handler[irq]) {
		Genode::error(__func__, "(): missing handler for irq ", irq);
		return -1;
	}
	
	irqsteer_handler[irq](irq, irqsteer_dev_id[irq]);

	return 0;
}


/****************************
 ** kernel/irq/irqdomain.c **
 ****************************/

struct irq_domain *__irq_domain_add(struct fwnode_handle *fwnode, int size,
				    irq_hw_number_t hwirq_max, int direct_max,
				    const struct irq_domain_ops *ops,
				    void *host_data)
{
	Genode::log("__irq_domain_add()");

	static struct irq_domain domain = {
		.ops = ops,
		.host_data = host_data,
	};

	{
		/* trigger a call of 'irq_set_chip_and_handler()' to get access to the irq_chip struct */
		static bool mapped = false;
		if (!mapped) {
			mapped = true;
			ops->map(&domain, 0, 0);
		}
	}

	return &domain;
}


/*************************
 ** kernel/irq/manage.c **
 *************************/

void enable_irq(unsigned int irq)
{
	//lx_printf("*** enable_irq(): %d\n", irq);

	if (irq < 32) {
		if (!irqsteer_chip)
			panic("'irqsteer_chip' uninitialized");

		struct irq_data irq_data {
			.hwirq = irq,
			.chip = irqsteer_chip,
			.chip_data = irqsteer_irq_desc.irq_common_data.handler_data,
		};

		irqsteer_chip->irq_unmask(&irq_data);
		return;
	}

	Lx::Irq::irq().enable_irq(irq);
}

void disable_irq(unsigned int irq)
{
	//lx_printf("*** disable_irq(): %d\n", irq);

	if (irq < 32) {
		if (!irqsteer_chip)
			panic("'irqsteer_chip' uninitialized");

		struct irq_data irq_data {
			.hwirq = irq,
			.chip = irqsteer_chip,
			.chip_data = irqsteer_irq_desc.irq_common_data.handler_data,
		};

		irqsteer_chip->irq_mask(&irq_data);
		return;
	}

	Lx::Irq::irq().disable_irq(irq);
}

int disable_irq_nosync(unsigned int irq)
{
	disable_irq(irq);
	return 0;
}


/******************
 ** lib/devres.c **
 ******************/

static void *_ioremap(phys_addr_t phys_addr, unsigned long size, int wc)
{
	try {
		Genode::Attached_io_mem_dataspace *ds = new(Lx::Malloc::mem())
			Genode::Attached_io_mem_dataspace(Lx_kit::env().env(), phys_addr, size, !!wc);
		return ds->local_addr<void>();
	} catch (...) {
		//panic("Failed to request I/O memory: [%lx,%lx)", phys_addr, phys_addr + size);
		return 0;
	}
}

void *devm_ioremap(struct device *dev, resource_size_t offset,
                   unsigned long size)
{
lx_printf("devm_ioremap(): offset: %lx, size: 0x%lx\n", offset, size);
	return _ioremap(offset, size, 0);
}

void *devm_ioremap_resource(struct device *dev, struct resource *res)
{
	return _ioremap(res->start, (res->end - res->start) + 1, 0);
}


/******************
 ** lib/string.c **
 ******************/

int strcmp(const char *a,const char *b)
{
	return Genode::strcmp(a, b);
}


/************************
 ** linux/completion.h **
 ************************/

void reinit_completion(struct completion *work)
{
	init_completion(work);
}


/********************
 ** linux/device.h **
 ********************/

/**
 * Simple driver management class
 */
class Driver : public Genode::List<Driver>::Element
{
	private:

		struct device_driver *_drv; /* Linux driver */

	public:

		Driver(struct device_driver *drv) : _drv(drv)
		{
			list()->insert(this);
		}

		/**
		 * List of all currently registered drivers
		 */
		static Genode::List<Driver> *list()
		{
			static Genode::List<Driver> _list;
			return &_list;
		}

		/**
		 * Match device and drivers
		 */
		bool match(struct device *dev)
		{
			/*
			 *  Don't try if buses don't match, since drivers often use 'container_of'
			 *  which might cast the device to non-matching type
			 */
			if (_drv->bus != dev->bus) {
				Genode::log("match(): bus mismatch");
				return false;
			}

			return _drv->bus->match ? _drv->bus->match(dev, _drv) : true;
		}

		/**
		 * Probe device with driver
		 */
		int probe(struct device *dev)
		{
			dev->driver = _drv;

			if (dev->bus->probe)
				return dev->bus->probe(dev);
			else if (_drv->probe)
				return _drv->probe(dev);

			return 0;
		}
};


int driver_register(struct device_driver *drv)
{
	new (Lx::Malloc::mem()) Driver(drv);
	return 0;
}


int device_add(struct device *dev)
{
lx_printf("device_add(): %s, %p\n", dev->name, __builtin_return_address(0));
	if (dev->driver)
		return 0;

	/* foreach driver match and probe device */
	for (Driver *driver = Driver::list()->first(); driver; driver = driver->next())
		if (driver->match(dev)) {
			int ret = driver->probe(dev);

			if (!ret) return 0;
		}

	return 0;
}



/*************************
 ** linux/dma-mapping.h **
 *************************/

struct Dma_wc_dataspace : Genode::Attached_ram_dataspace,
                          Genode::List<Dma_wc_dataspace>::Element
{
	Dma_wc_dataspace(size_t size)
	: Genode::Attached_ram_dataspace(Lx_kit::env().ram(),
	                                 Lx_kit::env().rm(),
	                                 size,
	                                 Genode::Cache_attribute::WRITE_COMBINED) { }
};

static Genode::List<Dma_wc_dataspace> _dma_wc_ds_list;

void *dma_alloc_wc(struct device *dev, size_t size,
                   dma_addr_t *dma_addr, gfp_t gfp)
{
	Dma_wc_dataspace *dma_wc_ds = new (Lx::Malloc::mem()) Dma_wc_dataspace(size);

	_dma_wc_ds_list.insert(dma_wc_ds);

	*dma_addr = Genode::Dataspace_client(dma_wc_ds->cap()).phys_addr();
	return dma_wc_ds->local_addr<void>();
}

void dma_free_wc(struct device *dev, size_t size,
                 void *cpu_addr, dma_addr_t dma_addr)
{
	for (Dma_wc_dataspace *ds = _dma_wc_ds_list.first(); ds; ds = ds->next()) {
		if (ds->local_addr<void>() == cpu_addr) {
			_dma_wc_ds_list.remove(ds);
			destroy(Lx::Malloc::mem(), ds);
			return;
		}
	}

	Genode::error("dma_free_wc(): unknown address");
}


/***********************
 ** linux/interrupt.h **
 ***********************/

int devm_request_irq(struct device *dev, unsigned int irq,
                     irq_handler_t handler, unsigned long irqflags,
                     const char *devname, void *dev_id)
{
//	Genode::log("*** devm_request_irq(): ", Genode::Cstring(devname), ", ", irq);

	if (irq < 32) {
		irqsteer_handler[irq] = handler;
		irqsteer_dev_id[irq] = dev_id;
		enable_irq(irq);
	} else
		Lx::Irq::irq().request_irq(Platform::Device::create(Lx_kit::env().env(), irq),
		                           irq, handler, dev_id);

	return 0;
}


/*****************
 ** linux/irq.h **
 *****************/

void irq_set_chip_and_handler(unsigned int irq, struct irq_chip *chip,
                              irq_flow_handler_t)
{
	lx_printf("irq_set_chip_and_handler(): %p\n", chip);
	irqsteer_chip = chip;
}


/****************
 ** linux/of.h **
 ****************/

bool of_property_read_bool(const struct device_node *np, const char *propname)
{
	if ((Genode::strcmp(np->name, "hdmi", strlen(np->name)) == 0)) {

		if ((Genode::strcmp(propname, "fsl,cec", strlen(np->name)) == 0) ||
		    (Genode::strcmp(propname, "fsl,use_digpll_pclock", strlen(np->name)) == 0) ||
		    (Genode::strcmp(propname, "fsl,no_edid", strlen(np->name)) == 0))
			return false;

		Genode::error(__func__, "(): unhandled property '", propname,
		                        "' of device '", Genode::Cstring(np->name), "'");

		return false;
	}

	Genode::error(__func__, "(): unhandled device '", Genode::Cstring(np->name),
	                        "' (property: '", Genode::Cstring(propname), "')");

	return false;
}

int of_property_read_string(const struct device_node *np, const char *propname,
                            const char **out_string)
{
	if (Genode::strcmp(np->name, "hdmi", strlen(np->name)) == 0) {

	    if (Genode::strcmp(propname, "compatible") == 0) {
			*out_string = "fsl,imx8mq-hdmi";
			return 0;
		}

		Genode::error(__func__, "(): unhandled property '", propname,
		                        "' of device '", Genode::Cstring(np->name), "'");
		return -1;
	}

	Genode::error(__func__, "(): unhandled device '", Genode::Cstring(np->name),
	                        "' (property: '", Genode::Cstring(propname), "')");
	return -1;
}

int of_property_read_u32(const struct device_node *np, const char *propname, u32 *out_value)
{

	if ((Genode::strcmp(np->name, "imx-irqsteer", strlen(np->name)) == 0)) {

		if (Genode::strcmp(propname, "nxp,irqsteer_chans") == 0) {

			*out_value = 2;
			return 0;

		} else if (Genode::strcmp(propname, "nxp,endian") == 0) {

			*out_value = 1;
			return 0;
		}

		Genode::error(__func__, "(): unhandled property '", propname,
		                        "' of device '", Genode::Cstring(np->name), "'");
		return -1;

	} else if (Genode::strcmp(np->name, "hdmi", strlen(np->name)) == 0) {

	    if (Genode::strcmp(propname, "hdcp-config") == 0) {
		    /* no such property in original device tree */
			return -1;
		}

		Genode::error(__func__, "(): unhandled property '", propname,
		                        "' of device '", Genode::Cstring(np->name), "'");
		return -1;
	}

	Genode::error(__func__, "(): unhandled device '", Genode::Cstring(np->name),
	                        "' (property: '", Genode::Cstring(propname), "')");

	return -1;
}


/***************
 ** mm/util.c **
 ***************/

void kvfree(const void *p)
{
	kfree(p);
}


#if 0
/* Genode includes */
#include <util/bit_allocator.h>
#include <base/log.h>
#include <base/attached_io_mem_dataspace.h>
#include <os/reporter.h>
#endif
#if 0
/* DRM-specific includes */
#include <lx_emul.h>
#include <lx_emul_c.h>
#include <lx_emul/extern_c_begin.h>
#include <drm/drmP.h>
#include <drm/drm_gem.h>
#include "drm_crtc_internal.h"
#include <lx_emul/extern_c_end.h>
#include <drm/i915_drm.h>

#include <lx_emul/impl/kernel.h>
#include <lx_emul/impl/delay.h>
#include <lx_emul/impl/slab.h>
#include <lx_emul/impl/gfp.h>
#include <lx_emul/impl/io.h>
#include <lx_emul/impl/pci.h>
#include <lx_emul/impl/spinlock.h>
#include <lx_emul/impl/mutex.h>
#include <lx_emul/impl/sched.h>
#include <lx_emul/impl/timer.h>
#include <lx_emul/impl/completion.h>
#include <lx_emul/impl/wait.h>
#endif
static struct drm_device * lx_drm_device = nullptr;
#if 0
struct irq_chip dummy_irq_chip;
#endif
enum { MAX_BRIGHTNESS = 100U }; /* we prefer percentage */

struct Mutex_guard
{
	struct mutex &_mutex;
	Mutex_guard(struct mutex &m) : _mutex(m) { mutex_lock(&_mutex); }
	~Mutex_guard() { mutex_unlock(&_mutex); }
};

struct Drm_guard
{
	drm_device * _dev;

	Drm_guard(drm_device *dev) : _dev(dev)
	{
		if (dev) {
			mutex_lock(&dev->mode_config.mutex);
			mutex_lock(&dev->mode_config.blob_lock);
			drm_modeset_lock_all(dev);
		}
	}

	~Drm_guard()
	{
		if (_dev) {
			drm_modeset_unlock_all(_dev);
			mutex_unlock(&_dev->mode_config.mutex);
			mutex_unlock(&_dev->mode_config.blob_lock);
		}
	}
};


template <typename FUNCTOR>
static inline void lx_for_each_connector(drm_device * dev, FUNCTOR f)
{
	struct drm_connector *connector;
	list_for_each_entry(connector, &dev->mode_config.connector_list, head)
		f(connector);
}


drm_display_mode *
Framebuffer::Driver::_preferred_mode(drm_connector *connector,
                                     unsigned &brightness)
{
	using namespace Genode;
	using Genode::size_t;

	/* try to read configuration for connector */
	try {
		Xml_node config = _session.config();
		Xml_node xn = config.sub_node();
		for (unsigned i = 0; i < config.num_sub_nodes(); xn = xn.next()) {
			if (!xn.has_type("connector"))
				continue;

			typedef String<64> Name;
			Name const con_policy = xn.attribute_value("name", Name());
			if (con_policy != connector->name)
				continue;

			bool enabled = xn.attribute_value("enabled", true);
			if (!enabled)
				return nullptr;

			brightness = xn.attribute_value("brightness",
			                                (unsigned)MAX_BRIGHTNESS + 1);

			unsigned long const width  = xn.attribute_value("width",  0UL);
			unsigned long const height = xn.attribute_value("height", 0UL);
			long          const hz     = xn.attribute_value("hz",     0L);

			struct drm_display_mode *mode;
			list_for_each_entry(mode, &connector->modes, head) {
			if (mode->hdisplay == (int) width &&
				mode->vdisplay == (int) height)
				if (!hz || hz == mode->vrefresh)
					return mode;
		};
		}
	} catch (...) {

Genode::log("_preferred_mode(): catch, connected: ", (connector->status == connector_status_connected));
		/**
		 * If no config is given, we take the most wide mode of a
		 * connector as long as it is connected at all
		 */
		if (connector->status != connector_status_connected)
			return nullptr;

		struct drm_display_mode *mode = nullptr, *tmp;
		list_for_each_entry(tmp, &connector->modes, head) {
			if (!mode || tmp->hdisplay > mode->hdisplay) mode = tmp;
		};
		return mode;
   	}

	return nullptr;
}


void Framebuffer::Driver::finish_initialization()
{
	if (!lx_drm_device) {
		Genode::error("no drm device");
		return;
	}

	lx_c_set_driver(lx_drm_device, (void*)this);

	generate_report();

	_session.config_changed();
}

#if 0
#include <lx_kit/irq.h>
#endif
#if 0
void Framebuffer::Driver::_poll()
{
	Lx::Pci_dev * pci_dev = (Lx::Pci_dev*) lx_drm_device->pdev->bus;
	Lx::Irq::irq().inject_irq(pci_dev->client());
}

void Framebuffer::Driver::set_polling(Genode::uint64_t poll)
{
	if (poll == _poll_ms) return;

	_poll_ms = poll;

	if (_poll_ms) {
		_timer.sigh(_poll_handler);
		_timer.trigger_periodic(_poll_ms * 1000);
	} else {
		_timer.sigh(Genode::Signal_context_capability());
	}
}
#endif

void Framebuffer::Driver::update_mode()
{
	using namespace Genode;

	Configuration old = _config;
	_config = Configuration();
Genode::log("update_mode()");

	lx_for_each_connector(lx_drm_device, [&] (drm_connector *c) {
Genode::log("update_mode(): lx_for_each_connector()");
		unsigned brightness;
		drm_display_mode * mode = _preferred_mode(c, brightness);
Genode::log("update_mode(): mode: ", mode);
		if (!mode) return;

		if (mode->hdisplay > _config._lx.width)  _config._lx.width  = mode->hdisplay;
		if (mode->vdisplay > _config._lx.height) _config._lx.height = mode->vdisplay;

	});

	lx_c_allocate_framebuffer(lx_drm_device, &_config._lx);

	if (!_config._lx.lx_fb) {
		Genode::error("updating framebuffer failed");
		return;
	}

	{
		Drm_guard guard(lx_drm_device);
		lx_for_each_connector(lx_drm_device, [&] (drm_connector *c) {
			unsigned brightness = MAX_BRIGHTNESS + 1;

			/* set mode */
			lx_c_set_mode(lx_drm_device, c, _config._lx.lx_fb,
			              _preferred_mode(c, brightness));
#if 0
			/* set sane brightness, ignore unsane values and let as is */
			if (brightness <= MAX_BRIGHTNESS)
				lx_c_set_brightness(c, brightness, MAX_BRIGHTNESS);
#endif
		});
	}

Genode::log("update_mode(): check");

#if 1
	/* force virtual framebuffer size if requested */
	if (int w = _session.force_width_from_config())
		_config._lx.width = min(_config._lx.width, w);
	if (int h = _session.force_height_from_config())
		_config._lx.height = min(_config._lx.height, h);

Genode::log("update_mode(): check 2");

	//if (old._lx.addr)  Lx::iounmap(old._lx.addr);
	if (old._lx.lx_fb) {
Genode::log("update_mode(): check 3");

		if (drm_framebuffer_read_refcount(old._lx.lx_fb) > 1) {
			/*
			 * If one sees this message, we are going to leak a lot of
			 * memory (e.g. framebuffer) and this will cause later on
			 * resource requests by this driver ...
			 */
			Genode::warning("framebuffer refcount ",
			                drm_framebuffer_read_refcount(old._lx.lx_fb));
		}
Genode::log("update_mode(): check 4");
		drm_framebuffer_remove(old._lx.lx_fb);
Genode::log("update_mode(): check 5");
	}
#endif
}

#if 1
void Framebuffer::Driver::generate_report()
{
Genode::log("Framebuffer::Driver::generate_report()");
	/* detect mode information per connector */
	{
		Mutex_guard mutex(lx_drm_device->mode_config.mutex);

		struct drm_connector *c;
		list_for_each_entry(c, &lx_drm_device->mode_config.connector_list,
		                    head)
		{
			/*
			 * All states unequal to disconnected are handled as connected,
			 * since some displays stay in unknown state if not fill_modes()
			 * is called at least one time.
			 */
			bool connected = c->status != connector_status_disconnected;
			if ((connected && list_empty(&c->modes)) ||
			    (!connected && !list_empty(&c->modes)))
				c->funcs->fill_modes(c, 0, 0);
		}
	}

	/* check for report configuration option */
	try {
		_reporter.enabled(_session.config().sub_node("report")
		                 .attribute_value(_reporter.name().string(), false));
	} catch (...) {
		_reporter.enabled(false);
	}
	if (!_reporter.enabled()) return;

	/* write new report */
	try {
		Genode::Reporter::Xml_generator xml(_reporter, [&] ()
		{
			Drm_guard guard(lx_drm_device);
			struct drm_connector *c;
			list_for_each_entry(c, &lx_drm_device->mode_config.connector_list,
			                    head) {
				xml.node("connector", [&] ()
				{
					bool connected = c->status == connector_status_connected;
					xml.attribute("name", c->name);
					xml.attribute("connected", connected);
#if 0
					/* unsane values means no brightness support */
					unsigned brightness = lx_c_get_brightness(c, MAX_BRIGHTNESS + 1);
					if (brightness <= MAX_BRIGHTNESS)
						xml.attribute("brightness", brightness);
#endif
					if (!connected) return;

					struct drm_display_mode *mode;
					list_for_each_entry(mode, &c->modes, head) {
						xml.node("mode", [&] ()
						{
							xml.attribute("width",  mode->hdisplay);
							xml.attribute("height", mode->vdisplay);
							xml.attribute("hz",     mode->vrefresh);
						});
					}
				});
			}
		});
	} catch (...) {
		Genode::warning("Failed to generate report");
	}
Genode::log("Framebuffer::Driver::generate_report() finished");
}
#endif

extern "C" {

#if 0
/**********************
 ** Global variables **
 **********************/

struct task_struct *current;

struct boot_cpu_data boot_cpu_data =
{
	.x86_clflush_size = (sizeof(void*) == 8) ? 64 : 32,
};
#endif

/****************************
 ** kernel/printk/printk.c **
 ****************************/
 
int oops_in_progress;

#if 0
/********************
 ** linux/string.h **
 ********************/

char *strcpy(char *to, const char *from)
{
	char *save = to;
	for (; (*to = *from); ++from, ++to);
	return(save);
}
#endif
char *strncpy(char *dst, const char* src, size_t n)
{
	return Genode::strncpy(dst, src, n);
}

int strncmp(const char *cs, const char *ct, size_t count)
{
	return Genode::strcmp(cs, ct, count);
}

int memcmp(const void *cs, const void *ct, size_t count)
{
	/* original implementation from lib/string.c */
	const unsigned char *su1, *su2;
	int res = 0;

	for (su1 = (unsigned char*)cs, su2 = (unsigned char*)ct;
	     0 < count; ++su1, ++su2, count--)
		if ((res = *su1 - *su2) != 0)
			break;
	return res;
}

void *memchr_inv(const void *s, int cc, size_t n)
{
	if (!s)
		return NULL;

	uint8_t const c = cc;
	uint8_t const * start = (uint8_t const *)s;

	for (uint8_t const *i = start; i >= start && i < start + n; i++)
		if (*i != c)
			return (void *)i;

	return NULL;
}

size_t strlen(const char *s)
{
	return Genode::strlen(s);
}

long simple_strtol(const char *cp, char **endp, unsigned int base)
{
	unsigned long result = 0;
	size_t ret = Genode::ascii_to_unsigned(cp, result, base);
	if (endp) *endp = (char*)cp + ret;
	return result;
}

size_t strlcpy(char *dest, const char *src, size_t size)
{
	size_t ret = strlen(src);

	if (size) {
		size_t len = (ret >= size) ? size - 1 : ret;
		Genode::memcpy(dest, src, len);
		dest[len] = '\0';
	}
	return ret;
}
#if 0
size_t strlcat(char *dest, const char *src, size_t count)
{
	size_t dsize = strlen(dest);
	size_t len = strlen(src);
	size_t res = dsize + len;

	/* This would be a bug */
	BUG_ON(dsize >= count);

	dest += dsize;
	count -= dsize;
	if (len >= count)
		len = count-1;
	memcpy(dest, src, len);
	dest[len] = 0;
	return res;
}

int sysfs_create_link(struct kobject *kobj, struct kobject *target, const char *name)
{
	TRACE;
	return 0;
}


/*****************
 ** linux/dmi.h **
 *****************/

int dmi_check_system(const struct dmi_system_id *list)
{
	TRACE;
	/*
	 * Is used to check for quirks of the platform.
	 */
	return 0;
}

/*******************
 ** Kernel memory **
 *******************/

dma_addr_t page_to_phys(struct page *page)
{
	return page->paddr;
}

void *kmem_cache_zalloc(struct kmem_cache *cache, gfp_t flags)
{
	void * const ret = kmem_cache_alloc(cache, flags);
	if (ret)
		memset(ret, 0, cache->size());
	return ret;
}
#endif
void *krealloc(const void *p, size_t size, gfp_t flags)
{
	/* use const-less version from <impl/slab.h> */
	return krealloc(const_cast<void*>(p), size, flags);
}

#if 0
/**********************
 ** asm/cacheflush.h **
 **********************/

int set_pages_uc(struct page *page, int numpages)
{
	TRACE;
	return 0;
}


/********************
 ** linux/ioport.h **
 ********************/

struct resource iomem_resource;


/*********
 ** PCI **
 *********/

extern "C" void intel_graphics_quirks(int num, int slot, int func);

u8 read_pci_config_byte(u8 const bus, u8 const slot, u8 const func, u8 const offset)
{
	u8 pci_value = 0xffu;

	for (Lx::Pci_dev *pci_dev = Lx::pci_dev_registry()->first(); pci_dev;
	     pci_dev = pci_dev->next())
	{
		unsigned char dev_bus = 0, dev_slot = 0, dev_fn = 0;
		pci_dev->client().bus_address(&dev_bus, &dev_slot, &dev_fn);

		if (dev_bus != bus || dev_slot != slot || dev_fn != func)
			continue;

		pci_read_config_byte(pci_dev, offset, &pci_value);
		return pci_value;
	}

	using Genode::Hex;
	error(__func__, " - unknown device called ",
	      Hex(bus, Hex::OMIT_PREFIX, Hex::PAD), ":",
	      Hex(slot, Hex::OMIT_PREFIX, Hex::PAD), ".", func);
	return pci_value;
}

u16 read_pci_config_16(u8 const bus, u8 const slot, u8 const func, u8 const offset)
{
	u16 pci_value = 0xffffu;

	for (Lx::Pci_dev *pci_dev = Lx::pci_dev_registry()->first(); pci_dev;
	     pci_dev = pci_dev->next())
	{
		unsigned char dev_bus = 0, dev_slot = 0, dev_fn = 0;
		pci_dev->client().bus_address(&dev_bus, &dev_slot, &dev_fn);

		if (dev_bus != bus || dev_slot != slot || dev_fn != func)
			continue;

		pci_read_config_word(pci_dev, offset, &pci_value);
		return pci_value;
	}

	using Genode::Hex;
	error(__func__, " - unknown device called ",
	      Hex(bus, Hex::OMIT_PREFIX, Hex::PAD), ":",
	      Hex(slot, Hex::OMIT_PREFIX, Hex::PAD), ".", func);
	return pci_value;
}

u32 read_pci_config(u8 const bus, u8 const slot, u8 const func, u8 const offset)
{
	u32 pci_value = ~0U;

	for (Lx::Pci_dev *pci_dev = Lx::pci_dev_registry()->first(); pci_dev;
	     pci_dev = pci_dev->next())
	{
		unsigned char dev_bus = 0, dev_slot = 0, dev_fn = 0;
		pci_dev->client().bus_address(&dev_bus, &dev_slot, &dev_fn);

		if (dev_bus != bus || dev_slot != slot || dev_fn != func)
			continue;

		pci_read_config_dword(pci_dev, offset, &pci_value);
		return pci_value;
	}

	using Genode::Hex;
	error(__func__, " - unknown device called ",
	      Hex(bus, Hex::OMIT_PREFIX, Hex::PAD), ":",
	      Hex(slot, Hex::OMIT_PREFIX, Hex::PAD), ".", func);
	return pci_value;
}

struct pci_dev *pci_get_bus_and_slot(unsigned int bus, unsigned int devfn)
{
	if (bus != 0 || devfn != 0)
		TRACE_AND_STOP;

	pci_dev *pci_dev = nullptr;

	auto lamda = [&] (Platform::Device_capability cap) {

		Platform::Device_client client(cap);

		/* request bus address of device from platform driver */
		unsigned char dev_bus = 0, dev_slot = 0, dev_fn = 0;
		client.bus_address(&dev_bus, &dev_slot, &dev_fn);

		if (dev_bus == bus && PCI_SLOT(devfn) == dev_slot && PCI_FUNC(devfn) == dev_fn) {
			Lx::Pci_dev *dev = new (Lx::Malloc::mem()) Lx::Pci_dev(cap);
			Lx::pci_dev_registry()->insert(dev);
			pci_dev = dev;
			return true;
		}

		return false;
	};

	Lx::for_each_pci_device(lamda);


	/* XXX better place to apply quirks ? */
	if (!intel_graphics_stolen_res.start) {
		for (Lx::Pci_dev *pci_dev = Lx::pci_dev_registry()->first(); pci_dev;
		     pci_dev = pci_dev->next())
		{
			unsigned char dev_bus = 0, dev_slot = 0, dev_fn = 0;
			pci_dev->client().bus_address(&dev_bus, &dev_slot, &dev_fn);

			intel_graphics_quirks(dev_bus, dev_slot, dev_fn);
		}
	}

	if (intel_graphics_stolen_res.start)
		Genode::log("Intel graphics stolen memory ",
		            Genode::Hex(intel_graphics_stolen_res.start), "-",
		            Genode::Hex(intel_graphics_stolen_res.end));
	else
		Genode::error("Intel graphics stolen memory missing - trouble ahead!");

	return pci_dev;
}


void pci_dev_put(struct pci_dev *pci_dev)
{
	Lx::Pci_dev *dev = static_cast<Lx::Pci_dev*>(pci_dev);
	if (!dev)
		return;

	Lx::pci_dev_registry()->remove(dev);
	Genode::destroy(Lx::Malloc::mem(), dev);
}


struct pci_dev *pci_get_class(unsigned int class_code, struct pci_dev *from)
{
	/*
	 * The function is solely called by the i915 initialization code to
	 * probe for the ISA bridge device in order to detect the hardware
	 * generation.
	 *
	 * We look up the bridge but don't need to support the iteration over
	 * multiple devices of the given class.
	 */
	if (from) {
		TRACE_AND_STOP;
		return nullptr;
	}

	pci_dev *pci_dev = nullptr;

	auto lamda = [&] (Platform::Device_capability cap) {

		Platform::Device_client client(cap);

		Lx::Pci_dev * const dev = new (Lx::Malloc::mem()) Lx::Pci_dev(cap);
		Lx::pci_dev_registry()->insert(dev);
		pci_dev = dev;

		return true;
	};

	Lx::for_each_pci_device(lamda, class_code, class_code);

	return pci_dev;
}


void __iomem *pci_iomap(struct pci_dev *dev, int bar, unsigned long max)
{
	return pci_ioremap_bar(dev, bar);
}


struct pci_dev *pci_dev_get(struct pci_dev *dev)
{
	TRACE;
	return dev;
};


int vga_get_uninterruptible(struct pci_dev *pdev, unsigned int rsrc)
{
	/*
	 * This function locks the VGA device. It is normally provided by the
	 * VGA arbiter in the Linux kernel. We don't need this arbitration because
	 * the platform-driver enforces the exclusive access to the VGA resources
	 * by our driver.
	 *
	 * At the time when this function is called, the 'pci_dev' structure for
	 * the VGA card was already requested. Hence, subsequent I/O accesses
	 * should work.
	 */
	TRACE;
	return 0;
}


void vga_put(struct pci_dev *pdev, unsigned int rsrc)
{
	TRACE;
}


int pci_bus_alloc_resource(struct pci_bus *, struct resource *, resource_size_t,
                           resource_size_t, resource_size_t, unsigned int,
                           resource_size_t (*)(void *, const struct resource *,
                                               resource_size_t, resource_size_t),
                           void *alignf_data)
{
	TRACE;
	return 0;
}


void pci_set_master(struct pci_dev *dev)
{
	TRACE;
}


int pci_enable_msi(struct pci_dev *dev)
{
	TRACE;
	return 0;
}


dma_addr_t pci_map_page(struct pci_dev *hwdev, struct page *page,
                        unsigned long offset, size_t size, int direction) {
	return page->paddr + offset;
}


int pci_dma_mapping_error(struct pci_dev *pdev, dma_addr_t dma_addr)
{
	TRACE;
	return 0;
}


void iounmap(volatile void *addr)
{
	/* do not unmap here, but when client requests new dataspace */
	TRACE_AND_STOP;
}


/****************
 ** linux/io.h **
 ****************/

int arch_phys_wc_add(unsigned long base, unsigned long size)
{
	/*
	 * Linux tries to manipulate physical memory attributes here (e.g.,
	 * using MTRRs). But when using PAT, this is not needed. When running
	 * on top of a microkernel, we cannot manipulate the attributes
	 * anyway.
	 */
	TRACE;
	return 0;
}
#endif

/*******************************
 ** arch/x86/include/asm/io.h **
 *******************************/

void memset_io(void *addr, int val, size_t count)
{
	memset((void __force *)addr, val, count);
}

#if 0
/********************
 ** linux/device.h **
 ********************/

struct subsys_private { int dummy; };

int bus_register(struct bus_type *bus)
{
	/*
	 * called by i2c-core init
	 *
	 * The subsequent code checks for the 'p' member of the 'bus'. So
	 * we have to supply a valid pointer there.
	 */

	static subsys_private priv = { 0 };
	bus->p = &priv;

	TRACE;
	return 0;
}


/**
 * Assuming that driver_register is only called for i2c device driver
 * registration, we can store its pointer here
 */
static struct device_driver *i2c_device_driver = nullptr;

int driver_register(struct device_driver *drv)
{
	TRACE;
	ASSERT(!i2c_device_driver);
	i2c_device_driver = drv;
	return 0;
}


void device_unregister(struct device *dev)
{
	if (dev->type && dev->type->release)
		dev->type->release(dev);
}


int device_for_each_child(struct device *dev, void *data, int (*fn)(struct device *dev, void *data))
{
	/*
	 * Called when a connector is removed
	 */
	TRACE;
	return 0;
}


int bus_for_each_dev(struct bus_type *bus, struct device *start, void *data,
                     int (*fn)(struct device *dev, void *data))
{
	/*
	 * Called bu the i2c-core driver after registering the driver. This
	 * function is called to process drivers that are present at initialization
	 * time. Since we initialize the i2c driver prior the others, we don't
	 * need to perform anything.
	 */
	TRACE;
	return 0;
}


int dev_set_name(struct device *dev, const char *name, ...)
{
	TRACE;
	return 0;
}


int device_register(struct device *dev)
{
	TRACE;
	return 0;
}


int bus_for_each_drv(struct bus_type *bus, struct device_driver *start,
                     void *data, int (*fn)(struct device_driver *, void *))
{
	TRACE;
	return fn(i2c_device_driver, data);
}
#endif

void *devm_kzalloc(struct device *dev, size_t size, gfp_t gfp)
{
	return kzalloc(size, gfp);
}


/***********************
 ** linux/workqueue.h **
 ***********************/

struct workqueue_struct *system_wq = nullptr;
#if 0
struct workqueue_struct *system_long_wq = nullptr;
struct workqueue_struct *system_unbound_wq = nullptr;
#endif
struct workqueue_struct *alloc_workqueue(const char *fmt, unsigned int flags,
                                         int max_active, ...)
{
	workqueue_struct *wq = (workqueue_struct *)kzalloc(sizeof(workqueue_struct), 0);
	Lx::Work *work = Lx::Work::alloc_work_queue(&Lx::Malloc::mem(), fmt);
	wq->task       = (void *)work;

	return wq;
}

struct workqueue_struct *alloc_ordered_workqueue(char const *fmt , unsigned int flags, ...)
{
	return alloc_workqueue(fmt, flags, 1);
}
#if 0
bool flush_work(struct work_struct *work)
{
	TRACE_AND_STOP;
	cancel_work_sync(work);
	return 0;
}
#endif
bool mod_delayed_work(struct workqueue_struct *wq, struct delayed_work *dwork,
                      unsigned long delay)
{
	TRACE;
	return queue_delayed_work(wq, dwork, delay);
}
#if 0
bool flush_delayed_work(struct delayed_work *dwork)
{
	TRACE_AND_STOP;
	return false;
}

void schedule(void)
{
	TRACE_AND_STOP;
}
#endif
void flush_workqueue(struct workqueue_struct *wq)
{
	Lx::Task *current = Lx::scheduler().current();
	if (!current) {
		Genode::error("BUG: flush_workqueue executed without task");
		Genode::sleep_forever();
	}

	Lx::Work *lx_work = (wq && wq->task) ? (Lx::Work*) wq->task
	                                     : &Lx::Work::work_queue();

	lx_work->flush(*current);
	Lx::scheduler().current()->block_and_schedule();
}

#if 0
/***************
 ** Execution **
 ***************/

bool in_atomic()
{
	return false;
}
#endif
static bool irq_state_disabled = false;
#if 0
void local_bh_enable(void)
{
	TRACE;
	irq_state_disabled = false;
}

void local_bh_disable(void)
{
	TRACE;
	irq_state_disabled = true;
}
#endif
void preempt_enable(void)
{
	TRACE;
}

void preempt_disable(void)
{
	TRACE;
}
#if 0
bool irqs_disabled()
{
	return irq_state_disabled;
}
#endif

void usleep_range(unsigned long min, unsigned long max)
{
	udelay(min);
}

#if 0
/*******************
 ** linux/timer.h **
 *******************/

static unsigned long _round_jiffies(unsigned long j, bool force_up)
{
	unsigned remainder = j % HZ;

	/*
	 * from timer.c
	 *
	 * If the target jiffie is just after a whole second (which can happen
	 * due to delays of the timer irq, long irq off times etc etc) then
	 * we should round down to the whole second, not up. Use 1/4th second
	 * as cutoff for this rounding as an extreme upper bound for this.
	 * But never round down if @force_up is set.
	 */

	/* per default round down */
	j = j - remainder;

	/* round up if remainder more than 1/4 second (or if we're forced to) */
	if (remainder >= HZ/4 || force_up)
		j += HZ;

	return j;
}

unsigned long round_jiffies_up_relative(unsigned long j)
{
	j += jiffies;
	return j - (j%HZ) + HZ;
}

unsigned long round_jiffies_up(unsigned long j)
{
	return _round_jiffies(j, true);
}
#endif

struct callback_timer {
	void (*function)(unsigned long);
	unsigned long data;
};

/*
 * For compatibility with 4.4.3 drivers, the argument of this callback function
 * is the 'data' member of the 'timer_list' object, which normally points to
 * the 'timer_list' object itself when initialized with 'timer_setup()', but
 * here it was overridden in 'setup_timer()' to point to the 'callback_timer'
 * object instead.
 */
static void timer_callback(struct timer_list *t)
{
	struct callback_timer * tc = (struct callback_timer *)t;
	tc->function(tc->data);
}

extern "C" void setup_timer(struct timer_list *timer, void (*function)(unsigned long),
                            unsigned long data)
{
	callback_timer * tc = new (Lx::Malloc::mem()) callback_timer;
	tc->function = function;
	tc->data     = data;

	timer_setup(timer, timer_callback, 0u);
	timer->data = (unsigned long)tc;
}


/************************
 ** DRM implementation **
 ************************/

unsigned int drm_debug = 0xff;

struct drm_device *drm_dev_alloc(struct drm_driver *driver,
				 struct device *parent)
{
	struct drm_device *dev;
	int ret;

	dev = (struct drm_device*)kzalloc(sizeof(*dev), GFP_KERNEL);
	if (!dev)
		return (struct drm_device*)ERR_PTR(-ENOMEM);

	ret = drm_dev_init(dev, driver, parent);
	if (ret) {
		kfree(dev);
		return (struct drm_device*)ERR_PTR(ret);
	}

	return dev;
}


int drm_dev_init(struct drm_device *dev, struct drm_driver *driver,
                 struct device *parent)
{
	TRACE;

	kref_init(&dev->ref);
	dev->dev = parent;
	dev->driver = driver;

	INIT_LIST_HEAD(&dev->filelist);
	INIT_LIST_HEAD(&dev->ctxlist);
	INIT_LIST_HEAD(&dev->vmalist);
	INIT_LIST_HEAD(&dev->maplist);
	INIT_LIST_HEAD(&dev->vblank_event_list);

	spin_lock_init(&dev->buf_lock);
	spin_lock_init(&dev->event_lock);
	mutex_init(&dev->struct_mutex);
	mutex_init(&dev->filelist_mutex);
	mutex_init(&dev->ctxlist_mutex);
	mutex_init(&dev->master_mutex);

	if (drm_gem_init(dev) != 0)
		DRM_ERROR("Cannot initialize graphics execution manager (GEM)\n");

	return 0;
}


void drm_send_event_locked(struct drm_device *dev, struct drm_pending_event *e)
{
	if (e->completion) {
		complete_all(e->completion);
		e->completion_release(e->completion);
		e->completion = NULL;
	}

	if (e->fence) {
		TRACE_AND_STOP;
	}
}

static void drm_get_minor(struct drm_device *dev, struct drm_minor **minor, int type)
{
	struct drm_minor *new_minor = (struct drm_minor*)
		kzalloc(sizeof(struct drm_minor), GFP_KERNEL);
	ASSERT(new_minor);
	new_minor->type = type;
	new_minor->dev = dev;
	*minor = new_minor;
}

int drm_dev_register(struct drm_device *dev, unsigned long flags)
{
	drm_get_minor(dev, &dev->primary, DRM_MINOR_PRIMARY);

	int ret = 0;

	ASSERT(!lx_drm_device);
	lx_drm_device = dev;

	dev->registered = true;

	if (dev->driver->load) {
		ret = dev->driver->load(dev, flags);
		if (ret)
			return ret;
	}

	if (drm_core_check_feature(dev, DRIVER_MODESET))
		drm_modeset_register_all(dev);

	DRM_INFO("Initialized %s %d.%d.%d %s on minor %d\n",
		 dev->driver->name, dev->driver->major, dev->driver->minor,
		 dev->driver->patchlevel, dev->driver->date,
		 dev->primary->index);

	return 0;
}

#if 0
int request_irq(unsigned int irq, irq_handler_t handler, unsigned long flags,
                const char *name, void *dev)
{
	struct drm_device * drm_dev = (struct drm_device*) dev;
	Lx::Pci_dev * pci_dev = (Lx::Pci_dev*) drm_dev->pdev->bus;
	Lx::Irq::irq().request_irq(pci_dev->client(), handler, dev);
	return 0;
}

/***************************
 ** arch/x86/kernel/tsc.c **
 ***************************/

unsigned int tsc_khz = 0;
#endif

/**************************************
 ** arch/arm64/include/asm/processor.h **
 **************************************/

void cpu_relax(void)
{
	Lx::timer_update_jiffies();
	asm volatile("yield" ::: "memory");
}
#if 0
/***********************
 ** drivers/pci/rom.c **
 ***********************/

static Genode::Constructible<Genode::Attached_io_mem_dataspace> video_rom;

void __iomem __must_check *pci_map_rom(struct pci_dev *pdev, size_t *size)
{
	/* solely available in BIOS legacy mode ... */
	enum { VIDEO_ROM_BASE = 0xC0000, VIDEO_ROM_SIZE = 0x20000 };

	BUG_ON(video_rom.constructed());

	try {
		video_rom.construct(Lx_kit::env().env(), VIDEO_ROM_BASE, VIDEO_ROM_SIZE);
		*size = VIDEO_ROM_SIZE;
		return video_rom->local_addr<void*>();
	} catch (...) {
		Genode::warning("could not map ",
		                Genode::Hex_range<Genode::addr_t>(VIDEO_ROM_BASE,
		                                                  VIDEO_ROM_SIZE),
		                " video rom region");
	}

	return nullptr;
}

void pci_unmap_rom(struct pci_dev *pdev, void __iomem *rom) {
	BUG_ON(!video_rom.constructed());
	video_rom.destruct();
}
#endif

/******************
 ** linux/kref.h **
 ******************/

void kref_init(struct kref *kref) {
	kref->refcount.counter = 1; }

void kref_get(struct kref *kref)
{
	if (!kref->refcount.counter)
		Genode::error(__func__, " kref already zero");

	kref->refcount.counter++;
}

int kref_put(struct kref *kref, void (*release) (struct kref *kref))
{
	if (!kref->refcount.counter) {
		Genode::error(__func__, " kref already zero");
		return 1;
	}

	kref->refcount.counter--;
	if (kref->refcount.counter == 0) {
		release(kref);
		return 1;
	}
	return 0;
}

int kref_put_mutex(struct kref *kref, void (*release)(struct kref *kref), struct mutex *lock)
{
	if (kref_put(kref, release)) {
		mutex_lock(lock);
		return 1;
	}
	return 0;
}

int kref_get_unless_zero(struct kref *kref)
{
	if (!kref->refcount.counter)
		return 0;

	kref_get(kref);
	return 1;
}

void *kmalloc_array(size_t n, size_t size, gfp_t flags)
{
	if (size != 0 && n > SIZE_MAX / size) return NULL;
	return kmalloc(n * size, flags);
}

unsigned int kref_read(const struct kref *kref)
{
	TRACE;
	return atomic_read(&kref->refcount);
}
#if 0
bool refcount_dec_and_test(atomic_t *a)
{
	if ((unsigned)a->counter == UINT_MAX)
		return false;

	if (a->counter == 0)
		printk("Underflow of atomic variable ...\n");

	return atomic_dec_and_test(a);
}

/**************************************
 ** Stubs for non-ported driver code **
 **************************************/

void pm_qos_add_request(struct pm_qos_request *req, int pm_qos_class, s32 value)
{
	TRACE;
}

void pm_qos_update_request(struct pm_qos_request *req, s32 new_value)
{
	TRACE;
}

int vga_client_register(struct pci_dev *pdev, void *cookie,
                        void (*irq_set_state)(void *cookie, bool state),
                        unsigned int (*set_vga_decode)(void *cookie, bool state))
{
	TRACE;
	return -ENODEV;
}

int vga_switcheroo_register_client(struct pci_dev *dev,
                                   const struct vga_switcheroo_client_ops *ops,
                                   bool driver_power_control)
{
	TRACE;
	return 0;
}

struct resource * devm_request_mem_region(struct device *dev,
                                          resource_size_t start,
                                          resource_size_t n,
                                          const char *name)
{
	/*
	 * This function solely called for keeping the stolen memory preserved
	 * for the driver only ('i915_stolen_to_physical'). The returned pointer is
	 * just checked for NULL but not used otherwise.
	 */
	TRACE;
	static struct resource dummy;
	return &dummy;
}

int acpi_lid_notifier_register(struct notifier_block *nb)
{
	TRACE;
	return 0;
}
#endif
int drm_sysfs_connector_add(struct drm_connector *connector)
{
	TRACE;
	connector->kdev = (struct device*)
		kmalloc(sizeof(struct device), GFP_KERNEL);
	DRM_DEBUG("adding \"%s\" to sysfs\n", connector->name);
	drm_sysfs_hotplug_event(connector->dev);
	return 0;
}

void drm_sysfs_connector_remove(struct drm_connector *connector)
{
	kfree(connector->kdev);
	connector->kdev = nullptr;
	DRM_DEBUG("removing \"%s\" from sysfs\n", connector->name);
	drm_sysfs_hotplug_event(connector->dev);
}

void spin_lock_irq(spinlock_t *lock)
{
	TRACE;
	irq_state_disabled = true;
}

void spin_unlock_irq(spinlock_t *lock)
{
	TRACE;
	irq_state_disabled = false;
}
#if 0
void spin_lock_irqsave_nested(spinlock_t *lock, unsigned flags, int subclass)
{
	TRACE;
}

int autoremove_wake_function(struct wait_queue_entry *wq_entry, unsigned mode, int sync, void *key)
{
	TRACE_AND_STOP;
}
#endif
int fb_get_options(const char *name, char **option)
{
	return 0;
}
#if 0
void trace_intel_gpu_freq_change(int)
{
	TRACE;
}


int of_alias_get_highest_id(const char *stem)
{
	TRACE;
	return 0;
}

void down_write(struct rw_semaphore *sem)
{
	TRACE;
}

void up_write(struct rw_semaphore *sem)
{
	TRACE;
}


void intel_csr_ucode_init(struct drm_device *dev)
{
	TRACE;
}

void i915_gem_shrinker_init(struct drm_i915_private *dev_priv)
{
	TRACE;
}

bool static_key_false(struct static_key *key)
{
	TRACE;
	return false;
}

int i915_gem_init_userptr(struct drm_device *dev)
{
	TRACE;
	return 0;
}
#endif
void spin_lock(spinlock_t *lock)
{
	TRACE;
}
#if 0
struct inode *file_inode(struct file *f)
{
	return f->f_inode;
}

void mapping_set_gfp_mask(struct address_space *m, gfp_t mask)
{
	TRACE;
}

gfp_t mapping_gfp_constraint(struct address_space *mapping, gfp_t gfp_mask)
{
	TRACE;
	return 0;
}

unsigned long __get_free_pages(gfp_t gfp_mask, unsigned int order)
{
	struct page * pages = alloc_pages(gfp_mask, order);
	if (!pages)
		return 0;

	return (unsigned long)pages->addr;
}
#endif
struct file *shmem_file_setup(const char *name, loff_t const size,
                              unsigned long flags)
{
	TRACE;

	struct file * file = (struct file *)kzalloc(sizeof(*file), GFP_KERNEL);
	file->f_mapping = (struct address_space *)kzalloc(sizeof(*file->f_mapping), GFP_KERNEL);

	size_t const npages = (size + PAGE_SIZE - 1) >> PAGE_SHIFT;
	size_t sz_log2 = Genode::log2(npages);
	sz_log2 += ((npages > (1UL << sz_log2)) ? 1 : 0);

	struct page *pages = alloc_pages(GFP_DMA, sz_log2);

	file->f_mapping->my_page = pages;

	return file;
}

void fput(struct file *file)
{
	if (!file)
		return;

	if (file->f_mapping) {
		if (file->f_mapping->my_page) {
			free_pages((unsigned long)file->f_mapping->my_page->addr, /* unknown order */ 0);
			file->f_mapping->my_page = nullptr;
		}
		kfree(file->f_mapping);
	}
	kfree(file);
}
#if 0
struct page *shmem_read_mapping_page_gfp(struct address_space *mapping,
                                         pgoff_t index, gfp_t gfp_mask)
{
	return mapping->my_page;
}

struct page * pfn_to_page(dma_addr_t pfn)
{
	struct page * page = Lx::Addr_to_page_mapping::find_page_by_paddr(pfn * PAGE_SIZE);
	if (!page)
		TRACE_AND_STOP;

	return page;
}

dma_addr_t page_to_pfn(struct page *page)
{
	return page->paddr / PAGE_SIZE;
}

void *vmap(struct page **pages, unsigned int count, unsigned long b, pgprot_t g)
{
	if (!count || !pages || !pages[0])
		return 0;

	bool same = (count == 1);

	for (unsigned i = 1; i < count; i++) {
		same = (pages[i]->paddr == pages[0]->paddr) &&
		       (pages[i]->addr  == pages[0]->addr);

		if (!same)
			break;

	}
	if (!same)
		TRACE_AND_STOP;

	return pages[0]->addr;
}


struct page* Lx::Addr_to_page_mapping::find_page_by_paddr(unsigned long paddr)
{
	for (Addr_to_page_mapping *m = _list()->first(); m; m = m->next())
		if (m->_page->paddr == paddr)
			return m->_page;

	return 0;
}

int dma_map_sg_attrs(struct device *dev, struct scatterlist *sg, int nents,
                     enum dma_data_direction dir, struct dma_attrs *attrs)
{
	int i;
	struct scatterlist *s;
	Genode::addr_t base = page_to_phys(sg_page(sg));
	Genode::size_t offs = 0;
	for_each_sg(sg, s, nents, i) {
		s->dma_address = base + offs;
		offs += s->length;
	}
	return nents;
}

void dma_unmap_sg_attrs(struct device *dev, struct scatterlist *sg, int nents,
                        enum dma_data_direction dir, struct dma_attrs *attrs)
{
	TRACE;
}

dma_addr_t dma_map_page(struct device *dev, struct page *page,
                        unsigned long offset, size_t size,
                        enum dma_data_direction direction)
{
	return page_to_phys(page) + offset;
}

void mark_page_accessed(struct page *p)
{
}

int dma_mapping_error(struct device *dev, dma_addr_t dma_addr)
{
	TRACE;
	return 0;
}

int on_each_cpu(void (*func) (void *info), void *info, int wait)
{
	func(info);
	return 0;
}

u64 ktime_get_raw_ns(void)
{
	return ktime_get();
}

s64 ktime_us_delta(const ktime_t later, const ktime_t earlier)
{
	return ktime_to_us(ktime_sub(later, earlier));
}

s64 ktime_ms_delta(const ktime_t later, const ktime_t earlier)
{
	return ktime_to_ms(ktime_sub(later, earlier));
}

ktime_t ktime_get_boottime(void)
{
	TRACE;
	return ktime_get();
}

void i915_setup_sysfs(struct drm_device *dev_priv)
{
	TRACE;
}

int acpi_video_register(void)
{
	TRACE;
	return 0;
}
#endif
void ww_mutex_init(struct ww_mutex *lock, struct ww_class *ww_class)
{
	lock->ctx = NULL;
	lock->locked = false;
}

void ww_acquire_init(struct ww_acquire_ctx *ctx, struct ww_class *ww_class)
{
	TRACE;
}

int  ww_mutex_lock(struct ww_mutex *lock, struct ww_acquire_ctx *ctx)
{
	if (ctx && (lock->ctx == ctx))
		return -EALREADY;

	if (lock->locked) {
		Genode::warning(__func__, " already locked");
		return 1;
	}

	lock->ctx = ctx;
	lock->locked = true;
	return 0;
}

void ww_mutex_unlock(struct ww_mutex *lock)
{
	lock->ctx = NULL;
	lock->locked = false;
}

bool ww_mutex_is_locked(struct ww_mutex *lock)
{
	return lock->locked;
}

void ww_acquire_fini(struct ww_acquire_ctx *ctx)
{
	TRACE;
}
#if 0
void local_irq_disable()
{
	TRACE;
}

void local_irq_enable()
{
	TRACE;
}

bool preemptible()
{
	TRACE_AND_STOP;
	return false;
}
#endif

void drm_sysfs_hotplug_event(struct drm_device *dev)
{
	Genode::log("*** drm_sysfs_hotplug_event(): ", __builtin_return_address(0));

	Framebuffer::Driver * driver = (Framebuffer::Driver*)
		lx_c_get_driver(lx_drm_device);

	if (driver) {
		DRM_DEBUG("generating hotplug event\n");
		driver->generate_report();
		driver->trigger_reconfiguration();
	}
}

#if 0
void intel_audio_codec_enable(struct intel_encoder *encoder)
{
	TRACE;
}


void clflush(volatile void *page)
{
	unsigned int i;
	const int size = 64;

	ASSERT(sizeof(unsigned long) == 8);

	// FIXME clflush with other opcode see X86_FEATURE_CLFLUSHOPT
	for (i = 0; i < PAGE_SIZE; i += size)
		asm volatile(".byte 0x3e; clflush %P0"
		             : "+m" (*((volatile char __force *)(page)+i)));
}


void intel_audio_codec_disable(struct intel_encoder *encoder)
{
	TRACE;
}

struct backlight_device *backlight_device_register(const char *name,
  struct device *dev, void *devdata, const struct backlight_ops *ops,
  const struct backlight_properties *props)
{
	struct backlight_device *new_bd;
	new_bd = (backlight_device*) kzalloc(sizeof(struct backlight_device), GFP_KERNEL);
	if (!new_bd)
		return nullptr;

	new_bd->ops = ops;
	if (props)
		new_bd->props = *props;
	/* apply our preferred user range scheme */
	new_bd->props.max_brightness = MAX_BRIGHTNESS;
	new_bd->props.brightness = MAX_BRIGHTNESS / 4 * 3;

	return new_bd;
}

void synchronize_irq(unsigned int irq)
{
	TRACE;
}
#endif
#define BITMAP_FIRST_WORD_MASK(start) (~0UL << ((start) & (BITS_PER_LONG - 1)))
#define BITMAP_LAST_WORD_MASK(nbits) (~0UL >> (-(nbits) & (BITS_PER_LONG - 1)))

unsigned long find_next_bit(const unsigned long *addr, unsigned long nbits,
                            unsigned long start)
{
	unsigned long tmp;

	if (!nbits || start >= nbits)
		return nbits;

	tmp = addr[start / BITS_PER_LONG] ^ 0UL;

	/* Handle 1st word. */
	tmp &= BITMAP_FIRST_WORD_MASK(start);
	start = round_down(start, BITS_PER_LONG);

	while (!tmp) {
		start += BITS_PER_LONG;
		if (start >= nbits)
			return nbits;
		tmp = addr[start / BITS_PER_LONG] ^ 0UL;
	}

	return min(start + __ffs(tmp), nbits);
}

void bitmap_set(unsigned long *map, unsigned int start, int len)
{
	unsigned long *p = map + BIT_WORD(start);
	const unsigned int size = start + len;
	int bits_to_set = BITS_PER_LONG - (start % BITS_PER_LONG);
	unsigned long mask_to_set = BITMAP_FIRST_WORD_MASK(start);

	while (len - bits_to_set >= 0) {
		*p |= mask_to_set;
		len -= bits_to_set;
		bits_to_set = BITS_PER_LONG;
		mask_to_set = ~0UL;
		p++;
	}
	if (len) {
		mask_to_set &= BITMAP_LAST_WORD_MASK(size);
		*p |= mask_to_set;
	}
}
#if 0
void bitmap_clear(unsigned long *p, unsigned int start, unsigned int count)
{
	unsigned const size_a = sizeof(*p) * 8;

	if (start < size_a && start+count < size_a) {
		for (unsigned i = start; i < start + count; i++)
			*p = *p & ~(1UL << i);
	} else
		TRACE_AND_STOP;
}
#endif
unsigned long find_next_zero_bit(unsigned long const *addr, unsigned long size,
                                 unsigned long offset)
{
	unsigned long i, j;

	for (i = offset; i < (size / BITS_PER_LONG); i++)
		if (addr[i] != ~0UL)
			break;

	if (i == size)
		return size;

	for (j = 0; j < BITS_PER_LONG; j++)
		if ((~addr[i]) & (1UL << j))
			break;

	return (i * BITS_PER_LONG) + j;
}
#if 0
void tasklet_init(struct tasklet_struct *t, void (*f)(unsigned long),
                  unsigned long data)
{
	TRACE;

	t->state = 0;
	t->func = f;
	t->data = data;
}

void tasklet_hi_schedule(struct tasklet_struct *tasklet)
{
	TRACE;
	tasklet_schedule(tasklet);
}

void tasklet_schedule(struct tasklet_struct *tasklet)
{
	TRACE;
	Lx::Work::work_queue().schedule_tasklet(tasklet);
}

signed long schedule_timeout_uninterruptible(signed long timeout)
{
	return schedule_timeout(timeout);
}

long io_schedule_timeout(long timeout)
{
	printk("%s timeout %ld %lx\n", __func__, timeout, timeout);
	return schedule_timeout(timeout);
}

int device_add_properties(struct device *, const struct property_entry *)
{
	TRACE_AND_STOP;
}

void device_remove_properties(struct device *)
{
	TRACE_AND_STOP;
}

void irqd_set_trigger_type(struct irq_data *, u32)
{
	TRACE_AND_STOP;
}

void irq_set_chip_and_handler(unsigned int, struct irq_chip *,
                              irq_flow_handler_t)
{
	TRACE_AND_STOP;
}

void handle_simple_irq(struct irq_desc *)
{
	TRACE_AND_STOP;
}

struct irq_data *irq_get_irq_data(unsigned int)
{
	TRACE_AND_STOP;
}
#endif

unsigned int irq_find_mapping(struct irq_domain *, irq_hw_number_t hwirq)
{
	/* only irqsteer irqs (< 32) are expected */

	if (hwirq > 31) {
		Genode::error(__func__, "(): unexpected hwirq ", hwirq);
		Genode::sleep_forever();
	}

	return hwirq;
}

#if 0
unsigned int irq_create_mapping(struct irq_domain *, irq_hw_number_t)
{
	TRACE_AND_STOP;
}
#endif
void drm_printk(const char *level, unsigned int category, const char *format,
                ...)
{
	struct va_format vaf;
	va_list args;

	if (category && !(drm_debug & category))
		return;

	va_start(args, format);
	vaf.fmt = format;
	vaf.va = &args;

	printk("%s" "[drm:%ps]%s %pV\n",
	       level, __builtin_return_address(0),
	       strcmp(level, KERN_ERR) == 0 ? " *ERROR*" : "", &vaf);
	(void)vaf;

	va_end(args);
}
#if 0
void __drm_printfn_debug(struct drm_printer *p, struct va_format *vaf)
{
	pr_debug("%s %pV", p->prefix, vaf);
}

void drm_printf(struct drm_printer *p, const char *f, ...)
{
	va_list args;

	va_start(args, f);
	drm_vprintf(p, f, &args);
	va_end(args);
}
#endif
int vsnprintf(char *str, size_t size, const char *format, va_list args)
{
	Genode::String_console sc(str, size);
	sc.vprintf(format, args);

	return sc.len();
}

char *kvasprintf(gfp_t gfp, const char *fmt, va_list ap)
{
	size_t const bad_guess = strlen(fmt) + 10;
	char * const p = (char *)kmalloc(bad_guess, gfp);
	if (!p)
		return NULL;

	vsnprintf(p, bad_guess, fmt, ap);

	return p;
}
#if 0
void __add_wait_queue_entry_tail(struct wait_queue_head *wq_head, struct wait_queue_entry *wq_entry)
{
	TRACE_AND_STOP;
}
#endif
static void _completion_timeout(struct timer_list *list)
{
	struct process_timer *timeout = from_timer(timeout, list, timer);
	timeout->task.unblock();
}

long __wait_completion(struct completion *work, unsigned long timeout)
{
	Lx::timer_update_jiffies();
	unsigned long j = timeout ? jiffies + timeout : 0;

	Lx::Task & cur_task = *Lx::scheduler().current();
	struct process_timer timer { cur_task };

	if (timeout) {
		timer_setup(&timer.timer, _completion_timeout, 0);
		mod_timer(&timer.timer, j);
	}

	while (!work->done) {

		if (j && j <= jiffies) {
			lx_log(1, "timeout jiffies %lu", jiffies);
			return 0;
		}

		Lx::Task *task = Lx::scheduler().current();
		work->task = (void *)task;
		task->block_and_schedule();
	}

	if (timeout)
		del_timer(&timer.timer);

	return (j  || j == jiffies) ? 1 : j - jiffies;
}
#if 0
size_t strnlen(const char *s, size_t maxlen)
{
	size_t c;
	for (c = 0; c <maxlen; c++)
		if (!s[c])
			return c;

	return maxlen;
}

u64 local_clock(void)
{
	Lx::timer_update_jiffies();
	return jiffies_to_nsecs(jiffies);
}


/******************************************************************
 ** ACPI related function called by Intel driver (-DCONFIG_ACPI) **
 ******************************************************************/


static Genode::Constructible<Genode::Attached_io_mem_dataspace> opregion;

void *memremap(resource_size_t offset, size_t size, unsigned long flags)
{
	Genode::addr_t paddr = offset & ~0xfffUL;
	Genode::addr_t psize = size + (offset & 0xfff);

	BUG_ON(opregion.constructed());

	try {
		opregion.construct(Lx_kit::env().env(), paddr, psize);
		return opregion->local_addr<uint8_t>() + (offset & 0xfff);
	} catch (...) {
		Genode::warning("could not map ",
		                Genode::Hex_range<Genode::addr_t>(offset, size),
		                " ACPI opregion");
	}
	return nullptr;
}

void memunmap(void *addr)
{
	BUG_ON(!opregion.constructed());
	opregion.destruct();
}


void intel_register_dsm_handler(void)
{
	Genode::warning(__func__, " called");
}

void i2c_acpi_register_devices(struct i2c_adapter *adap)
{
	Genode::warning(__func__, " called");
}

int register_acpi_notifier(struct notifier_block *nb)
{
	Genode::warning(__func__, " called");
	return 0;
}

int acpi_reconfig_notifier_register(struct notifier_block *nb)
{
	Genode::warning(__func__, " called");
	return 0;
}
#endif

} /* extern "C" */
