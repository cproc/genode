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

#if ENABLE(SVG)

#include "JSSVGImageElement.h"

#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "ExceptionCode.h"
#include "JSCSSStyleDeclaration.h"
#include "JSCSSValue.h"
#include "JSDOMBinding.h"
#include "JSSVGAnimatedBoolean.h"
#include "JSSVGAnimatedLength.h"
#include "JSSVGAnimatedPreserveAspectRatio.h"
#include "JSSVGAnimatedString.h"
#include "JSSVGAnimatedTransformList.h"
#include "JSSVGElement.h"
#include "JSSVGMatrix.h"
#include "JSSVGRect.h"
#include "JSSVGStringList.h"
#include "KURL.h"
#include "SVGElement.h"
#include "SVGImageElement.h"
#include "SVGMatrix.h"
#include "SVGRect.h"
#include "SVGStringList.h"
#include "StylePropertySet.h"
#include <runtime/Error.h>
#include <runtime/JSString.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSSVGImageElementTableValues[] =
{
    { "x", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGImageElementX), (intptr_t)0, NoIntrinsic },
    { "y", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGImageElementY), (intptr_t)0, NoIntrinsic },
    { "width", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGImageElementWidth), (intptr_t)0, NoIntrinsic },
    { "height", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGImageElementHeight), (intptr_t)0, NoIntrinsic },
    { "preserveAspectRatio", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGImageElementPreserveAspectRatio), (intptr_t)0, NoIntrinsic },
    { "href", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGImageElementHref), (intptr_t)0, NoIntrinsic },
    { "requiredFeatures", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGImageElementRequiredFeatures), (intptr_t)0, NoIntrinsic },
    { "requiredExtensions", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGImageElementRequiredExtensions), (intptr_t)0, NoIntrinsic },
    { "systemLanguage", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGImageElementSystemLanguage), (intptr_t)0, NoIntrinsic },
    { "xmllang", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGImageElementXmllang), (intptr_t)setJSSVGImageElementXmllang, NoIntrinsic },
    { "xmlspace", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGImageElementXmlspace), (intptr_t)setJSSVGImageElementXmlspace, NoIntrinsic },
    { "externalResourcesRequired", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGImageElementExternalResourcesRequired), (intptr_t)0, NoIntrinsic },
    { "className", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGImageElementClassName), (intptr_t)0, NoIntrinsic },
    { "style", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGImageElementStyle), (intptr_t)0, NoIntrinsic },
    { "transform", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGImageElementTransform), (intptr_t)0, NoIntrinsic },
    { "nearestViewportElement", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGImageElementNearestViewportElement), (intptr_t)0, NoIntrinsic },
    { "farthestViewportElement", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGImageElementFarthestViewportElement), (intptr_t)0, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGImageElementConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGImageElementTable = { 68, 63, JSSVGImageElementTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSSVGImageElementConstructorTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGImageElementConstructorTable = { 1, 0, JSSVGImageElementConstructorTableValues, 0 };
const ClassInfo JSSVGImageElementConstructor::s_info = { "SVGImageElementConstructor", &Base::s_info, &JSSVGImageElementConstructorTable, 0, CREATE_METHOD_TABLE(JSSVGImageElementConstructor) };

JSSVGImageElementConstructor::JSSVGImageElementConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSSVGImageElementConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSSVGImageElementPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSSVGImageElementConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSSVGImageElementConstructor, JSDOMWrapper>(exec, &JSSVGImageElementConstructorTable, jsCast<JSSVGImageElementConstructor*>(cell), propertyName, slot);
}

