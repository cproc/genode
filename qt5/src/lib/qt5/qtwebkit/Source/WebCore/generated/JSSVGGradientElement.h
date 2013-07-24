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

#ifndef JSSVGGradientElement_h
#define JSSVGGradientElement_h

#if ENABLE(SVG)

#include "JSDOMBinding.h"
#include "JSSVGElement.h"
#include "SVGElement.h"
#include "SVGGradientElement.h"
#include <runtime/JSObject.h>

namespace WebCore {

class JSSVGGradientElement : public JSSVGElement {
public:
    typedef JSSVGElement Base;
    static JSSVGGradientElement* create(JSC::Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<SVGGradientElement> impl)
    {
        JSSVGGradientElement* ptr = new (NotNull, JSC::allocateCell<JSSVGGradientElement>(globalObject->globalData().heap)) JSSVGGradientElement(structure, globalObject, impl);
        ptr->finishCreation(globalObject->globalData());
        return ptr;
    }

    static JSC::JSObject* createPrototype(JSC::ExecState*, JSC::JSGlobalObject*);
    static bool getOwnPropertySlot(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName, JSC::PropertySlot&);
    static bool getOwnPropertyDescriptor(JSC::JSObject*, JSC::ExecState*, JSC::PropertyName, JSC::PropertyDescriptor&);
    static const JSC::ClassInfo s_info;

    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), &s_info);
    }

    static JSC::JSValue getConstructor(JSC::ExecState*, JSC::JSGlobalObject*);
protected:
    JSSVGGradientElement(JSC::Structure*, JSDOMGlobalObject*, PassRefPtr<SVGGradientElement>);
    void finishCreation(JSC::JSGlobalData&);
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | JSC::InterceptsGetOwnPropertySlotByIndexEvenWhenLengthIsNotZero | Base::StructureFlags;
};


class JSSVGGradientElementPrototype : public JSC::JSNonFinalObject {
public:
    typedef JSC::JSNonFinalObject Base;
    static JSC::JSObject* self(JSC::ExecState*, JSC::JSGlobalObject*);
    static JSSVGGradientElementPrototype* create(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::Structure* structure)
    {
        JSSVGGradientElementPrototype* ptr = new (NotNull, JSC::allocateCell<JSSVGGradientElementPrototype>(globalData.heap)) JSSVGGradientElementPrototype(globalData, globalObject, structure);
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
    JSSVGGradientElementPrototype(JSC::JSGlobalData& globalData, JSC::JSGlobalObject*, JSC::Structure* structure) : JSC::JSNonFinalObject(globalData, structure) { }
protected:
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | Base::StructureFlags;
};

class JSSVGGradientElementConstructor : public DOMConstructorObject {
private:
    JSSVGGradientElementConstructor(JSC::Structure*, JSDOMGlobalObject*);
    void finishCreation(JSC::ExecState*, JSDOMGlobalObject*);

public:
    typedef DOMConstructorObject Base;
    static JSSVGGradientElementConstructor* create(JSC::ExecState* exec, JSC::Structure* structure, JSDOMGlobalObject* globalObject)
    {
        JSSVGGradientElementConstructor* ptr = new (NotNull, JSC::allocateCell<JSSVGGradientElementConstructor>(*exec->heap())) JSSVGGradientElementConstructor(structure, globalObject);
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

JSC::EncodedJSValue JSC_HOST_CALL jsSVGGradientElementPrototypeFunctionGetPresentationAttribute(JSC::ExecState*);
// Attributes

JSC::JSValue jsSVGGradientElementGradientUnits(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGGradientElementGradientTransform(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGGradientElementSpreadMethod(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGGradientElementHref(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGGradientElementExternalResourcesRequired(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGGradientElementClassName(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGGradientElementStyle(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGGradientElementConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
// Constants

JSC::JSValue jsSVGGradientElementSVG_SPREADMETHOD_UNKNOWN(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGGradientElementSVG_SPREADMETHOD_PAD(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGGradientElementSVG_SPREADMETHOD_REFLECT(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGGradientElementSVG_SPREADMETHOD_REPEAT(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);

} // namespace WebCore

#endif // ENABLE(SVG)

#endif
