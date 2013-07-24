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

#include "config.h"

#if ENABLE(SVG) && ENABLE(SVG_FONTS)

#include "JSSVGGlyphRefElement.h"

#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "ExceptionCode.h"
#include "JSCSSStyleDeclaration.h"
#include "JSCSSValue.h"
#include "JSDOMBinding.h"
#include "JSSVGAnimatedString.h"
#include "KURL.h"
#include "SVGGlyphRefElement.h"
#include "SVGNames.h"
#include "StylePropertySet.h"
#include <runtime/Error.h>
#include <runtime/JSString.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSSVGGlyphRefElementTableValues[] =
{
    { "glyphRef", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGGlyphRefElementGlyphRef), (intptr_t)setJSSVGGlyphRefElementGlyphRef, NoIntrinsic },
    { "format", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGGlyphRefElementFormat), (intptr_t)setJSSVGGlyphRefElementFormat, NoIntrinsic },
    { "x", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGGlyphRefElementX), (intptr_t)setJSSVGGlyphRefElementX, NoIntrinsic },
    { "y", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGGlyphRefElementY), (intptr_t)setJSSVGGlyphRefElementY, NoIntrinsic },
    { "dx", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGGlyphRefElementDx), (intptr_t)setJSSVGGlyphRefElementDx, NoIntrinsic },
    { "dy", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGGlyphRefElementDy), (intptr_t)setJSSVGGlyphRefElementDy, NoIntrinsic },
    { "href", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGGlyphRefElementHref), (intptr_t)0, NoIntrinsic },
    { "className", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGGlyphRefElementClassName), (intptr_t)0, NoIntrinsic },
    { "style", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGGlyphRefElementStyle), (intptr_t)0, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGGlyphRefElementConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGGlyphRefElementTable = { 33, 31, JSSVGGlyphRefElementTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSSVGGlyphRefElementConstructorTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGGlyphRefElementConstructorTable = { 1, 0, JSSVGGlyphRefElementConstructorTableValues, 0 };
const ClassInfo JSSVGGlyphRefElementConstructor::s_info = { "SVGGlyphRefElementConstructor", &Base::s_info, &JSSVGGlyphRefElementConstructorTable, 0, CREATE_METHOD_TABLE(JSSVGGlyphRefElementConstructor) };

JSSVGGlyphRefElementConstructor::JSSVGGlyphRefElementConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSSVGGlyphRefElementConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSSVGGlyphRefElementPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSSVGGlyphRefElementConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSSVGGlyphRefElementConstructor, JSDOMWrapper>(exec, &JSSVGGlyphRefElementConstructorTable, jsCast<JSSVGGlyphRefElementConstructor*>(cell), propertyName, slot);
}

bool JSSVGGlyphRefElementConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSSVGGlyphRefElementConstructor, JSDOMWrapper>(exec, &JSSVGGlyphRefElementConstructorTable, jsCast<JSSVGGlyphRefElementConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSSVGGlyphRefElementPrototypeTableValues[] =
{
    { "getPresentationAttribute", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsSVGGlyphRefElementPrototypeFunctionGetPresentationAttribute), (intptr_t)1, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGGlyphRefElementPrototypeTable = { 2, 1, JSSVGGlyphRefElementPrototypeTableValues, 0 };
const ClassInfo JSSVGGlyphRefElementPrototype::s_info = { "SVGGlyphRefElementPrototype", &Base::s_info, &JSSVGGlyphRefElementPrototypeTable, 0, CREATE_METHOD_TABLE(JSSVGGlyphRefElementPrototype) };

JSObject* JSSVGGlyphRefElementPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSSVGGlyphRefElement>(exec, globalObject);
}

bool JSSVGGlyphRefElementPrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSSVGGlyphRefElementPrototype* thisObject = jsCast<JSSVGGlyphRefElementPrototype*>(cell);
    return getStaticFunctionSlot<JSObject>(exec, &JSSVGGlyphRefElementPrototypeTable, thisObject, propertyName, slot);
}