bool JSSVGImageElementConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSSVGImageElementConstructor, JSDOMWrapper>(exec, &JSSVGImageElementConstructorTable, jsCast<JSSVGImageElementConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSSVGImageElementPrototypeTableValues[] =
{
    { "hasExtension", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsSVGImageElementPrototypeFunctionHasExtension), (intptr_t)1, NoIntrinsic },
    { "getPresentationAttribute", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsSVGImageElementPrototypeFunctionGetPresentationAttribute), (intptr_t)1, NoIntrinsic },
    { "getBBox", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsSVGImageElementPrototypeFunctionGetBBox), (intptr_t)0, NoIntrinsic },
    { "getCTM", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsSVGImageElementPrototypeFunctionGetCTM), (intptr_t)0, NoIntrinsic },
    { "getScreenCTM", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsSVGImageElementPrototypeFunctionGetScreenCTM), (intptr_t)0, NoIntrinsic },
    { "getTransformToElement", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsSVGImageElementPrototypeFunctionGetTransformToElement), (intptr_t)1, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGImageElementPrototypeTable = { 17, 15, JSSVGImageElementPrototypeTableValues, 0 };
const ClassInfo JSSVGImageElementPrototype::s_info = { "SVGImageElementPrototype", &Base::s_info, &JSSVGImageElementPrototypeTable, 0, CREATE_METHOD_TABLE(JSSVGImageElementPrototype) };

JSObject* JSSVGImageElementPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSSVGImageElement>(exec, globalObject);
}

bool JSSVGImageElementPrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSSVGImageElementPrototype* thisObject = jsCast<JSSVGImageElementPrototype*>(cell);
    return getStaticFunctionSlot<JSObject>(exec, &JSSVGImageElementPrototypeTable, thisObject, propertyName, slot);
}

bool JSSVGImageElementPrototype::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSSVGImageElementPrototype* thisObject = jsCast<JSSVGImageElementPrototype*>(object);
    return getStaticFunctionDescriptor<JSObject>(exec, &JSSVGImageElementPrototypeTable, thisObject, propertyName, descriptor);
}

const ClassInfo JSSVGImageElement::s_info = { "SVGImageElement", &Base::s_info, &JSSVGImageElementTable, 0 , CREATE_METHOD_TABLE(JSSVGImageElement) };

JSSVGImageElement::JSSVGImageElement(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<SVGImageElement> impl)
    : JSSVGElement(structure, globalObject, impl)
{
}

void JSSVGImageElement::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSSVGImageElement::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSSVGImageElementPrototype::create(exec->globalData(), globalObject, JSSVGImageElementPrototype::createStructure(exec->globalData(), globalObject, JSSVGElementPrototype::self(exec, globalObject)));
}

bool JSSVGImageElement::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSSVGImageElement* thisObject = jsCast<JSSVGImageElement*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSSVGImageElement, Base>(exec, &JSSVGImageElementTable, thisObject, propertyName, slot);
}

bool JSSVGImageElement::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSSVGImageElement* thisObject = jsCast<JSSVGImageElement*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSSVGImageElement, Base>(exec, &JSSVGImageElementTable, thisObject, propertyName, descriptor);
}

