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

#ifndef JSSVGGlyphRefElement_h
#define JSSVGGlyphRefElement_h

#if ENABLE(SVG) && ENABLE(SVG_FONTS)

#include "JSDOMBinding.h"
#include "JSSVGElement.h"
#include "SVGElement.h"
#include "SVGGlyphRefElement.h"
#include <runtime/JSObject.h>

namespace WebCore {

class JSSVGGlyphRefElement : public JSSVGElement {
public:
    typedef JSSVGElement Base;
    static JSSVGGlyphRefElement* create(JSC::Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<SVGGlyphRefElement> impl)
    {
        JSSVGGlyphRefElement* ptr = new (NotNull, JSC::allocateCell<JSSVGGlyphRefElement>(globalObject->globalData().heap)) JSSVGGlyphRefElement(structure, globalObject, impl);
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
    JSSVGGlyphRefElement(JSC::Structure*, JSDOMGlobalObject*, PassRefPtr<SVGGlyphRefElement>);
    void finishCreation(JSC::JSGlobalData&);
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | JSC::InterceptsGetOwnPropertySlotByIndexEvenWhenLengthIsNotZero | Base::StructureFlags;
};


class JSSVGGlyphRefElementPrototype : public JSC::JSNonFinalObject {
public:
    typedef JSC::JSNonFinalObject Base;
    static JSC::JSObject* self(JSC::ExecState*, JSC::JSGlobalObject*);
    static JSSVGGlyphRefElementPrototype* create(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::Structure* structure)
    {
        JSSVGGlyphRefElementPrototype* ptr = new (NotNull, JSC::allocateCell<JSSVGGlyphRefElementPrototype>(globalData.heap)) JSSVGGlyphRefElementPrototype(globalData, globalObject, structure);
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
    JSSVGGlyphRefElementPrototype(JSC::JSGlobalData& globalData, JSC::JSGlobalObject*, JSC::Structure* structure) : JSC::JSNonFinalObject(globalData, structure) { }
protected:
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | Base::StructureFlags;
};

class JSSVGGlyphRefElementConstructor : public DOMConstructorObject {
private:
    JSSVGGlyphRefElementConstructor(JSC::Structure*, JSDOMGlobalObject*);
    void finishCreation(JSC::ExecState*, JSDOMGlobalObject*);

public:
    typedef DOMConstructorObject Base;
    static JSSVGGlyphRefElementConstructor* create(JSC::ExecState* exec, JSC::Structure* structure, JSDOMGlobalObject* globalObject)
    {
        JSSVGGlyphRefElementConstructor* ptr = new (NotNull, JSC::allocateCell<JSSVGGlyphRefElementConstructor>(*exec->heap())) JSSVGGlyphRefElementConstructor(structure, globalObject);
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

JSC::EncodedJSValue JSC_HOST_CALL jsSVGGlyphRefElementPrototypeFunctionGetPresentationAttribute(JSC::ExecState*);
// Attributes

JSC::JSValue jsSVGGlyphRefElementGlyphRef(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSSVGGlyphRefElementGlyphRef(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsSVGGlyphRefElementFormat(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSSVGGlyphRefElementFormat(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsSVGGlyphRefElementX(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSSVGGlyphRefElementX(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsSVGGlyphRefElementY(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSSVGGlyphRefElementY(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsSVGGlyphRefElementDx(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSSVGGlyphRefElementDx(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsSVGGlyphRefElementDy(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSSVGGlyphRefElementDy(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsSVGGlyphRefElementHref(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGGlyphRefElementClassName(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGGlyphRefElementStyle(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsSVGGlyphRefElementConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);

} // namespace WebCore

#endif // ENABLE(SVG) && ENABLE(SVG_FONTS)

#endif
