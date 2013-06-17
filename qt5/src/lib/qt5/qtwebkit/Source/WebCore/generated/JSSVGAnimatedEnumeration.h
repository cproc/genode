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

#ifndef JSSVGAnimatedEnumeration_h
#define JSSVGAnimatedEnumeration_h

#if ENABLE(SVG)

#include "JSDOMBinding.h"
#include "SVGAnimatedEnumeration.h"
#include "SVGElement.h"
#include <runtime/JSGlobalObject.h>
#include <runtime/JSObject.h>
#include <runtime/ObjectPrototype.h>

namespace WebCore {

class JSSVGAnimatedEnumeration : public JSDOMWrapper {
public:
    typedef JSDOMWrapper Base;
    static JSSVGAnimatedEnumeration* create(JSC::Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<SVGAnimatedEnumeration> impl)
    {
        JSSVGAnimatedEnumeration* ptr = new (NotNull, JSC::allocateCell<JSSVGAnimatedEnumeration>(globalObject->globalData().heap)) JSSVGAnimatedEnumeration(structure, globalObject, impl);
        ptr->finishCreation(globalObject->globalData());
        return ptr;
    }

    static JSC::JSObject* createPrototype(JSC::ExecState*, JSC::JSGlobalObject*);
    static bool getOwnPropertySlot(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName, JSC::PropertySlot&);
    static bool getOwnPropertyDescriptor(JSC::JSObject*, JSC::ExecState*, JSC::PropertyName, JSC::PropertyDescriptor&);
    static void put(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName, JSC::JSValue, JSC::PutPropertySlot&);
    static void destroy(JSC::JSCell*);
    ~JSSVGAnimatedEnumeration();
    static const JSC::ClassInfo s_info;

    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), &s_info);
    }

    static JSC::JSValue getConstructor(JSC::ExecState*, JSC::JSGlobalObject*);
    SVGAnimatedEnumeration* impl() const { return m_impl; }
    void releaseImpl() { m_impl->deref(); m_impl = 0; }

    void releaseImplIfNotNull() { if (m_impl) { m_impl->deref(); m_impl = 0; } }

private:
    SVGAnimatedEnumeration* m_impl;
protected:
    JSSVGAnimatedEnumeration(JSC::Structure*, JSDOMGlobalObject*, PassRefPtr<SVGAnimatedEnumeration>);
    void finishCreation(JSC::JSGlobalData&);
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | JSC::InterceptsGetOwnPropertySlotByIndexEvenWhenLengthIsNotZero | Base::StructureFlags;
};

class JSSVGAnimatedEnumerationOwner : public JSC::WeakHandleOwner {
public:
    virtual bool isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown>, void* context, JSC::SlotVisitor&);
    virtual void finalize(JSC::Handle<JSC::Unknown>, void* context);
};

inline JSC::WeakHandleOwner* wrapperOwner(DOMWrapperWorld*, SVGAnimatedEnumeration*)
{
    DEFINE_STATIC_LOCAL(JSSVGAnimatedEnumerationOwner, jsSVGAnimatedEnumerationOwner, ());
    return &jsSVGAnimatedEnumerationOwner;
}

inline void* wrapperContext(DOMWrapperWorld* world, SVGAnimatedEnumeration*)
{
    return world;
}

JSC::JSValue toJS(JSC::ExecState*, JSDOMGlobalObject*, SVGAnimatedEnumeration*);
SVGAnimatedEnumeration* toSVGAnimatedEnumeration(JSC::JSValue);

class JSSVGAnimatedEnumerationPrototype : public JSC::JSNonFinalObject {
public:
    typedef JSC::JSNonFinalObject Base;
    static JSC::JSObject* self(JSC::ExecState*, JSC::JSGlobalObject*);
    static JSSVGAnimatedEnumerationPrototype* create(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::Structure* structure)
    {
        JSSVGAnimatedEnumerationPrototype* ptr = new (NotNull, JSC::allocateCell<JSSVGAnimatedEnumerationPrototype>(globalData.heap)) JSSVGAnimatedEnumerationPrototype(globalData, globalObject, structure);
        ptr->finishCreation(globalData);
        return ptr;
    }

    static const JSC::ClassInfo s_info;
    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), &s_info);
    }

private:
    JSSVGAnimatedEnumerationPrototype(JSC::JSGlobalData& globalData, JSC::JSGlobalObject*, JSC::Structure* structure) : JSC::JSNonFinalObject(globalData, structure) { }
protected:
    static const unsigned StructureFlags = Base::StructureFlags;
};

class JSSVGAnimatedEnumerationConstructor : public DOMConstructorObject {
private:
    JSSVGAnimatedEnumerationConstructor(JSC::Structure*, JSDOMGlobalObject*);
    void finishCreation(JSC::ExecState*, JSDOMGlobalObject*);

public:
    typedef DOMConstructorObject Base;
    static JSSVGAnimatedEnumerationConstructor* create(JSC::ExecState* exec, JSC::Structure* structure, JSDOMGlobalObject* globalObject)
    {
        JSSVGAnimatedEnumerationConstructor* ptr = new (NotNull, JSC::allocateCell<JSSVGAnimatedEnumerationConstructor>(*exec->heap())) JSSVGAnimatedEnumerationConstructor(structure, globalObject);
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

JSC::JSValue jsSVGAnimatedEnumerationBaseVal(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSSVGAnimatedEnumerationBaseVal(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsSVGAnimatedEnumerationAnimVal(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGAnimatedEnumerationConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);

} // namespace WebCore

#endif // ENABLE(SVG)

#endif
