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

#ifndef JSSVGFilterElement_h
#define JSSVGFilterElement_h

#if ENABLE(FILTERS) && ENABLE(SVG)

#include "JSDOMBinding.h"
#include "JSSVGElement.h"
#include "SVGElement.h"
#include "SVGFilterElement.h"
#include <runtime/JSObject.h>

namespace WebCore {

class JSSVGFilterElement : public JSSVGElement {
public:
    typedef JSSVGElement Base;
    static JSSVGFilterElement* create(JSC::Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<SVGFilterElement> impl)
    {
        JSSVGFilterElement* ptr = new (NotNull, JSC::allocateCell<JSSVGFilterElement>(globalObject->globalData().heap)) JSSVGFilterElement(structure, globalObject, impl);
        ptr->finishCreation(globalObject->globalData());
        return ptr;
    }

    static JSC::JSObject* createPrototype(JSC::ExecState*, JSC::JSGlobalObject*);
    static bool getOwnPropertySlot(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName, JSC::PropertySlot&);
    static bool getOwnPropertyDescriptor(JSC::JSObject*, JSC::ExecState*, JSC::PropertyName, JSC::PropertyDescriptor&);
    static void put(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName, JSC::JSValue, JSC::PutPropertySlot&);
    static const JSC::ClassInfo s_info;

    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), &s_info);
    }

    static JSC::JSValue getConstructor(JSC::ExecState*, JSC::JSGlobalObject*);
protected:
    JSSVGFilterElement(JSC::Structure*, JSDOMGlobalObject*, PassRefPtr<SVGFilterElement>);
    void finishCreation(JSC::JSGlobalData&);
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | JSC::InterceptsGetOwnPropertySlotByIndexEvenWhenLengthIsNotZero | Base::StructureFlags;
};


class JSSVGFilterElementPrototype : public JSC::JSNonFinalObject {
public:
    typedef JSC::JSNonFinalObject Base;
    static JSC::JSObject* self(JSC::ExecState*, JSC::JSGlobalObject*);
    static JSSVGFilterElementPrototype* create(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::Structure* structure)
    {
        JSSVGFilterElementPrototype* ptr = new (NotNull, JSC::allocateCell<JSSVGFilterElementPrototype>(globalData.heap)) JSSVGFilterElementPrototype(globalData, globalObject, structure);
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
    JSSVGFilterElementPrototype(JSC::JSGlobalData& globalData, JSC::JSGlobalObject*, JSC::Structure* structure) : JSC::JSNonFinalObject(globalData, structure) { }
protected:
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | Base::StructureFlags;
};

class JSSVGFilterElementConstructor : public DOMConstructorObject {
private:
    JSSVGFilterElementConstructor(JSC::Structure*, JSDOMGlobalObject*);
    void finishCreation(JSC::ExecState*, JSDOMGlobalObject*);

public:
    typedef DOMConstructorObject Base;
    static JSSVGFilterElementConstructor* create(JSC::ExecState* exec, JSC::Structure* structure, JSDOMGlobalObject* globalObject)
    {
        JSSVGFilterElementConstructor* ptr = new (NotNull, JSC::allocateCell<JSSVGFilterElementConstructor>(*exec->heap())) JSSVGFilterElementConstructor(structure, globalObject);
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

JSC::EncodedJSValue JSC_HOST_CALL jsSVGFilterElementPrototypeFunctionSetFilterRes(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsSVGFilterElementPrototypeFunctionGetPresentationAttribute(JSC::ExecState*);
// Attributes

JSC::JSValue jsSVGFilterElementFilterUnits(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGFilterElementPrimitiveUnits(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGFilterElementX(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGFilterElementY(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGFilterElementWidth(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGFilterElementHeight(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGFilterElementFilterResX(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGFilterElementFilterResY(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGFilterElementHref(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGFilterElementXmllang(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSSVGFilterElementXmllang(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsSVGFilterElementXmlspace(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSSVGFilterElementXmlspace(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsSVGFilterElementExternalResourcesRequired(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGFilterElementClassName(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGFilterElementStyle(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGFilterElementConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);

} // namespace WebCore

#endif // ENABLE(FILTERS) && ENABLE(SVG)

#endif
