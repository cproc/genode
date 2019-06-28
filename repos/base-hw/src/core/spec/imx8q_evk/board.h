/*
 * \brief  Board driver for core
 * \author Stefan Kalkowski
 * \date   2019-06-12
 */

/*
 * Copyright (C) 2019 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _CORE__SPEC__IMX8Q_EVK__BOARD_H_
#define _CORE__SPEC__IMX8Q_EVK__BOARD_H_

#include <hw/spec/arm_64/imx8q_evk_board.h>

namespace Board {
	using namespace Hw::Imx8q_evk_board;

	static constexpr bool SMP = true;
};

#endif /* _CORE__SPEC__IMX8Q_EVK__BOARD_H_ */
