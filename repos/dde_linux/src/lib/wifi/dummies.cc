/**
 * \brief  Dummy functions
 * \author Josef Soentgen
 * \date   2014-03-03
 */

/*
 * Copyright (C) 2014-2016 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <base/printf.h>
#include <base/sleep.h>

extern "C" {
	typedef long DUMMY;

enum {
	SHOW_DUMMY = 1,
	SHOW_SKIP  = 0,
	SHOW_RET   = 0,
};

#define DUMMY(retval, name) \
	DUMMY name(void) { \
	if (SHOW_DUMMY) \
		PDBG( #name " called (from %p) not implemented", __builtin_return_address(0)); \
	return retval; \
}

#define DUMMY_SKIP(retval, name) \
	DUMMY name(void) { \
		if (SHOW_SKIP) \
			PLOG( #name " called (from %p) skipped", __builtin_return_address(0)); \
	return retval; \
}

#define DUMMY_STOP(retval, name) \
	DUMMY name(void) { \
		do { \
			PWRN( #name " called (from %p) stopped", __builtin_return_address(0)); \
			Genode::sleep_forever(); \
		} while (0); \
	return retval; \
}

#define DUMMY_RET(retval, name) \
	DUMMY name(void) { \
		if (SHOW_RET) \
			PWRN( #name " called (from %p) return %d", __builtin_return_address(0), retval); \
	return retval; \
}

/* return sucessful */
DUMMY_RET(0, netdev_kobject_init)
DUMMY_RET(0, vlan_tx_nonzero_tag_present)
DUMMY_RET(0, vlan_tx_tag_present)
DUMMY_RET(1, capable)
DUMMY_RET(1, rcu_read_lock_bh_held)
DUMMY_RET(1, softirq_count);
DUMMY_RET(1, try_module_get)
DUMMY_RET(0, security_sk_alloc)
DUMMY_RET(0, net_ratelimit)
DUMMY_RET(0, static_key_false)
DUMMY_RET(1, dma_supported)
DUMMY_RET(1, ns_capable)
DUMMY_RET(0, sock_tx_timestamp)

/* not needed */
DUMMY_SKIP(-1, might_sleep)
DUMMY_SKIP(-1, read_lock_bh)
DUMMY_SKIP(-1, read_unlock_bh)
DUMMY_SKIP(-1, rwlock_init)
DUMMY_SKIP(-1, spin_lock_irqsave)
DUMMY_SKIP(-1, spin_unlock_irqrestore)
DUMMY_SKIP(-1, write_lock_bh)
DUMMY_SKIP(-1, write_unlock_bh)
DUMMY_SKIP(0, debugfs_create_dir)
DUMMY_SKIP(0, debugfs_remove_recursive)
DUMMY_SKIP(0, debugfs_rename)
DUMMY_SKIP(0, destroy_workqueue)
DUMMY_SKIP(0, device_create)
DUMMY_SKIP(0, device_initialize)
DUMMY_SKIP(0, down_read)
DUMMY_SKIP(0, down_write)
DUMMY_SKIP(0, free_irq)
DUMMY_SKIP(0, init_rwsem)
DUMMY_SKIP(0, iounmap)
DUMMY_SKIP(0, kparam_block_sysfs_write)
DUMMY_SKIP(0, kparam_unblock_sysfs_write)
DUMMY_SKIP(0, pci_disable_device)
DUMMY_SKIP(0, pci_disable_link_state)
DUMMY_SKIP(0, pci_disable_msi)
DUMMY_SKIP(0, pci_enable_device)
DUMMY_SKIP(0, pci_enable_msi)
DUMMY_SKIP(0, pci_release_regions)
DUMMY_SKIP(0, pci_request_regions)
DUMMY_SKIP(0, pci_set_master)
DUMMY_SKIP(0, put_device)
DUMMY_SKIP(0, scm_destroy)
DUMMY_SKIP(0, scm_recv)
DUMMY_SKIP(0, scm_send)
DUMMY_SKIP(0, security_netlink_send)
DUMMY_SKIP(0, sk_filter)
DUMMY_SKIP(0, sk_filter_release_rcu)
DUMMY_SKIP(-1, sk_filter_size)
DUMMY_SKIP(0, spin_lock)
DUMMY_SKIP(0, spin_lock_bh)
DUMMY_SKIP(0, spin_lock_init)
DUMMY_SKIP(0, spin_lock_irq)
DUMMY_SKIP(0, spin_unlock)
DUMMY_SKIP(0, spin_unlock_bh)
DUMMY_SKIP(0, spin_unlock_irq)
DUMMY_SKIP(0, up_read)
DUMMY_SKIP(0, up_write)
DUMMY_SKIP(1, in_interrupt) /* XXX * */
DUMMY_SKIP(0, in_softirq) /* XXX */
DUMMY_SKIP(0, is_vmalloc_addr) /* XXX */
DUMMY_SKIP(0, in_irq) /* XXX */

