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

#ifndef JSStyleSheetList_h
#define JSStyleSheetList_h

#include "JSDOMBinding.h"
#include "StyleSheetList.h"
#include <runtime/JSGlobalObject.h>
#include <runtime/JSObject.h>
#include <runtime/ObjectPrototype.h>

namespace WebCore {

class JSStyleSheetList : public JSDOMWrapper {
public:
    typedef JSDOMWrapper Base;
    static JSStyleSheetList* create(JSC::Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<StyleSheetList> impl)
    {
        JSStyleSheetList* ptr = new (NotNull, JSC::allocateCell<JSStyleSheetList>(globalObject->globalData().heap)) JSStyleSheetList(structure, globalObject, impl);
        ptr->finishCreation(globalObject->globalData());
        return ptr;
    }

    static JSC::JSObject* createPrototype(JSC::ExecState*, JSC::JSGlobalObject*);
    static bool getOwnPropertySlot(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName, JSC::PropertySlot&);
    static bool getOwnPropertyDescriptor(JSC::JSObject*, JSC::ExecState*, JSC::PropertyName, JSC::PropertyDescriptor&);
    static bool getOwnPropertySlotByIndex(JSC::JSCell*, JSC::ExecState*, unsigned propertyName, JSC::PropertySlot&);
    static void destroy(JSC::JSCell*);
    ~JSStyleSheetList();
    static const JSC::ClassInfo s_info;

    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), &s_info);
    }

    static void getOwnPropertyNames(JSC::JSObject*, JSC::ExecState*, JSC::PropertyNameArray&, JSC::EnumerationMode mode = JSC::ExcludeDontEnumProperties);
    static JSC::JSValue getConstructor(JSC::ExecState*, JSC::JSGlobalObject*);
    StyleSheetList* impl() const { return m_impl; }
    void releaseImpl() { m_impl->deref(); m_impl = 0; }

    void releaseImplIfNotNull() { if (m_impl) { m_impl->deref(); m_impl = 0; } }

private:
    StyleSheetList* m_impl;
protected:
    JSStyleSheetList(JSC::Structure*, JSDOMGlobalObject*, PassRefPtr<StyleSheetList>);
    void finishCreation(JSC::JSGlobalData&);
    static const unsigned StructureFlags = JSC::OverridesGetPropertyNames | JSC::OverridesGetOwnPropertySlot | JSC::InterceptsGetOwnPropertySlotByIndexEvenWhenLengthIsNotZero | Base::StructureFlags;
    static JSC::JSValue indexGetter(JSC::ExecState*, JSC::JSValue, unsigned);
private:
    static bool canGetItemsForName(JSC::ExecState*, StyleSheetList*, JSC::PropertyName);
    static JSC::JSValue nameGetter(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
};

class JSStyleSheetListOwner : public JSC::WeakHandleOwner {
public:
    virtual bool isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown>, void* context, JSC::SlotVisitor&);
    virtual void finalize(JSC::Handle<JSC::Unknown>, void* context);
};

inline JSC::WeakHandleOwner* wrapperOwner(DOMWrapperWorld*, StyleSheetList*)
{
    DEFINE_STATIC_LOCAL(JSStyleSheetListOwner, jsStyleSheetListOwner, ());
    return &jsStyleSheetListOwner;
}

inline void* wrapperContext(DOMWrapperWorld* world, StyleSheetList*)
{
    return world;
}

JSC::JSValue toJS(JSC::ExecState*, JSDOMGlobalObject*, StyleSheetList*);
StyleSheetList* toStyleSheetList(JSC::JSValue);

class JSStyleSheetListPrototype : public JSC::JSNonFinalObject {
public:
    typedef JSC::JSNonFinalObject Base;
    static JSC::JSObject* self(JSC::ExecState*, JSC::JSGlobalObject*);
    static JSStyleSheetListPrototype* create(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::Structure* structure)
    {
        JSStyleSheetListPrototype* ptr = new (NotNull, JSC::allocateCell<JSStyleSheetListPrototype>(globalData.heap)) JSStyleSheetListPrototype(globalData, globalObject, structure);
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
    JSStyleSheetListPrototype(JSC::JSGlobalData& globalData, JSC::JSGlobalObject*, JSC::Structure* structure) : JSC::JSNonFinalObject(globalData, structure) { }
protected:
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | Base::StructureFlags;
};

class JSStyleSheetListConstructor : public DOMConstructorObject {
private:
    JSStyleSheetListConstructor(JSC::Structure*, JSDOMGlobalObject*);
    void finishCreation(JSC::ExecState*, JSDOMGlobalObject*);

public:
    typedef DOMConstructorObject Base;
    static JSStyleSheetListConstructor* create(JSC::ExecState* exec, JSC::Structure* structure, JSDOMGlobalObject* globalObject)
    {
        JSStyleSheetListConstructor* ptr = new (NotNull, JSC::allocateCell<JSStyleSheetListConstructor>(*exec->heap())) JSStyleSheetListConstructor(structure, globalObject);
        ptr->finishCreation(exec, globalObject);
        return ptr;
    }

    static bool getOwnPropertySlot(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName, JSC::PropertySlot&);
    static bool getOwnPropertyDescriptor(JSC::JSObject*, JSC::ExecState*, JSC::PropertyName, JSC::PropertyDescriptor&);
    static const JSC::ClassInfo s_info;
    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), &s_info);
    }
protected:
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | JSC::ImplementsHasInstance | DOMConstructorObject::StructureFlags;
};

// Functions

JSC::EncodedJSValue JSC_HOST_CALL jsStyleSheetListPrototypeFunctionItem(JSC::ExecState*);
// Attributes

JSC::JSValue jsStyleSheetListLength(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsStyleSheetListConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);

} // namespace WebCore

#endif
