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

#if ENABLE(FILTERS) && ENABLE(SVG)

#include "JSSVGFEDropShadowElement.h"

#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "ExceptionCode.h"
#include "JSCSSStyleDeclaration.h"
#include "JSCSSValue.h"
#include "JSDOMBinding.h"
#include "JSSVGAnimatedLength.h"
#include "JSSVGAnimatedNumber.h"
#include "JSSVGAnimatedString.h"
#include "SVGFEDropShadowElement.h"
#include "StylePropertySet.h"
#include <runtime/Error.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSSVGFEDropShadowElementTableValues[] =
{
    { "in1", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGFEDropShadowElementIn1), (intptr_t)0, NoIntrinsic },
    { "dx", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGFEDropShadowElementDx), (intptr_t)0, NoIntrinsic },
    { "dy", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGFEDropShadowElementDy), (intptr_t)0, NoIntrinsic },
    { "stdDeviationX", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGFEDropShadowElementStdDeviationX), (intptr_t)0, NoIntrinsic },
    { "stdDeviationY", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGFEDropShadowElementStdDeviationY), (intptr_t)0, NoIntrinsic },
    { "x", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGFEDropShadowElementX), (intptr_t)0, NoIntrinsic },
    { "y", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGFEDropShadowElementY), (intptr_t)0, NoIntrinsic },
    { "width", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGFEDropShadowElementWidth), (intptr_t)0, NoIntrinsic },
    { "height", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGFEDropShadowElementHeight), (intptr_t)0, NoIntrinsic },
    { "result", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGFEDropShadowElementResult), (intptr_t)0, NoIntrinsic },
    { "className", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGFEDropShadowElementClassName), (intptr_t)0, NoIntrinsic },
    { "style", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGFEDropShadowElementStyle), (intptr_t)0, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGFEDropShadowElementConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGFEDropShadowElementTable = { 35, 31, JSSVGFEDropShadowElementTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSSVGFEDropShadowElementConstructorTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGFEDropShadowElementConstructorTable = { 1, 0, JSSVGFEDropShadowElementConstructorTableValues, 0 };
const ClassInfo JSSVGFEDropShadowElementConstructor::s_info = { "SVGFEDropShadowElementConstructor", &Base::s_info, &JSSVGFEDropShadowElementConstructorTable, 0, CREATE_METHOD_TABLE(JSSVGFEDropShadowElementConstructor) };

JSSVGFEDropShadowElementConstructor::JSSVGFEDropShadowElementConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSSVGFEDropShadowElementConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSSVGFEDropShadowElementPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSSVGFEDropShadowElementConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSSVGFEDropShadowElementConstructor, JSDOMWrapper>(exec, &JSSVGFEDropShadowElementConstructorTable, jsCast<JSSVGFEDropShadowElementConstructor*>(cell), propertyName, slot);
}

bool JSSVGFEDropShadowElementConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSSVGFEDropShadowElementConstructor, JSDOMWrapper>(exec, &JSSVGFEDropShadowElementConstructorTable, jsCast<JSSVGFEDropShadowElementConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSSVGFEDropShadowElementPrototypeTableValues[] =
{
    { "setStdDeviation", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsSVGFEDropShadowElementPrototypeFunctionSetStdDeviation), (intptr_t)2, NoIntrinsic },
    { "getPresentationAttribute", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsSVGFEDropShadowElementPrototypeFunctionGetPresentationAttribute), (intptr_t)1, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGFEDropShadowElementPrototypeTable = { 4, 3, JSSVGFEDropShadowElementPrototypeTableValues, 0 };
const ClassInfo JSSVGFEDropShadowElementPrototype::s_info = { "SVGFEDropShadowElementPrototype", &Base::s_info, &JSSVGFEDropShadowElementPrototypeTable, 0, CREATE_METHOD_TABLE(JSSVGFEDropShadowElementPrototype) };

JSObject* JSSVGFEDropShadowElementPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSSVGFEDropShadowElement>(exec, globalObject);
}

bool JSSVGFEDropShadowElementPrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSSVGFEDropShadowElementPrototype* thisObject = jsCast<JSSVGFEDropShadowElementPrototype*>(cell);
    return getStaticFunctionSlot<JSObject>(exec, &JSSVGFEDropShadowElementPrototypeTable, thisObject, propertyName, slot);
}

bool JSSVGFEDropShadowElementPrototype::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSSVGFEDropShadowElementPrototype* thisObject = jsCast<JSSVGFEDropShadowElementPrototype*>(object);
    return getStaticFunctionDescriptor<JSObject>(exec, &JSSVGFEDropShadowElementPrototypeTable, thisObject, propertyName, descriptor);
}

const ClassInfo JSSVGFEDropShadowElement::s_info = { "SVGFEDropShadowElement", &Base::s_info, &JSSVGFEDropShadowElementTable, 0 , CREATE_METHOD_TABLE(JSSVGFEDropShadowElement) };

JSSVGFEDropShadowElement::JSSVGFEDropShadowElement(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<SVGFEDropShadowElement> impl)
    : JSSVGElement(structure, globalObject, impl)
{
}

void JSSVGFEDropShadowElement::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSSVGFEDropShadowElement::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSSVGFEDropShadowElementPrototype::create(exec->globalData(), globalObject, JSSVGFEDropShadowElementPrototype::createStructure(exec->globalData(), globalObject, JSSVGElementPrototype::self(exec, globalObject)));
}

bool JSSVGFEDropShadowElement::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSSVGFEDropShadowElement* thisObject = jsCast<JSSVGFEDropShadowElement*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSSVGFEDropShadowElement, Base>(exec, &JSSVGFEDropShadowElementTable, thisObject, propertyName, slot);
}

