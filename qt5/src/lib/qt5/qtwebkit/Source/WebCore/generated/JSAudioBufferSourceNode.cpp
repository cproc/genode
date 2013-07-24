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

#include "JSAudioBufferSourceNode.h"

#include "AudioBuffer.h"
#include "AudioBufferSourceNode.h"
#include "AudioGain.h"
#include "AudioParam.h"
#include "ExceptionCode.h"
#include "JSAudioBuffer.h"
#include "JSAudioGain.h"
#include "JSAudioParam.h"
#include "JSDOMBinding.h"
#include <runtime/Error.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSAudioBufferSourceNodeTableValues[] =
{
    { "buffer", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsAudioBufferSourceNodeBuffer), (intptr_t)setJSAudioBufferSourceNodeBuffer, NoIntrinsic },
    { "playbackState", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsAudioBufferSourceNodePlaybackState), (intptr_t)0, NoIntrinsic },
    { "gain", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsAudioBufferSourceNodeGain), (intptr_t)0, NoIntrinsic },
    { "playbackRate", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsAudioBufferSourceNodePlaybackRate), (intptr_t)0, NoIntrinsic },
    { "loop", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsAudioBufferSourceNodeLoop), (intptr_t)setJSAudioBufferSourceNodeLoop, NoIntrinsic },
    { "loopStart", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsAudioBufferSourceNodeLoopStart), (intptr_t)setJSAudioBufferSourceNodeLoopStart, NoIntrinsic },
    { "loopEnd", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsAudioBufferSourceNodeLoopEnd), (intptr_t)setJSAudioBufferSourceNodeLoopEnd, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsAudioBufferSourceNodeConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSAudioBufferSourceNodeTable = { 18, 15, JSAudioBufferSourceNodeTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSAudioBufferSourceNodeConstructorTableValues[] =
{
    { "UNSCHEDULED_STATE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsAudioBufferSourceNodeUNSCHEDULED_STATE), (intptr_t)0, NoIntrinsic },
    { "SCHEDULED_STATE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsAudioBufferSourceNodeSCHEDULED_STATE), (intptr_t)0, NoIntrinsic },
    { "PLAYING_STATE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsAudioBufferSourceNodePLAYING_STATE), (intptr_t)0, NoIntrinsic },
    { "FINISHED_STATE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsAudioBufferSourceNodeFINISHED_STATE), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSAudioBufferSourceNodeConstructorTable = { 10, 7, JSAudioBufferSourceNodeConstructorTableValues, 0 };

COMPILE_ASSERT(0 == AudioBufferSourceNode::UNSCHEDULED_STATE, AudioBufferSourceNodeEnumUNSCHEDULED_STATEIsWrongUseDoNotCheckConstants);
COMPILE_ASSERT(1 == AudioBufferSourceNode::SCHEDULED_STATE, AudioBufferSourceNodeEnumSCHEDULED_STATEIsWrongUseDoNotCheckConstants);
COMPILE_ASSERT(2 == AudioBufferSourceNode::PLAYING_STATE, AudioBufferSourceNodeEnumPLAYING_STATEIsWrongUseDoNotCheckConstants);
COMPILE_ASSERT(3 == AudioBufferSourceNode::FINISHED_STATE, AudioBufferSourceNodeEnumFINISHED_STATEIsWrongUseDoNotCheckConstants);

const ClassInfo JSAudioBufferSourceNodeConstructor::s_info = { "AudioBufferSourceNodeConstructor", &Base::s_info, &JSAudioBufferSourceNodeConstructorTable, 0, CREATE_METHOD_TABLE(JSAudioBufferSourceNodeConstructor) };

JSAudioBufferSourceNodeConstructor::JSAudioBufferSourceNodeConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSAudioBufferSourceNodeConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSAudioBufferSourceNodePrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSAudioBufferSourceNodeConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSAudioBufferSourceNodeConstructor, JSDOMWrapper>(exec, &JSAudioBufferSourceNodeConstructorTable, jsCast<JSAudioBufferSourceNodeConstructor*>(cell), propertyName, slot);
}

bool JSAudioBufferSourceNodeConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSAudioBufferSourceNodeConstructor, JSDOMWrapper>(exec, &JSAudioBufferSourceNodeConstructorTable, jsCast<JSAudioBufferSourceNodeConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSAudioBufferSourceNodePrototypeTableValues[] =
{
    { "UNSCHEDULED_STATE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsAudioBufferSourceNodeUNSCHEDULED_STATE), (intptr_t)0, NoIntrinsic },
    { "SCHEDULED_STATE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsAudioBufferSourceNodeSCHEDULED_STATE), (intptr_t)0, NoIntrinsic },
    { "PLAYING_STATE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsAudioBufferSourceNodePLAYING_STATE), (intptr_t)0, NoIntrinsic },
    { "FINISHED_STATE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsAudioBufferSourceNodeFINISHED_STATE), (intptr_t)0, NoIntrinsic },
    { "start", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsAudioBufferSourceNodePrototypeFunctionStart), (intptr_t)1, NoIntrinsic },
    { "stop", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsAudioBufferSourceNodePrototypeFunctionStop), (intptr_t)1, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSAudioBufferSourceNodePrototypeTable = { 18, 15, JSAudioBufferSourceNodePrototypeTableValues, 0 };
const ClassInfo JSAudioBufferSourceNodePrototype::s_info = { "AudioBufferSourceNodePrototype", &Base::s_info, &JSAudioBufferSourceNodePrototypeTable, 0, CREATE_METHOD_TABLE(JSAudioBufferSourceNodePrototype) };

JSObject* JSAudioBufferSourceNodePrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSAudioBufferSourceNode>(exec, globalObject);
}

bool JSAudioBufferSourceNodePrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSAudioBufferSourceNodePrototype* thisObject = jsCast<JSAudioBufferSourceNodePrototype*>(cell);
    return getStaticPropertySlot<JSAudioBufferSourceNodePrototype, JSObject>(exec, &JSAudioBufferSourceNodePrototypeTable, thisObject, propertyName, slot);
}

bool JSAudioBufferSourceNodePrototype::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSAudioBufferSourceNodePrototype* thisObject = jsCast<JSAudioBufferSourceNodePrototype*>(object);
    return getStaticPropertyDescriptor<JSAudioBufferSourceNodePrototype, JSObject>(exec, &JSAudioBufferSourceNodePrototypeTable, thisObject, propertyName, descriptor);
}

const ClassInfo JSAudioBufferSourceNode::s_info = { "AudioBufferSourceNode", &Base::s_info, &JSAudioBufferSourceNodeTable, 0 , CREATE_METHOD_TABLE(JSAudioBufferSourceNode) };

JSAudioBufferSourceNode::JSAudioBufferSourceNode(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<AudioBufferSourceNode> impl)
    : JSAudioSourceNode(structure, globalObject, impl)
{
}

void JSAudioBufferSourceNode::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSAudioBufferSourceNode::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSAudioBufferSourceNodePrototype::create(exec->globalData(), globalObject, JSAudioBufferSourceNodePrototype::createStructure(exec->globalData(), globalObject, JSAudioSourceNodePrototype::self(exec, globalObject)));
}

bool JSAudioBufferSourceNode::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSAudioBufferSourceNode* thisObject = jsCast<JSAudioBufferSourceNode*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSAudioBufferSourceNode, Base>(exec, &JSAudioBufferSourceNodeTable, thisObject, propertyName, slot);
}

bool JSAudioBufferSourceNode::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSAudioBufferSourceNode* thisObject = jsCast<JSAudioBufferSourceNode*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSAudioBufferSourceNode, Base>(exec, &JSAudioBufferSourceNodeTable, thisObject, propertyName, descriptor);
}

JSValue jsAudioBufferSourceNodeBuffer(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSAudioBufferSourceNode* castedThis = jsCast<JSAudioBufferSourceNode*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    AudioBufferSourceNode* impl = static_cast<AudioBufferSourceNode*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->buffer()));
    return result;
}


JSValue jsAudioBufferSourceNodePlaybackState(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSAudioBufferSourceNode* castedThis = jsCast<JSAudioBufferSourceNode*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    AudioBufferSourceNode* impl = static_cast<AudioBufferSourceNode*>(castedThis->impl());
    JSValue result = jsNumber(impl->playbackState());
    return result;
}


JSValue jsAudioBufferSourceNodeGain(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSAudioBufferSourceNode* castedThis = jsCast<JSAudioBufferSourceNode*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    AudioBufferSourceNode* impl = static_cast<AudioBufferSourceNode*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->gain()));
    return result;
}


