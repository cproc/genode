/*
 * \brief  Intel framebuffer driver
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
#include <base/log.h>
#include <base/component.h>
#include <base/heap.h>
#include <base/attached_rom_dataspace.h>

/* Server related local includes */
#include <component.h>

/* Linux emulation environment includes */
#include <lx_emul.h>
#include <lx_kit/env.h>
#include <lx_kit/malloc.h>
#include <lx_kit/scheduler.h>
#include <lx_kit/timer.h>
#include <lx_kit/irq.h>
//#include <lx_kit/pci_dev_registry.h>
//#include <lx_kit/backend_alloc.h>
#include <lx_kit/work.h>

/* Linux module functions */
//extern "C" void postcore_i2c_init(void); /* i2c-core-base.c */
//extern "C" int module_i915_init();  /* i915_drv.c */
extern "C" void radix_tree_init(); /* called by start_kernel(void) normally */
extern "C" void drm_connector_ida_init(); /* called by drm_core_init(void) normally */
extern "C" int module_imx_drm_pdrv_init();
extern "C" int module_dcss_driver_init();
extern "C" int module_dcss_crtc_driver_init();


static void run_linux(void * m);

unsigned long jiffies;


struct Main
{
	Genode::Env                   &env;
	Genode::Entrypoint            &ep     { env.ep() };
	Genode::Attached_rom_dataspace config { env, "config" };
	Genode::Heap                   heap   { env.ram(), env.rm() };
	Framebuffer::Root              root   { env, heap, config };


	/* Linux task that handles the initialization */
	Genode::Constructible<Lx::Task> linux;

	Main(Genode::Env &env) : env(env)
	{
		Genode::log("--- i.MX8 framebuffer driver ---");

		Lx_kit::construct_env(env);

		LX_MUTEX_INIT(bridge_lock);
//		LX_MUTEX_INIT(core_lock);

		LX_MUTEX_INIT(component_mutex);

		/* init singleton Lx::Scheduler */
		Lx::scheduler(&env);

//		Lx::pci_init(env, env.ram(), heap);
		Lx::malloc_init(env, heap);

		/* init singleton Lx::Timer */
		Lx::timer(&env, &ep, &heap, &jiffies);

		/* init singleton Lx::Irq */
		Lx::Irq::irq(&ep, &heap);

		/* init singleton Lx::Work */
		Lx::Work::work_queue(&heap);

		linux.construct(run_linux, reinterpret_cast<void*>(this),
		                "linux", Lx::Task::PRIORITY_0, Lx::scheduler());

		/* give all task a first kick before returning */
		Lx::scheduler().schedule();
	}

	void announce() { env.parent().announce(ep.manage(root)); }
#if 0
	Lx::Task &linux_task() { return *linux; }
#endif
};

#if 0
struct Policy_agent
{
	Main &main;
	Genode::Signal_handler<Policy_agent> handler;
	bool _pending = false;

	void handle()
	{
		_pending = true;
		main.linux_task().unblock();
		Lx::scheduler().schedule();
	}

	bool pending()
	{
		bool ret = _pending;
		_pending = false;
		return ret;
	}

	Policy_agent(Main &m)
	: main(m), handler(main.ep, *this, &Policy_agent::handle) {}
};
#endif

static void run_linux(void * m)
{
	Main * main = reinterpret_cast<Main*>(m);

	system_wq  = alloc_workqueue("system_wq", 0, 0);

	radix_tree_init();
	drm_connector_ida_init();
#if 0
	postcore_i2c_init();
	module_i915_init();
#endif

	module_dcss_driver_init();
	module_imx_drm_pdrv_init();
	module_dcss_crtc_driver_init();

	/**
	 * This device is originally created with the name '32e00000.dcss'
	 * via 'of_platform_bus_create()'. Here it is called 'dcss-core' to match
	 * the driver name.
	 */

	struct platform_device *dcss_pdev =
		platform_device_alloc("dcss-core", 0);

	static resource dcss_resources[] = 	{
		{ 0x32e00000, 0x32efffff, "dcss",       IORESOURCE_MEM },
		{          3,          3, "dpr_dc_ch0", IORESOURCE_IRQ },
		{          4,          4, "dpr_dc_ch1", IORESOURCE_IRQ },
		{          5,          5, "dpr_dc_ch2", IORESOURCE_IRQ },
		{          6,          6, "ctx_ld",     IORESOURCE_IRQ },
		{          8,          8, "ctxld_kick", IORESOURCE_IRQ },
		{          9,          9, "dtg_prg1",   IORESOURCE_IRQ },
		{         16,         16, "dtrc_ch1",   IORESOURCE_IRQ },
		{         17,         17, "dtrc_ch2",   IORESOURCE_IRQ },
	};

	dcss_pdev->num_resources = 8;
	dcss_pdev->resource = dcss_resources;

	platform_device_register(dcss_pdev);

	/**
	 * This device is originally created with the name 'display-subsystem'
	 * via 'of_platform_bus_create()'. Here it is called 'imx-drm' to match
	 * the driver name.
	 */

	struct platform_device *display_subsystem_pdev =
		platform_device_alloc("imx-drm", 0);

	static device_node display_subsystem_of_node = { "display-subsystem" };

	display_subsystem_pdev->dev.of_node = &display_subsystem_of_node;

	platform_device_register(display_subsystem_pdev);

	//extern struct platform_device *imx_dcss_crtc_pdev;

	//Genode::log("imx_dcss_crtc_pdev: ", imx_dcss_crtc_pdev);

	//extern "C" int dcss_crtc_bind(struct device *dev, struct device *master, void *data);

	/* bind devices */
	//dcss_crtc_bind(imx_dcss_crtc_pdev->dev, display_subsystem_pdev->dev, nullptr);

	main->root.session.driver().finish_initialization();
	main->announce();
#if 0
	Policy_agent pa(*main);
	main->root.session.driver().config_sigh(pa.handler);
	main->config.sigh(pa.handler);
#endif
	while (1) {
		Lx::scheduler().current()->block_and_schedule();
//		while (pa.pending())
//			main->root.session.config_changed();
	}
}


void Component::construct(Genode::Env &env)
{
	/* XXX execute constructors of global statics */
	env.exec_static_constructors();

	static Main m(env);
}
