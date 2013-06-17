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
#include "JSHTMLEmbedElement.h"

#include "ExceptionCode.h"
#include "HTMLEmbedElement.h"
#include "HTMLNames.h"
#include "JSDOMBinding.h"
#include "JSHTMLEmbedElementCustom.h"
#include "JSSVGDocument.h"
#include "KURL.h"
#include "SVGDocument.h"
#include <runtime/Error.h>
#include <runtime/JSString.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSHTMLEmbedElementTableValues[] =
{
    { "align", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLEmbedElementAlign), (intptr_t)setJSHTMLEmbedElementAlign, NoIntrinsic },
    { "height", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLEmbedElementHeight), (intptr_t)setJSHTMLEmbedElementHeight, NoIntrinsic },
    { "name", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLEmbedElementName), (intptr_t)setJSHTMLEmbedElementName, NoIntrinsic },
    { "src", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLEmbedElementSrc), (intptr_t)setJSHTMLEmbedElementSrc, NoIntrinsic },
    { "type", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLEmbedElementType), (intptr_t)setJSHTMLEmbedElementType, NoIntrinsic },
    { "width", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLEmbedElementWidth), (intptr_t)setJSHTMLEmbedElementWidth, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLEmbedElementConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSHTMLEmbedElementTable = { 19, 15, JSHTMLEmbedElementTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSHTMLEmbedElementConstructorTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSHTMLEmbedElementConstructorTable = { 1, 0, JSHTMLEmbedElementConstructorTableValues, 0 };
const ClassInfo JSHTMLEmbedElementConstructor::s_info = { "HTMLEmbedElementConstructor", &Base::s_info, &JSHTMLEmbedElementConstructorTable, 0, CREATE_METHOD_TABLE(JSHTMLEmbedElementConstructor) };

JSHTMLEmbedElementConstructor::JSHTMLEmbedElementConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSHTMLEmbedElementConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSHTMLEmbedElementPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSHTMLEmbedElementConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSHTMLEmbedElementConstructor, JSDOMWrapper>(exec, &JSHTMLEmbedElementConstructorTable, jsCast<JSHTMLEmbedElementConstructor*>(cell), propertyName, slot);
}

bool JSHTMLEmbedElementConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSHTMLEmbedElementConstructor, JSDOMWrapper>(exec, &JSHTMLEmbedElementConstructorTable, jsCast<JSHTMLEmbedElementConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSHTMLEmbedElementPrototypeTableValues[] =
{
    { "getSVGDocument", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsHTMLEmbedElementPrototypeFunctionGetSVGDocument), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSHTMLEmbedElementPrototypeTable = { 2, 1, JSHTMLEmbedElementPrototypeTableValues, 0 };
const ClassInfo JSHTMLEmbedElementPrototype::s_info = { "HTMLEmbedElementPrototype", &Base::s_info, &JSHTMLEmbedElementPrototypeTable, 0, CREATE_METHOD_TABLE(JSHTMLEmbedElementPrototype) };

JSObject* JSHTMLEmbedElementPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSHTMLEmbedElement>(exec, globalObject);
}

bool JSHTMLEmbedElementPrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSHTMLEmbedElementPrototype* thisObject = jsCast<JSHTMLEmbedElementPrototype*>(cell);
    return getStaticFunctionSlot<JSObject>(exec, &JSHTMLEmbedElementPrototypeTable, thisObject, propertyName, slot);
}

bool JSHTMLEmbedElementPrototype::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSHTMLEmbedElementPrototype* thisObject = jsCast<JSHTMLEmbedElementPrototype*>(object);
    return getStaticFunctionDescriptor<JSObject>(exec, &JSHTMLEmbedElementPrototypeTable, thisObject, propertyName, descriptor);
}

const ClassInfo JSHTMLEmbedElement::s_info = { "HTMLEmbedElement", &Base::s_info, &JSHTMLEmbedElementTable, 0 , CREATE_METHOD_TABLE(JSHTMLEmbedElement) };

JSHTMLEmbedElement::JSHTMLEmbedElement(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<HTMLEmbedElement> impl)
    : JSHTMLElement(structure, globalObject, impl)
{
}

