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

#ifndef JSLocation_h
#define JSLocation_h

#include "JSDOMBinding.h"
#include "Location.h"
#include <runtime/JSGlobalObject.h>
#include <runtime/JSObject.h>
#include <runtime/ObjectPrototype.h>

namespace WebCore {

class JSLocation : public JSDOMWrapper {
public:
    typedef JSDOMWrapper Base;
    static JSLocation* create(JSC::Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<Location> impl)
    {
        JSLocation* ptr = new (NotNull, JSC::allocateCell<JSLocation>(globalObject->globalData().heap)) JSLocation(structure, globalObject, impl);
        ptr->finishCreation(globalObject->globalData());
        return ptr;
    }

    static JSC::JSObject* createPrototype(JSC::ExecState*, JSC::JSGlobalObject*);
    static bool getOwnPropertySlot(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName, JSC::PropertySlot&);
    static bool getOwnPropertyDescriptor(JSC::JSObject*, JSC::ExecState*, JSC::PropertyName, JSC::PropertyDescriptor&);
    static bool getOwnPropertySlotByIndex(JSC::JSCell*, JSC::ExecState*, unsigned propertyName, JSC::PropertySlot&);
    bool getOwnPropertySlotDelegate(JSC::ExecState*, JSC::PropertyName, JSC::PropertySlot&);
    bool getOwnPropertyDescriptorDelegate(JSC::ExecState*, JSC::PropertyName, JSC::PropertyDescriptor&);
    static void put(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName, JSC::JSValue, JSC::PutPropertySlot&);
    static void putByIndex(JSC::JSCell*, JSC::ExecState*, unsigned propertyName, JSC::JSValue, bool shouldThrow);
    bool putDelegate(JSC::ExecState*, JSC::PropertyName, JSC::JSValue, JSC::PutPropertySlot&);
    static void destroy(JSC::JSCell*);
    ~JSLocation();
    static const JSC::ClassInfo s_info;

    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), &s_info);
    }

    static bool deleteProperty(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName);
    static bool deletePropertyByIndex(JSC::JSCell*, JSC::ExecState*, unsigned);
    static void getOwnPropertyNames(JSC::JSObject*, JSC::ExecState*, JSC::PropertyNameArray&, JSC::EnumerationMode mode = JSC::ExcludeDontEnumProperties);
    static bool defineOwnProperty(JSC::JSObject*, JSC::ExecState*, JSC::PropertyName, JSC::PropertyDescriptor&, bool shouldThrow);

    // Custom attributes
    void setHref(JSC::ExecState*, JSC::JSValue);
    void setProtocol(JSC::ExecState*, JSC::JSValue);
    void setHost(JSC::ExecState*, JSC::JSValue);
    void setHostname(JSC::ExecState*, JSC::JSValue);
    void setPort(JSC::ExecState*, JSC::JSValue);
    void setPathname(JSC::ExecState*, JSC::JSValue);
    void setSearch(JSC::ExecState*, JSC::JSValue);
    void setHash(JSC::ExecState*, JSC::JSValue);

    // Custom functions
    JSC::JSValue assign(JSC::ExecState*);
    JSC::JSValue replace(JSC::ExecState*);
    JSC::JSValue reload(JSC::ExecState*);
    JSC::JSValue toStringFunction(JSC::ExecState*);
    Location* impl() const { return m_impl; }
    void releaseImpl() { m_impl->deref(); m_impl = 0; }

    void releaseImplIfNotNull() { if (m_impl) { m_impl->deref(); m_impl = 0; } }

private:
    Location* m_impl;
protected:
    JSLocation(JSC::Structure*, JSDOMGlobalObject*, PassRefPtr<Location>);
    void finishCreation(JSC::JSGlobalData&);
    static const unsigned StructureFlags = JSC::OverridesGetPropertyNames | JSC::OverridesGetOwnPropertySlot | JSC::InterceptsGetOwnPropertySlotByIndexEvenWhenLengthIsNotZero | Base::StructureFlags;
};

class JSLocationOwner : public JSC::WeakHandleOwner {
public:
    virtual bool isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown>, void* context, JSC::SlotVisitor&);
    virtual void finalize(JSC::Handle<JSC::Unknown>, void* context);
};

inline JSC::WeakHandleOwner* wrapperOwner(DOMWrapperWorld*, Location*)
{
    DEFINE_STATIC_LOCAL(JSLocationOwner, jsLocationOwner, ());
    return &jsLocationOwner;
}

inline void* wrapperContext(DOMWrapperWorld* world, Location*)
{
    return world;
}

JSC::JSValue toJS(JSC::ExecState*, JSDOMGlobalObject*, Location*);
Location* toLocation(JSC::JSValue);

class JSLocationPrototype : public JSC::JSNonFinalObject {
public:
    typedef JSC::JSNonFinalObject Base;
    static JSC::JSObject* self(JSC::ExecState*, JSC::JSGlobalObject*);
    static JSLocationPrototype* create(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::Structure* structure)
    {
        JSLocationPrototype* ptr = new (NotNull, JSC::allocateCell<JSLocationPrototype>(globalData.heap)) JSLocationPrototype(globalData, globalObject, structure);
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
    static void put(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName, JSC::JSValue, JSC::PutPropertySlot&);
    bool putDelegate(JSC::ExecState*, JSC::PropertyName, JSC::JSValue, JSC::PutPropertySlot&);
    static bool defineOwnProperty(JSC::JSObject*, JSC::ExecState*, JSC::PropertyName, JSC::PropertyDescriptor&, bool shouldThrow);

private:
    JSLocationPrototype(JSC::JSGlobalData& globalData, JSC::JSGlobalObject*, JSC::Structure* structure) : JSC::JSNonFinalObject(globalData, structure) { }
protected:
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | Base::StructureFlags;
};

// Functions

JSC::EncodedJSValue JSC_HOST_CALL jsLocationPrototypeFunctionAssign(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsLocationPrototypeFunctionReplace(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsLocationPrototypeFunctionReload(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsLocationPrototypeFunctionToString(JSC::ExecState*);
// Attributes

JSC::JSValue jsLocationHref(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSLocationHref(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsLocationProtocol(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSLocationProtocol(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsLocationHost(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSLocationHost(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsLocationHostname(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSLocationHostname(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsLocationPort(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSLocationPort(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsLocationPathname(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSLocationPathname(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsLocationSearch(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSLocationSearch(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsLocationHash(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSLocationHash(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsLocationOrigin(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsLocationAncestorOrigins(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);

} // namespace WebCore

#endif
