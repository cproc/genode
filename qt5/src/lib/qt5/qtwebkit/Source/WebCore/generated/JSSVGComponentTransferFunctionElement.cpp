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

#include "JSSVGComponentTransferFunctionElement.h"

#include "JSSVGAnimatedEnumeration.h"
#include "JSSVGAnimatedNumber.h"
#include "JSSVGAnimatedNumberList.h"
#include "SVGComponentTransferFunctionElement.h"
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSSVGComponentTransferFunctionElementTableValues[] =
{
    { "type", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGComponentTransferFunctionElementType), (intptr_t)0, NoIntrinsic },
    { "tableValues", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGComponentTransferFunctionElementTableValues), (intptr_t)0, NoIntrinsic },
    { "slope", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGComponentTransferFunctionElementSlope), (intptr_t)0, NoIntrinsic },
    { "intercept", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGComponentTransferFunctionElementIntercept), (intptr_t)0, NoIntrinsic },
    { "amplitude", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGComponentTransferFunctionElementAmplitude), (intptr_t)0, NoIntrinsic },
    { "exponent", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGComponentTransferFunctionElementExponent), (intptr_t)0, NoIntrinsic },
    { "offset", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGComponentTransferFunctionElementOffset), (intptr_t)0, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGComponentTransferFunctionElementConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGComponentTransferFunctionElementTable = { 19, 15, JSSVGComponentTransferFunctionElementTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSSVGComponentTransferFunctionElementConstructorTableValues[] =
{
    { "SVG_FECOMPONENTTRANSFER_TYPE_UNKNOWN", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGComponentTransferFunctionElementSVG_FECOMPONENTTRANSFER_TYPE_UNKNOWN), (intptr_t)0, NoIntrinsic },
    { "SVG_FECOMPONENTTRANSFER_TYPE_IDENTITY", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGComponentTransferFunctionElementSVG_FECOMPONENTTRANSFER_TYPE_IDENTITY), (intptr_t)0, NoIntrinsic },
    { "SVG_FECOMPONENTTRANSFER_TYPE_TABLE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGComponentTransferFunctionElementSVG_FECOMPONENTTRANSFER_TYPE_TABLE), (intptr_t)0, NoIntrinsic },
    { "SVG_FECOMPONENTTRANSFER_TYPE_DISCRETE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGComponentTransferFunctionElementSVG_FECOMPONENTTRANSFER_TYPE_DISCRETE), (intptr_t)0, NoIntrinsic },
    { "SVG_FECOMPONENTTRANSFER_TYPE_LINEAR", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGComponentTransferFunctionElementSVG_FECOMPONENTTRANSFER_TYPE_LINEAR), (intptr_t)0, NoIntrinsic },
    { "SVG_FECOMPONENTTRANSFER_TYPE_GAMMA", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGComponentTransferFunctionElementSVG_FECOMPONENTTRANSFER_TYPE_GAMMA), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGComponentTransferFunctionElementConstructorTable = { 17, 15, JSSVGComponentTransferFunctionElementConstructorTableValues, 0 };
const ClassInfo JSSVGComponentTransferFunctionElementConstructor::s_info = { "SVGComponentTransferFunctionElementConstructor", &Base::s_info, &JSSVGComponentTransferFunctionElementConstructorTable, 0, CREATE_METHOD_TABLE(JSSVGComponentTransferFunctionElementConstructor) };

JSSVGComponentTransferFunctionElementConstructor::JSSVGComponentTransferFunctionElementConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSSVGComponentTransferFunctionElementConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSSVGComponentTransferFunctionElementPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSSVGComponentTransferFunctionElementConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSSVGComponentTransferFunctionElementConstructor, JSDOMWrapper>(exec, &JSSVGComponentTransferFunctionElementConstructorTable, jsCast<JSSVGComponentTransferFunctionElementConstructor*>(cell), propertyName, slot);
}

bool JSSVGComponentTransferFunctionElementConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSSVGComponentTransferFunctionElementConstructor, JSDOMWrapper>(exec, &JSSVGComponentTransferFunctionElementConstructorTable, jsCast<JSSVGComponentTransferFunctionElementConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSSVGComponentTransferFunctionElementPrototypeTableValues[] =
{
    { "SVG_FECOMPONENTTRANSFER_TYPE_UNKNOWN", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGComponentTransferFunctionElementSVG_FECOMPONENTTRANSFER_TYPE_UNKNOWN), (intptr_t)0, NoIntrinsic },
    { "SVG_FECOMPONENTTRANSFER_TYPE_IDENTITY", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGComponentTransferFunctionElementSVG_FECOMPONENTTRANSFER_TYPE_IDENTITY), (intptr_t)0, NoIntrinsic },
    { "SVG_FECOMPONENTTRANSFER_TYPE_TABLE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGComponentTransferFunctionElementSVG_FECOMPONENTTRANSFER_TYPE_TABLE), (intptr_t)0, NoIntrinsic },
    { "SVG_FECOMPONENTTRANSFER_TYPE_DISCRETE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGComponentTransferFunctionElementSVG_FECOMPONENTTRANSFER_TYPE_DISCRETE), (intptr_t)0, NoIntrinsic },
    { "SVG_FECOMPONENTTRANSFER_TYPE_LINEAR", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGComponentTransferFunctionElementSVG_FECOMPONENTTRANSFER_TYPE_LINEAR), (intptr_t)0, NoIntrinsic },
    { "SVG_FECOMPONENTTRANSFER_TYPE_GAMMA", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGComponentTransferFunctionElementSVG_FECOMPONENTTRANSFER_TYPE_GAMMA), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGComponentTransferFunctionElementPrototypeTable = { 17, 15, JSSVGComponentTransferFunctionElementPrototypeTableValues, 0 };
const ClassInfo JSSVGComponentTransferFunctionElementPrototype::s_info = { "SVGComponentTransferFunctionElementPrototype", &Base::s_info, &JSSVGComponentTransferFunctionElementPrototypeTable, 0, CREATE_METHOD_TABLE(JSSVGComponentTransferFunctionElementPrototype) };

JSObject* JSSVGComponentTransferFunctionElementPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSSVGComponentTransferFunctionElement>(exec, globalObject);
}

bool JSSVGComponentTransferFunctionElementPrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSSVGComponentTransferFunctionElementPrototype* thisObject = jsCast<JSSVGComponentTransferFunctionElementPrototype*>(cell);
    return getStaticValueSlot<JSSVGComponentTransferFunctionElementPrototype, JSObject>(exec, &JSSVGComponentTransferFunctionElementPrototypeTable, thisObject, propertyName, slot);
}

bool JSSVGComponentTransferFunctionElementPrototype::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSSVGComponentTransferFunctionElementPrototype* thisObject = jsCast<JSSVGComponentTransferFunctionElementPrototype*>(object);
    return getStaticValueDescriptor<JSSVGComponentTransferFunctionElementPrototype, JSObject>(exec, &JSSVGComponentTransferFunctionElementPrototypeTable, thisObject, propertyName, descriptor);
}

const ClassInfo JSSVGComponentTransferFunctionElement::s_info = { "SVGComponentTransferFunctionElement", &Base::s_info, &JSSVGComponentTransferFunctionElementTable, 0 , CREATE_METHOD_TABLE(JSSVGComponentTransferFunctionElement) };

JSSVGComponentTransferFunctionElement::JSSVGComponentTransferFunctionElement(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<SVGComponentTransferFunctionElement> impl)
    : JSSVGElement(structure, globalObject, impl)
{
}

void JSSVGComponentTransferFunctionElement::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSSVGComponentTransferFunctionElement::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSSVGComponentTransferFunctionElementPrototype::create(exec->globalData(), globalObject, JSSVGComponentTransferFunctionElementPrototype::createStructure(exec->globalData(), globalObject, JSSVGElementPrototype::self(exec, globalObject)));
}

bool JSSVGComponentTransferFunctionElement::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSSVGComponentTransferFunctionElement* thisObject = jsCast<JSSVGComponentTransferFunctionElement*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSSVGComponentTransferFunctionElement, Base>(exec, &JSSVGComponentTransferFunctionElementTable, thisObject, propertyName, slot);
}