void JSHTMLEmbedElement::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSHTMLEmbedElement::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSHTMLEmbedElementPrototype::create(exec->globalData(), globalObject, JSHTMLEmbedElementPrototype::createStructure(exec->globalData(), globalObject, JSHTMLElementPrototype::self(exec, globalObject)));
}

bool JSHTMLEmbedElement::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSHTMLEmbedElement* thisObject = jsCast<JSHTMLEmbedElement*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    if (thisObject->getOwnPropertySlotDelegate(exec, propertyName, slot))
        return true;
    return getStaticValueSlot<JSHTMLEmbedElement, Base>(exec, &JSHTMLEmbedElementTable, thisObject, propertyName, slot);
}

bool JSHTMLEmbedElement::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSHTMLEmbedElement* thisObject = jsCast<JSHTMLEmbedElement*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    if (thisObject->getOwnPropertyDescriptorDelegate(exec, propertyName, descriptor))
        return true;
    return getStaticValueDescriptor<JSHTMLEmbedElement, Base>(exec, &JSHTMLEmbedElementTable, thisObject, propertyName, descriptor);
}

bool JSHTMLEmbedElement::getOwnPropertySlotByIndex(JSCell* cell, ExecState* exec, unsigned index, PropertySlot& slot)
{
    JSHTMLEmbedElement* thisObject = jsCast<JSHTMLEmbedElement*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    PropertyName propertyName = Identifier::from(exec, index);
    if (thisObject->getOwnPropertySlotDelegate(exec, propertyName, slot))
        return true;
    return Base::getOwnPropertySlotByIndex(thisObject, exec, index, slot);
}

JSValue jsHTMLEmbedElementAlign(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLEmbedElement* castedThis = jsCast<JSHTMLEmbedElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLEmbedElement* impl = static_cast<HTMLEmbedElement*>(castedThis->impl());
    JSValue result = jsStringWithCache(exec, impl->getAttribute(WebCore::HTMLNames::alignAttr));
    return result;
}


JSValue jsHTMLEmbedElementHeight(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLEmbedElement* castedThis = jsCast<JSHTMLEmbedElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLEmbedElement* impl = static_cast<HTMLEmbedElement*>(castedThis->impl());
    JSValue result = jsStringWithCache(exec, impl->getAttribute(WebCore::HTMLNames::heightAttr));
    return result;
}


JSValue jsHTMLEmbedElementName(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLEmbedElement* castedThis = jsCast<JSHTMLEmbedElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLEmbedElement* impl = static_cast<HTMLEmbedElement*>(castedThis->impl());
    JSValue result = jsStringWithCache(exec, impl->getAttribute(WebCore::HTMLNames::nameAttr));
    return result;
}


JSValue jsHTMLEmbedElementSrc(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLEmbedElement* castedThis = jsCast<JSHTMLEmbedElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLEmbedElement* impl = static_cast<HTMLEmbedElement*>(castedThis->impl());
    JSValue result = jsStringWithCache(exec, impl->getURLAttribute(WebCore::HTMLNames::srcAttr));
    return result;
}


JSValue jsHTMLEmbedElementType(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLEmbedElement* castedThis = jsCast<JSHTMLEmbedElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLEmbedElement* impl = static_cast<HTMLEmbedElement*>(castedThis->impl());
    JSValue result = jsStringWithCache(exec, impl->getAttribute(WebCore::HTMLNames::typeAttr));
    return result;
}


JSValue jsHTMLEmbedElementWidth(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLEmbedElement* castedThis = jsCast<JSHTMLEmbedElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLEmbedElement* impl = static_cast<HTMLEmbedElement*>(castedThis->impl());
    JSValue result = jsStringWithCache(exec, impl->getAttribute(WebCore::HTMLNames::widthAttr));
    return result;
}


JSValue jsHTMLEmbedElementConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLEmbedElement* domObject = jsCast<JSHTMLEmbedElement*>(asObject(slotBase));
    return JSHTMLEmbedElement::getConstructor(exec, domObject->globalObject());
}

