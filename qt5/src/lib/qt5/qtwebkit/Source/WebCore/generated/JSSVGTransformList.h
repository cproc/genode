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

#ifndef JSSVGTransformList_h
#define JSSVGTransformList_h

#if ENABLE(SVG)

#include "JSDOMBinding.h"
#include "SVGAnimatedListPropertyTearOff.h"
#include "SVGElement.h"
#include "SVGTransformListPropertyTearOff.h"
#include <runtime/JSGlobalObject.h>
#include <runtime/JSObject.h>
#include <runtime/ObjectPrototype.h>

namespace WebCore {

class JSSVGTransformList : public JSDOMWrapper {
public:
    typedef JSDOMWrapper Base;
    static JSSVGTransformList* create(JSC::Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<SVGTransformListPropertyTearOff > impl)
    {
        JSSVGTransformList* ptr = new (NotNull, JSC::allocateCell<JSSVGTransformList>(globalObject->globalData().heap)) JSSVGTransformList(structure, globalObject, impl);
        ptr->finishCreation(globalObject->globalData());
        return ptr;
    }

    static JSC::JSObject* createPrototype(JSC::ExecState*, JSC::JSGlobalObject*);
    static bool getOwnPropertySlot(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName, JSC::PropertySlot&);
    static bool getOwnPropertyDescriptor(JSC::JSObject*, JSC::ExecState*, JSC::PropertyName, JSC::PropertyDescriptor&);
    static void destroy(JSC::JSCell*);
    ~JSSVGTransformList();
    static const JSC::ClassInfo s_info;

    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), &s_info);
    }

    static JSC::JSValue getConstructor(JSC::ExecState*, JSC::JSGlobalObject*);
    SVGTransformListPropertyTearOff * impl() const { return m_impl; }
    void releaseImpl() { m_impl->deref(); m_impl = 0; }

    void releaseImplIfNotNull() { if (m_impl) { m_impl->deref(); m_impl = 0; } }

private:
    SVGTransformListPropertyTearOff * m_impl;
protected:
    JSSVGTransformList(JSC::Structure*, JSDOMGlobalObject*, PassRefPtr<SVGTransformListPropertyTearOff >);
    void finishCreation(JSC::JSGlobalData&);
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | JSC::InterceptsGetOwnPropertySlotByIndexEvenWhenLengthIsNotZero | Base::StructureFlags;
};

class JSSVGTransformListOwner : public JSC::WeakHandleOwner {
public:
    virtual bool isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown>, void* context, JSC::SlotVisitor&);
    virtual void finalize(JSC::Handle<JSC::Unknown>, void* context);
};

inline JSC::WeakHandleOwner* wrapperOwner(DOMWrapperWorld*, SVGTransformListPropertyTearOff *)
{
    DEFINE_STATIC_LOCAL(JSSVGTransformListOwner, jsSVGTransformListOwner, ());
    return &jsSVGTransformListOwner;
}

inline void* wrapperContext(DOMWrapperWorld* world, SVGTransformListPropertyTearOff *)
{
    return world;
}

JSC::JSValue toJS(JSC::ExecState*, JSDOMGlobalObject*, SVGTransformListPropertyTearOff *);
SVGTransformListPropertyTearOff * toSVGTransformList(JSC::JSValue);

class JSSVGTransformListPrototype : public JSC::JSNonFinalObject {
public:
    typedef JSC::JSNonFinalObject Base;
    static JSC::JSObject* self(JSC::ExecState*, JSC::JSGlobalObject*);
    static JSSVGTransformListPrototype* create(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::Structure* structure)
    {
        JSSVGTransformListPrototype* ptr = new (NotNull, JSC::allocateCell<JSSVGTransformListPrototype>(globalData.heap)) JSSVGTransformListPrototype(globalData, globalObject, structure);
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
    JSSVGTransformListPrototype(JSC::JSGlobalData& globalData, JSC::JSGlobalObject*, JSC::Structure* structure) : JSC::JSNonFinalObject(globalData, structure) { }
protected:
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | Base::StructureFlags;
};

class JSSVGTransformListConstructor : public DOMConstructorObject {
private:
    JSSVGTransformListConstructor(JSC::Structure*, JSDOMGlobalObject*);
    void finishCreation(JSC::ExecState*, JSDOMGlobalObject*);

public:
    typedef DOMConstructorObject Base;
    static JSSVGTransformListConstructor* create(JSC::ExecState* exec, JSC::Structure* structure, JSDOMGlobalObject* globalObject)
    {
        JSSVGTransformListConstructor* ptr = new (NotNull, JSC::allocateCell<JSSVGTransformListConstructor>(*exec->heap())) JSSVGTransformListConstructor(structure, globalObject);
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

JSC::EncodedJSValue JSC_HOST_CALL jsSVGTransformListPrototypeFunctionClear(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsSVGTransformListPrototypeFunctionInitialize(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsSVGTransformListPrototypeFunctionGetItem(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsSVGTransformListPrototypeFunctionInsertItemBefore(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsSVGTransformListPrototypeFunctionReplaceItem(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsSVGTransformListPrototypeFunctionRemoveItem(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsSVGTransformListPrototypeFunctionAppendItem(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsSVGTransformListPrototypeFunctionCreateSVGTransformFromMatrix(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsSVGTransformListPrototypeFunctionConsolidate(JSC::ExecState*);
// Attributes

JSC::JSValue jsSVGTransformListNumberOfItems(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGTransformListConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);

} // namespace WebCore

#endif // ENABLE(SVG)

#endif
