#
# Global variables required by platform_drv-related functions
#

set build_components { }
set config { }
set boot_modules { }

source ${genode_dir}/repos/base/run/platform_drv.inc

#
# Helper functions
#

proc use_fb_drv { } {
	return [have_spec framebuffer]
}

proc fb_drv_binary { } {
	if {[have_spec x86]}       { return vesa_fb_drv    }
	if {[have_spec imx8q_evk]} { return imx8_fb_drv    }
	return no_fb_drv_available
}

proc use_fb_sdl { } {
	return [have_spec linux]
}

proc nic_drv_binary { } {
	if {[have_spec linux]}     { return linux_nic_drv   }
	if {[have_spec x86]}       { return ipxe_nic_drv    }
	if {[have_spec imx8q_evk]} { return fec_nic_drv     }
	return no_nic_drv_available
}

proc use_ps2_drv { } {
	return [have_spec ps2]
}

proc use_usb_drv { } {
	return [expr { ([need_usb_hid] ||
	               ([have_spec x86] && ![have_spec linux]))}]
}

proc usb_drv_binary { } {
	if {[have_spec x86]}       { return x86_pc_usb_host_drv  }
	if {[have_spec imx8q_evk]} { return imx8q_evk_usb_host_drv }
	return no_usb_drv_available
}

#
# Keyboard layout - this function can be overridden in a run script
# 
proc language_chargen { } { return "en_us" }

#
# Build
#

proc drivers_build_components { } {

	set build_components { }

	# This function appends to the global 'build_components' variable, not to
	# the local version defined above.
	append_platform_drv_build_components

	lappend build_components drivers/nic
	lappend build_components server/event_filter

	lappend_if [use_fb_drv]  build_components drivers/framebuffer
	lappend_if [use_fb_sdl]  build_components drivers/framebuffer/sdl
	lappend_if [use_ps2_drv] build_components drivers/ps2
	lappend_if [use_usb_drv] build_components drivers/usb_host
	lappend_if [use_usb_drv] build_components drivers/usb_hid

	return $build_components
}

#
# Configuration
#

proc drivers_parent_provides { } {

	set parent_provides { }

	append parent_provides {
		<service name="ROM"/>
		<service name="IRQ"/>
		<service name="IO_MEM"/>
		<service name="IO_PORT"/>
		<service name="PD"/>
		<service name="RM"/>
		<service name="CPU"/>
		<service name="LOG"/>
		<service name="TRACE"/>
	}

	return $parent_provides
}

