/*
 * \brief  Genode backend for GDBServer
 * \author Christian Prochaska
 * \date   2011-05-06
 */

/*
 * Copyright (C) 2011-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/*
 * Based on gdbserver/linux-low.h
 */

#ifndef GENODE_LOW_H
#define GENODE_LOW_H

void *genode_start_inferior();

void genode_wait_for_target_main_thread();
#if 0
void genode_detect_all_threads();
#endif
unsigned long genode_find_lwpid_of_new_thread();

void genode_stop_all_threads();
void genode_resume_all_threads();

void genode_stop_thread(unsigned long lwpid);
void genode_continue_thread(unsigned long lwpid, int single_step);

int genode_thread_signal_pipe_read_fd(unsigned long lwpid);
int genode_send_signal_to_thread(unsigned long lwipd, int signo, unsigned long *payload);

int genode_fetch_register(int regno, unsigned long *reg_content);
void genode_store_register(int regno, unsigned long reg_content);
unsigned char genode_read_memory_byte(void *addr);
void genode_write_memory_byte(void *addr, unsigned char value);

int genode_detach(int pid);
int genode_kill(int pid);

#endif /* GENODE_LOW_H */
