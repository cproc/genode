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
#include "JSHTMLBaseFontElement.h"

#include "HTMLBaseFontElement.h"
#include "HTMLNames.h"
#include "KURL.h"
#include <runtime/JSString.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSHTMLBaseFontElementTableValues[] =
{
    { "color", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLBaseFontElementColor), (intptr_t)setJSHTMLBaseFontElementColor, NoIntrinsic },
    { "face", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLBaseFontElementFace), (intptr_t)setJSHTMLBaseFontElementFace, NoIntrinsic },
    { "size", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLBaseFontElementSize), (intptr_t)setJSHTMLBaseFontElementSize, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLBaseFontElementConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSHTMLBaseFontElementTable = { 9, 7, JSHTMLBaseFontElementTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSHTMLBaseFontElementConstructorTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSHTMLBaseFontElementConstructorTable = { 1, 0, JSHTMLBaseFontElementConstructorTableValues, 0 };
const ClassInfo JSHTMLBaseFontElementConstructor::s_info = { "HTMLBaseFontElementConstructor", &Base::s_info, &JSHTMLBaseFontElementConstructorTable, 0, CREATE_METHOD_TABLE(JSHTMLBaseFontElementConstructor) };

JSHTMLBaseFontElementConstructor::JSHTMLBaseFontElementConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSHTMLBaseFontElementConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSHTMLBaseFontElementPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSHTMLBaseFontElementConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSHTMLBaseFontElementConstructor, JSDOMWrapper>(exec, &JSHTMLBaseFontElementConstructorTable, jsCast<JSHTMLBaseFontElementConstructor*>(cell), propertyName, slot);
}

bool JSHTMLBaseFontElementConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSHTMLBaseFontElementConstructor, JSDOMWrapper>(exec, &JSHTMLBaseFontElementConstructorTable, jsCast<JSHTMLBaseFontElementConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSHTMLBaseFontElementPrototypeTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSHTMLBaseFontElementPrototypeTable = { 1, 0, JSHTMLBaseFontElementPrototypeTableValues, 0 };
const ClassInfo JSHTMLBaseFontElementPrototype::s_info = { "HTMLBaseFontElementPrototype", &Base::s_info, &JSHTMLBaseFontElementPrototypeTable, 0, CREATE_METHOD_TABLE(JSHTMLBaseFontElementPrototype) };

JSObject* JSHTMLBaseFontElementPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSHTMLBaseFontElement>(exec, globalObject);
}

const ClassInfo JSHTMLBaseFontElement::s_info = { "HTMLBaseFontElement", &Base::s_info, &JSHTMLBaseFontElementTable, 0 , CREATE_METHOD_TABLE(JSHTMLBaseFontElement) };

JSHTMLBaseFontElement::JSHTMLBaseFontElement(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<HTMLBaseFontElement> impl)
    : JSHTMLElement(structure, globalObject, impl)
{
}

void JSHTMLBaseFontElement::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSHTMLBaseFontElement::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSHTMLBaseFontElementPrototype::create(exec->globalData(), globalObject, JSHTMLBaseFontElementPrototype::createStructure(exec->globalData(), globalObject, JSHTMLElementPrototype::self(exec, globalObject)));
}

bool JSHTMLBaseFontElement::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSHTMLBaseFontElement* thisObject = jsCast<JSHTMLBaseFontElement*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSHTMLBaseFontElement, Base>(exec, &JSHTMLBaseFontElementTable, thisObject, propertyName, slot);
}

bool JSHTMLBaseFontElement::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSHTMLBaseFontElement* thisObject = jsCast<JSHTMLBaseFontElement*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSHTMLBaseFontElement, Base>(exec, &JSHTMLBaseFontElementTable, thisObject, propertyName, descriptor);
}

JSValue jsHTMLBaseFontElementColor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLBaseFontElement* castedThis = jsCast<JSHTMLBaseFontElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLBaseFontElement* impl = static_cast<HTMLBaseFontElement*>(castedThis->impl());
    JSValue result = jsStringWithCache(exec, impl->getAttribute(WebCore::HTMLNames::colorAttr));
    return result;
}


JSValue jsHTMLBaseFontElementFace(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLBaseFontElement* castedThis = jsCast<JSHTMLBaseFontElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLBaseFontElement* impl = static_cast<HTMLBaseFontElement*>(castedThis->impl());
    JSValue result = jsStringWithCache(exec, impl->getAttribute(WebCore::HTMLNames::faceAttr));
    return result;
}


JSValue jsHTMLBaseFontElementSize(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLBaseFontElement* castedThis = jsCast<JSHTMLBaseFontElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLBaseFontElement* impl = static_cast<HTMLBaseFontElement*>(castedThis->impl());
    JSValue result = jsNumber(impl->getIntegralAttribute(WebCore::HTMLNames::sizeAttr));
    return result;
}


JSValue jsHTMLBaseFontElementConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLBaseFontElement* domObject = jsCast<JSHTMLBaseFontElement*>(asObject(slotBase));
    return JSHTMLBaseFontElement::getConstructor(exec, domObject->globalObject());
}

void JSHTMLBaseFontElement::put(JSCell* cell, ExecState* exec, PropertyName propertyName, JSValue value, PutPropertySlot& slot)
{
    JSHTMLBaseFontElement* thisObject = jsCast<JSHTMLBaseFontElement*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    lookupPut<JSHTMLBaseFontElement, Base>(exec, propertyName, value, &JSHTMLBaseFontElementTable, thisObject, slot);
}

void setJSHTMLBaseFontElementColor(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSHTMLBaseFontElement* castedThis = jsCast<JSHTMLBaseFontElement*>(thisObject);
    HTMLBaseFontElement* impl = static_cast<HTMLBaseFontElement*>(castedThis->impl());
    impl->setAttribute(WebCore::HTMLNames::colorAttr, valueToStringWithNullCheck(exec, value));
}


void setJSHTMLBaseFontElementFace(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSHTMLBaseFontElement* castedThis = jsCast<JSHTMLBaseFontElement*>(thisObject);
    HTMLBaseFontElement* impl = static_cast<HTMLBaseFontElement*>(castedThis->impl());
    impl->setAttribute(WebCore::HTMLNames::faceAttr, valueToStringWithNullCheck(exec, value));
}


void setJSHTMLBaseFontElementSize(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSHTMLBaseFontElement* castedThis = jsCast<JSHTMLBaseFontElement*>(thisObject);
    HTMLBaseFontElement* impl = static_cast<HTMLBaseFontElement*>(castedThis->impl());
    impl->setIntegralAttribute(WebCore::HTMLNames::sizeAttr, value.toInt32(exec));
}


JSValue JSHTMLBaseFontElement::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSHTMLBaseFontElementConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}


}