JSValue jsSVGImageElementX(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGImageElement* castedThis = jsCast<JSSVGImageElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGImageElement* impl = static_cast<SVGImageElement*>(castedThis->impl());
    RefPtr<SVGAnimatedLength> obj = impl->xAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGImageElementY(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGImageElement* castedThis = jsCast<JSSVGImageElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGImageElement* impl = static_cast<SVGImageElement*>(castedThis->impl());
    RefPtr<SVGAnimatedLength> obj = impl->yAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGImageElementWidth(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGImageElement* castedThis = jsCast<JSSVGImageElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGImageElement* impl = static_cast<SVGImageElement*>(castedThis->impl());
    RefPtr<SVGAnimatedLength> obj = impl->widthAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGImageElementHeight(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGImageElement* castedThis = jsCast<JSSVGImageElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGImageElement* impl = static_cast<SVGImageElement*>(castedThis->impl());
    RefPtr<SVGAnimatedLength> obj = impl->heightAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGImageElementPreserveAspectRatio(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGImageElement* castedThis = jsCast<JSSVGImageElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGImageElement* impl = static_cast<SVGImageElement*>(castedThis->impl());
    RefPtr<SVGAnimatedPreserveAspectRatio> obj = impl->preserveAspectRatioAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGImageElementHref(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGImageElement* castedThis = jsCast<JSSVGImageElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGImageElement* impl = static_cast<SVGImageElement*>(castedThis->impl());
    RefPtr<SVGAnimatedString> obj = impl->hrefAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGImageElementRequiredFeatures(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGImageElement* castedThis = jsCast<JSSVGImageElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGImageElement* impl = static_cast<SVGImageElement*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(SVGStaticListPropertyTearOff<SVGStringList>::create(impl, impl->requiredFeatures())));
    return result;
}


JSValue jsSVGImageElementRequiredExtensions(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGImageElement* castedThis = jsCast<JSSVGImageElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGImageElement* impl = static_cast<SVGImageElement*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(SVGStaticListPropertyTearOff<SVGStringList>::create(impl, impl->requiredExtensions())));
    return result;
}


JSValue jsSVGImageElementSystemLanguage(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGImageElement* castedThis = jsCast<JSSVGImageElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGImageElement* impl = static_cast<SVGImageElement*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(SVGStaticListPropertyTearOff<SVGStringList>::create(impl, impl->systemLanguage())));
    return result;
}


JSValue jsSVGImageElementXmllang(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGImageElement* castedThis = jsCast<JSSVGImageElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGImageElement* impl = static_cast<SVGImageElement*>(castedThis->impl());
    JSValue result = jsStringWithCache(exec, impl->xmllang());
    return result;
}


JSValue jsSVGImageElementXmlspace(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGImageElement* castedThis = jsCast<JSSVGImageElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGImageElement* impl = static_cast<SVGImageElement*>(castedThis->impl());
    JSValue result = jsStringWithCache(exec, impl->xmlspace());
    return result;
}


JSValue jsSVGImageElementExternalResourcesRequired(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGImageElement* castedThis = jsCast<JSSVGImageElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGImageElement* impl = static_cast<SVGImageElement*>(castedThis->impl());
    RefPtr<SVGAnimatedBoolean> obj = impl->externalResourcesRequiredAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGImageElementClassName(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGImageElement* castedThis = jsCast<JSSVGImageElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGImageElement* impl = static_cast<SVGImageElement*>(castedThis->impl());
    RefPtr<SVGAnimatedString> obj = impl->classNameAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGImageElementStyle(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGImageElement* castedThis = jsCast<JSSVGImageElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGImageElement* impl = static_cast<SVGImageElement*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->style()));
    return result;
}


JSValue jsSVGImageElementTransform(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGImageElement* castedThis = jsCast<JSSVGImageElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGImageElement* impl = static_cast<SVGImageElement*>(castedThis->impl());
    RefPtr<SVGAnimatedTransformList> obj = impl->transformAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGImageElementNearestViewportElement(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGImageElement* castedThis = jsCast<JSSVGImageElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGImageElement* impl = static_cast<SVGImageElement*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->nearestViewportElement()));
    return result;
}


JSValue jsSVGImageElementFarthestViewportElement(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGImageElement* castedThis = jsCast<JSSVGImageElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGImageElement* impl = static_cast<SVGImageElement*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->farthestViewportElement()));
    return result;
}


JSValue jsSVGImageElementConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGImageElement* domObject = jsCast<JSSVGImageElement*>(asObject(slotBase));
    return JSSVGImageElement::getConstructor(exec, domObject->globalObject());
}

void JSSVGImageElement::put(JSCell* cell, ExecState* exec, PropertyName propertyName, JSValue value, PutPropertySlot& slot)
{
    JSSVGImageElement* thisObject = jsCast<JSSVGImageElement*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    lookupPut<JSSVGImageElement, Base>(exec, propertyName, value, &JSSVGImageElementTable, thisObject, slot);
}

void setJSSVGImageElementXmllang(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSSVGImageElement* castedThis = jsCast<JSSVGImageElement*>(thisObject);
    SVGImageElement* impl = static_cast<SVGImageElement*>(castedThis->impl());
    impl->setXmllang(value.isEmpty() ? String() : value.toString(exec)->value(exec));
}


