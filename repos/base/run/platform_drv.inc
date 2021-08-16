proc have_platform_drv {} {
	return [expr [have_board imx53_qsb] \
	          || [have_board imx53_qsb_tz] \
	          || [have_board imx8q_evk] \
	          || [have_board rpi] \
	          || [have_board pc]]
}


##
# Return name of the USB driver binary
#
proc usb_host_drv_binary { } {
	if {[have_board rpi]}             { return rpi_usb_host_drv       }
	if {[have_board imx6q_sabrelite]} { return imx6q_sabrelite_usb_host_drv }
	if {[have_board imx8q_evk]}       { return imx8q_evk_usb_host_drv }
	if {[have_board pc]}              { return x86_pc_usb_host_drv    }
	return no_usb_drv_available
}


##
# Return name of the audio driver binary
#
proc audio_drv_binary { } {
	if {[have_board linux]} { return linux_audio_drv }
	if {[have_board pc]}    { return pci_audio_drv }
	return no_audio_drv_available
}

##
# Return attributes of the audio driver's <start> node
#
proc audio_drv_start_attr { } {
	if {[have_board linux]} { return {ld="no"} }
	return ""
}

proc acpi_drv_name { } {
	global use_acpica_as_acpi_drv
	if {[info exists use_acpica_as_acpi_drv] && $use_acpica_as_acpi_drv} {
		return acpica }

	return acpi_drv
}

proc platform_drv_build_components {} {
	set drv_build_components ""
	lappend_if [have_platform_drv] drv_build_components drivers/platform
	if {[have_board pc]} {
		lappend drv_build_components server/report_rom
		if {[acpi_drv_name] eq "acpi_drv"} {
			lappend drv_build_components drivers/acpi
		}
		if {[acpi_drv_name] eq "acpica"} {
			lappend drv_build_components app/acpica
		}
	}
	return $drv_build_components
}


proc append_platform_drv_build_components {} {
	global build_components
	append build_components { } [platform_drv_build_components]
}

proc platform_drv_binary {} {
	if {[have_board imx53_qsb]}    { return imx53_platform_drv }
	if {[have_board imx53_qsb_tz]} { return imx53_platform_drv }
	if {[have_board imx8q_evk]}    { return imx8mq_platform_drv }
	if {[have_board rpi]}          { return rpi_platform_drv   }
	if {[have_board pc]}           { return platform_drv       }
	return no_platform_drv_available
}

proc platform_drv_boot_modules {} {
	set drv_boot_modules ""
	lappend_if [have_platform_drv] drv_boot_modules [platform_drv_binary]

	if {[have_board pc]} {
		lappend drv_boot_modules report_rom
		lappend drv_boot_modules [acpi_drv_name]
	}

	return $drv_boot_modules
}

proc append_platform_drv_boot_modules {} {
	global boot_modules
	append boot_modules { } [platform_drv_boot_modules]
}


proc platform_drv_policy {} {

	if {[have_board imx8q_evk]} {
		return {
			<policy label="usb_drv -> " info="yes"> <device name="usb_host_2"/> </policy>
			<policy label="fb_drv -> "  info="yes">
				<device name="dcss"/>
				<device name="hdmi"/>
				<device name="mipi_dsi"/>
				<device name="src"/>
			</policy>
			<policy label="nic_drv -> " info="yes"> <device name="fec"/> </policy>
		}
	}

	if {![have_board pc]} {
		return {}
	}

	if (![have_spec x86]) {
		return $drv_policy
	}

	if {[acpi_drv_name] eq "acpica"} {
		append drv_policy {
			<policy label="acpi_drv -> ">    <pci class="ALL"/> </policy>}
	}

	append drv_policy {
			<policy label_prefix="ps2_drv">  <device name="PS2"/> </policy>
			<policy label_prefix="nic_drv">  <pci class="ETHERNET"/> </policy>
			<policy label_prefix="fb_drv">   <pci class="VGA"/> </policy>
			<policy label_prefix="wifi_drv" msix="false"> <pci class="WIFI"/> </policy>
			<policy label_prefix="usb_drv">  <pci class="USB"/> </policy>
			<policy label_prefix="ahci_drv"> <pci class="AHCI"/> </policy>
			<policy label_prefix="nvme_drv"> <pci class="NVME"/> </policy>
			<policy label_prefix="audio_drv"> <pci class="AUDIO"/> <pci class="HDAUDIO"/> </policy>
			<policy label_prefix="intel_fb_drv">
				<pci class="VGA"/>
				<pci bus="0" device="0" function="0"/>
				<pci class="ISABRIDGE"/>
			</policy>}

	return $drv_policy
}


