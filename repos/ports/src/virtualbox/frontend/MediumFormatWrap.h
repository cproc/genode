#ifndef ____H_MEDIUMFORMATWRAP
#define ____H_MEDIUMFORMATWRAP

#include "VirtualBoxBase.h"

class MediumFormatWrap :
	public VirtualBoxBase,
	VBOX_SCRIPTABLE_IMPL(IMediumFormatWrap)
{
	public:

		VIRTUALBOXBASE_ADD_ERRORINFO_SUPPORT(VirtualBoxErrorInfo, IVirtualBoxErrorInfo)
};

#endif /* ____H_MEDIUMFORMATWRAP */
