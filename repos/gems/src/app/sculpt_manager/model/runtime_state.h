/*
 * \brief  State of the components hosted in the runtime subsystem
 * \author Norman Feske
 * \date   2018-08-22
 */

/*
 * Copyright (C) 2018 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _MODEL__RUNTIME_STATE_H_
#define _MODEL__RUNTIME_STATE_H_

/* Genode includes */
#include <util/xml_node.h>
#include <util/list_model.h>

/* local includes */
#include <types.h>
#include <runtime.h>

namespace Sculpt { class Runtime_state; }

class Sculpt::Runtime_state : public Runtime_info
{
	public:

		struct Info
		{
			bool selected;

			unsigned long assigned_ram;
			unsigned long avail_ram;

			unsigned long assigned_caps;
			unsigned long avail_caps;
		};

	private:

		Allocator &_alloc;

		struct Child : List_model<Child>::Element
		{
			Start_name const name;

			Info info { false, 0, 0, 0, 0 };

			bool abandoned_by_user = false;

			Child(Start_name const &name) : name(name) { }
		};

		List_model<Child> _children { };

		/**
		 * Child present in initial deploy config but interactively removed
		 */
		struct Abandoned_child : Interface
		{
			Start_name const name;
			Abandoned_child(Start_name const &name) : name(name) { };
		};

		Registry<Registered<Abandoned_child> > _abandoned_children { };

		/**
		 * Child that was interactively launched
		 */
		struct Launched_child : Interface
		{
			Start_name const name;
			Path       const launcher;
			Launched_child(Start_name const &name, Path const &launcher)
			: name(name), launcher(launcher) { };
		};

		Registry<Registered<Launched_child> > _launched_children { };

		struct Update_policy : List_model<Child>::Update_policy
		{
			Allocator &_alloc;

			Update_policy(Allocator &alloc) : _alloc(alloc) { }

			void destroy_element(Child &elem)
			{
				destroy(_alloc, &elem);
			}

			Child &create_element(Xml_node node)
			{
				return *new (_alloc)
					Child(node.attribute_value("name", Start_name()));
			}

			void update_element(Child &child, Xml_node node)
			{
				if (node.has_sub_node("ram")) {
					Xml_node const ram = node.sub_node("ram");
					child.info.assigned_ram = max(ram.attribute_value("assigned", Number_of_bytes()),
					                              ram.attribute_value("quota",    Number_of_bytes()));
					child.info.avail_ram    = ram.attribute_value("avail",    Number_of_bytes());
				}

				if (node.has_sub_node("caps")) {
					Xml_node const caps = node.sub_node("caps");
					child.info.assigned_caps = max(caps.attribute_value("assigned", 0UL),
					                               caps.attribute_value("quota",    0UL));
					child.info.avail_caps    = caps.attribute_value("avail",    0UL);
				}
			}

			static bool element_matches_xml_node(Child const &elem, Xml_node node)
			{
				return node.attribute_value("name", Start_name()) == elem.name;
			}
		};

	public:

		Runtime_state(Allocator &alloc) : _alloc(alloc) { }

		~Runtime_state() { reset_abandoned_and_launched_children(); }

		void update_from_state_report(Xml_node state)
		{
			Update_policy policy(_alloc);
			_children.update_from_xml(policy, state);
		}

		/**
		 * Runtime_info interface
		 */
		bool present_in_runtime(Start_name const &name) const override
		{
			bool result = false;
			_children.for_each([&] (Child const &child) {
				if (!result && child.name == name)
					result = true; });
			return result;
		}

		/**
		 * Runtime_info interface
		 */
		bool abandoned_by_user(Start_name const &name) const override
		{
			bool result = false;
			_abandoned_children.for_each([&] (Abandoned_child const &child) {
				if (!result && child.name == name)
					result = true; });
			return result;
		}

		/**
		 * Runtime_info interface
		 */
		void gen_launched_deploy_start_nodes(Xml_generator &xml) const override
		{
			_launched_children.for_each([&] (Launched_child const &child) {
				gen_named_node(xml, "start", child.name, [&] () {
					if (child.name != child.launcher)
						xml.attribute("launcher", child.launcher); }); });
		}

		Info info(Start_name const &name) const
		{
			Info result { .selected = false, 0, 0, 0, 0 };
			_children.for_each([&] (Child const &child) {
				if (child.name == name)
					result = child.info; });
			return result;
		}

		Start_name selected() const
		{
			Start_name result;
			_children.for_each([&] (Child const &child) {
				if (child.info.selected)
					result = child.name; });
			return result;
		}

		void toggle_selection(Start_name const &name)
		{
			_children.for_each([&] (Child &child) {
				child.info.selected = (child.name == name) && !child.info.selected; });
		}

		void abandon(Start_name const &name)
		{
			/*
			 * If child was launched interactively, remove corresponding
			 * entry from '_launched_children'.
			 */
			bool was_interactively_launched = false;
			_launched_children.for_each([&] (Launched_child &child) {
				if (child.name == name) {
					was_interactively_launched = true;
					destroy(_alloc, &child);
				}
			});

			if (was_interactively_launched)
				return;

			/*
			 * Child was present at initial deploy config, mark as abandoned
			 */
			new (_alloc) Registered<Abandoned_child>(_abandoned_children, name);
		}

		void launch(Start_name const &name, Path const &launcher)
		{
			new (_alloc) Registered<Launched_child>(_launched_children, name, launcher);
		}

		void reset_abandoned_and_launched_children()
		{
			_abandoned_children.for_each([&] (Abandoned_child &child) {
				destroy(_alloc, &child); });

			_launched_children.for_each([&] (Launched_child &child) {
				destroy(_alloc, &child); });
		}
};

#endif /* _MODEL__RUNTIME_STATE_H_ */