DUMMY_SKIP(0, local_bh_disable)
DUMMY_SKIP(0, local_bh_enable)
DUMMY_SKIP(0, dma_unmap_page)

DUMMY_SKIP(0, dma_set_coherent_mask) /* we set the mask always to ~0UL */
DUMMY_SKIP(0, dma_set_mask)          /* in the PCI driver */
DUMMY(-1, dma_sync_single_for_cpu)
DUMMY(-1, dma_sync_single_for_device)


/* XXX DUMMY_SKIP safe? */
DUMMY_SKIP(0, dma_unmap_single)
DUMMY_SKIP(0, kunmap)
DUMMY_SKIP(0, kunmap_atomic)

DUMMY_SKIP(-1, dump_stack)
DUMMY_SKIP(-1, gfp_pfmemalloc_allowed)
DUMMY_SKIP(-1, prefetchw)
DUMMY_SKIP(-1, write_lock_irq)
DUMMY_SKIP(-1, write_lock_irqsave)
DUMMY_SKIP(-1, write_unlock_irq)
DUMMY_SKIP(-1, write_unlock_irqrestore)
DUMMY_SKIP(0, read_lock)
DUMMY_SKIP(0, read_unlock)
DUMMY_SKIP(0, sysfs_create_group)
DUMMY_SKIP(0, sysfs_remove_group)
DUMMY_SKIP(0, sysfs_create_link)
DUMMY_SKIP(0, sysfs_remove_link)
DUMMY_SKIP(0, module_put)

