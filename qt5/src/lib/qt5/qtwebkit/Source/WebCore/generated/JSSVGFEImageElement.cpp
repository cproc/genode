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

#include "JSSVGFEImageElement.h"

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
#include "KURL.h"
#include "SVGFEImageElement.h"
#include "StylePropertySet.h"
#include <runtime/Error.h>
#include <runtime/JSString.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSSVGFEImageElementTableValues[] =
{
    { "preserveAspectRatio", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGFEImageElementPreserveAspectRatio), (intptr_t)0, NoIntrinsic },
    { "href", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGFEImageElementHref), (intptr_t)0, NoIntrinsic },
    { "xmllang", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGFEImageElementXmllang), (intptr_t)setJSSVGFEImageElementXmllang, NoIntrinsic },
    { "xmlspace", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGFEImageElementXmlspace), (intptr_t)setJSSVGFEImageElementXmlspace, NoIntrinsic },
    { "externalResourcesRequired", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGFEImageElementExternalResourcesRequired), (intptr_t)0, NoIntrinsic },
    { "x", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGFEImageElementX), (intptr_t)0, NoIntrinsic },
    { "y", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGFEImageElementY), (intptr_t)0, NoIntrinsic },
    { "width", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGFEImageElementWidth), (intptr_t)0, NoIntrinsic },
    { "height", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGFEImageElementHeight), (intptr_t)0, NoIntrinsic },
    { "result", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGFEImageElementResult), (intptr_t)0, NoIntrinsic },
    { "className", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGFEImageElementClassName), (intptr_t)0, NoIntrinsic },
    { "style", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGFEImageElementStyle), (intptr_t)0, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGFEImageElementConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGFEImageElementTable = { 33, 31, JSSVGFEImageElementTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSSVGFEImageElementConstructorTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGFEImageElementConstructorTable = { 1, 0, JSSVGFEImageElementConstructorTableValues, 0 };
const ClassInfo JSSVGFEImageElementConstructor::s_info = { "SVGFEImageElementConstructor", &Base::s_info, &JSSVGFEImageElementConstructorTable, 0, CREATE_METHOD_TABLE(JSSVGFEImageElementConstructor) };

JSSVGFEImageElementConstructor::JSSVGFEImageElementConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSSVGFEImageElementConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSSVGFEImageElementPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSSVGFEImageElementConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSSVGFEImageElementConstructor, JSDOMWrapper>(exec, &JSSVGFEImageElementConstructorTable, jsCast<JSSVGFEImageElementConstructor*>(cell), propertyName, slot);
}

bool JSSVGFEImageElementConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSSVGFEImageElementConstructor, JSDOMWrapper>(exec, &JSSVGFEImageElementConstructorTable, jsCast<JSSVGFEImageElementConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSSVGFEImageElementPrototypeTableValues[] =
{
    { "getPresentationAttribute", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsSVGFEImageElementPrototypeFunctionGetPresentationAttribute), (intptr_t)1, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSSVGFEImageElementPrototypeTable = { 2, 1, JSSVGFEImageElementPrototypeTableValues, 0 };
const ClassInfo JSSVGFEImageElementPrototype::s_info = { "SVGFEImageElementPrototype", &Base::s_info, &JSSVGFEImageElementPrototypeTable, 0, CREATE_METHOD_TABLE(JSSVGFEImageElementPrototype) };

JSObject* JSSVGFEImageElementPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSSVGFEImageElement>(exec, globalObject);
}

bool JSSVGFEImageElementPrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSSVGFEImageElementPrototype* thisObject = jsCast<JSSVGFEImageElementPrototype*>(cell);
    return getStaticFunctionSlot<JSObject>(exec, &JSSVGFEImageElementPrototypeTable, thisObject, propertyName, slot);
}

bool JSSVGFEImageElementPrototype::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSSVGFEImageElementPrototype* thisObject = jsCast<JSSVGFEImageElementPrototype*>(object);
    return getStaticFunctionDescriptor<JSObject>(exec, &JSSVGFEImageElementPrototypeTable, thisObject, propertyName, descriptor);
}

const ClassInfo JSSVGFEImageElement::s_info = { "SVGFEImageElement", &Base::s_info, &JSSVGFEImageElementTable, 0 , CREATE_METHOD_TABLE(JSSVGFEImageElement) };

JSSVGFEImageElement::JSSVGFEImageElement(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<SVGFEImageElement> impl)
    : JSSVGElement(structure, globalObject, impl)
{
}

void JSSVGFEImageElement::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSSVGFEImageElement::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSSVGFEImageElementPrototype::create(exec->globalData(), globalObject, JSSVGFEImageElementPrototype::createStructure(exec->globalData(), globalObject, JSSVGElementPrototype::self(exec, globalObject)));
}

bool JSSVGFEImageElement::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSSVGFEImageElement* thisObject = jsCast<JSSVGFEImageElement*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSSVGFEImageElement, Base>(exec, &JSSVGFEImageElementTable, thisObject, propertyName, slot);
}

bool JSSVGFEImageElement::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSSVGFEImageElement* thisObject = jsCast<JSSVGFEImageElement*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSSVGFEImageElement, Base>(exec, &JSSVGFEImageElementTable, thisObject, propertyName, descriptor);
}

