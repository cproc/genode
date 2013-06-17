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
#include "JSHTMLModElement.h"

#include "HTMLModElement.h"
#include "HTMLNames.h"
#include "KURL.h"
#include <runtime/JSString.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSHTMLModElementTableValues[] =
{
    { "cite", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLModElementCite), (intptr_t)setJSHTMLModElementCite, NoIntrinsic },
    { "dateTime", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLModElementDateTime), (intptr_t)setJSHTMLModElementDateTime, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLModElementConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSHTMLModElementTable = { 10, 7, JSHTMLModElementTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSHTMLModElementConstructorTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSHTMLModElementConstructorTable = { 1, 0, JSHTMLModElementConstructorTableValues, 0 };
const ClassInfo JSHTMLModElementConstructor::s_info = { "HTMLModElementConstructor", &Base::s_info, &JSHTMLModElementConstructorTable, 0, CREATE_METHOD_TABLE(JSHTMLModElementConstructor) };

JSHTMLModElementConstructor::JSHTMLModElementConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSHTMLModElementConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSHTMLModElementPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSHTMLModElementConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSHTMLModElementConstructor, JSDOMWrapper>(exec, &JSHTMLModElementConstructorTable, jsCast<JSHTMLModElementConstructor*>(cell), propertyName, slot);
}

bool JSHTMLModElementConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSHTMLModElementConstructor, JSDOMWrapper>(exec, &JSHTMLModElementConstructorTable, jsCast<JSHTMLModElementConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSHTMLModElementPrototypeTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSHTMLModElementPrototypeTable = { 1, 0, JSHTMLModElementPrototypeTableValues, 0 };
const ClassInfo JSHTMLModElementPrototype::s_info = { "HTMLModElementPrototype", &Base::s_info, &JSHTMLModElementPrototypeTable, 0, CREATE_METHOD_TABLE(JSHTMLModElementPrototype) };

JSObject* JSHTMLModElementPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSHTMLModElement>(exec, globalObject);
}

const ClassInfo JSHTMLModElement::s_info = { "HTMLModElement", &Base::s_info, &JSHTMLModElementTable, 0 , CREATE_METHOD_TABLE(JSHTMLModElement) };

JSHTMLModElement::JSHTMLModElement(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<HTMLModElement> impl)
    : JSHTMLElement(structure, globalObject, impl)
{
}

void JSHTMLModElement::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSHTMLModElement::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSHTMLModElementPrototype::create(exec->globalData(), globalObject, JSHTMLModElementPrototype::createStructure(exec->globalData(), globalObject, JSHTMLElementPrototype::self(exec, globalObject)));
}

bool JSHTMLModElement::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSHTMLModElement* thisObject = jsCast<JSHTMLModElement*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSHTMLModElement, Base>(exec, &JSHTMLModElementTable, thisObject, propertyName, slot);
}

bool JSHTMLModElement::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSHTMLModElement* thisObject = jsCast<JSHTMLModElement*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSHTMLModElement, Base>(exec, &JSHTMLModElementTable, thisObject, propertyName, descriptor);
}

JSValue jsHTMLModElementCite(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLModElement* castedThis = jsCast<JSHTMLModElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLModElement* impl = static_cast<HTMLModElement*>(castedThis->impl());
    JSValue result = jsStringWithCache(exec, impl->getURLAttribute(WebCore::HTMLNames::citeAttr));
    return result;
}


JSValue jsHTMLModElementDateTime(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLModElement* castedThis = jsCast<JSHTMLModElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLModElement* impl = static_cast<HTMLModElement*>(castedThis->impl());
    JSValue result = jsStringWithCache(exec, impl->getAttribute(WebCore::HTMLNames::datetimeAttr));
    return result;
}


JSValue jsHTMLModElementConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLModElement* domObject = jsCast<JSHTMLModElement*>(asObject(slotBase));
    return JSHTMLModElement::getConstructor(exec, domObject->globalObject());
}

void JSHTMLModElement::put(JSCell* cell, ExecState* exec, PropertyName propertyName, JSValue value, PutPropertySlot& slot)
{
    JSHTMLModElement* thisObject = jsCast<JSHTMLModElement*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    lookupPut<JSHTMLModElement, Base>(exec, propertyName, value, &JSHTMLModElementTable, thisObject, slot);
}

void setJSHTMLModElementCite(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSHTMLModElement* castedThis = jsCast<JSHTMLModElement*>(thisObject);
    HTMLModElement* impl = static_cast<HTMLModElement*>(castedThis->impl());
    impl->setAttribute(WebCore::HTMLNames::citeAttr, valueToStringWithNullCheck(exec, value));
}


void setJSHTMLModElementDateTime(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSHTMLModElement* castedThis = jsCast<JSHTMLModElement*>(thisObject);
    HTMLModElement* impl = static_cast<HTMLModElement*>(castedThis->impl());
    impl->setAttribute(WebCore::HTMLNames::datetimeAttr, valueToStringWithNullCheck(exec, value));
}


JSValue JSHTMLModElement::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSHTMLModElementConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}


}
