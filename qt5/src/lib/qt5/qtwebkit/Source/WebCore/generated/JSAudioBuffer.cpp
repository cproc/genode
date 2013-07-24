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

#include "JSAudioBuffer.h"

#include "AudioBuffer.h"
#include "ExceptionCode.h"
#include "JSDOMBinding.h"
#include "JSFloat32Array.h"
#include <runtime/Error.h>
#include <wtf/Float32Array.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSAudioBufferTableValues[] =
{
    { "length", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsAudioBufferLength), (intptr_t)0, NoIntrinsic },
    { "duration", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsAudioBufferDuration), (intptr_t)0, NoIntrinsic },
    { "sampleRate", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsAudioBufferSampleRate), (intptr_t)0, NoIntrinsic },
    { "gain", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsAudioBufferGain), (intptr_t)setJSAudioBufferGain, NoIntrinsic },
    { "numberOfChannels", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsAudioBufferNumberOfChannels), (intptr_t)0, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsAudioBufferConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSAudioBufferTable = { 16, 15, JSAudioBufferTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSAudioBufferConstructorTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSAudioBufferConstructorTable = { 1, 0, JSAudioBufferConstructorTableValues, 0 };
const ClassInfo JSAudioBufferConstructor::s_info = { "AudioBufferConstructor", &Base::s_info, &JSAudioBufferConstructorTable, 0, CREATE_METHOD_TABLE(JSAudioBufferConstructor) };

JSAudioBufferConstructor::JSAudioBufferConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSAudioBufferConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSAudioBufferPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSAudioBufferConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSAudioBufferConstructor, JSDOMWrapper>(exec, &JSAudioBufferConstructorTable, jsCast<JSAudioBufferConstructor*>(cell), propertyName, slot);
}

bool JSAudioBufferConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSAudioBufferConstructor, JSDOMWrapper>(exec, &JSAudioBufferConstructorTable, jsCast<JSAudioBufferConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSAudioBufferPrototypeTableValues[] =
{
    { "getChannelData", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsAudioBufferPrototypeFunctionGetChannelData), (intptr_t)1, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSAudioBufferPrototypeTable = { 2, 1, JSAudioBufferPrototypeTableValues, 0 };
const ClassInfo JSAudioBufferPrototype::s_info = { "AudioBufferPrototype", &Base::s_info, &JSAudioBufferPrototypeTable, 0, CREATE_METHOD_TABLE(JSAudioBufferPrototype) };

JSObject* JSAudioBufferPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSAudioBuffer>(exec, globalObject);
}

bool JSAudioBufferPrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSAudioBufferPrototype* thisObject = jsCast<JSAudioBufferPrototype*>(cell);
    return getStaticFunctionSlot<JSObject>(exec, &JSAudioBufferPrototypeTable, thisObject, propertyName, slot);
}

bool JSAudioBufferPrototype::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSAudioBufferPrototype* thisObject = jsCast<JSAudioBufferPrototype*>(object);
    return getStaticFunctionDescriptor<JSObject>(exec, &JSAudioBufferPrototypeTable, thisObject, propertyName, descriptor);
}

const ClassInfo JSAudioBuffer::s_info = { "AudioBuffer", &Base::s_info, &JSAudioBufferTable, 0 , CREATE_METHOD_TABLE(JSAudioBuffer) };

JSAudioBuffer::JSAudioBuffer(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<AudioBuffer> impl)
    : JSDOMWrapper(structure, globalObject)
    , m_impl(impl.leakRef())
{
}

void JSAudioBuffer::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSAudioBuffer::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSAudioBufferPrototype::create(exec->globalData(), globalObject, JSAudioBufferPrototype::createStructure(globalObject->globalData(), globalObject, globalObject->objectPrototype()));
}

void JSAudioBuffer::destroy(JSC::JSCell* cell)
{
    JSAudioBuffer* thisObject = static_cast<JSAudioBuffer*>(cell);
    thisObject->JSAudioBuffer::~JSAudioBuffer();
}

JSAudioBuffer::~JSAudioBuffer()
{
    releaseImplIfNotNull();
}

bool JSAudioBuffer::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSAudioBuffer* thisObject = jsCast<JSAudioBuffer*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSAudioBuffer, Base>(exec, &JSAudioBufferTable, thisObject, propertyName, slot);
}

bool JSAudioBuffer::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSAudioBuffer* thisObject = jsCast<JSAudioBuffer*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSAudioBuffer, Base>(exec, &JSAudioBufferTable, thisObject, propertyName, descriptor);
}

