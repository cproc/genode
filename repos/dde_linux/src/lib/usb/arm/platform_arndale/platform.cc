/*
 * \brief  EHCI for Arndale initializaion code
 * \author Sebastian Sumpf
 * \date   2013-02-20
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode */
#include <os/attached_io_mem_dataspace.h>
#include <io_mem_session/connection.h>
#include <timer_session/connection.h>
#include <util/mmio.h>

/* Emulation */
#include <platform/platform.h>
#include <lx_emul.h>

/* Linux */
#include <linux/platform_data/usb-ehci-s5p.h>
#include <linux/platform_data/dwc3-exynos.h>

using namespace Genode;

enum {
	EHCI_BASE     = 0x12110000,
	DWC3_BASE     = 0x12000000,
	DWC3_PHY_BASE = 0x12100000,
	GPIO_BASE     = 0x11400000,
	EHCI_IRQ      = 103,
	DWC3_IRQ      = 104,
	PM_BASE       = 0x10040000,
};

static resource _ehci[] =
{
	{ EHCI_BASE, EHCI_BASE + 0xfff, "ehci", IORESOURCE_MEM },
	{ EHCI_IRQ, EHCI_IRQ, "ehci-irq", IORESOURCE_IRQ },
};

static resource _dwc3[] =
{
	{ DWC3_BASE, DWC3_BASE + 0xcfff, "dwc3", IORESOURCE_MEM },
	{ DWC3_IRQ, DWC3_IRQ, "dwc3-irq", IORESOURCE_IRQ },
};

static struct s5p_ehci_platdata _ehci_data;
static struct dwc3_exynos_data  _dwc3_data;


/**
 * EHCI controller
 */
struct Ehci : Genode::Mmio
{
	Ehci(addr_t const mmio_base) : Mmio(mmio_base)
	{
		write<Cmd>(0);

		/* reset */
		write<Cmd::Reset>(1);

		while(read<Cmd::Reset>())
			msleep(1);
	}

	struct Cmd : Register<0x10, 32>
	{
		struct Reset : Bitfield<1, 1> { };
	};
};


/**
 * Gpio handling
 */
struct Gpio_bank {
	unsigned con;
	unsigned dat;
};


static inline
unsigned con_mask(unsigned val) { return 0xf << ((val) << 2); }


static inline
unsigned con_sfr(unsigned x, unsigned v) { return (v) << ((x) << 2); }


static void gpio_cfg_pin(Gpio_bank *bank, int gpio, int cfg)
{
	unsigned int value;

	value = readl(&bank->con);
	value &= ~con_mask(gpio);
	value |= con_sfr(gpio, cfg);
	writel(value, &bank->con);
}


static void gpio_direction_output(Gpio_bank *bank, int gpio, int en)
{
	unsigned int value;
	enum { GPIO_OUTPUT = 0x1 };

	gpio_cfg_pin(bank, gpio, GPIO_OUTPUT);

	value = readl(&bank->dat);
	value &= ~(0x1 << gpio);
	if (en)
		value |= 0x1 << gpio;
	writel(value, &bank->dat);
}


static void arndale_ehci_init()
{
	enum Gpio_offset { D1 = 0x180, X3 = 0xc60 };

	/* reset hub via GPIO */
	Io_mem_connection io_gpio(GPIO_BASE, 0x1000);
	addr_t gpio_base = (addr_t)env()->rm_session()->attach(io_gpio.dataspace());

	Gpio_bank *d1 = reinterpret_cast<Gpio_bank *>(gpio_base + D1);
	Gpio_bank *x3 = reinterpret_cast<Gpio_bank *>(gpio_base + X3);

	/* hub reset */
	gpio_direction_output(x3, 5, 0);
	/* hub connect */
	gpio_direction_output(d1, 7, 0);

	gpio_direction_output(x3, 5, 1);
	gpio_direction_output(d1, 7, 1);

	env()->rm_session()->detach(gpio_base);

	/* reset ehci controller */
	Io_mem_connection io_ehci(EHCI_BASE, 0x1000);
	addr_t ehci_base = (addr_t)env()->rm_session()->attach(io_ehci.dataspace());

	Ehci ehci(ehci_base);
	env()->rm_session()->detach(ehci_base);
}


extern "C" void module_ehci_hcd_init();
extern "C" void module_usbnet_init();
extern "C" void module_asix_driver_init();
extern "C" void module_dwc3_exynos_driver_init();
extern "C" void module_dwc3_driver_init();
extern "C" void module_xhci_hcd_init();

void ehci_setup(Services *services)
{
	if (services->nic)
		module_asix_driver_init();

	/* register EHCI controller */
	module_ehci_hcd_init();

	/* setup controller */
	arndale_ehci_init();

	/* setup EHCI-controller platform device */
	platform_device *pdev   = (platform_device *)kzalloc(sizeof(platform_device), 0);
	pdev->name              = (char *)"s5p-ehci";
	pdev->id                = 0;
	pdev->num_resources     = 2;
	pdev->resource          = _ehci;
	pdev->dev.platform_data = &_ehci_data;

	/*needed for DMA buffer allocation. See 'hcd_buffer_alloc' in 'buffer.c' */
	static u64 dma_mask         = ~(u64)0;
	pdev->dev.dma_mask          = &dma_mask;
	pdev->dev.coherent_dma_mask = ~0;

	platform_device_register(pdev);
}

struct Power : Genode::Mmio
{
	struct Usbdrd_phy_control : Register<0x704, 32> { };
	struct Usbhost_phy_control : Register<0x708, 32> { };

