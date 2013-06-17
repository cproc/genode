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

#include "JSAudioProcessingEvent.h"

#include "AudioBuffer.h"
#include "AudioProcessingEvent.h"
#include "JSAudioBuffer.h"
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSAudioProcessingEventTableValues[] =
{
    { "inputBuffer", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsAudioProcessingEventInputBuffer), (intptr_t)0, NoIntrinsic },
    { "outputBuffer", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsAudioProcessingEventOutputBuffer), (intptr_t)0, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsAudioProcessingEventConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSAudioProcessingEventTable = { 9, 7, JSAudioProcessingEventTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSAudioProcessingEventConstructorTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSAudioProcessingEventConstructorTable = { 1, 0, JSAudioProcessingEventConstructorTableValues, 0 };
const ClassInfo JSAudioProcessingEventConstructor::s_info = { "AudioProcessingEventConstructor", &Base::s_info, &JSAudioProcessingEventConstructorTable, 0, CREATE_METHOD_TABLE(JSAudioProcessingEventConstructor) };

JSAudioProcessingEventConstructor::JSAudioProcessingEventConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSAudioProcessingEventConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSAudioProcessingEventPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSAudioProcessingEventConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSAudioProcessingEventConstructor, JSDOMWrapper>(exec, &JSAudioProcessingEventConstructorTable, jsCast<JSAudioProcessingEventConstructor*>(cell), propertyName, slot);
}

bool JSAudioProcessingEventConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSAudioProcessingEventConstructor, JSDOMWrapper>(exec, &JSAudioProcessingEventConstructorTable, jsCast<JSAudioProcessingEventConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSAudioProcessingEventPrototypeTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSAudioProcessingEventPrototypeTable = { 1, 0, JSAudioProcessingEventPrototypeTableValues, 0 };
const ClassInfo JSAudioProcessingEventPrototype::s_info = { "AudioProcessingEventPrototype", &Base::s_info, &JSAudioProcessingEventPrototypeTable, 0, CREATE_METHOD_TABLE(JSAudioProcessingEventPrototype) };

JSObject* JSAudioProcessingEventPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSAudioProcessingEvent>(exec, globalObject);
}

const ClassInfo JSAudioProcessingEvent::s_info = { "AudioProcessingEvent", &Base::s_info, &JSAudioProcessingEventTable, 0 , CREATE_METHOD_TABLE(JSAudioProcessingEvent) };

JSAudioProcessingEvent::JSAudioProcessingEvent(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<AudioProcessingEvent> impl)
    : JSEvent(structure, globalObject, impl)
{
}

void JSAudioProcessingEvent::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSAudioProcessingEvent::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSAudioProcessingEventPrototype::create(exec->globalData(), globalObject, JSAudioProcessingEventPrototype::createStructure(exec->globalData(), globalObject, JSEventPrototype::self(exec, globalObject)));
}

bool JSAudioProcessingEvent::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSAudioProcessingEvent* thisObject = jsCast<JSAudioProcessingEvent*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSAudioProcessingEvent, Base>(exec, &JSAudioProcessingEventTable, thisObject, propertyName, slot);
}

bool JSAudioProcessingEvent::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSAudioProcessingEvent* thisObject = jsCast<JSAudioProcessingEvent*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSAudioProcessingEvent, Base>(exec, &JSAudioProcessingEventTable, thisObject, propertyName, descriptor);
}

JSValue jsAudioProcessingEventInputBuffer(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSAudioProcessingEvent* castedThis = jsCast<JSAudioProcessingEvent*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    AudioProcessingEvent* impl = static_cast<AudioProcessingEvent*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->inputBuffer()));
    return result;
}


JSValue jsAudioProcessingEventOutputBuffer(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSAudioProcessingEvent* castedThis = jsCast<JSAudioProcessingEvent*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    AudioProcessingEvent* impl = static_cast<AudioProcessingEvent*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->outputBuffer()));
    return result;
}


JSValue jsAudioProcessingEventConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSAudioProcessingEvent* domObject = jsCast<JSAudioProcessingEvent*>(asObject(slotBase));
    return JSAudioProcessingEvent::getConstructor(exec, domObject->globalObject());
}

JSValue JSAudioProcessingEvent::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSAudioProcessingEventConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}

JSC::JSValue toJS(JSC::ExecState* exec, JSDOMGlobalObject* globalObject, AudioProcessingEvent* impl)
{
    return wrap<JSAudioProcessingEvent>(exec, globalObject, impl);
}


}

#endif // ENABLE(WEB_AUDIO)
