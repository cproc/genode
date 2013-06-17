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

#include "JSAudioNode.h"

#include "AudioContext.h"
#include "AudioNode.h"
#include "ExceptionCode.h"
#include "JSAudioContext.h"
#include "JSAudioNode.h"
#include "JSAudioParam.h"
#include "JSDOMBinding.h"
#include <runtime/Error.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSAudioNodeTableValues[] =
{
    { "context", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsAudioNodeContext), (intptr_t)0, NoIntrinsic },
    { "numberOfInputs", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsAudioNodeNumberOfInputs), (intptr_t)0, NoIntrinsic },
    { "numberOfOutputs", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsAudioNodeNumberOfOutputs), (intptr_t)0, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsAudioNodeConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSAudioNodeTable = { 8, 7, JSAudioNodeTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSAudioNodeConstructorTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSAudioNodeConstructorTable = { 1, 0, JSAudioNodeConstructorTableValues, 0 };
const ClassInfo JSAudioNodeConstructor::s_info = { "AudioNodeConstructor", &Base::s_info, &JSAudioNodeConstructorTable, 0, CREATE_METHOD_TABLE(JSAudioNodeConstructor) };

JSAudioNodeConstructor::JSAudioNodeConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSAudioNodeConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSAudioNodePrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSAudioNodeConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSAudioNodeConstructor, JSDOMWrapper>(exec, &JSAudioNodeConstructorTable, jsCast<JSAudioNodeConstructor*>(cell), propertyName, slot);
}

bool JSAudioNodeConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSAudioNodeConstructor, JSDOMWrapper>(exec, &JSAudioNodeConstructorTable, jsCast<JSAudioNodeConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSAudioNodePrototypeTableValues[] =
{
    { "connect", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsAudioNodePrototypeFunctionConnect), (intptr_t)3, NoIntrinsic },
    { "disconnect", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsAudioNodePrototypeFunctionDisconnect), (intptr_t)1, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSAudioNodePrototypeTable = { 8, 7, JSAudioNodePrototypeTableValues, 0 };
const ClassInfo JSAudioNodePrototype::s_info = { "AudioNodePrototype", &Base::s_info, &JSAudioNodePrototypeTable, 0, CREATE_METHOD_TABLE(JSAudioNodePrototype) };

JSObject* JSAudioNodePrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSAudioNode>(exec, globalObject);
}

bool JSAudioNodePrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSAudioNodePrototype* thisObject = jsCast<JSAudioNodePrototype*>(cell);
    return getStaticFunctionSlot<JSObject>(exec, &JSAudioNodePrototypeTable, thisObject, propertyName, slot);
}

bool JSAudioNodePrototype::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSAudioNodePrototype* thisObject = jsCast<JSAudioNodePrototype*>(object);
    return getStaticFunctionDescriptor<JSObject>(exec, &JSAudioNodePrototypeTable, thisObject, propertyName, descriptor);
}

const ClassInfo JSAudioNode::s_info = { "AudioNode", &Base::s_info, &JSAudioNodeTable, 0 , CREATE_METHOD_TABLE(JSAudioNode) };

JSAudioNode::JSAudioNode(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<AudioNode> impl)
    : JSDOMWrapper(structure, globalObject)
    , m_impl(impl.leakRef())
{
}

void JSAudioNode::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSAudioNode::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSAudioNodePrototype::create(exec->globalData(), globalObject, JSAudioNodePrototype::createStructure(globalObject->globalData(), globalObject, globalObject->objectPrototype()));
}

void JSAudioNode::destroy(JSC::JSCell* cell)
{
    JSAudioNode* thisObject = static_cast<JSAudioNode*>(cell);
    thisObject->JSAudioNode::~JSAudioNode();
}

JSAudioNode::~JSAudioNode()
{
    releaseImplIfNotNull();
}

bool JSAudioNode::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSAudioNode* thisObject = jsCast<JSAudioNode*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSAudioNode, Base>(exec, &JSAudioNodeTable, thisObject, propertyName, slot);
}

bool JSAudioNode::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSAudioNode* thisObject = jsCast<JSAudioNode*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSAudioNode, Base>(exec, &JSAudioNodeTable, thisObject, propertyName, descriptor);
}

JSValue jsAudioNodeContext(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSAudioNode* castedThis = jsCast<JSAudioNode*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    AudioNode* impl = static_cast<AudioNode*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->context()));
    return result;
}


JSValue jsAudioNodeNumberOfInputs(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSAudioNode* castedThis = jsCast<JSAudioNode*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    AudioNode* impl = static_cast<AudioNode*>(castedThis->impl());
    JSValue result = jsNumber(impl->numberOfInputs());
    return result;
}


JSValue jsAudioNodeNumberOfOutputs(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSAudioNode* castedThis = jsCast<JSAudioNode*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    AudioNode* impl = static_cast<AudioNode*>(castedThis->impl());
    JSValue result = jsNumber(impl->numberOfOutputs());
    return result;
}


JSValue jsAudioNodeConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSAudioNode* domObject = jsCast<JSAudioNode*>(asObject(slotBase));
    return JSAudioNode::getConstructor(exec, domObject->globalObject());
}

