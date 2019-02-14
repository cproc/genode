create_boot_directory

append build_components {
	core init
	timer
	server/nic_bridge
	server/nic_loopback
	test/tcp
	drivers/nic
}

source ${genode_dir}/repos/base/run/platform_drv.inc
append_platform_drv_build_components

append_socket_fs_build_components

build $build_components


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
	<default caps="256"/>
	<start name="timer">
		<resource name="RAM" quantum="1M"/>
		<provides> <service name="Timer"/> </provides>
	</start>
<!--
	<start name="nic_loopback">
		<resource name="RAM" quantum="1M"/>
		<provides> <service name="Nic"/> </provides>
	</start>
	<start name="nic_bridge">
		<resource name="RAM" quantum="8M"/>
		<provides><service name="Nic"/></provides>
		<config verbose="no">
			<policy label_prefix="recv" ip_addr="192.168.1.1" />
			<policy label_prefix="send" ip_addr="192.168.1.2" />
		</config>
		<route>
			<service name="Nic"> <child name="nic_loopback"/> </service>
			<any-service> <parent/> <any-child/> </any-service>
		</route>
	</start>
-->

	<start name="nic_drv" ld="yes">
		<resource name="RAM" quantum="4M"/>
		<provides> <service name="Nic"/> </provides>
		<config>
			<nic tap="tap_chpro"/>
		</config>
	</start>
<!--
	<start name="recv">
		<binary name="test-tcp"/>
		<resource name="RAM" quantum="32M"/>
		<config>
			<arg value="./recv"/>
			<libc stdout="/log" stderr="/log" socket="/sockets"/>
			<vfs>
				<log/>
				<dir name="sockets">
					<} [socket_fs_plugin] { dhcp="yes"/>
				</dir>
			</vfs>
		</config>
	</start>
-->

	<start name="send">
		<binary name="test-tcp"/>
		<resource name="RAM" quantum="32M"/>
		<config>
			<arg value="./send"/>
			<arg value="10.0.2.2"/>
			<libc stdout="/log" stderr="/log" socket="/sockets"/>
			<vfs>
				<log/>
				<dir name="sockets">
					<} [socket_fs_plugin] { dhcp="yes"/>
				</dir>
			</vfs>
		</config>
	</start>

}

append_platform_drv_config

append config {
</config>
}


install_config $config

append boot_modules {
	core ld.lib.so init timer
	nic_bridge
	nic_loopback
	posix.lib.so libc.lib.so libm.lib.so vfs.lib.so vfs_lwip.lib.so
	test-tcp
	nic_drv
}

append_socket_fs_boot_modules

append_platform_drv_boot_modules

build_boot_image $boot_modules

append qemu_args " -nographic "
#append qemu_args " -device e1000,netdev=net0 -netdev user,id=net0,hostfwd=tcp::2222-:2222"

#run_genode_until {child "recv" exited with exit value 0} 240
run_genode_until forever

# vi: set ft=tcl :