JSValue jsAudioBufferLength(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSAudioBuffer* castedThis = jsCast<JSAudioBuffer*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    AudioBuffer* impl = static_cast<AudioBuffer*>(castedThis->impl());
    JSValue result = jsNumber(impl->length());
    return result;
}


JSValue jsAudioBufferDuration(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSAudioBuffer* castedThis = jsCast<JSAudioBuffer*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    AudioBuffer* impl = static_cast<AudioBuffer*>(castedThis->impl());
    JSValue result = jsNumber(impl->duration());
    return result;
}


JSValue jsAudioBufferSampleRate(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSAudioBuffer* castedThis = jsCast<JSAudioBuffer*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    AudioBuffer* impl = static_cast<AudioBuffer*>(castedThis->impl());
    JSValue result = jsNumber(impl->sampleRate());
    return result;
}


JSValue jsAudioBufferGain(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSAudioBuffer* castedThis = jsCast<JSAudioBuffer*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    AudioBuffer* impl = static_cast<AudioBuffer*>(castedThis->impl());
    JSValue result = jsNumber(impl->gain());
    return result;
}


JSValue jsAudioBufferNumberOfChannels(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSAudioBuffer* castedThis = jsCast<JSAudioBuffer*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    AudioBuffer* impl = static_cast<AudioBuffer*>(castedThis->impl());
    JSValue result = jsNumber(impl->numberOfChannels());
    return result;
}


JSValue jsAudioBufferConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSAudioBuffer* domObject = jsCast<JSAudioBuffer*>(asObject(slotBase));
    return JSAudioBuffer::getConstructor(exec, domObject->globalObject());
}

void JSAudioBuffer::put(JSCell* cell, ExecState* exec, PropertyName propertyName, JSValue value, PutPropertySlot& slot)
{
    JSAudioBuffer* thisObject = jsCast<JSAudioBuffer*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    lookupPut<JSAudioBuffer, Base>(exec, propertyName, value, &JSAudioBufferTable, thisObject, slot);
}

void setJSAudioBufferGain(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSAudioBuffer* castedThis = jsCast<JSAudioBuffer*>(thisObject);
    AudioBuffer* impl = static_cast<AudioBuffer*>(castedThis->impl());
    impl->setGain(value.toFloat(exec));
}


JSValue JSAudioBuffer::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSAudioBufferConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}

EncodedJSValue JSC_HOST_CALL jsAudioBufferPrototypeFunctionGetChannelData(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSAudioBuffer::s_info))
        return throwVMTypeError(exec);
    JSAudioBuffer* castedThis = jsCast<JSAudioBuffer*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSAudioBuffer::s_info);
    AudioBuffer* impl = static_cast<AudioBuffer*>(castedThis->impl());
    if (exec->argumentCount() < 1)
        return throwVMError(exec, createNotEnoughArgumentsError(exec));
    ExceptionCode ec = 0;
    unsigned channelIndex(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).toUInt32(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    JSC::JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->getChannelData(channelIndex, ec)));
    setDOMException(exec, ec);
    return JSValue::encode(result);
}

static inline bool isObservable(JSAudioBuffer* jsAudioBuffer)
{
    if (jsAudioBuffer->hasCustomProperties())
        return true;
    return false;
}

bool JSAudioBufferOwner::isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown> handle, void*, SlotVisitor& visitor)
{
    JSAudioBuffer* jsAudioBuffer = jsCast<JSAudioBuffer*>(handle.get().asCell());
    if (!isObservable(jsAudioBuffer))
        return false;
    UNUSED_PARAM(visitor);
    return false;
}

void JSAudioBufferOwner::finalize(JSC::Handle<JSC::Unknown> handle, void* context)
{
    JSAudioBuffer* jsAudioBuffer = jsCast<JSAudioBuffer*>(handle.get().asCell());
    DOMWrapperWorld* world = static_cast<DOMWrapperWorld*>(context);
    uncacheWrapper(world, jsAudioBuffer->impl(), jsAudioBuffer);
    jsAudioBuffer->releaseImpl();
}

JSC::JSValue toJS(JSC::ExecState* exec, JSDOMGlobalObject* globalObject, AudioBuffer* impl)
{
    return wrap<JSAudioBuffer>(exec, globalObject, impl);
}

AudioBuffer* toAudioBuffer(JSC::JSValue value)
{
    return value.inherits(&JSAudioBuffer::s_info) ? jsCast<JSAudioBuffer*>(asObject(value))->impl() : 0;
}

}

#endif // ENABLE(WEB_AUDIO)
