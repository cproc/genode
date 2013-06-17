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

#ifndef JSIDBDatabase_h
#define JSIDBDatabase_h

#if ENABLE(INDEXED_DATABASE)

#include "IDBDatabase.h"
#include "JSDOMBinding.h"
#include <runtime/JSGlobalObject.h>
#include <runtime/JSObject.h>
#include <runtime/ObjectPrototype.h>

namespace WebCore {

class JSIDBDatabase : public JSDOMWrapper {
public:
    typedef JSDOMWrapper Base;
    static JSIDBDatabase* create(JSC::Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<IDBDatabase> impl)
    {
        JSIDBDatabase* ptr = new (NotNull, JSC::allocateCell<JSIDBDatabase>(globalObject->globalData().heap)) JSIDBDatabase(structure, globalObject, impl);
        ptr->finishCreation(globalObject->globalData());
        return ptr;
    }

    static JSC::JSObject* createPrototype(JSC::ExecState*, JSC::JSGlobalObject*);
    static bool getOwnPropertySlot(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName, JSC::PropertySlot&);
    static bool getOwnPropertyDescriptor(JSC::JSObject*, JSC::ExecState*, JSC::PropertyName, JSC::PropertyDescriptor&);
    static void put(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName, JSC::JSValue, JSC::PutPropertySlot&);
    static void destroy(JSC::JSCell*);
    ~JSIDBDatabase();
    static const JSC::ClassInfo s_info;

    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), &s_info);
    }

    static JSC::JSValue getConstructor(JSC::ExecState*, JSC::JSGlobalObject*);
    static void visitChildren(JSCell*, JSC::SlotVisitor&);

    IDBDatabase* impl() const { return m_impl; }
    void releaseImpl() { m_impl->deref(); m_impl = 0; }

    void releaseImplIfNotNull() { if (m_impl) { m_impl->deref(); m_impl = 0; } }

private:
    IDBDatabase* m_impl;
protected:
    JSIDBDatabase(JSC::Structure*, JSDOMGlobalObject*, PassRefPtr<IDBDatabase>);
    void finishCreation(JSC::JSGlobalData&);
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | JSC::InterceptsGetOwnPropertySlotByIndexEvenWhenLengthIsNotZero | JSC::OverridesVisitChildren | Base::StructureFlags;
};

class JSIDBDatabaseOwner : public JSC::WeakHandleOwner {
public:
    virtual bool isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown>, void* context, JSC::SlotVisitor&);
    virtual void finalize(JSC::Handle<JSC::Unknown>, void* context);
};

inline JSC::WeakHandleOwner* wrapperOwner(DOMWrapperWorld*, IDBDatabase*)
{
    DEFINE_STATIC_LOCAL(JSIDBDatabaseOwner, jsIDBDatabaseOwner, ());
    return &jsIDBDatabaseOwner;
}

inline void* wrapperContext(DOMWrapperWorld* world, IDBDatabase*)
{
    return world;
}

JSC::JSValue toJS(JSC::ExecState*, JSDOMGlobalObject*, IDBDatabase*);
IDBDatabase* toIDBDatabase(JSC::JSValue);

class JSIDBDatabasePrototype : public JSC::JSNonFinalObject {
public:
    typedef JSC::JSNonFinalObject Base;
    static JSC::JSObject* self(JSC::ExecState*, JSC::JSGlobalObject*);
    static JSIDBDatabasePrototype* create(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::Structure* structure)
    {
        JSIDBDatabasePrototype* ptr = new (NotNull, JSC::allocateCell<JSIDBDatabasePrototype>(globalData.heap)) JSIDBDatabasePrototype(globalData, globalObject, structure);
        ptr->finishCreation(globalData);
        return ptr;
    }

    static const JSC::ClassInfo s_info;
    static bool getOwnPropertySlot(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName, JSC::PropertySlot&);
    static bool getOwnPropertyDescriptor(JSC::JSObject*, JSC::ExecState*, JSC::PropertyName, JSC::PropertyDescriptor&);
    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), &s_info);
    }

private:
    JSIDBDatabasePrototype(JSC::JSGlobalData& globalData, JSC::JSGlobalObject*, JSC::Structure* structure) : JSC::JSNonFinalObject(globalData, structure) { }
protected:
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | JSC::OverridesVisitChildren | Base::StructureFlags;
};

class JSIDBDatabaseConstructor : public DOMConstructorObject {
private:
    JSIDBDatabaseConstructor(JSC::Structure*, JSDOMGlobalObject*);
    void finishCreation(JSC::ExecState*, JSDOMGlobalObject*);

public:
    typedef DOMConstructorObject Base;
    static JSIDBDatabaseConstructor* create(JSC::ExecState* exec, JSC::Structure* structure, JSDOMGlobalObject* globalObject)
    {
        JSIDBDatabaseConstructor* ptr = new (NotNull, JSC::allocateCell<JSIDBDatabaseConstructor>(*exec->heap())) JSIDBDatabaseConstructor(structure, globalObject);
        ptr->finishCreation(exec, globalObject);
        return ptr;
    }

    static bool getOwnPropertySlot(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName, JSC::PropertySlot&);
    static bool getOwnPropertyDescriptor(JSC::JSObject*, JSC::ExecState*, JSC::PropertyName, JSC::PropertyDescriptor&);
    static const JSC::ClassInfo s_info;
    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), &s_info);
    }
protected:
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | JSC::ImplementsHasInstance | DOMConstructorObject::StructureFlags;
};

// Functions

JSC::EncodedJSValue JSC_HOST_CALL jsIDBDatabasePrototypeFunctionCreateObjectStore(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsIDBDatabasePrototypeFunctionDeleteObjectStore(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsIDBDatabasePrototypeFunctionTransaction(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsIDBDatabasePrototypeFunctionClose(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsIDBDatabasePrototypeFunctionAddEventListener(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsIDBDatabasePrototypeFunctionRemoveEventListener(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsIDBDatabasePrototypeFunctionDispatchEvent(JSC::ExecState*);
// Attributes

JSC::JSValue jsIDBDatabaseName(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsIDBDatabaseVersion(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsIDBDatabaseObjectStoreNames(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
JSC::JSValue jsIDBDatabaseOnabort(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSIDBDatabaseOnabort(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsIDBDatabaseOnerror(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSIDBDatabaseOnerror(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsIDBDatabaseOnversionchange(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSIDBDatabaseOnversionchange(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsIDBDatabaseConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);

} // namespace WebCore

#endif // ENABLE(INDEXED_DATABASE)

#endif
