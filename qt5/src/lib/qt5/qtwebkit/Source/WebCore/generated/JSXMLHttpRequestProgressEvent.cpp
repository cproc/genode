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
#include "JSXMLHttpRequestProgressEvent.h"

#include "XMLHttpRequestProgressEvent.h"
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSXMLHttpRequestProgressEventTableValues[] =
{
    { "position", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsXMLHttpRequestProgressEventPosition), (intptr_t)0, NoIntrinsic },
    { "totalSize", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsXMLHttpRequestProgressEventTotalSize), (intptr_t)0, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsXMLHttpRequestProgressEventConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSXMLHttpRequestProgressEventTable = { 9, 7, JSXMLHttpRequestProgressEventTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSXMLHttpRequestProgressEventConstructorTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSXMLHttpRequestProgressEventConstructorTable = { 1, 0, JSXMLHttpRequestProgressEventConstructorTableValues, 0 };
static const HashTable* getJSXMLHttpRequestProgressEventConstructorTable(ExecState* exec)
{
    return getHashTableForGlobalData(exec->globalData(), &JSXMLHttpRequestProgressEventConstructorTable);
}

const ClassInfo JSXMLHttpRequestProgressEventConstructor::s_info = { "XMLHttpRequestProgressEventConstructor", &Base::s_info, 0, getJSXMLHttpRequestProgressEventConstructorTable, CREATE_METHOD_TABLE(JSXMLHttpRequestProgressEventConstructor) };

JSXMLHttpRequestProgressEventConstructor::JSXMLHttpRequestProgressEventConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSXMLHttpRequestProgressEventConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSXMLHttpRequestProgressEventPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSXMLHttpRequestProgressEventConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSXMLHttpRequestProgressEventConstructor, JSDOMWrapper>(exec, getJSXMLHttpRequestProgressEventConstructorTable(exec), jsCast<JSXMLHttpRequestProgressEventConstructor*>(cell), propertyName, slot);
}

bool JSXMLHttpRequestProgressEventConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSXMLHttpRequestProgressEventConstructor, JSDOMWrapper>(exec, getJSXMLHttpRequestProgressEventConstructorTable(exec), jsCast<JSXMLHttpRequestProgressEventConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSXMLHttpRequestProgressEventPrototypeTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSXMLHttpRequestProgressEventPrototypeTable = { 1, 0, JSXMLHttpRequestProgressEventPrototypeTableValues, 0 };
static const HashTable* getJSXMLHttpRequestProgressEventPrototypeTable(ExecState* exec)
{
    return getHashTableForGlobalData(exec->globalData(), &JSXMLHttpRequestProgressEventPrototypeTable);
}

const ClassInfo JSXMLHttpRequestProgressEventPrototype::s_info = { "XMLHttpRequestProgressEventPrototype", &Base::s_info, 0, getJSXMLHttpRequestProgressEventPrototypeTable, CREATE_METHOD_TABLE(JSXMLHttpRequestProgressEventPrototype) };

JSObject* JSXMLHttpRequestProgressEventPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSXMLHttpRequestProgressEvent>(exec, globalObject);
}

static const HashTable* getJSXMLHttpRequestProgressEventTable(ExecState* exec)
{
    return getHashTableForGlobalData(exec->globalData(), &JSXMLHttpRequestProgressEventTable);
}

const ClassInfo JSXMLHttpRequestProgressEvent::s_info = { "XMLHttpRequestProgressEvent", &Base::s_info, 0, getJSXMLHttpRequestProgressEventTable , CREATE_METHOD_TABLE(JSXMLHttpRequestProgressEvent) };

JSXMLHttpRequestProgressEvent::JSXMLHttpRequestProgressEvent(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<XMLHttpRequestProgressEvent> impl)
    : JSProgressEvent(structure, globalObject, impl)
{
}

void JSXMLHttpRequestProgressEvent::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSXMLHttpRequestProgressEvent::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSXMLHttpRequestProgressEventPrototype::create(exec->globalData(), globalObject, JSXMLHttpRequestProgressEventPrototype::createStructure(exec->globalData(), globalObject, JSProgressEventPrototype::self(exec, globalObject)));
}

bool JSXMLHttpRequestProgressEvent::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSXMLHttpRequestProgressEvent* thisObject = jsCast<JSXMLHttpRequestProgressEvent*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSXMLHttpRequestProgressEvent, Base>(exec, getJSXMLHttpRequestProgressEventTable(exec), thisObject, propertyName, slot);
}

bool JSXMLHttpRequestProgressEvent::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSXMLHttpRequestProgressEvent* thisObject = jsCast<JSXMLHttpRequestProgressEvent*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSXMLHttpRequestProgressEvent, Base>(exec, getJSXMLHttpRequestProgressEventTable(exec), thisObject, propertyName, descriptor);
}

JSValue jsXMLHttpRequestProgressEventPosition(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSXMLHttpRequestProgressEvent* castedThis = jsCast<JSXMLHttpRequestProgressEvent*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    XMLHttpRequestProgressEvent* impl = static_cast<XMLHttpRequestProgressEvent*>(castedThis->impl());
    JSValue result = jsNumber(impl->position());
    return result;
}


JSValue jsXMLHttpRequestProgressEventTotalSize(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSXMLHttpRequestProgressEvent* castedThis = jsCast<JSXMLHttpRequestProgressEvent*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    XMLHttpRequestProgressEvent* impl = static_cast<XMLHttpRequestProgressEvent*>(castedThis->impl());
    JSValue result = jsNumber(impl->totalSize());
    return result;
}


JSValue jsXMLHttpRequestProgressEventConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSXMLHttpRequestProgressEvent* domObject = jsCast<JSXMLHttpRequestProgressEvent*>(asObject(slotBase));
    return JSXMLHttpRequestProgressEvent::getConstructor(exec, domObject->globalObject());
}

JSValue JSXMLHttpRequestProgressEvent::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSXMLHttpRequestProgressEventConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}


}