DUMMY(0, device_release_driver)
DUMMY(-1, __vlan_put_tag)
DUMMY(-1, dev_printk_emit)
DUMMY(-1, gen_kill_estimator)
DUMMY(-1, generic_pipe_buf_confirm)
DUMMY(-1, generic_pipe_buf_map)
DUMMY(-1, generic_pipe_buf_unmap)
DUMMY(-1, linkwatch_fire_event)
DUMMY(-1, need_resched)
DUMMY(-1, open_softirq)
DUMMY(-1, raw_notifier_chain_unregister)
DUMMY(-1, vlan_do_receive)
DUMMY(-1, vlan_untag)
DUMMY(-1, yield)
DUMMY(0, IS_ERR)
DUMMY(0, __raise_softirq_irqoff)
DUMMY(0, __this_cpu_read)
DUMMY(-1, add_device_randomness)
DUMMY(0, add_uevent_var)
DUMMY(0, atomic64_read)
DUMMY(0, bitmap_empty)
DUMMY_SKIP(0, call_rcu)
DUMMY(0, csum_block_add)
DUMMY(0, csum_fold)
DUMMY(0, csum_partial)
DUMMY(0, device_del)
DUMMY(0, device_rename)
DUMMY(0, device_unregister)
DUMMY(0, do_posix_clock_monotonic_gettime)
DUMMY(0, do_softirq)
DUMMY(0, flush_delayed_work)
DUMMY(0, flush_work)
DUMMY(0, flush_workqueue)
DUMMY(0, genl_dump_check_consistent)
DUMMY(0, genl_info_net)
DUMMY(0, genlmsg_cancel)
DUMMY(0, genlmsg_end)
DUMMY(0, genlmsg_multicast)
DUMMY(0, genlmsg_multicast_netns)
DUMMY(0, genlmsg_reply)
DUMMY(0, genlmsg_unicast)
DUMMY(0, get_cpu)
DUMMY(0, get_random_bytes)
DUMMY(0, get_seconds)
DUMMY(0, hweight16)
DUMMY(0, hweight64)
DUMMY(0, idr_alloc)
DUMMY(0, idr_destroy)
DUMMY(0, idr_find)
DUMMY(0, idr_for_each)
DUMMY(0, idr_init)
DUMMY(0, idr_remove)
DUMMY(0, ip_hdr)
DUMMY(0, ipv4_get_dsfield)
DUMMY(0, ipv6_get_dsfield)
DUMMY(0, ipv6_hdr)
DUMMY(0, irqs_disabled)
DUMMY(0, isalpha)
DUMMY(0, jhash_2words)
DUMMY(0, kmem_cache_destroy)
DUMMY(0, kobject_uevent)
DUMMY(0, kobject_uevent_env)
DUMMY(0, kstrtoul)
DUMMY(0, linkwatch_init_dev)
DUMMY(0, local_softirq_pending)
DUMMY(0, mb)
DUMMY_SKIP(0, misc_register)
DUMMY(0, mmiowb)
DUMMY(0, net_dmaengine_put)
DUMMY(0, net_info_ratelimited)
DUMMY(0, net_ns_type_operations)
DUMMY(0, netdev_pick_tx)
DUMMY(0, netdev_register_kobject)
DUMMY(0, netdev_unregister_kobject)
DUMMY(0, netpoll_poll_lock)
DUMMY(0, netpoll_poll_unlock)
DUMMY(0, netpoll_receive_skb)
DUMMY(0, netpoll_rx)
DUMMY(0, netpoll_rx_disable)
DUMMY(0, netpoll_rx_enable)
DUMMY(0, nla_data)
DUMMY(0, nla_get_be32)
DUMMY(0, nla_get_flag)
DUMMY(0, nla_get_s8)
DUMMY(0, nla_get_u16)
DUMMY(0, nla_get_u32)
DUMMY(0, nla_get_u64)
DUMMY(0, nla_get_u8)
DUMMY(0, nla_len)
DUMMY(0, nla_nest_end)
DUMMY(0, nla_nest_start)
DUMMY(0, nla_parse_nested)
DUMMY(0, nla_put_be32)
DUMMY(0, nla_put_flag)
DUMMY(0, nla_put_string)
DUMMY(0, nla_put_u16)
DUMMY(0, nla_put_u32)
DUMMY(0, nla_put_u64)
DUMMY(0, nla_put_u8)
DUMMY(0, nla_type)
DUMMY(0, nlmsg_free)
DUMMY(0, nlmsg_new)
DUMMY(0, nlmsg_parse)
DUMMY(0, nonseekable_open)
DUMMY(0, ntohs)
DUMMY(0, pci_pme_capable)
DUMMY(0, pm_qos_add_notifier)
DUMMY(0, pm_qos_remove_notifier)
DUMMY(0, pm_qos_request)
DUMMY(0, poll_wait)
DUMMY(0, preempt_disable)
DUMMY(0, preempt_enable)
DUMMY(0, put_cpu)
DUMMY(0, put_unaligned_le64)
DUMMY(0, qdisc_all_tx_empty)
DUMMY(0, raise_softirq_irqoff)
DUMMY(0, rcu_barrier)
DUMMY(0, rcu_read_lock_bh)
DUMMY(0, rcu_read_lock_held)
DUMMY(0, rcu_read_unlock_bh)
DUMMY(-1, release_net)
DUMMY(0, rtnl_dereference)
DUMMY(-1, seq_printf)
DUMMY(0, smp_mb)
DUMMY(0, smp_mb__before_clear_bit)
DUMMY(0, smp_processor_id)
DUMMY(0, smp_rmb)
DUMMY(0, sock_net)
DUMMY(0, sock_release)
DUMMY(0, spin_trylock)
DUMMY(0, sscanf)
DUMMY(0, synchronize_irq)
DUMMY(0, synchronize_rcu_expedited)
DUMMY(0, tasklet_disable)
DUMMY(0, tasklet_enable)
DUMMY(0, tasklet_kill)
DUMMY(0, tcp_hdrlen)
DUMMY(0, this_cpu_dec)
DUMMY(0, this_cpu_inc)
DUMMY(0, toupper)
DUMMY(0, wait_event_interruptible)
DUMMY(0, work_busy)
DUMMY_SKIP(0, class_register)
DUMMY(0, class_unregister)
DUMMY(0, debugfs_remove)
DUMMY(0, dev_set_uevent_suppress)
DUMMY(0, ethtool_op_get_link)
DUMMY(0, genl_register_family_with_ops)
DUMMY(0, genl_register_mc_group)
DUMMY(0, init_utsname)
DUMMY(0, platform_device_unregister)
DUMMY(0, atomic_notifier_chain_register)
DUMMY(0, __get_free_pages)
DUMMY(0, __get_free_page)
DUMMY(0, __set_current_state)
DUMMY(0, add_wait_queue)
DUMMY(0, add_wait_queue_exclusive)
DUMMY(0, atomic_notifier_call_chain)
DUMMY(0, cond_resched)
DUMMY(0, current)
DUMMY(0, debug_check_no_locks_freed)
DUMMY(0, get_user)
DUMMY(0, jhash_1word)
DUMMY(0, kernel_sendmsg)
DUMMY(0, put_cmsg)
DUMMY(0, put_cred)
DUMMY(0, put_pid)
DUMMY(0, schedule)
DUMMY(-1, security_sk_free)
DUMMY(0, set_current_state)
DUMMY(0, signal_pending)
DUMMY(0, smp_wmb)
DUMMY_SKIP(0, sock_update_classid)
DUMMY_SKIP(0, sock_update_netprioidx)
DUMMY(0, sock_wake_async)
DUMMY(0, static_key_slow_dec)
DUMMY(0, task_tgid_vnr)
DUMMY(0, totalram_pages)
DUMMY(0, wake_up_interruptible_poll)