bool JSSVGComponentTransferFunctionElement::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSSVGComponentTransferFunctionElement* thisObject = jsCast<JSSVGComponentTransferFunctionElement*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSSVGComponentTransferFunctionElement, Base>(exec, &JSSVGComponentTransferFunctionElementTable, thisObject, propertyName, descriptor);
}

JSValue jsSVGComponentTransferFunctionElementType(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGComponentTransferFunctionElement* castedThis = jsCast<JSSVGComponentTransferFunctionElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGComponentTransferFunctionElement* impl = static_cast<SVGComponentTransferFunctionElement*>(castedThis->impl());
    RefPtr<SVGAnimatedEnumeration> obj = impl->typeAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGComponentTransferFunctionElementTableValues(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGComponentTransferFunctionElement* castedThis = jsCast<JSSVGComponentTransferFunctionElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGComponentTransferFunctionElement* impl = static_cast<SVGComponentTransferFunctionElement*>(castedThis->impl());
    RefPtr<SVGAnimatedNumberList> obj = impl->tableValuesAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGComponentTransferFunctionElementSlope(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGComponentTransferFunctionElement* castedThis = jsCast<JSSVGComponentTransferFunctionElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGComponentTransferFunctionElement* impl = static_cast<SVGComponentTransferFunctionElement*>(castedThis->impl());
    RefPtr<SVGAnimatedNumber> obj = impl->slopeAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGComponentTransferFunctionElementIntercept(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGComponentTransferFunctionElement* castedThis = jsCast<JSSVGComponentTransferFunctionElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGComponentTransferFunctionElement* impl = static_cast<SVGComponentTransferFunctionElement*>(castedThis->impl());
    RefPtr<SVGAnimatedNumber> obj = impl->interceptAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGComponentTransferFunctionElementAmplitude(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGComponentTransferFunctionElement* castedThis = jsCast<JSSVGComponentTransferFunctionElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGComponentTransferFunctionElement* impl = static_cast<SVGComponentTransferFunctionElement*>(castedThis->impl());
    RefPtr<SVGAnimatedNumber> obj = impl->amplitudeAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGComponentTransferFunctionElementExponent(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGComponentTransferFunctionElement* castedThis = jsCast<JSSVGComponentTransferFunctionElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGComponentTransferFunctionElement* impl = static_cast<SVGComponentTransferFunctionElement*>(castedThis->impl());
    RefPtr<SVGAnimatedNumber> obj = impl->exponentAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGComponentTransferFunctionElementOffset(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGComponentTransferFunctionElement* castedThis = jsCast<JSSVGComponentTransferFunctionElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGComponentTransferFunctionElement* impl = static_cast<SVGComponentTransferFunctionElement*>(castedThis->impl());
    RefPtr<SVGAnimatedNumber> obj = impl->offsetAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGComponentTransferFunctionElementConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGComponentTransferFunctionElement* domObject = jsCast<JSSVGComponentTransferFunctionElement*>(asObject(slotBase));
    return JSSVGComponentTransferFunctionElement::getConstructor(exec, domObject->globalObject());
}

JSValue JSSVGComponentTransferFunctionElement::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSSVGComponentTransferFunctionElementConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}

// Constant getters

JSValue jsSVGComponentTransferFunctionElementSVG_FECOMPONENTTRANSFER_TYPE_UNKNOWN(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(0));
}

JSValue jsSVGComponentTransferFunctionElementSVG_FECOMPONENTTRANSFER_TYPE_IDENTITY(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(1));
}

JSValue jsSVGComponentTransferFunctionElementSVG_FECOMPONENTTRANSFER_TYPE_TABLE(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(2));
}

JSValue jsSVGComponentTransferFunctionElementSVG_FECOMPONENTTRANSFER_TYPE_DISCRETE(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(3));
}

JSValue jsSVGComponentTransferFunctionElementSVG_FECOMPONENTTRANSFER_TYPE_LINEAR(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(4));
}

JSValue jsSVGComponentTransferFunctionElementSVG_FECOMPONENTTRANSFER_TYPE_GAMMA(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(5));
}


}

#endif // ENABLE(FILTERS) && ENABLE(SVG)
