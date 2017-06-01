#
# Build
#

if {[have_spec odroid_xu]} {
	puts "Run script does not support this platform."
	exit 0
}

set build_components {
	core init
	drivers/timer drivers/nic
	server/nic_bridge
	test/lwip/pingpong/server
	test/lwip/pingpong/client
}

lappend_if [expr [have_spec omap4] || [have_spec arndale]] build_components drivers/usb
lappend_if [have_spec gpio]             build_components drivers/gpio

proc gpio_drv { } { if {[have_spec rpi] && [have_spec hw]}  { return hw_gpio_drv }
                    if {[have_spec rpi] && [have_spec foc]} { return foc_gpio_drv }
                    return gpio_drv }

source ${genode_dir}/repos/base/run/platform_drv.inc
append_platform_drv_build_components

build $build_components

create_boot_directory

#
# Generate config
#

append config {
<config verbose="yes">
	<parent-provides>
		<service name="ROM"/>
		<service name="IRQ"/>
		<service name="IO_MEM"/>
		<service name="IO_PORT"/>
		<service name="PD"/>
		<service name="RM"/>
		<service name="CPU"/>
		<service name="LOG"/>
	</parent-provides>
	<default-route>
		<any-service> <parent/> <any-child/> </any-service>
	</default-route>
	<default caps="100"/>
	<start name="timer">
		<resource name="RAM" quantum="1M"/>
		<provides> <service name="Timer"/> </provides>
	</start>
	<start name="nic_bridge" caps="200">
		<resource name="RAM" quantum="24M"/>
		<provides><service name="Nic"/></provides>
		<config>
			<policy label_prefix="} $server_binary {" ip_addr="10.0.2.52"/>
			<policy label_prefix="} $client_binary {" ip_addr="10.0.2.53"/>
		</config>
		<route>
			<service name="Nic"> <child name="nic_drv"/> </service>
			<any-service> <parent/> <any-child/> </any-service>
		</route>
	</start>
	<start name="} $server_binary {" caps="200">
		<resource name="RAM" quantum="32M"/>
		<route>
			<service name="Nic"> <child name="nic_bridge"/> </service>
			<any-service> <parent/> <any-child/> </any-service>
		</route>
		<config>
			<arg value="} $server_binary {"/>
			<arg value="-verbose"/>
			<arg value="1"/>
			<vfs> <dir name="dev"> <log/> </dir> </vfs>
			<libc stdout="/dev/log" stderr="/dev/log"
			      ip_addr="10.0.2.52" netmask="255.255.255.0" gateway="0.0.0.0"/>
		</config>
	</start>
	<sleep ms="10000"/>
	<start name="} $client_binary {" caps="200">
		<resource name="RAM" quantum="32M"/>
		<route>
			<service name="Nic"> <child name="nic_bridge"/> </service>
			<any-service> <parent/> <any-child/> </any-service>
		</route>
		<config>
			<arg value="} $client_binary {"/>
			<arg value="-verbose"/>
			<arg value="1"/>
			<arg value="-startsize"/>
			<arg value="1024"/>
			<arg value="-endsize"/>
			<arg value="1024"/>
			<arg value="-serverip"/>
			<arg value="10.0.2.52"/>
			<vfs> <dir name="dev"> <log/> </dir> </vfs>
			<libc stdout="/dev/log" stderr="/dev/log"
			      ip_addr="10.0.2.53" netmask="255.255.255.0" gateway="0.0.0.0"/>
		</config>
	</start> }

append_if [have_spec gpio] config "
	<start name=\"[gpio_drv]\">
		<resource name=\"RAM\" quantum=\"4M\"/>
		<provides><service name=\"Gpio\"/></provides>
		<config/>
	</start>"

append_if [expr [have_spec omap4] || [have_spec arndale]] config {
	<start name="usb_drv" priority="-1">
		<resource name="RAM" quantum="12M"/>
		<provides>
			<service name="Nic"/>
		</provides>
		<config ehci="yes">
			<nic mac="02:00:00:00:01:01" />
		</config>
	</start>}

append_if [expr ![have_spec omap4] && ![have_spec arndale]] config {
	<start name="nic_drv">
		<binary name="} [nic_drv_binary] {"/>
		<resource name="RAM" quantum="4M"/>
		<provides><service name="Nic"/></provides>
	</start> }

append_platform_drv_config

append config {
</config>
}

install_config $config

#
# Boot modules
#

# generic modules
append boot_modules {
	core init timer
	nic_bridge
	ld.lib.so libc.lib.so libm.lib.so posix.lib.so
} $server_binary {
} $client_binary

# platform-specific modules
lappend_if [expr [have_spec omap4] || [have_spec arndale]]   boot_modules usb_drv
lappend_if [expr ![have_spec omap4] && ![have_spec arndale]] boot_modules [nic_drv_binary]
lappend_if [have_spec gpio]                                  boot_modules [gpio_drv]

append_platform_drv_boot_modules

build_boot_image $boot_modules

#
# Execute test case
#

# qemu config
append qemu_args "  -nographic "

append_if [have_spec x86]     qemu_args " -net nic,model=e1000 "
append_if [have_spec lan9118] qemu_args " -net nic,model=lan9118 "

append qemu_args " -net user "

run_genode_until {(?n)^.*server.* 1024	1036.*$} 10