DUMMY(-1, __this_cpu_write)
DUMMY(-1, csum_block_add_ext)
DUMMY(-1, csum_partial_ext)
DUMMY(-1, genl_register_family_with_ops_groups)
DUMMY(-1, ktime_sub)
DUMMY(-1, sg_init_one)
DUMMY(-1, vlan_hw_offload_capable)
DUMMY(-1, vlan_tx_tag_get_id)
DUMMY(-1, vzalloc)
DUMMY(-1, tsk_restore_flags)
DUMMY(-1, put_user)

DUMMY(-1, write_seqcount_begin)
DUMMY(-1, write_seqcount_end)
DUMMY(-1, net_dmaengine_get)
DUMMY(-1, audit_enabled)
DUMMY(-1, audit_get_sessionid)
DUMMY(-1, audit_get_loginuid)
DUMMY(-1, init_user_ns)
DUMMY(-1, from_kgid)
DUMMY(-1, from_kuid)
DUMMY(-1, pci_num_vf)
DUMMY(-1, audit_log)
DUMMY(-1, linkwatch_forget_dev)
DUMMY(-1, linkwatch_run_queue)
DUMMY(-1, on_each_cpu)
DUMMY(-1, kobject_put)
DUMMY(-1, mq_qdisc_ops)
DUMMY(-1, get_net_ns_by_pid)
DUMMY(-1, get_net_ns_by_fd)
DUMMY(-1, unregister_pernet_device)

DUMMY(0, __hw_addr_init)
DUMMY(0, __hw_addr_sync)
DUMMY(0, __hw_addr_unsync)
DUMMY_SKIP(0, dev_alloc_name)
DUMMY(0, dev_change_net_namespace)
DUMMY(0, dev_close)
DUMMY(0, dev_kfree_skb_any)
DUMMY_SKIP(0, dev_net_set)
DUMMY(0, dev_open)
DUMMY_SKIP(0, dev_hold)
DUMMY_SKIP(0, dev_put)
DUMMY(0, dst_release)
DUMMY(0, free_netdev)
DUMMY(0, net_disable_timestamp)
DUMMY(0, next_net_device)
DUMMY_SKIP(0, synchronize_net)
DUMMY(0, unregister_netdevice_many)
DUMMY(0, unregister_netdevice_notifier)
DUMMY(0, unregister_netdevice_queue)
DUMMY(-1, task_pid_nr)
DUMMY(-1, pid_vnr)
DUMMY(-1, from_kuid_munged)
DUMMY(-1, from_kgid_munged)
DUMMY(-1, sk_get_filter)
DUMMY(-1, sk_detach_filter)
DUMMY(-1, cond_resched_softirq)
DUMMY(-1, prefetch)
DUMMY(-1, security_socket_getpeersec_stream)
DUMMY(-1, net_enable_timestamp)
DUMMY(-1, __lock_sock)
DUMMY(-1, bpf_tell_extensions)

DUMMY(0, atomic64_set)
DUMMY(0, device_can_wakeup)
DUMMY(0, device_reprobe)
DUMMY(0, device_set_wakeup_enable)
DUMMY(0, ip_fast_csum)
DUMMY(0, request_firmware)
DUMMY(0, tcp_v4_check)
DUMMY(0, sk_attach_filter)