JSValue JSAudioNode::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSAudioNodeConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}

static EncodedJSValue JSC_HOST_CALL jsAudioNodePrototypeFunctionConnect1(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSAudioNode::s_info))
        return throwVMTypeError(exec);
    JSAudioNode* castedThis = jsCast<JSAudioNode*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSAudioNode::s_info);
    AudioNode* impl = static_cast<AudioNode*>(castedThis->impl());
    if (exec->argumentCount() < 1)
        return throwVMError(exec, createNotEnoughArgumentsError(exec));
    ExceptionCode ec = 0;
    AudioNode* destination(toAudioNode(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    unsigned output(MAYBE_MISSING_PARAMETER(exec, 1, DefaultIsUndefined).toUInt32(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    unsigned input(MAYBE_MISSING_PARAMETER(exec, 2, DefaultIsUndefined).toUInt32(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    impl->connect(destination, output, input, ec);
    setDOMException(exec, ec);
    return JSValue::encode(jsUndefined());
}

static EncodedJSValue JSC_HOST_CALL jsAudioNodePrototypeFunctionConnect2(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSAudioNode::s_info))
        return throwVMTypeError(exec);
    JSAudioNode* castedThis = jsCast<JSAudioNode*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSAudioNode::s_info);
    AudioNode* impl = static_cast<AudioNode*>(castedThis->impl());
    if (exec->argumentCount() < 1)
        return throwVMError(exec, createNotEnoughArgumentsError(exec));
    ExceptionCode ec = 0;
    AudioParam* destination(toAudioParam(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    unsigned output(MAYBE_MISSING_PARAMETER(exec, 1, DefaultIsUndefined).toUInt32(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    impl->connect(destination, output, ec);
    setDOMException(exec, ec);
    return JSValue::encode(jsUndefined());
}

EncodedJSValue JSC_HOST_CALL jsAudioNodePrototypeFunctionConnect(ExecState* exec)
{
    size_t argsCount = exec->argumentCount();
    JSValue arg0(exec->argument(0));
    if ((argsCount == 1 && (arg0.isNull() || (arg0.isObject() && asObject(arg0)->inherits(&JSAudioNode::s_info)))) || (argsCount == 2 && (arg0.isNull() || (arg0.isObject() && asObject(arg0)->inherits(&JSAudioNode::s_info)))) || (argsCount == 3 && (arg0.isNull() || (arg0.isObject() && asObject(arg0)->inherits(&JSAudioNode::s_info)))))
        return jsAudioNodePrototypeFunctionConnect1(exec);
    if ((argsCount == 1 && (arg0.isNull() || (arg0.isObject() && asObject(arg0)->inherits(&JSAudioParam::s_info)))) || (argsCount == 2 && (arg0.isNull() || (arg0.isObject() && asObject(arg0)->inherits(&JSAudioParam::s_info)))))
        return jsAudioNodePrototypeFunctionConnect2(exec);
    if (argsCount < 1)
        return throwVMError(exec, createNotEnoughArgumentsError(exec));
    return throwVMTypeError(exec);
}

EncodedJSValue JSC_HOST_CALL jsAudioNodePrototypeFunctionDisconnect(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSAudioNode::s_info))
        return throwVMTypeError(exec);
    JSAudioNode* castedThis = jsCast<JSAudioNode*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSAudioNode::s_info);
    AudioNode* impl = static_cast<AudioNode*>(castedThis->impl());
    ExceptionCode ec = 0;
    unsigned output(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).toUInt32(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    impl->disconnect(output, ec);
    setDOMException(exec, ec);
    return JSValue::encode(jsUndefined());
}

static inline bool isObservable(JSAudioNode* jsAudioNode)
{
    if (jsAudioNode->hasCustomProperties())
        return true;
    return false;
}

bool JSAudioNodeOwner::isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown> handle, void*, SlotVisitor& visitor)
{
    JSAudioNode* jsAudioNode = jsCast<JSAudioNode*>(handle.get().asCell());
    if (!isObservable(jsAudioNode))
        return false;
    UNUSED_PARAM(visitor);
    return false;
}

void JSAudioNodeOwner::finalize(JSC::Handle<JSC::Unknown> handle, void* context)
{
    JSAudioNode* jsAudioNode = jsCast<JSAudioNode*>(handle.get().asCell());
    DOMWrapperWorld* world = static_cast<DOMWrapperWorld*>(context);
    uncacheWrapper(world, jsAudioNode->impl(), jsAudioNode);
    jsAudioNode->releaseImpl();
}

JSC::JSValue toJS(JSC::ExecState* exec, JSDOMGlobalObject* globalObject, AudioNode* impl)
{
    return wrap<JSAudioNode>(exec, globalObject, impl);
}

AudioNode* toAudioNode(JSC::JSValue value)
{
    return value.inherits(&JSAudioNode::s_info) ? jsCast<JSAudioNode*>(asObject(value))->impl() : 0;
}

}

#endif // ENABLE(WEB_AUDIO)
