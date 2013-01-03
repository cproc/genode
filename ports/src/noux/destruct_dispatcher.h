/*
 * \brief  Signal_dispatcher which adds a destruct queue element into a
 *         destruct queue
 * \author Christian Prochaska
 * \date   2013-01-03
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _NOUX__DESTRUCT_DISPATCHER_H_
#define _NOUX__DESTRUCT_DISPATCHER_H_

/* Noux includes */
#include <destruct_queue.h>
#include <signal_dispatcher.h>

namespace Noux {

	class Destruct_dispatcher : public Signal_dispatcher
	{
		private:

			Destruct_queue               &_destruct_queue;
			Destruct_queue::Element_base *_element;

		public:

			Destruct_dispatcher(Destruct_queue &destruct_queue, Destruct_queue::Element_base *element)
			: _destruct_queue(destruct_queue), _element(element) { }

			void dispatch()
			{
				_destruct_queue.insert(_element);
			}
	};

}

#endif /* _NOUX__DESTRUCT_DISPATCHER_H_ */