bool JSSVGFEDropShadowElement::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSSVGFEDropShadowElement* thisObject = jsCast<JSSVGFEDropShadowElement*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSSVGFEDropShadowElement, Base>(exec, &JSSVGFEDropShadowElementTable, thisObject, propertyName, descriptor);
}

JSValue jsSVGFEDropShadowElementIn1(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGFEDropShadowElement* castedThis = jsCast<JSSVGFEDropShadowElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGFEDropShadowElement* impl = static_cast<SVGFEDropShadowElement*>(castedThis->impl());
    RefPtr<SVGAnimatedString> obj = impl->in1Animated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGFEDropShadowElementDx(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGFEDropShadowElement* castedThis = jsCast<JSSVGFEDropShadowElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGFEDropShadowElement* impl = static_cast<SVGFEDropShadowElement*>(castedThis->impl());
    RefPtr<SVGAnimatedNumber> obj = impl->dxAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGFEDropShadowElementDy(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGFEDropShadowElement* castedThis = jsCast<JSSVGFEDropShadowElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGFEDropShadowElement* impl = static_cast<SVGFEDropShadowElement*>(castedThis->impl());
    RefPtr<SVGAnimatedNumber> obj = impl->dyAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGFEDropShadowElementStdDeviationX(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGFEDropShadowElement* castedThis = jsCast<JSSVGFEDropShadowElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGFEDropShadowElement* impl = static_cast<SVGFEDropShadowElement*>(castedThis->impl());
    RefPtr<SVGAnimatedNumber> obj = impl->stdDeviationXAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGFEDropShadowElementStdDeviationY(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGFEDropShadowElement* castedThis = jsCast<JSSVGFEDropShadowElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGFEDropShadowElement* impl = static_cast<SVGFEDropShadowElement*>(castedThis->impl());
    RefPtr<SVGAnimatedNumber> obj = impl->stdDeviationYAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGFEDropShadowElementX(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGFEDropShadowElement* castedThis = jsCast<JSSVGFEDropShadowElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGFEDropShadowElement* impl = static_cast<SVGFEDropShadowElement*>(castedThis->impl());
    RefPtr<SVGAnimatedLength> obj = impl->xAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGFEDropShadowElementY(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGFEDropShadowElement* castedThis = jsCast<JSSVGFEDropShadowElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGFEDropShadowElement* impl = static_cast<SVGFEDropShadowElement*>(castedThis->impl());
    RefPtr<SVGAnimatedLength> obj = impl->yAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGFEDropShadowElementWidth(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGFEDropShadowElement* castedThis = jsCast<JSSVGFEDropShadowElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGFEDropShadowElement* impl = static_cast<SVGFEDropShadowElement*>(castedThis->impl());
    RefPtr<SVGAnimatedLength> obj = impl->widthAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGFEDropShadowElementHeight(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGFEDropShadowElement* castedThis = jsCast<JSSVGFEDropShadowElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGFEDropShadowElement* impl = static_cast<SVGFEDropShadowElement*>(castedThis->impl());
    RefPtr<SVGAnimatedLength> obj = impl->heightAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGFEDropShadowElementResult(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGFEDropShadowElement* castedThis = jsCast<JSSVGFEDropShadowElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGFEDropShadowElement* impl = static_cast<SVGFEDropShadowElement*>(castedThis->impl());
    RefPtr<SVGAnimatedString> obj = impl->resultAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGFEDropShadowElementClassName(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGFEDropShadowElement* castedThis = jsCast<JSSVGFEDropShadowElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGFEDropShadowElement* impl = static_cast<SVGFEDropShadowElement*>(castedThis->impl());
    RefPtr<SVGAnimatedString> obj = impl->classNameAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGFEDropShadowElementStyle(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGFEDropShadowElement* castedThis = jsCast<JSSVGFEDropShadowElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGFEDropShadowElement* impl = static_cast<SVGFEDropShadowElement*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->style()));
    return result;
}


JSValue jsSVGFEDropShadowElementConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGFEDropShadowElement* domObject = jsCast<JSSVGFEDropShadowElement*>(asObject(slotBase));
    return JSSVGFEDropShadowElement::getConstructor(exec, domObject->globalObject());
}

JSValue JSSVGFEDropShadowElement::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSSVGFEDropShadowElementConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}

EncodedJSValue JSC_HOST_CALL jsSVGFEDropShadowElementPrototypeFunctionSetStdDeviation(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSSVGFEDropShadowElement::s_info))
        return throwVMTypeError(exec);
    JSSVGFEDropShadowElement* castedThis = jsCast<JSSVGFEDropShadowElement*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSSVGFEDropShadowElement::s_info);
    SVGFEDropShadowElement* impl = static_cast<SVGFEDropShadowElement*>(castedThis->impl());
    float stdDeviationX(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).toFloat(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    float stdDeviationY(MAYBE_MISSING_PARAMETER(exec, 1, DefaultIsUndefined).toFloat(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    impl->setStdDeviation(stdDeviationX, stdDeviationY);
    return JSValue::encode(jsUndefined());
}

EncodedJSValue JSC_HOST_CALL jsSVGFEDropShadowElementPrototypeFunctionGetPresentationAttribute(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSSVGFEDropShadowElement::s_info))
        return throwVMTypeError(exec);
    JSSVGFEDropShadowElement* castedThis = jsCast<JSSVGFEDropShadowElement*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSSVGFEDropShadowElement::s_info);
    SVGFEDropShadowElement* impl = static_cast<SVGFEDropShadowElement*>(castedThis->impl());
    const String& name(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).isEmpty() ? String() : MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).toString(exec)->value(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->getPresentationAttribute(name)));
    return JSValue::encode(result);
}


}

#endif // ENABLE(FILTERS) && ENABLE(SVG)