void setJSSVGImageElementXmlspace(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSSVGImageElement* castedThis = jsCast<JSSVGImageElement*>(thisObject);
    SVGImageElement* impl = static_cast<SVGImageElement*>(castedThis->impl());
    impl->setXmlspace(value.isEmpty() ? String() : value.toString(exec)->value(exec));
}


JSValue JSSVGImageElement::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSSVGImageElementConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}

EncodedJSValue JSC_HOST_CALL jsSVGImageElementPrototypeFunctionHasExtension(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSSVGImageElement::s_info))
        return throwVMTypeError(exec);
    JSSVGImageElement* castedThis = jsCast<JSSVGImageElement*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSSVGImageElement::s_info);
    SVGImageElement* impl = static_cast<SVGImageElement*>(castedThis->impl());
    const String& extension(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).isEmpty() ? String() : MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).toString(exec)->value(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    JSC::JSValue result = jsBoolean(impl->hasExtension(extension));
    return JSValue::encode(result);
}

EncodedJSValue JSC_HOST_CALL jsSVGImageElementPrototypeFunctionGetPresentationAttribute(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSSVGImageElement::s_info))
        return throwVMTypeError(exec);
    JSSVGImageElement* castedThis = jsCast<JSSVGImageElement*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSSVGImageElement::s_info);
    SVGImageElement* impl = static_cast<SVGImageElement*>(castedThis->impl());
    const String& name(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).isEmpty() ? String() : MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).toString(exec)->value(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->getPresentationAttribute(name)));
    return JSValue::encode(result);
}

EncodedJSValue JSC_HOST_CALL jsSVGImageElementPrototypeFunctionGetBBox(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSSVGImageElement::s_info))
        return throwVMTypeError(exec);
    JSSVGImageElement* castedThis = jsCast<JSSVGImageElement*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSSVGImageElement::s_info);
    SVGImageElement* impl = static_cast<SVGImageElement*>(castedThis->impl());

    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(SVGPropertyTearOff<FloatRect>::create(impl->getBBox())));
    return JSValue::encode(result);
}

EncodedJSValue JSC_HOST_CALL jsSVGImageElementPrototypeFunctionGetCTM(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSSVGImageElement::s_info))
        return throwVMTypeError(exec);
    JSSVGImageElement* castedThis = jsCast<JSSVGImageElement*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSSVGImageElement::s_info);
    SVGImageElement* impl = static_cast<SVGImageElement*>(castedThis->impl());

    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(SVGPropertyTearOff<SVGMatrix>::create(impl->getCTM())));
    return JSValue::encode(result);
}

EncodedJSValue JSC_HOST_CALL jsSVGImageElementPrototypeFunctionGetScreenCTM(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSSVGImageElement::s_info))
        return throwVMTypeError(exec);
    JSSVGImageElement* castedThis = jsCast<JSSVGImageElement*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSSVGImageElement::s_info);
    SVGImageElement* impl = static_cast<SVGImageElement*>(castedThis->impl());

    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(SVGPropertyTearOff<SVGMatrix>::create(impl->getScreenCTM())));
    return JSValue::encode(result);
}

EncodedJSValue JSC_HOST_CALL jsSVGImageElementPrototypeFunctionGetTransformToElement(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSSVGImageElement::s_info))
        return throwVMTypeError(exec);
    JSSVGImageElement* castedThis = jsCast<JSSVGImageElement*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSSVGImageElement::s_info);
    SVGImageElement* impl = static_cast<SVGImageElement*>(castedThis->impl());
    ExceptionCode ec = 0;
    SVGElement* element(toSVGElement(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(SVGPropertyTearOff<SVGMatrix>::create(impl->getTransformToElement(element, ec))));
    setDOMException(exec, ec);
    return JSValue::encode(result);
}


}

#endif // ENABLE(SVG)
