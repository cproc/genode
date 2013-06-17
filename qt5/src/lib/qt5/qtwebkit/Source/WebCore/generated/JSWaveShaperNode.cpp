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

#include "JSWaveShaperNode.h"

#include "JSFloat32Array.h"
#include "WaveShaperNode.h"
#include <wtf/Float32Array.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

/* Hash table */

static const HashTableValue JSWaveShaperNodeTableValues[] =
{
    { "curve", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWaveShaperNodeCurve), (intptr_t)setJSWaveShaperNodeCurve, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsWaveShaperNodeConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSWaveShaperNodeTable = { 4, 3, JSWaveShaperNodeTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSWaveShaperNodeConstructorTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSWaveShaperNodeConstructorTable = { 1, 0, JSWaveShaperNodeConstructorTableValues, 0 };
const ClassInfo JSWaveShaperNodeConstructor::s_info = { "WaveShaperNodeConstructor", &Base::s_info, &JSWaveShaperNodeConstructorTable, 0, CREATE_METHOD_TABLE(JSWaveShaperNodeConstructor) };

JSWaveShaperNodeConstructor::JSWaveShaperNodeConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSWaveShaperNodeConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSWaveShaperNodePrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSWaveShaperNodeConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSWaveShaperNodeConstructor, JSDOMWrapper>(exec, &JSWaveShaperNodeConstructorTable, jsCast<JSWaveShaperNodeConstructor*>(cell), propertyName, slot);
}

bool JSWaveShaperNodeConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSWaveShaperNodeConstructor, JSDOMWrapper>(exec, &JSWaveShaperNodeConstructorTable, jsCast<JSWaveShaperNodeConstructor*>(object), propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSWaveShaperNodePrototypeTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSWaveShaperNodePrototypeTable = { 1, 0, JSWaveShaperNodePrototypeTableValues, 0 };
const ClassInfo JSWaveShaperNodePrototype::s_info = { "WaveShaperNodePrototype", &Base::s_info, &JSWaveShaperNodePrototypeTable, 0, CREATE_METHOD_TABLE(JSWaveShaperNodePrototype) };

JSObject* JSWaveShaperNodePrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSWaveShaperNode>(exec, globalObject);
}

const ClassInfo JSWaveShaperNode::s_info = { "WaveShaperNode", &Base::s_info, &JSWaveShaperNodeTable, 0 , CREATE_METHOD_TABLE(JSWaveShaperNode) };

JSWaveShaperNode::JSWaveShaperNode(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<WaveShaperNode> impl)
    : JSAudioNode(structure, globalObject, impl)
{
}

void JSWaveShaperNode::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSWaveShaperNode::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSWaveShaperNodePrototype::create(exec->globalData(), globalObject, JSWaveShaperNodePrototype::createStructure(exec->globalData(), globalObject, JSAudioNodePrototype::self(exec, globalObject)));
}

bool JSWaveShaperNode::getOwnPropertySlot(JSCell* cell, ExecState* exec, PropertyName propertyName, PropertySlot& slot)
{
    JSWaveShaperNode* thisObject = jsCast<JSWaveShaperNode*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSWaveShaperNode, Base>(exec, &JSWaveShaperNodeTable, thisObject, propertyName, slot);
}

bool JSWaveShaperNode::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, PropertyName propertyName, PropertyDescriptor& descriptor)
{
    JSWaveShaperNode* thisObject = jsCast<JSWaveShaperNode*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSWaveShaperNode, Base>(exec, &JSWaveShaperNodeTable, thisObject, propertyName, descriptor);
}

JSValue jsWaveShaperNodeCurve(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSWaveShaperNode* castedThis = jsCast<JSWaveShaperNode*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    WaveShaperNode* impl = static_cast<WaveShaperNode*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->curve()));
    return result;
}


JSValue jsWaveShaperNodeConstructor(ExecState* exec, JSValue slotBase, PropertyName)
{
    JSWaveShaperNode* domObject = jsCast<JSWaveShaperNode*>(asObject(slotBase));
    return JSWaveShaperNode::getConstructor(exec, domObject->globalObject());
}

void JSWaveShaperNode::put(JSCell* cell, ExecState* exec, PropertyName propertyName, JSValue value, PutPropertySlot& slot)
{
    JSWaveShaperNode* thisObject = jsCast<JSWaveShaperNode*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    lookupPut<JSWaveShaperNode, Base>(exec, propertyName, value, &JSWaveShaperNodeTable, thisObject, slot);
}

void setJSWaveShaperNodeCurve(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    JSWaveShaperNode* castedThis = jsCast<JSWaveShaperNode*>(thisObject);
    WaveShaperNode* impl = static_cast<WaveShaperNode*>(castedThis->impl());
    impl->setCurve(toFloat32Array(value));
}


JSValue JSWaveShaperNode::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSWaveShaperNodeConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}

JSC::JSValue toJS(JSC::ExecState* exec, JSDOMGlobalObject* globalObject, WaveShaperNode* impl)
{
    return wrap<JSWaveShaperNode>(exec, globalObject, impl);
}


}

#endif // ENABLE(WEB_AUDIO)
