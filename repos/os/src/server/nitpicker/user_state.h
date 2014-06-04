/*
 * \brief  Nitpicker user state handling
 * \author Norman Feske
 * \date   2006-08-09
 *
 * This class comprehends the policy of user interaction.
 * It manages the toggling of Nitpicker's different modes
 * and routes input events to corresponding client sessions.
 */

/*
 * Copyright (C) 2006-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _USER_STATE_H_
#define _USER_STATE_H_

#include "mode.h"
#include "menubar.h"
#include "view_stack.h"
#include "global_keys.h"

class User_state : public Mode, public View_stack
{
	private:

		/*
		 * Policy for the routing of global keys
		 */
		Global_keys &_global_keys;

		/*
		 * Menubar to display trusted labeling information
		 * according to the current Mitpicker mode and the
		 * focused session.
		 */
		Menubar &_menubar;

		/*
		 * Current mouse cursor position
		 */
		Point _mouse_pos;

		/*
		 * Currently pointed-at session
		 */
		Session *_pointed_session = nullptr;

		/*
		 * Session that receives the current stream of input events
		 */
		Session *_input_receiver = nullptr;

		/*
		 * True while a global key sequence is processed
		 */
		bool _global_key_sequence = false;

		void _update_all();

	public:

		/**
		 * Constructor
		 */
		User_state(Global_keys &, Area view_stack_size, Menubar &);

		/**
		 * Handle input event
		 *
		 * This function controls the Nitpicker mode and the user state
		 * variables.
		 */
		void handle_event(Input::Event ev);

		/**
		 * Accessors
		 */
		Point mouse_pos() { return _mouse_pos; }

		/**
		 * Mode interface
		 */
		void forget(Session const &) override;
		void focused_session(Session *) override;
};

#endif
