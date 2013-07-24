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

#ifndef JSSVGPreserveAspectRatio_h
#define JSSVGPreserveAspectRatio_h

#if ENABLE(SVG)

#include "JSDOMBinding.h"
#include "SVGAnimatedPropertyTearOff.h"
#include "SVGElement.h"
#include "SVGPreserveAspectRatio.h"
#include <runtime/JSGlobalObject.h>
#include <runtime/JSObject.h>
#include <runtime/ObjectPrototype.h>

namespace WebCore {

class JSSVGPreserveAspectRatio : public JSDOMWrapper {
public:
    typedef JSDOMWrapper Base;
    static JSSVGPreserveAspectRatio* create(JSC::Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<SVGPropertyTearOff<SVGPreserveAspectRatio> > impl)
    {
        JSSVGPreserveAspectRatio* ptr = new (NotNull, JSC::allocateCell<JSSVGPreserveAspectRatio>(globalObject->globalData().heap)) JSSVGPreserveAspectRatio(structure, globalObject, impl);
        ptr->finishCreation(globalObject->globalData());
        return ptr;
    }

    static JSC::JSObject* createPrototype(JSC::ExecState*, JSC::JSGlobalObject*);
    static bool getOwnPropertySlot(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName, JSC::PropertySlot&);
    static bool getOwnPropertyDescriptor(JSC::JSObject*, JSC::ExecState*, JSC::PropertyName, JSC::PropertyDescriptor&);
    static void put(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName, JSC::JSValue, JSC::PutPropertySlot&);
    static void destroy(JSC::JSCell*);
    ~JSSVGPreserveAspectRatio();
    static const JSC::ClassInfo s_info;

    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), &s_info);
    }

    static JSC::JSValue getConstructor(JSC::ExecState*, JSC::JSGlobalObject*);
    SVGPropertyTearOff<SVGPreserveAspectRatio> * impl() const { return m_impl; }
    void releaseImpl() { m_impl->deref(); m_impl = 0; }

    void releaseImplIfNotNull() { if (m_impl) { m_impl->deref(); m_impl = 0; } }

private:
    SVGPropertyTearOff<SVGPreserveAspectRatio> * m_impl;
protected:
    JSSVGPreserveAspectRatio(JSC::Structure*, JSDOMGlobalObject*, PassRefPtr<SVGPropertyTearOff<SVGPreserveAspectRatio> >);
    void finishCreation(JSC::JSGlobalData&);
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | JSC::InterceptsGetOwnPropertySlotByIndexEvenWhenLengthIsNotZero | Base::StructureFlags;
};

class JSSVGPreserveAspectRatioOwner : public JSC::WeakHandleOwner {
public:
    virtual bool isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown>, void* context, JSC::SlotVisitor&);
    virtual void finalize(JSC::Handle<JSC::Unknown>, void* context);
};

inline JSC::WeakHandleOwner* wrapperOwner(DOMWrapperWorld*, SVGPropertyTearOff<SVGPreserveAspectRatio> *)
{
    DEFINE_STATIC_LOCAL(JSSVGPreserveAspectRatioOwner, jsSVGPreserveAspectRatioOwner, ());
    return &jsSVGPreserveAspectRatioOwner;
}

inline void* wrapperContext(DOMWrapperWorld* world, SVGPropertyTearOff<SVGPreserveAspectRatio> *)
{
    return world;
}

JSC::JSValue toJS(JSC::ExecState*, JSDOMGlobalObject*, SVGPropertyTearOff<SVGPreserveAspectRatio> *);
SVGPropertyTearOff<SVGPreserveAspectRatio> * toSVGPreserveAspectRatio(JSC::JSValue);

class JSSVGPreserveAspectRatioPrototype : public JSC::JSNonFinalObject {
public:
    typedef JSC::JSNonFinalObject Base;
    static JSC::JSObject* self(JSC::ExecState*, JSC::JSGlobalObject*);
    static JSSVGPreserveAspectRatioPrototype* create(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::Structure* structure)
    {
        JSSVGPreserveAspectRatioPrototype* ptr = new (NotNull, JSC::allocateCell<JSSVGPreserveAspectRatioPrototype>(globalData.heap)) JSSVGPreserveAspectRatioPrototype(globalData, globalObject, structure);
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
    JSSVGPreserveAspectRatioPrototype(JSC::JSGlobalData& globalData, JSC::JSGlobalObject*, JSC::Structure* structure) : JSC::JSNonFinalObject(globalData, structure) { }
protected:
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | Base::StructureFlags;
};

class JSSVGPreserveAspectRatioConstructor : public DOMConstructorObject {
private:
    JSSVGPreserveAspectRatioConstructor(JSC::Structure*, JSDOMGlobalObject*);
    void finishCreation(JSC::ExecState*, JSDOMGlobalObject*);

public:
    typedef DOMConstructorObject Base;
    static JSSVGPreserveAspectRatioConstructor* create(JSC::ExecState* exec, JSC::Structure* structure, JSDOMGlobalObject* globalObject)
    {
        JSSVGPreserveAspectRatioConstructor* ptr = new (NotNull, JSC::allocateCell<JSSVGPreserveAspectRatioConstructor>(*exec->heap())) JSSVGPreserveAspectRatioConstructor(structure, globalObject);
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

JSC::JSValue jsSVGPreserveAspectRatioAlign(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSSVGPreserveAspectRatioAlign(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsSVGPreserveAspectRatioMeetOrSlice(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSSVGPreserveAspectRatioMeetOrSlice(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsSVGPreserveAspectRatioConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
// Constants

JSC::JSValue jsSVGPreserveAspectRatioSVG_PRESERVEASPECTRATIO_UNKNOWN(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGPreserveAspectRatioSVG_PRESERVEASPECTRATIO_NONE(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGPreserveAspectRatioSVG_PRESERVEASPECTRATIO_XMINYMIN(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGPreserveAspectRatioSVG_PRESERVEASPECTRATIO_XMIDYMIN(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGPreserveAspectRatioSVG_PRESERVEASPECTRATIO_XMAXYMIN(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGPreserveAspectRatioSVG_PRESERVEASPECTRATIO_XMINYMID(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGPreserveAspectRatioSVG_PRESERVEASPECTRATIO_XMIDYMID(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGPreserveAspectRatioSVG_PRESERVEASPECTRATIO_XMAXYMID(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGPreserveAspectRatioSVG_PRESERVEASPECTRATIO_XMINYMAX(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGPreserveAspectRatioSVG_PRESERVEASPECTRATIO_XMIDYMAX(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGPreserveAspectRatioSVG_PRESERVEASPECTRATIO_XMAXYMAX(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGPreserveAspectRatioSVG_MEETORSLICE_UNKNOWN(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGPreserveAspectRatioSVG_MEETORSLICE_MEET(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGPreserveAspectRatioSVG_MEETORSLICE_SLICE(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);

} // namespace WebCore

#endif // ENABLE(SVG)

#endif
