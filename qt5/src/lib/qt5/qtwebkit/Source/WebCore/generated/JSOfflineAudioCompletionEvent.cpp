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

#if ENABLE(WEB_AUDIO)

#include "JSOfflineAudioCompletionEvent.h"

#include "AudioBuffer.h"
#include "JSAudioBuffer.h"
#include "OfflineAudioCompletionEvent.h"
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSOfflineAudioCompletionEventTableValues[] =
{
    { "renderedBuffer", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsOfflineAudioCompletionEventRenderedBuffer), (intptr_t)0, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsOfflineAudioCompletionEventConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSOfflineAudioCompletionEventTable = { 4, 3, JSOfflineAudioCompletionEventTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSOfflineAudioCompletionEventConstructorTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSOfflineAudioCompletionEventConstructorTable = { 1, 0, JSOfflineAudioCompletionEventConstructorTableValues, 0 };
const ClassInfo JSOfflineAudioCompletionEventConstructor::s_info = { "OfflineAudioCompletionEventConstructor", &Base::s_info, &JSOfflineAudioCompletionEventConstructorTable, 0, CREATE_METHOD_TABLE(JSOfflineAudioCompletionEventConstructor) };

JSOfflineAudioCompletionEventConstructor::JSOfflineAudioCompletionEventConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSOfflineAudioCompletionEventConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSOfflineAudioCompletionEventPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSOfflineAudioCompletionEventConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSOfflineAudioCompletionEventConstructor, JSDOMWrapper>(exec, &JSOfflineAudioCompletionEventConstructorTable, jsCast<JSOfflineAudioCompletionEventConstructor*>(cell), propertyName, slot);
}

bool JSOfflineAudioCompletionEventConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSOfflineAudioCompletionEventConstructor, JSDOMWrapper>(exec, &JSOfflineAudioCompletionEventConstructorTable, jsCast<JSOfflineAudioCompletionEventConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSOfflineAudioCompletionEventPrototypeTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSOfflineAudioCompletionEventPrototypeTable = { 1, 0, JSOfflineAudioCompletionEventPrototypeTableValues, 0 };
const ClassInfo JSOfflineAudioCompletionEventPrototype::s_info = { "OfflineAudioCompletionEventPrototype", &Base::s_info, &JSOfflineAudioCompletionEventPrototypeTable, 0, CREATE_METHOD_TABLE(JSOfflineAudioCompletionEventPrototype) };

JSObject* JSOfflineAudioCompletionEventPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSOfflineAudioCompletionEvent>(exec, globalObject);
}

const ClassInfo JSOfflineAudioCompletionEvent::s_info = { "OfflineAudioCompletionEvent", &Base::s_info, &JSOfflineAudioCompletionEventTable, 0 , CREATE_METHOD_TABLE(JSOfflineAudioCompletionEvent) };

JSOfflineAudioCompletionEvent::JSOfflineAudioCompletionEvent(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<OfflineAudioCompletionEvent> impl)
    : JSEvent(structure, globalObject, impl)
{
}

void JSOfflineAudioCompletionEvent::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSOfflineAudioCompletionEvent::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSOfflineAudioCompletionEventPrototype::create(exec->globalData(), globalObject, JSOfflineAudioCompletionEventPrototype::createStructure(exec->globalData(), globalObject, JSEventPrototype::self(exec, globalObject)));
}

bool JSOfflineAudioCompletionEvent::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSOfflineAudioCompletionEvent* thisObject = jsCast<JSOfflineAudioCompletionEvent*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSOfflineAudioCompletionEvent, Base>(exec, &JSOfflineAudioCompletionEventTable, thisObject, propertyName, slot);
}

bool JSOfflineAudioCompletionEvent::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSOfflineAudioCompletionEvent* thisObject = jsCast<JSOfflineAudioCompletionEvent*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSOfflineAudioCompletionEvent, Base>(exec, &JSOfflineAudioCompletionEventTable, thisObject, propertyName, descriptor);
}

JSValue jsOfflineAudioCompletionEventRenderedBuffer(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSOfflineAudioCompletionEvent* castedThis = jsCast<JSOfflineAudioCompletionEvent*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    OfflineAudioCompletionEvent* impl = static_cast<OfflineAudioCompletionEvent*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->renderedBuffer()));
    return result;
}


JSValue jsOfflineAudioCompletionEventConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSOfflineAudioCompletionEvent* domObject = jsCast<JSOfflineAudioCompletionEvent*>(asObject(slotBase));
    return JSOfflineAudioCompletionEvent::getConstructor(exec, domObject->globalObject());
}

JSValue JSOfflineAudioCompletionEvent::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSOfflineAudioCompletionEventConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}

JSC::JSValue toJS(JSC::ExecState* exec, JSDOMGlobalObject* globalObject, OfflineAudioCompletionEvent* impl)
{
    return wrap<JSOfflineAudioCompletionEvent>(exec, globalObject, impl);
}


}

#endif // ENABLE(WEB_AUDIO)
