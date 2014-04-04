set feature(Input) 1

#
# Helper functions
#

proc use_usb_drv { feature_arg } {
	upvar $feature_arg feature
	return [expr {([info exists feature(Input)] ||
	               [info exists feature(Nic)]) &&
                  ([have_spec usb] && ![have_spec ps2]) &&
                  ([have_spec omap4] || [have_spec exynos5])}]
}

proc use_nic_drv { feature_arg } {
	upvar $feature_arg feature
	return [expr {[info exists feature(Nic)] &&
	              ![use_usb_drv feature]}]
}

#
# Build
#

proc build_components { feature_arg } {

	upvar $feature_arg feature

	set build_components {
		core init
		drivers/timer
		drivers/framebuffer
		server/nitpicker
	}

	lappend_if [use_usb_drv feature] build_components drivers/usb
	lappend_if [use_nic_drv feature] build_components drivers/nic
	lappend_if [have_spec ps2]  build_components drivers/input/ps2

	return $build_components
}


#
# Configuration
#

proc parent_provides { feature_arg } {

	upvar $feature_arg feature

	append parent_provides {
		<service name="ROM"/>
		<service name="RAM"/>
		<service name="IRQ"/>
		<service name="IO_MEM"/>
		<service name="IO_PORT"/>
		<service name="CAP"/>
		<service name="PD"/>
		<service name="RM"/>
		<service name="CPU"/>
		<service name="LOG"/>
		<service name="SIGNAL"/>
	}

	return $parent_provides
}

proc start_nodes { feature_arg } {
	
	upvar $feature_arg feature

	append start_nodes {
		<start name="timer">
			<resource name="RAM" quantum="1M"/>
			<provides><service name="Timer"/></provides>
		</start>
	}

	append_if [have_spec sdl] start_nodes {
		<start name="fb_sdl">
			<resource name="RAM" quantum="4M"/>
			<provides>
				<service name="Input"/>
				<service name="Framebuffer"/>
			</provides>
		</start>
	}

	append_if [have_spec framebuffer] start_nodes {
		<start name="fb_drv">
			<resource name="RAM" quantum="4M"/>
			<provides><service name="Framebuffer"/></provides>
		</start>
	}

	if { [use_usb_drv feature] } {
		append start_nodes {
			<start name="usb_drv">
				<resource name="RAM" quantum="12M"/>
				<provides><service name="Input"/></provides>
				<config uhci="yes" ehci="yes" xhci="no">
					<hid/> }
		append_if [info exists feature(Nic)] {
					<nic mac="2e:60:90:0c:4e:01" />}
		append start_nodes {
				</config>
			</start>
		}
	}

	append_if [have_spec ps2] start_nodes {
		<start name="ps2_drv">
			<resource name="RAM" quantum="1M"/>
			<provides><service name="Input"/></provides>
		</start>
	}

	append_if [use_nic_drv feature] start_nodes {
		<start name="nic_drv">
			<resource name="RAM" quantum="2M"/>
			<provides><service name="Nic"/></provides>
		</start>
	}	

	append start_nodes {
		<start name="nitpicker">
			<resource name="RAM" quantum="1M"/>
			<provides><service name="Nitpicker"/></provides>
		</start>
	}



	return $start_nodes
}

#
# Boot modules
#

proc boot_modules { feature_arg } {

	upvar $feature_arg feature

	set boot_modules ""

	append boot_modules {
		core
		init
		timer
		nitpicker
	}

	lappend_if [have_spec linux]         boot_modules fb_sdl
	lappend_if [have_spec ps2]           boot_modules ps2_drv
	lappend_if [have_spec framebuffer]   boot_modules fb_drv
	lappend_if [use_usb_drv feature]     boot_modules usb_drv
	lappend_if [use_nic_drv feature]     boot_modules nic_drv

	return $boot_modules
}

