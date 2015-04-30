#ifndef ___VBox_com_VirtualBox_h
#define ___VBox_com_VirtualBox_h

// for convenience
#include <VBox/com/defs.h>
#include <VBox/com/ptr.h>

/* definitions which are needed for VirtualBox_XPCOM.h */

#include "xpcom/nsID.h"

/* nscore.h */
#define NS_IMETHOD_(type) virtual type
#define NS_IMETHOD NS_IMETHOD_(nsresult)

/* nsISupports.h? */
class nsISupports
{
	public:

		/* make the class polymorphic, so it can be used with 'dynamic_cast' */
		virtual ~nsISupports() { }

		void AddRef() { }
		void Release() { }
};

/* nsIException.h? */
class nsIException : public nsISupports { };

typedef nsISupports IUnknown; 

// generated VirtualBox COM library definition file
#include <sdk/VirtualBox_XPCOM.h>

#endif