proc platform_drv_priority {} { return "" }
proc platform_drv_add_routing {} {

	if {[acpi_drv_name] eq "acpica"} {
		return {
			<service name="ROM" label="acpi_ready"> <child name="acpi_report_rom"/> </service>}
	}

	return ""
}


proc platform_drv_config_config {} {
	if {[acpi_drv_name] eq "acpica"} {
		return {
		<config acpi_ready="yes">}
	}

	return {<config>}
}


proc platform_drv_config {} {
	set drv_config ""

	if {[have_board pc]} {

		append drv_config {
	<start name="acpi_drv" } [platform_drv_priority] { caps="350" >
		<binary name="} [acpi_drv_name] {"/>}

		if {[acpi_drv_name] eq "acpica"} {
			append drv_config {
		<resource name="RAM" quantum="5M"/>
		<config acpi_ready="yes" act_as_acpi_drv="yes" report="yes"/>}
		} else {
			append drv_config {
		<resource name="RAM" quantum="4M"/>}
		}

		append drv_config {
		<route>
			<service name="IO_MEM"> <parent/> </service>
			<service name="LOG">    <parent/> </service>
			<service name="PD">     <parent/> </service>
			<service name="RM">     <parent/> </service>
			<service name="CPU">    <parent/> </service>
			<service name="ROM">    <parent/> </service>}

		append_if [expr {[acpi_drv_name] eq "acpica"}] drv_config {
			<service name="IO_PORT">  <parent/> </service>
			<service name="IRQ">      <parent/> </service>
			<service name="Timer">    <child name="timer"/> </service>
			<service name="Platform"> <child name="platform_drv"/> </service>
			<service name="Acpi">     <child name="platform_drv"/> </service>}

		append drv_config {
			<service name="Report"> <child name="acpi_report_rom"/> </service>
			<service name="ROM" label="platform_info"> <parent/> </service>
		</route>
	</start>}

		append drv_config "
	<start name=\"acpi_report_rom\" [platform_drv_priority]>"

		append drv_config {
		<binary name="report_rom"/>
		<resource name="RAM" quantum="2M"/>
		<provides>
			<service name="ROM" />
			<service name="Report" />
		</provides>
		<config>
			<policy label="smbios_decoder -> smbios_table" report="acpi_drv -> smbios_table"/>
			<policy label="platform_drv -> acpi"           report="acpi_drv -> acpi"/>}

		append_if [expr {[acpi_drv_name] eq "acpica"}] drv_config {
			<policy label="platform_drv -> acpi_ready" report="acpi_drv -> acpi_ready"/>}

		append drv_config {
		</config>
		<route>
			<service name="LOG"> <parent/> </service>
			<service name="PD">     <parent/> </service>
			<service name="CPU">    <parent/> </service>
			<service name="ROM">    <parent/> </service>
		</route>
	</start>}

	}

	if {[have_platform_drv]} {

		append drv_config {
	<start name="platform_drv" } [platform_drv_priority] { caps="800" managing_system="yes">
		<binary name="} [platform_drv_binary] {"/>
		<resource name="RAM" quantum="4M"/>
		<provides>
			<service name="Platform"/>}

		append_if [have_board pc] drv_config {
			<service name="Acpi"/>}

		append_if [have_spec arm] drv_config {
			<service name="Regulator"/>}

		append drv_config {
		</provides>
		<route>}

		append drv_config "[platform_drv_add_routing]"

		append_if [have_board pc] drv_config {
			<service name="ROM" label="acpi"> <child name="acpi_report_rom"/> </service>}

		append_if [expr [have_board pc]] drv_config {
			<service name="Report"> <child name="acpi_report_rom"/> </service>}

		append_if [expr [have_board rpi] || [have_board pc]] drv_config {
			<service name="Timer"> <any-child/> </service>}

		append drv_config {
			<any-service> <parent/> </any-service>
		</route>}

		append drv_config [platform_drv_config_config]

		if [have_board imx8q_evk] {
			append drv_config {
				<device name="usb_host_2" type="snps,dwc3">
					<io_mem       address="0x38200000" size="0x10000"/>
					<irq          number="73"/>
					<power-domain name="usb_otg_2"/>
					<clock        name="usb_phy_ref_clk_root"
				              	  driver_name="usb_phy_root_clk"
				              	  parent="system_pll1_div8"
				              	  rate="100000000"/>
					<clock        name="usb_core_ref_clk_root"
				              	  parent="system_pll1_div8"
				              	  rate="100000000"/>
					<clock        name="usb_bus_clk_root"
				              	  parent="system_pll2_div2"
				              	  rate="500000000"/>
					<clock        name="usb_ctrl2_gate"/>
					<clock        name="usb_phy2_gate"/>
					<property     name="dr_mode"    value="host"/>
					<property     name="snps,dis_u2_susphy_quirk"/>
				</device>

				<device name="dcss" type="nxp,imx8mq-dcss">
					<io_mem       address="0x32e00000" size="0x30000"/>
					<irq          number="50"/>
					<clock        name="display_apb_clk_root"
				              	  driver_name="apb"/>
					<clock        name="display_axi_clk_root"
				              	  parent="system_pll1_clk"
				              	  rate="800000000"
				              	  driver_name="axi"/>
					<clock        name="display_rtrm_clk_root"
				              	  parent="system_pll1_clk"
				              	  rate="400000000"
				              	  driver_name="rtrm"/>
					<clock        name="video_pll1_clk"
				              	  parent="25m_ref_clk"
				              	  rate="1200000000" />
					<clock        name="display_dtrc_clk_root"
				              	  driver_name="dtrc"/>
					<clock        name="dc_pixel_clk_root"
				              	  parent="video_pll1_clk"
				              	  rate="120000000"
				              	  driver_name="pix"/>
					<property     name="disp-dev"   value="hdmi_disp"/>
				</device>

				<!-- CAUTION: System reset controller access is currently required by
			     	 mipi_dsi -->
				<device name="src" type="fsl,imx8mq-src">
					<io_mem address="0x30390000" size="0x10000"/>
				</device>

				<device name="mipi_dsi" type="fsl,imx8mq-mipi-dsi_drm">
					<io_mem       address="0x30a00000" size="0x1000"/>
					<irq          number="66"/>
					<power-domain name="mipi"/>
					<clock        name="mipi_dsi_phy_ref_clk_root"
				              	  parent="video_pll1_clk"
				              	  rate="24000000"
				              	  driver_name="phy_ref"/>
					<clock        name="mipi_dsi_esc_rx_clk_root"
				              	  parent="system_pll1_div10"
				              	  rate="80000000"
				              	  driver_name="rx_esc"/>
					<clock        name="mipi_dsi_core_clk_root"
				              	  parent="system_pll1_div3"
				              	  rate="266000000"
				              	  driver_name="core"/>
				</device>

				<device name="hdmi" type="fsl,imx8mq-hdmi">
					<io_mem       address="0x32c00000" size="0x100000"/>
					<io_mem       address="0x32e40000" size="0x40000"/>
					<io_mem       address="0x32e2f000" size="0x10"/>
					<irq          number="48"/>
					<irq          number="57"/>
				</device>

				<device name="fec" type="fsl,imx6sx-fec">
					<io_mem   address="0x30be0000" size="0x4000"/>
					<irq      number="152"/>
					<irq      number="151"/>
					<irq      number="150"/>
					<property name="mii"        value="rgmii-id"/>
				</device>
			}


		}

		append drv_config [platform_drv_policy]

		append drv_config {
		</config>
	</start>}

	}

	return $drv_config
}


proc append_platform_drv_config {} {
	global config
	append config [platform_drv_config]
	return $config
}