JSValue jsSVGFEImageElementPreserveAspectRatio(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGFEImageElement* castedThis = jsCast<JSSVGFEImageElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGFEImageElement* impl = static_cast<SVGFEImageElement*>(castedThis->impl());
    RefPtr<SVGAnimatedPreserveAspectRatio> obj = impl->preserveAspectRatioAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGFEImageElementHref(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGFEImageElement* castedThis = jsCast<JSSVGFEImageElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGFEImageElement* impl = static_cast<SVGFEImageElement*>(castedThis->impl());
    RefPtr<SVGAnimatedString> obj = impl->hrefAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGFEImageElementXmllang(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGFEImageElement* castedThis = jsCast<JSSVGFEImageElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGFEImageElement* impl = static_cast<SVGFEImageElement*>(castedThis->impl());
    JSValue result = jsStringWithCache(exec, impl->xmllang());
    return result;
}


JSValue jsSVGFEImageElementXmlspace(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGFEImageElement* castedThis = jsCast<JSSVGFEImageElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGFEImageElement* impl = static_cast<SVGFEImageElement*>(castedThis->impl());
    JSValue result = jsStringWithCache(exec, impl->xmlspace());
    return result;
}


JSValue jsSVGFEImageElementExternalResourcesRequired(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGFEImageElement* castedThis = jsCast<JSSVGFEImageElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGFEImageElement* impl = static_cast<SVGFEImageElement*>(castedThis->impl());
    RefPtr<SVGAnimatedBoolean> obj = impl->externalResourcesRequiredAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGFEImageElementX(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGFEImageElement* castedThis = jsCast<JSSVGFEImageElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGFEImageElement* impl = static_cast<SVGFEImageElement*>(castedThis->impl());
    RefPtr<SVGAnimatedLength> obj = impl->xAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGFEImageElementY(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGFEImageElement* castedThis = jsCast<JSSVGFEImageElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGFEImageElement* impl = static_cast<SVGFEImageElement*>(castedThis->impl());
    RefPtr<SVGAnimatedLength> obj = impl->yAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGFEImageElementWidth(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGFEImageElement* castedThis = jsCast<JSSVGFEImageElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGFEImageElement* impl = static_cast<SVGFEImageElement*>(castedThis->impl());
    RefPtr<SVGAnimatedLength> obj = impl->widthAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGFEImageElementHeight(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGFEImageElement* castedThis = jsCast<JSSVGFEImageElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGFEImageElement* impl = static_cast<SVGFEImageElement*>(castedThis->impl());
    RefPtr<SVGAnimatedLength> obj = impl->heightAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGFEImageElementResult(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGFEImageElement* castedThis = jsCast<JSSVGFEImageElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGFEImageElement* impl = static_cast<SVGFEImageElement*>(castedThis->impl());
    RefPtr<SVGAnimatedString> obj = impl->resultAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGFEImageElementClassName(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGFEImageElement* castedThis = jsCast<JSSVGFEImageElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGFEImageElement* impl = static_cast<SVGFEImageElement*>(castedThis->impl());
    RefPtr<SVGAnimatedString> obj = impl->classNameAnimated();
    JSValue result =  toJS(exec, castedThis->globalObject(), obj.get());
    return result;
}


JSValue jsSVGFEImageElementStyle(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGFEImageElement* castedThis = jsCast<JSSVGFEImageElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGFEImageElement* impl = static_cast<SVGFEImageElement*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->style()));
    return result;
}


JSValue jsSVGFEImageElementConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSSVGFEImageElement* domObject = jsCast<JSSVGFEImageElement*>(asObject(slotBase));
    return JSSVGFEImageElement::getConstructor(exec, domObject->globalObject());
}

void JSSVGFEImageElement::put(JSCell* cell, ExecState* exec, PropertyName propertyName, JSValue value, PutPropertySlot& slot)
{
    JSSVGFEImageElement* thisObject = jsCast<JSSVGFEImageElement*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    lookupPut<JSSVGFEImageElement, Base>(exec, propertyName, value, &JSSVGFEImageElementTable, thisObject, slot);
}

void setJSSVGFEImageElementXmllang(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSSVGFEImageElement* castedThis = jsCast<JSSVGFEImageElement*>(thisObject);
    SVGFEImageElement* impl = static_cast<SVGFEImageElement*>(castedThis->impl());
    impl->setXmllang(value.isEmpty() ? String() : value.toString(exec)->value(exec));
}


void setJSSVGFEImageElementXmlspace(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSSVGFEImageElement* castedThis = jsCast<JSSVGFEImageElement*>(thisObject);
    SVGFEImageElement* impl = static_cast<SVGFEImageElement*>(castedThis->impl());
    impl->setXmlspace(value.isEmpty() ? String() : value.toString(exec)->value(exec));
}


JSValue JSSVGFEImageElement::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSSVGFEImageElementConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}

EncodedJSValue JSC_HOST_CALL jsSVGFEImageElementPrototypeFunctionGetPresentationAttribute(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSSVGFEImageElement::s_info))
        return throwVMTypeError(exec);
    JSSVGFEImageElement* castedThis = jsCast<JSSVGFEImageElement*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSSVGFEImageElement::s_info);
    SVGFEImageElement* impl = static_cast<SVGFEImageElement*>(castedThis->impl());
    const String& name(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).isEmpty() ? String() : MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).toString(exec)->value(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->getPresentationAttribute(name)));
    return JSValue::encode(result);
}


}

#endif // ENABLE(FILTERS) && ENABLE(SVG)