bool JSSVGGlyphRefElementPrototype::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSSVGGlyphRefElementPrototype* thisObject = jsCast<JSSVGGlyphRefElementPrototype*>(object);
    return getStaticFunctionDescriptor<JSObject>(exec, &JSSVGGlyphRefElementPrototypeTable, thisObject, propertyName, descriptor);
}

const ClassInfo JSSVGGlyphRefElement::s_info = { "SVGGlyphRefElement", &Base::s_info, &JSSVGGlyphRefElementTable, 0 , CREATE_METHOD_TABLE(JSSVGGlyphRefElement) };

JSSVGGlyphRefElement::JSSVGGlyphRefElement(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<SVGGlyphRefElement> impl)
    : JSSVGElement(structure, globalObject, impl)
{
}

void JSSVGGlyphRefElement::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSSVGGlyphRefElement::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSSVGGlyphRefElementPrototype::create(exec->globalData(), globalObject, JSSVGGlyphRefElementPrototype::createStructure(exec->globalData(), globalObject, JSSVGElementPrototype::self(exec, globalObject)));
}

bool JSSVGGlyphRefElement::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSSVGGlyphRefElement* thisObject = jsCast<JSSVGGlyphRefElement*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSSVGGlyphRefElement, Base>(exec, &JSSVGGlyphRefElementTable, thisObject, propertyName, slot);
}

bool JSSVGGlyphRefElement::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSSVGGlyphRefElement* thisObject = jsCast<JSSVGGlyphRefElement*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSSVGGlyphRefElement, Base>(exec, &JSSVGGlyphRefElementTable, thisObject, propertyName, descriptor);
}

JSValue jsSVGGlyphRefElementGlyphRef(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGGlyphRefElement* castedThis = jsCast<JSSVGGlyphRefElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGGlyphRefElement* impl = static_cast<SVGGlyphRefElement*>(castedThis->impl());
    JSValue result = jsStringWithCache(exec, impl->glyphRef());
    return result;
}


JSValue jsSVGGlyphRefElementFormat(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGGlyphRefElement* castedThis = jsCast<JSSVGGlyphRefElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGGlyphRefElement* impl = static_cast<SVGGlyphRefElement*>(castedThis->impl());
    JSValue result = jsStringWithCache(exec, impl->getAttribute(WebCore::SVGNames::formatAttr));
    return result;
}


JSValue jsSVGGlyphRefElementX(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGGlyphRefElement* castedThis = jsCast<JSSVGGlyphRefElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGGlyphRefElement* impl = static_cast<SVGGlyphRefElement*>(castedThis->impl());
    JSValue result = jsNumber(impl->x());
    return result;
}


JSValue jsSVGGlyphRefElementY(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGGlyphRefElement* castedThis = jsCast<JSSVGGlyphRefElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGGlyphRefElement* impl = static_cast<SVGGlyphRefElement*>(castedThis->impl());
    JSValue result = jsNumber(impl->y());
    return result;
}


JSValue jsSVGGlyphRefElementDx(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGGlyphRefElement* castedThis = jsCast<JSSVGGlyphRefElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGGlyphRefElement* impl = static_cast<SVGGlyphRefElement*>(castedThis->impl());
    JSValue result = jsNumber(impl->dx());
    return result;
}


JSValue jsSVGGlyphRefElementDy(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGGlyphRefElement* castedThis = jsCast<JSSVGGlyphRefElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGGlyphRefElement* impl = static_cast<SVGGlyphRefElement*>(castedThis->impl());
    JSValue result = jsNumber(impl->dy());
    return result;
}


JSValue jsSVGGlyphRefElementHref(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGGlyphRefElement* castedThis = jsCast<JSSVGGlyphRefElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGGlyphRefElement* impl = static_cast<SVGGlyphRefElement*>(castedThis->impl());
    RefPtr<SVGAnimatedString> obj = impl->hrefAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGGlyphRefElementClassName(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGGlyphRefElement* castedThis = jsCast<JSSVGGlyphRefElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGGlyphRefElement* impl = static_cast<SVGGlyphRefElement*>(castedThis->impl());
    RefPtr<SVGAnimatedString> obj = impl->classNameAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGGlyphRefElementStyle(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGGlyphRefElement* castedThis = jsCast<JSSVGGlyphRefElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGGlyphRefElement* impl = static_cast<SVGGlyphRefElement*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->style()));
    return result;
}


JSValue jsSVGGlyphRefElementConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGGlyphRefElement* domObject = jsCast<JSSVGGlyphRefElement*>(asObject(slotBase));
    return JSSVGGlyphRefElement::getConstructor(exec, domObject->globalObject());
}

void JSSVGGlyphRefElement::put(JSCell* cell, ExecState* exec, PropertyName propertyName, JSValue value, PutPropertySlot& slot)
{
    JSSVGGlyphRefElement* thisObject = jsCast<JSSVGGlyphRefElement*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    lookupPut<JSSVGGlyphRefElement, Base>(exec, propertyName, value, &JSSVGGlyphRefElementTable, thisObject, slot);
}

void setJSSVGGlyphRefElementGlyphRef(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSSVGGlyphRefElement* castedThis = jsCast<JSSVGGlyphRefElement*>(thisObject);
    SVGGlyphRefElement* impl = static_cast<SVGGlyphRefElement*>(castedThis->impl());
    ExceptionCode ec = 0;
    impl->setGlyphRef(value.isEmpty() ? String() : value.toString(exec)->value(exec), ec);
    setDOMException(exec, ec);
}


void setJSSVGGlyphRefElementFormat(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSSVGGlyphRefElement* castedThis = jsCast<JSSVGGlyphRefElement*>(thisObject);
    SVGGlyphRefElement* impl = static_cast<SVGGlyphRefElement*>(castedThis->impl());
    impl->setAttribute(WebCore::SVGNames::formatAttr, valueToStringWithNullCheck(exec, value));
}


void setJSSVGGlyphRefElementX(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSSVGGlyphRefElement* castedThis = jsCast<JSSVGGlyphRefElement*>(thisObject);
    SVGGlyphRefElement* impl = static_cast<SVGGlyphRefElement*>(castedThis->impl());
    ExceptionCode ec = 0;
    impl->setX(value.toFloat(exec), ec);
    setDOMException(exec, ec);
}


void setJSSVGGlyphRefElementY(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSSVGGlyphRefElement* castedThis = jsCast<JSSVGGlyphRefElement*>(thisObject);
    SVGGlyphRefElement* impl = static_cast<SVGGlyphRefElement*>(castedThis->impl());
    ExceptionCode ec = 0;
    impl->setY(value.toFloat(exec), ec);
    setDOMException(exec, ec);
}


void setJSSVGGlyphRefElementDx(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSSVGGlyphRefElement* castedThis = jsCast<JSSVGGlyphRefElement*>(thisObject);
    SVGGlyphRefElement* impl = static_cast<SVGGlyphRefElement*>(castedThis->impl());
    ExceptionCode ec = 0;
    impl->setDx(value.toFloat(exec), ec);
    setDOMException(exec, ec);
}


void setJSSVGGlyphRefElementDy(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSSVGGlyphRefElement* castedThis = jsCast<JSSVGGlyphRefElement*>(thisObject);
    SVGGlyphRefElement* impl = static_cast<SVGGlyphRefElement*>(castedThis->impl());
    ExceptionCode ec = 0;
    impl->setDy(value.toFloat(exec), ec);
    setDOMException(exec, ec);
}


JSValue JSSVGGlyphRefElement::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSSVGGlyphRefElementConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}

EncodedJSValue JSC_HOST_CALL jsSVGGlyphRefElementPrototypeFunctionGetPresentationAttribute(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSSVGGlyphRefElement::s_info))
        return throwVMTypeError(exec);
    JSSVGGlyphRefElement* castedThis = jsCast<JSSVGGlyphRefElement*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSSVGGlyphRefElement::s_info);
    SVGGlyphRefElement* impl = static_cast<SVGGlyphRefElement*>(castedThis->impl());
    const String& name(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).isEmpty() ? String() : MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).toString(exec)->value(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->getPresentationAttribute(name)));
    return JSValue::encode(result);
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_FONTS)