JSValue jsAudioBufferSourceNodePlaybackRate(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSAudioBufferSourceNode* castedThis = jsCast<JSAudioBufferSourceNode*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    AudioBufferSourceNode* impl = static_cast<AudioBufferSourceNode*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->playbackRate()));
    return result;
}


JSValue jsAudioBufferSourceNodeLoop(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSAudioBufferSourceNode* castedThis = jsCast<JSAudioBufferSourceNode*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    AudioBufferSourceNode* impl = static_cast<AudioBufferSourceNode*>(castedThis->impl());
    JSValue result = jsBoolean(impl->loop());
    return result;
}


JSValue jsAudioBufferSourceNodeLoopStart(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSAudioBufferSourceNode* castedThis = jsCast<JSAudioBufferSourceNode*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    AudioBufferSourceNode* impl = static_cast<AudioBufferSourceNode*>(castedThis->impl());
    JSValue result = jsNumber(impl->loopStart());
    return result;
}


JSValue jsAudioBufferSourceNodeLoopEnd(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSAudioBufferSourceNode* castedThis = jsCast<JSAudioBufferSourceNode*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    AudioBufferSourceNode* impl = static_cast<AudioBufferSourceNode*>(castedThis->impl());
    JSValue result = jsNumber(impl->loopEnd());
    return result;
}


JSValue jsAudioBufferSourceNodeConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSAudioBufferSourceNode* domObject = jsCast<JSAudioBufferSourceNode*>(asObject(slotBase));
    return JSAudioBufferSourceNode::getConstructor(exec, domObject->globalObject());
}

void JSAudioBufferSourceNode::put(JSCell* cell, ExecState* exec, PropertyName propertyName, JSValue value, PutPropertySlot& slot)
{
    JSAudioBufferSourceNode* thisObject = jsCast<JSAudioBufferSourceNode*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    lookupPut<JSAudioBufferSourceNode, Base>(exec, propertyName, value, &JSAudioBufferSourceNodeTable, thisObject, slot);
}

void setJSAudioBufferSourceNodeBuffer(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    jsCast<JSAudioBufferSourceNode*>(thisObject)->setBuffer(exec, value);
}


void setJSAudioBufferSourceNodeLoop(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSAudioBufferSourceNode* castedThis = jsCast<JSAudioBufferSourceNode*>(thisObject);
    AudioBufferSourceNode* impl = static_cast<AudioBufferSourceNode*>(castedThis->impl());
    impl->setLoop(value.toBoolean(exec));
}


void setJSAudioBufferSourceNodeLoopStart(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSAudioBufferSourceNode* castedThis = jsCast<JSAudioBufferSourceNode*>(thisObject);
    AudioBufferSourceNode* impl = static_cast<AudioBufferSourceNode*>(castedThis->impl());
    impl->setLoopStart(value.toNumber(exec));
}


void setJSAudioBufferSourceNodeLoopEnd(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSAudioBufferSourceNode* castedThis = jsCast<JSAudioBufferSourceNode*>(thisObject);
    AudioBufferSourceNode* impl = static_cast<AudioBufferSourceNode*>(castedThis->impl());
    impl->setLoopEnd(value.toNumber(exec));
}


JSValue JSAudioBufferSourceNode::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSAudioBufferSourceNodeConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}

static EncodedJSValue JSC_HOST_CALL jsAudioBufferSourceNodePrototypeFunctionStart1(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSAudioBufferSourceNode::s_info))
        return throwVMTypeError(exec);
    JSAudioBufferSourceNode* castedThis = jsCast<JSAudioBufferSourceNode*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSAudioBufferSourceNode::s_info);
    AudioBufferSourceNode* impl = static_cast<AudioBufferSourceNode*>(castedThis->impl());
    if (exec->argumentCount() < 1)
        return throwVMError(exec, createNotEnoughArgumentsError(exec));
    double when(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).toNumber(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    impl->start(when);
    return JSValue::encode(jsUndefined());
}

