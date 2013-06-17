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

#ifndef JSXPathResult_h
#define JSXPathResult_h

#include "JSDOMBinding.h"
#include "XPathResult.h"
#include <runtime/JSGlobalObject.h>
#include <runtime/JSObject.h>
#include <runtime/ObjectPrototype.h>

namespace WebCore {

class JSXPathResult : public JSDOMWrapper {
public:
    typedef JSDOMWrapper Base;
    static JSXPathResult* create(JSC::Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<XPathResult> impl)
    {
        JSXPathResult* ptr = new (NotNull, JSC::allocateCell<JSXPathResult>(globalObject->globalData().heap)) JSXPathResult(structure, globalObject, impl);
        ptr->finishCreation(globalObject->globalData());
        return ptr;
    }

    static JSC::JSObject* createPrototype(JSC::ExecState*, JSC::JSGlobalObject*);
    static bool getOwnPropertySlot(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName, JSC::PropertySlot&);
    static bool getOwnPropertyDescriptor(JSC::JSObject*, JSC::ExecState*, JSC::PropertyName, JSC::PropertyDescriptor&);
    static void destroy(JSC::JSCell*);
    ~JSXPathResult();
    static const JSC::ClassInfo s_info;

    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), &s_info);
    }

    static JSC::JSValue getConstructor(JSC::ExecState*, JSC::JSGlobalObject*);
    static void visitChildren(JSCell*, JSC::SlotVisitor&);

    XPathResult* impl() const { return m_impl; }
    void releaseImpl() { m_impl->deref(); m_impl = 0; }

    void releaseImplIfNotNull() { if (m_impl) { m_impl->deref(); m_impl = 0; } }

private:
    XPathResult* m_impl;
protected:
    JSXPathResult(JSC::Structure*, JSDOMGlobalObject*, PassRefPtr<XPathResult>);
    void finishCreation(JSC::JSGlobalData&);
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | JSC::InterceptsGetOwnPropertySlotByIndexEvenWhenLengthIsNotZero | JSC::OverridesVisitChildren | Base::StructureFlags;
};

class JSXPathResultOwner : public JSC::WeakHandleOwner {
public:
    virtual bool isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown>, void* context, JSC::SlotVisitor&);
    virtual void finalize(JSC::Handle<JSC::Unknown>, void* context);
};

inline JSC::WeakHandleOwner* wrapperOwner(DOMWrapperWorld*, XPathResult*)
{
    DEFINE_STATIC_LOCAL(JSXPathResultOwner, jsXPathResultOwner, ());
    return &jsXPathResultOwner;
}

inline void* wrapperContext(DOMWrapperWorld* world, XPathResult*)
{
    return world;
}

JSC::JSValue toJS(JSC::ExecState*, JSDOMGlobalObject*, XPathResult*);
XPathResult* toXPathResult(JSC::JSValue);

class JSXPathResultPrototype : public JSC::JSNonFinalObject {
public:
    typedef JSC::JSNonFinalObject Base;
    static JSC::JSObject* self(JSC::ExecState*, JSC::JSGlobalObject*);
    static JSXPathResultPrototype* create(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::Structure* structure)
    {
        JSXPathResultPrototype* ptr = new (NotNull, JSC::allocateCell<JSXPathResultPrototype>(globalData.heap)) JSXPathResultPrototype(globalData, globalObject, structure);
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
    JSXPathResultPrototype(JSC::JSGlobalData& globalData, JSC::JSGlobalObject*, JSC::Structure* structure) : JSC::JSNonFinalObject(globalData, structure) { }
protected:
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | JSC::OverridesVisitChildren | Base::StructureFlags;
};

class JSXPathResultConstructor : public DOMConstructorObject {
private:
    JSXPathResultConstructor(JSC::Structure*, JSDOMGlobalObject*);
    void finishCreation(JSC::ExecState*, JSDOMGlobalObject*);

public:
    typedef DOMConstructorObject Base;
    static JSXPathResultConstructor* create(JSC::ExecState* exec, JSC::Structure* structure, JSDOMGlobalObject* globalObject)
    {
        JSXPathResultConstructor* ptr = new (NotNull, JSC::allocateCell<JSXPathResultConstructor>(*exec->heap())) JSXPathResultConstructor(structure, globalObject);
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

JSC::EncodedJSValue JSC_HOST_CALL jsXPathResultPrototypeFunctionIterateNext(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsXPathResultPrototypeFunctionSnapshotItem(JSC::ExecState*);
// Attributes

JSC::JSValue jsXPathResultResultType(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsXPathResultNumberValue(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsXPathResultStringValue(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsXPathResultBooleanValue(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsXPathResultSingleNodeValue(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsXPathResultInvalidIteratorState(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsXPathResultSnapshotLength(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsXPathResultConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
// Constants

JSC::JSValue jsXPathResultANY_TYPE(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsXPathResultNUMBER_TYPE(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsXPathResultSTRING_TYPE(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsXPathResultBOOLEAN_TYPE(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsXPathResultUNORDERED_NODE_ITERATOR_TYPE(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsXPathResultORDERED_NODE_ITERATOR_TYPE(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsXPathResultUNORDERED_NODE_SNAPSHOT_TYPE(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsXPathResultORDERED_NODE_SNAPSHOT_TYPE(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsXPathResultANY_UNORDERED_NODE_TYPE(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsXPathResultFIRST_ORDERED_NODE_TYPE(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);

} // namespace WebCore

#endif
