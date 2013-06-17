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
#include "JSHTMLQuoteElement.h"

#include "HTMLNames.h"
#include "HTMLQuoteElement.h"
#include "KURL.h"
#include <runtime/JSString.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSHTMLQuoteElementTableValues[] =
{
    { "cite", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLQuoteElementCite), (intptr_t)setJSHTMLQuoteElementCite, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLQuoteElementConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSHTMLQuoteElementTable = { 5, 3, JSHTMLQuoteElementTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSHTMLQuoteElementConstructorTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSHTMLQuoteElementConstructorTable = { 1, 0, JSHTMLQuoteElementConstructorTableValues, 0 };
const ClassInfo JSHTMLQuoteElementConstructor::s_info = { "HTMLQuoteElementConstructor", &Base::s_info, &JSHTMLQuoteElementConstructorTable, 0, CREATE_METHOD_TABLE(JSHTMLQuoteElementConstructor) };

JSHTMLQuoteElementConstructor::JSHTMLQuoteElementConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSHTMLQuoteElementConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSHTMLQuoteElementPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSHTMLQuoteElementConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSHTMLQuoteElementConstructor, JSDOMWrapper>(exec, &JSHTMLQuoteElementConstructorTable, jsCast<JSHTMLQuoteElementConstructor*>(cell), propertyName, slot);
}

bool JSHTMLQuoteElementConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSHTMLQuoteElementConstructor, JSDOMWrapper>(exec, &JSHTMLQuoteElementConstructorTable, jsCast<JSHTMLQuoteElementConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSHTMLQuoteElementPrototypeTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSHTMLQuoteElementPrototypeTable = { 1, 0, JSHTMLQuoteElementPrototypeTableValues, 0 };
const ClassInfo JSHTMLQuoteElementPrototype::s_info = { "HTMLQuoteElementPrototype", &Base::s_info, &JSHTMLQuoteElementPrototypeTable, 0, CREATE_METHOD_TABLE(JSHTMLQuoteElementPrototype) };

JSObject* JSHTMLQuoteElementPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSHTMLQuoteElement>(exec, globalObject);
}

const ClassInfo JSHTMLQuoteElement::s_info = { "HTMLQuoteElement", &Base::s_info, &JSHTMLQuoteElementTable, 0 , CREATE_METHOD_TABLE(JSHTMLQuoteElement) };

JSHTMLQuoteElement::JSHTMLQuoteElement(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<HTMLQuoteElement> impl)
    : JSHTMLElement(structure, globalObject, impl)
{
}

void JSHTMLQuoteElement::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSHTMLQuoteElement::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSHTMLQuoteElementPrototype::create(exec->globalData(), globalObject, JSHTMLQuoteElementPrototype::createStructure(exec->globalData(), globalObject, JSHTMLElementPrototype::self(exec, globalObject)));
}

bool JSHTMLQuoteElement::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSHTMLQuoteElement* thisObject = jsCast<JSHTMLQuoteElement*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSHTMLQuoteElement, Base>(exec, &JSHTMLQuoteElementTable, thisObject, propertyName, slot);
}

bool JSHTMLQuoteElement::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSHTMLQuoteElement* thisObject = jsCast<JSHTMLQuoteElement*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSHTMLQuoteElement, Base>(exec, &JSHTMLQuoteElementTable, thisObject, propertyName, descriptor);
}

JSValue jsHTMLQuoteElementCite(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLQuoteElement* castedThis = jsCast<JSHTMLQuoteElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLQuoteElement* impl = static_cast<HTMLQuoteElement*>(castedThis->impl());
    JSValue result = jsStringWithCache(exec, impl->getURLAttribute(WebCore::HTMLNames::citeAttr));
    return result;
}


JSValue jsHTMLQuoteElementConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLQuoteElement* domObject = jsCast<JSHTMLQuoteElement*>(asObject(slotBase));
    return JSHTMLQuoteElement::getConstructor(exec, domObject->globalObject());
}

void JSHTMLQuoteElement::put(JSCell* cell, ExecState* exec, PropertyName propertyName, JSValue value, PutPropertySlot& slot)
{
    JSHTMLQuoteElement* thisObject = jsCast<JSHTMLQuoteElement*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    lookupPut<JSHTMLQuoteElement, Base>(exec, propertyName, value, &JSHTMLQuoteElementTable, thisObject, slot);
}

void setJSHTMLQuoteElementCite(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSHTMLQuoteElement* castedThis = jsCast<JSHTMLQuoteElement*>(thisObject);
    HTMLQuoteElement* impl = static_cast<HTMLQuoteElement*>(castedThis->impl());
    impl->setAttribute(WebCore::HTMLNames::citeAttr, valueToStringWithNullCheck(exec, value));
}


JSValue JSHTMLQuoteElement::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSHTMLQuoteElementConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}


}
