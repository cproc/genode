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

#if ENABLE(PROGRESS_ELEMENT)

#include "JSHTMLProgressElement.h"

#include "HTMLProgressElement.h"
#include "JSNodeList.h"
#include "NameNodeList.h"
#include "NodeList.h"
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSHTMLProgressElementTableValues[] =
{
    { "value", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLProgressElementValue), (intptr_t)setJSHTMLProgressElementValue, NoIntrinsic },
    { "max", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLProgressElementMax), (intptr_t)setJSHTMLProgressElementMax, NoIntrinsic },
    { "position", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLProgressElementPosition), (intptr_t)0, NoIntrinsic },
    { "labels", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLProgressElementLabels), (intptr_t)0, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLProgressElementConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSHTMLProgressElementTable = { 16, 15, JSHTMLProgressElementTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSHTMLProgressElementConstructorTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSHTMLProgressElementConstructorTable = { 1, 0, JSHTMLProgressElementConstructorTableValues, 0 };
const ClassInfo JSHTMLProgressElementConstructor::s_info = { "HTMLProgressElementConstructor", &Base::s_info, &JSHTMLProgressElementConstructorTable, 0, CREATE_METHOD_TABLE(JSHTMLProgressElementConstructor) };

JSHTMLProgressElementConstructor::JSHTMLProgressElementConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSHTMLProgressElementConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSHTMLProgressElementPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSHTMLProgressElementConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSHTMLProgressElementConstructor, JSDOMWrapper>(exec, &JSHTMLProgressElementConstructorTable, jsCast<JSHTMLProgressElementConstructor*>(cell), propertyName, slot);
}

bool JSHTMLProgressElementConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSHTMLProgressElementConstructor, JSDOMWrapper>(exec, &JSHTMLProgressElementConstructorTable, jsCast<JSHTMLProgressElementConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSHTMLProgressElementPrototypeTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSHTMLProgressElementPrototypeTable = { 1, 0, JSHTMLProgressElementPrototypeTableValues, 0 };
const ClassInfo JSHTMLProgressElementPrototype::s_info = { "HTMLProgressElementPrototype", &Base::s_info, &JSHTMLProgressElementPrototypeTable, 0, CREATE_METHOD_TABLE(JSHTMLProgressElementPrototype) };

JSObject* JSHTMLProgressElementPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSHTMLProgressElement>(exec, globalObject);
}

const ClassInfo JSHTMLProgressElement::s_info = { "HTMLProgressElement", &Base::s_info, &JSHTMLProgressElementTable, 0 , CREATE_METHOD_TABLE(JSHTMLProgressElement) };

JSHTMLProgressElement::JSHTMLProgressElement(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<HTMLProgressElement> impl)
    : JSHTMLElement(structure, globalObject, impl)
{
}

void JSHTMLProgressElement::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSHTMLProgressElement::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSHTMLProgressElementPrototype::create(exec->globalData(), globalObject, JSHTMLProgressElementPrototype::createStructure(exec->globalData(), globalObject, JSHTMLElementPrototype::self(exec, globalObject)));
}

bool JSHTMLProgressElement::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSHTMLProgressElement* thisObject = jsCast<JSHTMLProgressElement*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSHTMLProgressElement, Base>(exec, &JSHTMLProgressElementTable, thisObject, propertyName, slot);
}

bool JSHTMLProgressElement::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSHTMLProgressElement* thisObject = jsCast<JSHTMLProgressElement*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSHTMLProgressElement, Base>(exec, &JSHTMLProgressElementTable, thisObject, propertyName, descriptor);
}

JSValue jsHTMLProgressElementValue(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLProgressElement* castedThis = jsCast<JSHTMLProgressElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLProgressElement* impl = static_cast<HTMLProgressElement*>(castedThis->impl());
    JSValue result = jsNumber(impl->value());
    return result;
}


JSValue jsHTMLProgressElementMax(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLProgressElement* castedThis = jsCast<JSHTMLProgressElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLProgressElement* impl = static_cast<HTMLProgressElement*>(castedThis->impl());
    JSValue result = jsNumber(impl->max());
    return result;
}


JSValue jsHTMLProgressElementPosition(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLProgressElement* castedThis = jsCast<JSHTMLProgressElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLProgressElement* impl = static_cast<HTMLProgressElement*>(castedThis->impl());
    JSValue result = jsNumber(impl->position());
    return result;
}


JSValue jsHTMLProgressElementLabels(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLProgressElement* castedThis = jsCast<JSHTMLProgressElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLProgressElement* impl = static_cast<HTMLProgressElement*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->labels()));
    return result;
}


JSValue jsHTMLProgressElementConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSHTMLProgressElement* domObject = jsCast<JSHTMLProgressElement*>(asObject(slotBase));
    return JSHTMLProgressElement::getConstructor(exec, domObject->globalObject());
}

void JSHTMLProgressElement::put(JSCell* cell, ExecState* exec, PropertyName propertyName, JSValue value, PutPropertySlot& slot)
{
    JSHTMLProgressElement* thisObject = jsCast<JSHTMLProgressElement*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    lookupPut<JSHTMLProgressElement, Base>(exec, propertyName, value, &JSHTMLProgressElementTable, thisObject, slot);
}

void setJSHTMLProgressElementValue(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSHTMLProgressElement* castedThis = jsCast<JSHTMLProgressElement*>(thisObject);
    HTMLProgressElement* impl = static_cast<HTMLProgressElement*>(castedThis->impl());
    ExceptionCode ec = 0;
    impl->setValue(value.toNumber(exec), ec);
    setDOMException(exec, ec);
}


void setJSHTMLProgressElementMax(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSHTMLProgressElement* castedThis = jsCast<JSHTMLProgressElement*>(thisObject);
    HTMLProgressElement* impl = static_cast<HTMLProgressElement*>(castedThis->impl());
    ExceptionCode ec = 0;
    impl->setMax(value.toNumber(exec), ec);
    setDOMException(exec, ec);
}


JSValue JSHTMLProgressElement::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSHTMLProgressElementConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}


}

#endif // ENABLE(PROGRESS_ELEMENT)