DUMMY(0, __class_create)
DUMMY(0, __module_get)
DUMMY(0, __sock_recv_timestamp)
DUMMY(0, __sock_recv_wifi_status)
DUMMY(0, _sigprocmask)
DUMMY(0, acpi_match_device)
DUMMY(0, atomic_notifier_chain_unregister)
DUMMY(0, can_checksum_protocol)
DUMMY(0, class_destroy)
DUMMY(0, clk_disable)
DUMMY(0, clk_enable)
DUMMY(0, csum_ipv6_magic)
DUMMY(0, csum_partial_copy)
DUMMY(0, csum_sub)
DUMMY(0, csum_tcpudp_nofold)
DUMMY(0, devm_clk_get)
DUMMY(0, devm_gpiod_get_index)
DUMMY(0, devm_kzalloc)
DUMMY(0, file_inode)
DUMMY(0, gpiod_direction_output)
DUMMY(0, gpiod_set_value)
DUMMY(0, hex_dump_to_buffer)
DUMMY(0, __ilog2_u32)
DUMMY(0, __ilog2_u64)
DUMMY(0, input_close_device)
DUMMY(0, input_open_device)
DUMMY(0, input_register_handle)
DUMMY(0, input_register_handler)
DUMMY(0, input_unregister_handle)
DUMMY(0, input_unregister_handler)
DUMMY(0, ip_hdrlen)
DUMMY(0, ipv6_authlen)
DUMMY(0, ipv6_optlen)
DUMMY(0, ktime_to_timespec)
DUMMY(0, ktime_to_timeval)
DUMMY(0, misc_deregister)
DUMMY(0, net_gso_ok)
DUMMY(0, nosteal_pipe_buf_ops)
DUMMY(0, pci_unregister_driver)
DUMMY(0, platform_get_drvdata)
DUMMY(0, platform_set_drvdata)
DUMMY(0, regulator_disable)
DUMMY(0, regulator_enable)
DUMMY(0, regulator_get_exclusive)
DUMMY(0, regulator_is_enabled)
DUMMY(0, regulator_put)
DUMMY(0, rfkill_epo)
DUMMY(0, rfkill_get_global_sw_state)
DUMMY(0, rfkill_is_epo_lock_active)
DUMMY(0, rfkill_remove_epo_lock)
DUMMY(0, rfkill_restore_states)
DUMMY(0, rfkill_switch_all)
DUMMY(0, send_sigurg)
DUMMY(0, simple_strtoul)
DUMMY(0, skb_gro_len)
DUMMY(0, skb_gro_offset)
DUMMY(0, skb_network_protocol)
DUMMY(0, spin_lock_nested)
DUMMY(0, splice_to_pipe)
DUMMY(0, static_key_slow_inc)
DUMMY(0, tcp_hdr)
DUMMY(0, textsearch_find)
DUMMY(0, udp_hdr)
DUMMY(0, virt_to_page)
DUMMY(0, xfrm_sk_clone_policy)
DUMMY(0, xfrm_sk_free_policy)
DUMMY(0, strncpy)

DUMMY(0, __ethtool_get_settings)
DUMMY(0, __skb_get_hash)
DUMMY(0, __sock_recv_ts_and_drops)
DUMMY(0, cpu_relax)
DUMMY(0, dev_get_by_name)
DUMMY(0, dev_get_by_name_rcu)
DUMMY(0, dev_mc_add)
DUMMY(0, dev_mc_del)
DUMMY(0, dev_remove_pack)
DUMMY(0, dev_set_allmulti)
DUMMY(0, dev_set_promiscuity)
DUMMY(0, dev_uc_add)
DUMMY(0, dev_uc_del)
DUMMY(0, dev_xmit_complete)
DUMMY(0, ethtool_cmd_speed)
DUMMY(0, flush_dcache_page)
DUMMY(0, getnstimeofday)
DUMMY(0, ip_check_defrag)
DUMMY(0, ktime_to_timespec_cond)
DUMMY(0, netdev_get_tx_queue)
DUMMY(0, offset_in_page)
DUMMY(0, prandom_u32_max)
DUMMY(0, raw_smp_processor_id)
DUMMY(0, sk_run_filter)
DUMMY(0, skb_flow_dissect)
DUMMY(0, sock_unregister)
DUMMY(0, txq_trans_update)
DUMMY(0, unregister_pernet_subsys)
DUMMY(0, vm_insert_page)
DUMMY(0, vmalloc_to_page)