proc drivers_start_nodes { } {

	set start_nodes { }

	append start_nodes [platform_drv_config]

	append start_nodes {
		<start name="timer">
			<resource name="RAM" quantum="1M"/>
			<provides><service name="Timer"/></provides>
		</start>
	}

	append_if [use_fb_drv] start_nodes {
		<start name="fb_drv" caps="200">
			<binary name="} [fb_drv_binary] {"/>
			<resource name="RAM" quantum="64M"/>
			<config>
				<!-- imx8q_evk -->
				<connector name="HDMI-A-1" width="1920" height="1080" hz="60" enabled="true"/>
			</config>
			<route>
				<service name="Capture"> <child name="nitpicker"/> </service>
				<any-service> <parent/> <any-child/> </any-service>
			</route>
		</start>
	}

	append_if [use_fb_sdl] start_nodes {
		<start name="fb_sdl" ld="no">
			<resource name="RAM" quantum="16M"/>
			<route>
				<service name="Event"> <child name="event_filter" label="sdl"/> </service>
				<service name="Capture"> <child name="nitpicker"/> </service>
				<any-service> <parent/> <any-child/> </any-service>
			</route>
		</start>
	}

	exec cp -f [genode_dir]/repos/os/src/server/event_filter/[language_chargen].chargen bin/
	exec cp -f [genode_dir]/repos/os/src/server/event_filter/special.chargen bin/

	append start_nodes {
		<!-- toggle key mappings depending on the numlock state -->
		<start name="numlock_remap_rom">
			<binary name="rom_filter"/>
			<resource name="RAM" quantum="1M"/>
			<provides> <service name="ROM"/> </provides>
			<config>
				<input name="numlock_enabled" rom="numlock" node="numlock">
					<attribute name="enabled"/> </input>
				<output node="remap">
					<if>
						<has_value input="numlock_enabled" value="no"/>
						<then>
							<inline>
								<key name="KEY_KP0"   to="KEY_INSERT"/>
								<key name="KEY_KP1"   to="KEY_END"/>
								<key name="KEY_KP2"   to="KEY_DOWN"/>
								<key name="KEY_KP3"   to="KEY_PAGEDOWN"/>
								<key name="KEY_KP4"   to="KEY_LEFT"/>
								<key name="KEY_KP5"   to="KEY_RESERVED"/>
								<key name="KEY_KP6"   to="KEY_RIGHT"/>
								<key name="KEY_KP7"   to="KEY_HOME"/>
								<key name="KEY_KP8"   to="KEY_UP"/>
								<key name="KEY_KP9"   to="KEY_PAGEUP"/>
								<key name="KEY_KPDOT" to="KEY_DELETE"/>
							</inline>
						</then>
					</if>
				</output>
			</config>
			<route>
				<service name="ROM" label="numlock"> <child name="wm_report_rom"/> </service>
				<any-service> <parent/> </any-service>
			</route>
		</start>
	}


	append start_nodes {
		<start name="event_filter">
			<resource name="RAM" quantum="1280K" />
			<provides> <service name="Event" /> </provides>
			<config>
				<output>
					<chargen>
						<remap>
							<include rom="numlock.remap"/>
						<merge>}
	append_if [use_ps2_drv] start_nodes {
							<input name="ps2"/>}
	append_if [use_usb_drv] start_nodes {
							<input name="usb"/>}
	append_if [use_fb_sdl] start_nodes {
							<input name="sdl"/>}
	append start_nodes {
						</merge>
						</remap>
						<mod1>
							<key name="KEY_LEFTSHIFT"/> <key name="KEY_RIGHTSHIFT"/>
						</mod1>
						<mod2>
							<key name="KEY_LEFTCTRL"/> <key name="KEY_RIGHTCTRL"/>
						</mod2>
						<mod3>
							<key name="KEY_RIGHTALT"/> <!-- AltGr -->
						</mod3>
						<mod4>
							<rom name="capslock"/>
						</mod4>
						<repeat delay_ms="500" rate_ms="50"/>}
	append start_nodes "
						<include rom=\"[language_chargen].chargen\"/>"
	append start_nodes {
						<include rom="special.chargen"/>
					</chargen>
				</output>}
	append_if [use_ps2_drv] start_nodes {
				<policy label="ps2" input="ps2"/>}
	append_if [use_usb_drv] start_nodes {
				<policy label="usb" input="usb"/>}
	append_if [use_fb_sdl] start_nodes {
				<policy label="sdl" input="sdl"/>}
	append start_nodes {
			</config>
			<route>
				<service name="LOG"> <parent/> </service>
				<service name="CPU"> <parent/> </service>
				<service name="ROM" label="capslock"> <child name="wm_report_rom"/> </service>
				<service name="ROM" label="numlock.remap"> <child name="numlock_remap_rom"/> </service>
				<service name="ROM"> <parent/> </service>
				<service name="PD">  <parent/> </service>
				<service name="Timer"> <child name="timer"/> </service>
				<service name="Event"> <child name="nitpicker"/> </service>
			</route>
		</start>
	}

	# don't use the dynamic linker for loading the lx_hybrid nic_drv on Linux
	proc nic_drv_ld_attr {} {
		if {[have_spec linux]} { return {ld="no"} }
		return ""
	}

	append start_nodes {
		<start name="nic_drv" } [nic_drv_ld_attr] { caps="200">
			<binary name="} [nic_drv_binary] {"/>
			<resource name="RAM" quantum="16M"/>
			<provides><service name="Nic"/></provides>
			<config>
				<card type="fsl,imx6sx-fec" mii="rgmii" irq="152" mmio="0x30be0000"/>
			</config>
		</start>
	}

	append_if [use_ps2_drv] start_nodes {
		<start name="ps2_drv">
			<resource name="RAM" quantum="2M"/>
			<config capslock_led="rom" numlock_led="rom"/>
			<route>
				<service name="Event"> <child name="event_filter" label="ps2"/> </service>
				<service name="ROM" label="capslock"> <child name="wm_report_rom"/> </service>
				<service name="ROM" label="numlock"> <child name="wm_report_rom"/> </service>
				<any-service> <parent/> <any-child/> </any-service>
			</route>
		</start>
	}

	append_if [use_usb_drv] start_nodes {
		<start name="usb_drv" caps="150">
			<binary name="} [usb_drv_binary] {"/>
			<resource name="RAM" quantum="12M"/>
			<provides> <service name="Usb"/> </provides>
			<config bios_handoff="yes">
				<report devices="yes"/>
				<policy label_prefix="usb_hid_drv" class="0x3"/>
			</config>
			<route>
				<service name="Report"> <child name="wm_report_rom"/> </service>
				<any-service> <parent/> <any-child/> </any-service>
			</route>
		</start>

		<start name="usb_hid_drv" caps="140">
			<resource name="RAM" quantum="11M"/>
			<config use_report="yes"/>
			<route>
				<service name="ROM" label="report"> <child name="wm_report_rom"/> </service>
				<service name="Event"> <child name="event_filter" label="usb"/> </service>
				<any-service> <parent/> <any-child/> </any-service>
			</route>
		</start>
	}
	
	return $start_nodes
}

#
# Boot modules
#

proc drivers_boot_modules { } {

	set boot_modules { }

	# This function appends to the global 'boot_modules' variable, not to the
	# local version defined above.
	append_platform_drv_boot_modules

	lappend boot_modules event_filter
	lappend boot_modules [language_chargen].chargen
	lappend boot_modules special.chargen
	lappend boot_modules [nic_drv_binary]

	lappend_if [use_fb_drv]  boot_modules [fb_drv_binary]
	lappend_if [use_fb_sdl]  boot_modules fb_sdl
	lappend_if [use_ps2_drv] boot_modules ps2_drv
	lappend_if [use_usb_drv] boot_modules [usb_drv_binary]
	lappend_if [use_usb_drv] boot_modules usb_hid_drv

	return $boot_modules
}