static EncodedJSValue JSC_HOST_CALL jsAudioBufferSourceNodePrototypeFunctionStart2(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSAudioBufferSourceNode::s_info))
        return throwVMTypeError(exec);
    JSAudioBufferSourceNode* castedThis = jsCast<JSAudioBufferSourceNode*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSAudioBufferSourceNode::s_info);
    AudioBufferSourceNode* impl = static_cast<AudioBufferSourceNode*>(castedThis->impl());
    if (exec->argumentCount() < 2)
        return throwVMError(exec, createNotEnoughArgumentsError(exec));
    double when(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).toNumber(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    double grainOffset(MAYBE_MISSING_PARAMETER(exec, 1, DefaultIsUndefined).toNumber(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    impl->startGrain(when, grainOffset);
    return JSValue::encode(jsUndefined());
}

static EncodedJSValue JSC_HOST_CALL jsAudioBufferSourceNodePrototypeFunctionStart3(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSAudioBufferSourceNode::s_info))
        return throwVMTypeError(exec);
    JSAudioBufferSourceNode* castedThis = jsCast<JSAudioBufferSourceNode*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSAudioBufferSourceNode::s_info);
    AudioBufferSourceNode* impl = static_cast<AudioBufferSourceNode*>(castedThis->impl());
    if (exec->argumentCount() < 3)
        return throwVMError(exec, createNotEnoughArgumentsError(exec));
    double when(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).toNumber(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    double grainOffset(MAYBE_MISSING_PARAMETER(exec, 1, DefaultIsUndefined).toNumber(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    double grainDuration(MAYBE_MISSING_PARAMETER(exec, 2, DefaultIsUndefined).toNumber(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    impl->startGrain(when, grainOffset, grainDuration);
    return JSValue::encode(jsUndefined());
}

EncodedJSValue JSC_HOST_CALL jsAudioBufferSourceNodePrototypeFunctionStart(ExecState* exec)
{
    size_t argsCount = exec->argumentCount();
    if (argsCount == 1)
        return jsAudioBufferSourceNodePrototypeFunctionStart1(exec);
    if (argsCount == 2)
        return jsAudioBufferSourceNodePrototypeFunctionStart2(exec);
    if (argsCount == 3)
        return jsAudioBufferSourceNodePrototypeFunctionStart3(exec);
    if (argsCount < 1)
        return throwVMError(exec, createNotEnoughArgumentsError(exec));
    return throwVMTypeError(exec);
}

EncodedJSValue JSC_HOST_CALL jsAudioBufferSourceNodePrototypeFunctionStop(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSAudioBufferSourceNode::s_info))
        return throwVMTypeError(exec);
    JSAudioBufferSourceNode* castedThis = jsCast<JSAudioBufferSourceNode*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSAudioBufferSourceNode::s_info);
    AudioBufferSourceNode* impl = static_cast<AudioBufferSourceNode*>(castedThis->impl());
    if (exec->argumentCount() < 1)
        return throwVMError(exec, createNotEnoughArgumentsError(exec));
    double when(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).toNumber(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    impl->stop(when);
    return JSValue::encode(jsUndefined());
}

// Constant getters

JSValue jsAudioBufferSourceNodeUNSCHEDULED_STATE(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(0));
}

JSValue jsAudioBufferSourceNodeSCHEDULED_STATE(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(1));
}

JSValue jsAudioBufferSourceNodePLAYING_STATE(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(2));
}

JSValue jsAudioBufferSourceNodeFINISHED_STATE(ExecState* exec, JSValue, PropertyName)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(3));
}

JSC::JSValue toJS(JSC::ExecState* exec, JSDOMGlobalObject* globalObject, AudioBufferSourceNode* impl)
{
    return wrap<JSAudioBufferSourceNode>(exec, globalObject, impl);
}


}

#endif // ENABLE(WEB_AUDIO)
