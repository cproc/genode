/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef JSPerformanceNavigation_h
#define JSPerformanceNavigation_h

#if ENABLE(WEB_TIMING)

#include "JSDOMBinding.h"
#include "PerformanceNavigation.h"
#include <runtime/JSGlobalObject.h>
#include <runtime/JSObject.h>
#include <runtime/ObjectPrototype.h>

namespace WebCore {

class JSPerformanceNavigation : public JSDOMWrapper {
public:
    typedef JSDOMWrapper Base;
    static JSPerformanceNavigation* create(JSC::Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<PerformanceNavigation> impl)
    {
        JSPerformanceNavigation* ptr = new (NotNull, JSC::allocateCell<JSPerformanceNavigation>(globalObject->globalData().heap)) JSPerformanceNavigation(structure, globalObject, impl);
        ptr->finishCreation(globalObject->globalData());
        return ptr;
    }

    static JSC::JSObject* createPrototype(JSC::ExecState*, JSC::JSGlobalObject*);
    static bool getOwnPropertySlot(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName, JSC::PropertySlot&);
    static bool getOwnPropertyDescriptor(JSC::JSObject*, JSC::ExecState*, JSC::PropertyName, JSC::PropertyDescriptor&);
    static void destroy(JSC::JSCell*);
    ~JSPerformanceNavigation();
    static const JSC::ClassInfo s_info;

    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), &s_info);
    }

    PerformanceNavigation* impl() const { return m_impl; }
    void releaseImpl() { m_impl->deref(); m_impl = 0; }

    void releaseImplIfNotNull() { if (m_impl) { m_impl->deref(); m_impl = 0; } }

private:
    PerformanceNavigation* m_impl;
protected:
    JSPerformanceNavigation(JSC::Structure*, JSDOMGlobalObject*, PassRefPtr<PerformanceNavigation>);
    void finishCreation(JSC::JSGlobalData&);
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | JSC::InterceptsGetOwnPropertySlotByIndexEvenWhenLengthIsNotZero | Base::StructureFlags;
};

class JSPerformanceNavigationOwner : public JSC::WeakHandleOwner {
public:
    virtual bool isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown>, void* context, JSC::SlotVisitor&);
    virtual void finalize(JSC::Handle<JSC::Unknown>, void* context);
};

inline JSC::WeakHandleOwner* wrapperOwner(DOMWrapperWorld*, PerformanceNavigation*)
{
    DEFINE_STATIC_LOCAL(JSPerformanceNavigationOwner, jsPerformanceNavigationOwner, ());
    return &jsPerformanceNavigationOwner;
}

inline void* wrapperContext(DOMWrapperWorld* world, PerformanceNavigation*)
{
    return world;
}

JSC::JSValue toJS(JSC::ExecState*, JSDOMGlobalObject*, PerformanceNavigation*);
PerformanceNavigation* toPerformanceNavigation(JSC::JSValue);

class JSPerformanceNavigationPrototype : public JSC::JSNonFinalObject {
public:
    typedef JSC::JSNonFinalObject Base;
    static JSC::JSObject* self(JSC::ExecState*, JSC::JSGlobalObject*);
    static JSPerformanceNavigationPrototype* create(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::Structure* structure)
    {
        JSPerformanceNavigationPrototype* ptr = new (NotNull, JSC::allocateCell<JSPerformanceNavigationPrototype>(globalData.heap)) JSPerformanceNavigationPrototype(globalData, globalObject, structure);
        ptr->finishCreation(globalData);
        return ptr;
    }

    static const JSC::ClassInfo s_info;
    static bool getOwnPropertySlot(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName, JSC::PropertySlot&);
    static bool getOwnPropertyDescriptor(JSC::JSObject*, JSC::ExecState*, JSC::PropertyName, JSC::PropertyDescriptor&);
    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), &s_info);
    }

private:
    JSPerformanceNavigationPrototype(JSC::JSGlobalData& globalData, JSC::JSGlobalObject*, JSC::Structure* structure) : JSC::JSNonFinalObject(globalData, structure) { }
protected:
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | Base::StructureFlags;
};

// Attributes

JSC::JSValue jsPerformanceNavigationType(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsPerformanceNavigationRedirectCount(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
// Constants

JSC::JSValue jsPerformanceNavigationTYPE_NAVIGATE(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsPerformanceNavigationTYPE_RELOAD(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsPerformanceNavigationTYPE_BACK_FORWARD(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsPerformanceNavigationTYPE_RESERVED(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);

} // namespace WebCore

#endif // ENABLE(WEB_TIMING)

#endif