DUMMY(0, __dev_change_flags)
DUMMY(0, __dev_get_by_name)
DUMMY(0, __dev_notify_flags)
DUMMY(0, call_netdevice_notifiers)
DUMMY(0, dev_base_lock)
DUMMY(0, dev_change_carrier)
DUMMY(0, dev_change_flags)
DUMMY(0, dev_change_name)
DUMMY(0, dev_get_flags)
DUMMY(0, dev_get_phys_port_id)
DUMMY(0, dev_get_stats)
DUMMY(0, dev_mc_add_excl)
DUMMY(0, dev_set_alias)
DUMMY(0, dev_set_group)
DUMMY(0, dev_set_mac_address)
DUMMY(0, dev_set_mtu)
DUMMY(0, dev_uc_add_excl)
DUMMY(0, is_link_local_ether_addr)
DUMMY(0, jiffies_to_clock_t)
DUMMY(0, netdev_boot_setup)
DUMMY(0, netdev_master_upper_dev_get)
DUMMY(0, netdev_state_change)
DUMMY(0, netdev_unregistering_wq)

DUMMY(0, autoremove_wake_function)
DUMMY(0, get_user_pages_fast)
DUMMY(0, memcpy_toiovecend)
DUMMY(0, netdev_rx_csum_fault)
DUMMY(0, release_pages)
DUMMY(0, sk_busy_loop)
DUMMY(0, sk_can_busy_loop)

DUMMY_SKIP(0, complete_all)
DUMMY_SKIP(0, module_put_and_exit)
DUMMY_SKIP(0, simple_strtol)
DUMMY_SKIP(0, alg_test)

DUMMY(0, __cfg80211_leave_ocb)
DUMMY(0, __skb_flow_dissect)
DUMMY(0, __sock_tx_timestamp)
DUMMY(0, bpf_prog_create_from_user)
DUMMY(0, bpf_prog_destroy)
DUMMY(0, bpf_prog_run_clear_cb)
DUMMY(0, cfg80211_join_ocb)
DUMMY(0, cfg80211_leave_ocb)
DUMMY(0, class_find_device)
DUMMY(0, config_enabled)
DUMMY(0, dev_change_proto_down)
DUMMY(0, dev_get_iflink)
DUMMY(0, dev_get_phys_port_name)
DUMMY(0, device_create_with_groups)
DUMMY(0, device_enable_async_suspend)
DUMMY(0, fatal_signal_pending)
DUMMY_RET(1, file_ns_capable)
DUMMY(0, flow_keys_dissector)
DUMMY(0, get_net_ns_by_id)
DUMMY(0, gfpflags_allow_blocking)
DUMMY(0, init_dummy_netdev)
DUMMY(0, napi_gro_flush)
DUMMY(0, netdev_start_xmit)
DUMMY_RET(0, netdev_uses_dsa)
DUMMY_RET(0, page_is_pfmemalloc)
DUMMY_RET(0, peernet2id)
DUMMY(0, pr_err_once)
DUMMY(0, sk_attach_bpf)
DUMMY(0, sk_filter_uncharge)
DUMMY(0, skb_checksum_help)
DUMMY(0, skb_get_tx_queue)
DUMMY(0, skb_gso_segment)
DUMMY(0, skb_vlan_tag_get)
DUMMY(0, skb_vlan_tag_present)
DUMMY(0, smp_mb__before_atomic)
DUMMY(0, sock_diag_broadcast_destroy)
DUMMY(0, sock_diag_has_destroy_listeners)
DUMMY(0, switchdev_port_attr_get)

DUMMY_RET(0, netif_dormant)
DUMMY(0, netif_napi_add)
DUMMY(0, netif_napi_del)
DUMMY_STOP(0, netif_rx)
DUMMY(0, netif_skb_features)
DUMMY(0, netif_supports_nofcs)
DUMMY(0, netif_xmit_frozen_or_drv_stopped)
DUMMY(0, netif_xmit_frozen_or_stopped)
DUMMY_STOP(0, netif_rx_ni)
DUMMY_STOP(0, netif_tx_start_all_queues)
DUMMY_STOP(0, netif_tx_stop_all_queues)

DUMMY(0, peernet_has_id)
DUMMY(0, peernet2id_alloc)

} /* extern "C" */