	Power(addr_t const base) : Mmio(base)
	{
		write<Usbdrd_phy_control>(1);
		write<Usbhost_phy_control>(1);
	}
};

struct Phy_usb3 : Genode::Mmio
{
	struct Link_system : Register<0x4, 32>
	{
		struct Fladj                : Bitfield<1, 6>  { };
		struct Ehci_version_control : Bitfield<27, 1> { };
	};

	struct Phy_utmi : Register<0x8, 32> { };

	struct Phy_clk_rst : Register<0x10, 32>
	{
		struct Common_onn      : Bitfield<0, 1>  { };
		struct Port_reset      : Bitfield<1, 1>  { };
		struct Ref_clk_sel     : Bitfield<2, 2>  { };
		struct Retenablen      : Bitfield<4, 1>  { };
		struct Fsel            : Bitfield<5, 6>  { };
		struct Mpll_mult       : Bitfield<11, 7> { };
		struct Ref_ssp_en      : Bitfield<19, 1> { };
		struct Ssc_en          : Bitfield<20, 1> { };
		struct Ssc_ref_clk_sel : Bitfield<23, 8> { };
	};

	struct Phy_reg0    : Register<0x14, 32> { };

	struct Phy_param0 : Register<0x1c, 32>
	{
		struct Loss_level  : Bitfield<26, 5> { };
		struct Ref_use_pad : Bitfield<31, 1> { };
	};

	struct Phy_param1 : Register<0x20, 32>
	{
		struct Pcs_txdeemph : Bitfield<0, 5> { };
	};

	struct Phy_test : Register<0x28, 32>
	{
		struct Power_down_ssb_hsb : Bitfield<2, 2> { };
	};

	struct Phy_batchg : Register<0x30, 32>
	{
		struct Utmi_clksel : Bitfield<2, 1> { };
	};

	struct Phy_resume : Register<0x34, 32> { };

	Phy_usb3 (addr_t const base) : Mmio(base)
	{
		Timer::Connection timer;

		/* reset */
		write<Phy_reg0>(0);

		/* clock source */
		write<Phy_param0::Ref_use_pad>(0);

		/* set Loss-of-Signal Detector sensitivity */
		write<Phy_param0::Loss_level>(0x9);
		write<Phy_resume>(0);

		/*
		 * Setting the Frame length Adj value[6:1] to default 0x20
		 * See xHCI 1.0 spec, 5.2.4
		 */
		write<Link_system::Ehci_version_control>(1);
		write<Link_system::Fladj>(0x20);

		/* set Tx De-Emphasis level */
		write<Phy_param1::Pcs_txdeemph>(0x1c);

		/* set clock */
		write<Phy_batchg::Utmi_clksel>(1);

		/* PHYTEST POWERDOWN Control */
		write<Phy_test::Power_down_ssb_hsb>(0);

		/* UTMI power */
		enum { OTG_DISABLE = (1 << 6) };
		write<Phy_utmi>(OTG_DISABLE);

		/* setup clock  */
		Phy_clk_rst::access_t clk = 0;

		/* set external clock */
		Phy_clk_rst::Ref_clk_sel::set(clk, 3);
		/* 24 MHz */
		Phy_clk_rst::Fsel::set(clk, 0x5);
		Phy_clk_rst::Mpll_mult::set(clk, 0x68);
		Phy_clk_rst::Ssc_ref_clk_sel::set(clk, 0x88);

		/* port reset */
		Phy_clk_rst::Port_reset::set(clk, 1);

		/* digital power supply in normal operating mode */
		Phy_clk_rst::Retenablen::set(clk, 1);
		/* enable ref clock for SS function */
		Phy_clk_rst::Ref_ssp_en::set(clk, 1);
		/* enable spread spectrum */
		Phy_clk_rst::Ssc_en::set(clk, 1);
		/* power down HS Bias and PLL blocks in suspend mode */
		Phy_clk_rst::Common_onn::set(clk, 1);

		write<Phy_clk_rst>(clk);
		timer.usleep(10);
		write<Phy_clk_rst::Port_reset>(0);
	}
};

static void arndale_xhci_init()
{
	/* enable power of USB3 */
	Attached_io_mem_dataspace io_pm(PM_BASE, 0x1000);
	Power power((addr_t)io_pm.local_addr<addr_t>());

	/* setup PHY */
	Attached_io_mem_dataspace io_phy(DWC3_PHY_BASE, 0x1000);
	Phy_usb3 phy((addr_t)io_phy.local_addr<addr_t>());
}

void xhci_setup()
{
	arndale_xhci_init();

	module_dwc3_exynos_driver_init();
	module_dwc3_driver_init();
	module_xhci_hcd_init();

	/* setup DWC3-controller platform device */
	platform_device *pdev   = (platform_device *)kzalloc(sizeof(platform_device), 0);
	pdev->name              = (char *)"exynos-dwc3";
	pdev->id                = 0;
	pdev->num_resources     = 2;
	pdev->resource          = _dwc3;
	pdev->dev.platform_data = &_dwc3_data;

	/*needed for DMA buffer allocation. See 'hcd_buffer_alloc' in 'buffer.c' */
	static u64 dma_mask         = ~(u64)0;
	pdev->dev.dma_mask          = &dma_mask;
	pdev->dev.coherent_dma_mask = ~0;

	platform_device_register(pdev);
}

void platform_hcd_init(Services *services)
{
	/* register network */
	if (services->nic)
		module_usbnet_init();

	ehci_setup(services);
	xhci_setup();
}
