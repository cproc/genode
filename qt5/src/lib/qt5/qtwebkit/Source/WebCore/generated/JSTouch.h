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

#ifndef JSTouch_h
#define JSTouch_h

#if ENABLE(TOUCH_EVENTS)

#include "JSDOMBinding.h"
#include "Touch.h"
#include <runtime/JSGlobalObject.h>
#include <runtime/JSObject.h>
#include <runtime/ObjectPrototype.h>

namespace WebCore {

class JSTouch : public JSDOMWrapper {
public:
    typedef JSDOMWrapper Base;
    static JSTouch* create(JSC::Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<Touch> impl)
    {
        JSTouch* ptr = new (NotNull, JSC::allocateCell<JSTouch>(globalObject->globalData().heap)) JSTouch(structure, globalObject, impl);
        ptr->finishCreation(globalObject->globalData());
        return ptr;
    }

    static JSC::JSObject* createPrototype(JSC::ExecState*, JSC::JSGlobalObject*);
    static bool getOwnPropertySlot(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName, JSC::PropertySlot&);
    static bool getOwnPropertyDescriptor(JSC::JSObject*, JSC::ExecState*, JSC::PropertyName, JSC::PropertyDescriptor&);
    static void destroy(JSC::JSCell*);
    ~JSTouch();
    static const JSC::ClassInfo s_info;

    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), &s_info);
    }

    static JSC::JSValue getConstructor(JSC::ExecState*, JSC::JSGlobalObject*);
    Touch* impl() const { return m_impl; }
    void releaseImpl() { m_impl->deref(); m_impl = 0; }

    void releaseImplIfNotNull() { if (m_impl) { m_impl->deref(); m_impl = 0; } }

private:
    Touch* m_impl;
protected:
    JSTouch(JSC::Structure*, JSDOMGlobalObject*, PassRefPtr<Touch>);
    void finishCreation(JSC::JSGlobalData&);
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | JSC::InterceptsGetOwnPropertySlotByIndexEvenWhenLengthIsNotZero | Base::StructureFlags;
};

class JSTouchOwner : public JSC::WeakHandleOwner {
public:
    virtual bool isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown>, void* context, JSC::SlotVisitor&);
    virtual void finalize(JSC::Handle<JSC::Unknown>, void* context);
};

inline JSC::WeakHandleOwner* wrapperOwner(DOMWrapperWorld*, Touch*)
{
    DEFINE_STATIC_LOCAL(JSTouchOwner, jsTouchOwner, ());
    return &jsTouchOwner;
}

inline void* wrapperContext(DOMWrapperWorld* world, Touch*)
{
    return world;
}

JSC::JSValue toJS(JSC::ExecState*, JSDOMGlobalObject*, Touch*);
Touch* toTouch(JSC::JSValue);
JSC::JSValue toJSNewlyCreated(JSC::ExecState*, JSDOMGlobalObject*, Touch*);

class JSTouchPrototype : public JSC::JSNonFinalObject {
public:
    typedef JSC::JSNonFinalObject Base;
    static JSC::JSObject* self(JSC::ExecState*, JSC::JSGlobalObject*);
    static JSTouchPrototype* create(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::Structure* structure)
    {
        JSTouchPrototype* ptr = new (NotNull, JSC::allocateCell<JSTouchPrototype>(globalData.heap)) JSTouchPrototype(globalData, globalObject, structure);
        ptr->finishCreation(globalData);
        return ptr;
    }

    static const JSC::ClassInfo s_info;
    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), &s_info);
    }

private:
    JSTouchPrototype(JSC::JSGlobalData& globalData, JSC::JSGlobalObject*, JSC::Structure* structure) : JSC::JSNonFinalObject(globalData, structure) { }
protected:
    static const unsigned StructureFlags = Base::StructureFlags;
};

class JSTouchConstructor : public DOMConstructorObject {
private:
    JSTouchConstructor(JSC::Structure*, JSDOMGlobalObject*);
    void finishCreation(JSC::ExecState*, JSDOMGlobalObject*);

public:
    typedef DOMConstructorObject Base;
    static JSTouchConstructor* create(JSC::ExecState* exec, JSC::Structure* structure, JSDOMGlobalObject* globalObject)
    {
        JSTouchConstructor* ptr = new (NotNull, JSC::allocateCell<JSTouchConstructor>(*exec->heap())) JSTouchConstructor(structure, globalObject);
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

// Attributes

JSC::JSValue jsTouchClientX(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsTouchClientY(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsTouchScreenX(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsTouchScreenY(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsTouchPageX(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsTouchPageY(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsTouchTarget(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsTouchIdentifier(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsTouchWebkitRadiusX(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsTouchWebkitRadiusY(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsTouchWebkitRotationAngle(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsTouchWebkitForce(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsTouchConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);

} // namespace WebCore

#endif // ENABLE(TOUCH_EVENTS)

#endif