void JSHTMLEmbedElement::put(JSCell* cell, ExecState* exec, PropertyName propertyName, JSValue value, PutPropertySlot& slot)
{
    JSHTMLEmbedElement* thisObject = jsCast<JSHTMLEmbedElement*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    if (thisObject->putDelegate(exec, propertyName, value, slot))
        return;
    lookupPut<JSHTMLEmbedElement, Base>(exec, propertyName, value, &JSHTMLEmbedElementTable, thisObject, slot);
}

void JSHTMLEmbedElement::putByIndex(JSCell* cell, ExecState* exec, unsigned index, JSValue value, bool shouldThrow)
{
    JSHTMLEmbedElement* thisObject = jsCast<JSHTMLEmbedElement*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    PropertyName propertyName = Identifier::from(exec, index);
    PutPropertySlot slot(shouldThrow);
    if (thisObject->putDelegate(exec, propertyName, value, slot))
        return;
    Base::putByIndex(cell, exec, index, value, shouldThrow);
}

void setJSHTMLEmbedElementAlign(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSHTMLEmbedElement* castedThis = jsCast<JSHTMLEmbedElement*>(thisObject);
    HTMLEmbedElement* impl = static_cast<HTMLEmbedElement*>(castedThis->impl());
    impl->setAttribute(WebCore::HTMLNames::alignAttr, valueToStringWithNullCheck(exec, value));
}


void setJSHTMLEmbedElementHeight(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSHTMLEmbedElement* castedThis = jsCast<JSHTMLEmbedElement*>(thisObject);
    HTMLEmbedElement* impl = static_cast<HTMLEmbedElement*>(castedThis->impl());
    impl->setAttribute(WebCore::HTMLNames::heightAttr, valueToStringWithNullCheck(exec, value));
}


void setJSHTMLEmbedElementName(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSHTMLEmbedElement* castedThis = jsCast<JSHTMLEmbedElement*>(thisObject);
    HTMLEmbedElement* impl = static_cast<HTMLEmbedElement*>(castedThis->impl());
    impl->setAttribute(WebCore::HTMLNames::nameAttr, valueToStringWithNullCheck(exec, value));
}


void setJSHTMLEmbedElementSrc(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSHTMLEmbedElement* castedThis = jsCast<JSHTMLEmbedElement*>(thisObject);
    HTMLEmbedElement* impl = static_cast<HTMLEmbedElement*>(castedThis->impl());
    impl->setAttribute(WebCore::HTMLNames::srcAttr, valueToStringWithNullCheck(exec, value));
}


void setJSHTMLEmbedElementType(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSHTMLEmbedElement* castedThis = jsCast<JSHTMLEmbedElement*>(thisObject);
    HTMLEmbedElement* impl = static_cast<HTMLEmbedElement*>(castedThis->impl());
    impl->setAttribute(WebCore::HTMLNames::typeAttr, valueToStringWithNullCheck(exec, value));
}


void setJSHTMLEmbedElementWidth(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSHTMLEmbedElement* castedThis = jsCast<JSHTMLEmbedElement*>(thisObject);
    HTMLEmbedElement* impl = static_cast<HTMLEmbedElement*>(castedThis->impl());
    impl->setAttribute(WebCore::HTMLNames::widthAttr, valueToStringWithNullCheck(exec, value));
}


JSValue JSHTMLEmbedElement::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSHTMLEmbedElementConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}

EncodedJSValue JSC_HOST_CALL jsHTMLEmbedElementPrototypeFunctionGetSVGDocument(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSHTMLEmbedElement::s_info))
        return throwVMTypeError(exec);
    JSHTMLEmbedElement* castedThis = jsCast<JSHTMLEmbedElement*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSHTMLEmbedElement::s_info);
    HTMLEmbedElement* impl = static_cast<HTMLEmbedElement*>(castedThis->impl());
    ExceptionCode ec = 0;
    if (!shouldAllowAccessToNode(exec, impl->getSVGDocument(ec)))
        return JSValue::encode(jsNull());

    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->getSVGDocument(ec)));
    setDOMException(exec, ec);
    return JSValue::encode(result);
}


}
