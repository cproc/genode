/*
 * \brief  Array defining order of Linux Kernel initcalls
 * \author Automatically generated file - do no edit
 * \date   2023-10-11
 */

#pragma once

static const char * lx_emul_initcall_order[] = {
	"__initcall_init_hw_perf_eventsearly",
	"__initcall_start",
	"__initcall_do_init_real_modeearly",
	"__initcall_bp_init_aperfmperfearly",
	"__initcall_register_nmi_cpu_backtrace_handlerearly",
	"__initcall_spawn_ksoftirqdearly",
	"__initcall_migration_initearly",
	"__initcall_srcu_bootup_announceearly",
	"__initcall_rcu_spawn_gp_kthreadearly",
	"__initcall_check_cpu_stall_initearly",
	"__initcall_rcu_sysrq_initearly",
	"__initcall_cpu_stop_initearly",
	"__initcall_irq_work_init_threadsearly",
	"__initcall_static_call_initearly",
	"__initcall_init_zero_pfnearly",
	"__initcall_init_fs_inode_sysctlsearly",
	"__initcall_efi_memreserve_root_initearly",
	"__initcall_efi_earlycon_remap_fbearly",
	"__initcall_init_mmap_min_addr0",
	"__initcall_pci_realloc_setup_params0",
	"__initcall_e820__register_nvs_regions1",
	"__initcall_reboot_init1",
	"__initcall_wq_sysfs_init1",
	"__initcall_ksysfs_init1",
	"__initcall_rcu_set_runtime_mode1",
	"__initcall_init_jiffies_clocksource1",
	"__initcall_init_script_binfmt1",
	"__initcall_init_elf_binfmt1",
	"__initcall_pinctrl_init1",
	"__initcall_gpiolib_dev_init1",
	"__initcall_iommu_init1",
	"__initcall_cpuidle_init1",
	"__initcall_sock_init1",
	"__initcall_net_inuse_init1",
	"__initcall_net_defaults_init1",
	"__initcall_init_default_flow_dissectors1",
	"__initcall_netlink_proto_init1",
	"__initcall_genl_init1",
	"__initcall_irq_sysfs_init2",
	"__initcall_bdi_class_init2",
	"__initcall_mm_sysfs_init2",
	"__initcall_init_per_zone_wmark_min2",
	"__initcall_mpi_init2",
	"__initcall_acpi_gpio_setup_params2",
	"__initcall_pcibus_class_init2",
	"__initcall_pci_driver_init2",
	"__initcall_backlight_class_init2",
	"__initcall_tty_class_init2",
	"__initcall_vtconsole_class_init2",
	"__initcall_iommu_dev_init2",
	"__initcall_mipi_dsi_bus_init2",
	"__initcall_devlink_class_init2",
	"__initcall_software_node_init2",
	"__initcall_regmap_initcall2",
	"__initcall_i2c_init2",
	"__initcall_thermal_init2",
	"__initcall_init_menu2",
	"__initcall_amd_postcore_init2",
	"__initcall_kobject_uevent_init2",
	"__initcall_bts_init3",
	"__initcall_pt_init3",
	"__initcall_boot_params_ksysfs_init3",
	"__initcall_sbf_init3",
	"__initcall_arch_kdebugfs_init3",
	"__initcall_xfd_update_static_branch3",
	"__initcall_intel_pconfig_init3",
	"__initcall_ffh_cstate_init3",
	"__initcall_kcmp_cookies_init3",
	"__initcall_cryptomgr_init3",
	"__initcall_acpi_pci_init3",
	"__initcall_iommu_dma_init3",
	"__initcall_pci_arch_init3",
	"__initcall_init_vdso4",
	"__initcall_fixup_ht_bug4",
	"__initcall_topology_init4",
	"__initcall_uid_cache_init4",
	"__initcall_param_sysfs_init4",
	"__initcall_user_namespace_sysctl_init4",
	"__initcall_oom_init4",
	"__initcall_default_bdi_init4",
	"__initcall_percpu_enable_async4",
	"__initcall_init_user_reserve4",
	"__initcall_init_admin_reserve4",
	"__initcall_init_reserve_notifier4",
	"__initcall_rsa_init4",
	"__initcall_crypto_cmac_module_init4",
	"__initcall_crypto_null_mod_init4",
	"__initcall_sha256_generic_mod_init4",
	"__initcall_crypto_ctr_module_init4",
	"__initcall_crypto_gcm_module_init4",
	"__initcall_crypto_ccm_module_init4",
	"__initcall_aes_init4",
	"__initcall_ghash_mod_init4",
	"__initcall_pci_slot_init4",
	"__initcall_fbmem_init4",
	"__initcall_acpi_init4",
	"__initcall_pnp_init4",
	"__initcall_misc_init4",
	"__initcall_iommu_subsys_init4",
	"__initcall_dma_buf_init4",
	"__initcall_phy_init4",
	"__initcall_usb_common_init4",
	"__initcall_usb_init4",
	"__initcall_serio_init4",
	"__initcall_input_init4",
	"__initcall_dw_i2c_init_driver4",
	"__initcall_power_supply_class_init4",
	"__initcall_leds_init4",
	"__initcall_efisubsys_init4",
	"__initcall_init_soundcore4",
	"__initcall_alsa_sound_init4",
	"__initcall_hda_bus_init4",
	"__initcall_ac97_bus_init4",
	"__initcall_proto_init4",
	"__initcall_net_dev_init4",
	"__initcall_neigh_init4",
	"__initcall_fib_notifier_init4",
	"__initcall_ethnl_init4",
	"__initcall_ieee80211_init4",
	"__initcall_rfkill_init4",
	"__initcall_pci_subsys_init4",
	"__initcall_vsprintf_init_hashval4",
	"__initcall_acpi_wmi_init4s",
	"__initcall_nmi_warning_debugfs5",
	"__initcall_hpet_late_init5",
	"__initcall_init_amd_nbs5",
	"__initcall_iomem_init_inode5",
	"__initcall_clocksource_done_booting5",
	"__initcall_init_fs_stat_sysctls5",
	"__initcall_init_fs_exec_sysctls5",
	"__initcall_init_pipe_fs5",
	"__initcall_init_fs_namei_sysctls5",
	"__initcall_init_fs_dcache_sysctls5",
	"__initcall_init_fs_namespace_sysctls5",
	"__initcall_anon_inode_init5",
	"__initcall_proc_cmdline_init5",
	"__initcall_proc_consoles_init5",
	"__initcall_proc_cpuinfo_init5",
	"__initcall_proc_devices_init5",
	"__initcall_proc_interrupts_init5",
	"__initcall_proc_loadavg_init5",
	"__initcall_proc_meminfo_init5",
	"__initcall_proc_stat_init5",
	"__initcall_proc_uptime_init5",
	"__initcall_proc_version_init5",
	"__initcall_proc_softirqs_init5",
	"__initcall_proc_kmsg_init5",
	"__initcall_proc_page_init5",
	"__initcall_init_ramfs_fs5",
	"__initcall_acpi_event_init5",
	"__initcall_pnp_system_init5",
	"__initcall_pnpacpi_init5",
	"__initcall_chr_dev_init5",
	"__initcall_firmware_class_init5",
	"__initcall_init_acpi_pm_clocksource5",
	"__initcall_sysctl_core_init5",
	"__initcall_eth_offload_init5",
	"__initcall_cfg80211_init5",
	"__initcall_pcibios_assign_resources5",
	"__initcall_pci_apply_final_quirks5s",
	"__initcall_acpi_reserve_resources5s",
	"__initcall_populate_rootfsrootfs",
	"__initcall_pci_iommu_initrootfs",
	"__initcall_rapl_pmu_init6",
	"__initcall_amd_ibs_init6",
	"__initcall_msr_init6",
	"__initcall_intel_uncore_init6",
	"__initcall_cstate_pmu_init6",
	"__initcall_register_kernel_offset_dumper6",
	"__initcall_i8259A_init_ops6",
	"__initcall_init_tsc_clocksource6",
	"__initcall_add_rtc_cmos6",
	"__initcall_umwait_init6",
	"__initcall_ioapic_init_ops6",
	"__initcall_iosf_mbi_init6",
	"__initcall_proc_execdomains_init6",
	"__initcall_cpuhp_sysfs_init6",
	"__initcall_ioresources_init6",
	"__initcall_timer_sysctl_init6",
	"__initcall_timekeeping_init_ops6",
	"__initcall_init_clocksource_sysfs6",
	"__initcall_init_timer_list_procfs6",
	"__initcall_alarmtimer_init6",
	"__initcall_clockevents_init_sysfs6",
	"__initcall_utsname_sysctl_init6",
	"__initcall_perf_event_sysfs_init6",
	"__initcall_system_trusted_keyring_init6",
	"__initcall_kswapd_init6",
	"__initcall_mm_compute_batch_init6",
	"__initcall_workingset_init6",
	"__initcall_proc_vmalloc_init6",
	"__initcall_fcntl_init6",
	"__initcall_proc_filesystems_init6",
	"__initcall_start_dirtytime_writeback6",
	"__initcall_init_devpts_fs6",
	"__initcall_efivarfs_init6",
	"__initcall_key_proc_init6",
	"__initcall_asymmetric_key_init6",
	"__initcall_x509_key_init6",
	"__initcall_blake2s_mod_init6",
	"__initcall_percpu_counter_startup6",
	"__initcall_tgl_pinctrl_driver_init6",
	"__initcall_pci_proc_init6",
	"__initcall_ged_driver_init6",
	"__initcall_acpi_ac_init6",
	"__initcall_acpi_button_driver_init6",
	"__initcall_acpi_fan_driver_init6",
	"__initcall_acpi_video_init6",
	"__initcall_acpi_processor_driver_init6",
	"__initcall_acpi_thermal_init6",
	"__initcall_acpi_battery_init6",
	"__initcall_gpio_clk_driver_init6",
	"__initcall_n_null_init6",
	"__initcall_pty_init6",
	"__initcall_serial8250_init6",
	"__initcall_serial_pci_driver_init6",
	"__initcall_exar_pci_driver_init6",
	"__initcall_lpss8250_pci_driver_init6",
	"__initcall_mid8250_pci_driver_init6",
	"__initcall_pericom8250_pci_driver_init6",
	"__initcall_random_sysctls_init6",
	"__initcall_drm_core_init6",
	"__initcall_drm_buddy_module_init6",
	"__initcall_drm_display_helper_module_init6",
	"__initcall_i915_init6",
	"__initcall_topology_sysfs_init6",
	"__initcall_cacheinfo_sysfs_init6",
	"__initcall_intel_lpss_init6",
	"__initcall_intel_lpss_pci_driver_init6",
	"__initcall_blackhole_netdev_init6",
	"__initcall_phylink_init6",
	"__initcall_phy_module_init6",
	"__initcall_fixed_mdio_bus_init6",
	"__initcall_phy_module_init6",
	"__initcall_pcnet32_init_module6",
	"__initcall_e1000_init_module6",
	"__initcall_e1000_init_module6",
	"__initcall_rtl8169_pci_driver_init6",
	"__initcall_ath9k_init6",
	"__initcall_ath9k_cmn_init6",
	"__initcall_iwl_drv_init6",
	"__initcall_iwl_init6",
	"__initcall_iwl_mvm_init6",
	"__initcall_rtl_core_module_init6",
	"__initcall_rtl92ce_driver_init6",
	"__initcall_rtl88ee_driver_init6",
	"__initcall_asix_driver_init6",
	"__initcall_ax88179_178a_driver_init6",
	"__initcall_cdc_driver_init6",
	"__initcall_net1080_driver_init6",
	"__initcall_cdc_subset_driver_init6",
	"__initcall_zaurus_driver_init6",
	"__initcall_usbnet_init6",
	"__initcall_cdc_ncm_driver_init6",
	"__initcall_r8153_ecm_driver_init6",
	"__initcall_ehci_hcd_init6",
	"__initcall_ehci_pci_init6",
	"__initcall_ohci_hcd_mod_init6",
	"__initcall_ohci_pci_init6",
	"__initcall_uhci_hcd_init6",
	"__initcall_xhci_hcd_init6",
	"__initcall_xhci_pci_init6",
	"__initcall_i8042_init6",
	"__initcall_serport_init6",
	"__initcall_input_leds_init6",
	"__initcall_evdev_init6",
	"__initcall_atkbd_init6",
	"__initcall_psmouse_init6",
	"__initcall_pkg_temp_thermal_init6",
	"__initcall_thermal_throttle_init_device6",
	"__initcall_ledtrig_audio_init6",
	"__initcall_esrt_sysfs_init6",
	"__initcall_hid_init6",
	"__initcall_hid_generic_init6",
	"__initcall_i2c_hid_acpi_driver_init6",
	"__initcall_wmi_bmof_driver_init6",
	"__initcall_snd_ctl_led_init6",
	"__initcall_alsa_timer_init6",
	"__initcall_alsa_pcm_init6",
	"__initcall_generic_driver_init6",
	"__initcall_realtek_driver_init6",
	"__initcall_hdmi_driver_init6",
	"__initcall_snd_soc_init6",
	"__initcall_ac97_codec_driver_init6",
	"__initcall_dmic_driver_init6",
	"__initcall_hdmi_init6",
	"__initcall_skl_hda_audio_init6",
	"__initcall_sof_probes_client_drv_init6",
	"__initcall_snd_sof_pci_intel_skl_driver_init6",
	"__initcall_snd_sof_pci_intel_tgl_driver_init6",
	"__initcall_snd_sof_pci_intel_mtl_driver_init6",
	"__initcall_sock_diag_init6",
	"__initcall_packet_init6",
	"__initcall_kernel_do_mounts_initrd_sysctls_init7",
	"__initcall_sld_mitigate_sysctl_init7",
	"__initcall_hpet_insert_resource7",
	"__initcall_start_sync_check_timer7",
	"__initcall_update_mp_table7",
	"__initcall_lapic_insert_resource7",
	"__initcall_print_ipi_mode7",
	"__initcall_print_ICs7",
	"__initcall_create_tlb_single_page_flush_ceiling7",
	"__initcall_kernel_panic_sysctls_init7",
	"__initcall_kernel_panic_sysfs_init7",
	"__initcall_kernel_exit_sysctls_init7",
	"__initcall_kernel_exit_sysfs_init7",
	"__initcall_reboot_ksysfs_init7",
	"__initcall_sched_core_sysctl_init7",
	"__initcall_sched_fair_sysctl_init7",
	"__initcall_sched_rt_sysctl_init7",
	"__initcall_sched_dl_sysctl_init7",
	"__initcall_sched_clock_init_late7",
	"__initcall_sched_init_debug7",
	"__initcall_cpu_latency_qos_init7",
	"__initcall_printk_late_init7",
	"__initcall_load_system_certificate_list7",
	"__initcall_check_early_ioremap_leak7",
	"__initcall_init_root_keyring7",
	"__initcall_crypto_algapi_init7",
	"__initcall_pci_resource_alignment_sysfs_init7",
	"__initcall_pci_sysfs_init7",
	"__initcall_dmar_free_unused_resources7",
	"__initcall_sync_state_resume_initcall7",
	"__initcall_deferred_probe_initcall7",
	"__initcall_efi_shutdown_init7",
	"__initcall_efi_earlycon_unmap_fb7",
	"__initcall_regulatory_init_db7",
	"__initcall_pci_mmcfg_late_insert_resources7",
	"__initcall_acpi_gpio_handle_deferred_request_irqs7s",
	"__initcall_clk_disable_unused7s",
	"__initcall_alsa_sound_last_init7s",
	"__initcall_con_initcon",
	"__initcall_end",
	"__initcall_univ8250_console_initcon",
	"END_OF_INITCALL_ORDER_ARRAY_DUMMY_ENTRY"
};
