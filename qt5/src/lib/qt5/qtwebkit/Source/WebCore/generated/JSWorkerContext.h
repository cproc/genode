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

#ifndef JSWorkerContext_h
#define JSWorkerContext_h

#if ENABLE(WORKERS)

#include "JSWorkerContextBase.h"
#include <runtime/JSObject.h>

namespace WebCore {

class WorkerContext;

class JSWorkerContext : public JSWorkerContextBase {
public:
    typedef JSWorkerContextBase Base;
    static JSWorkerContext* create(JSC::JSGlobalData& globalData, JSC::Structure* structure, PassRefPtr<WorkerContext> impl)
    {
        JSWorkerContext* ptr = new (NotNull, JSC::allocateCell<JSWorkerContext>(globalData.heap)) JSWorkerContext(globalData, structure, impl);
        ptr->finishCreation(globalData);
        globalData.heap.addFinalizer(ptr, destroy);
        return ptr;
    }

    static const bool needsDestruction = false;

    static bool getOwnPropertySlot(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName, JSC::PropertySlot&);
    static bool getOwnPropertyDescriptor(JSC::JSObject*, JSC::ExecState*, JSC::PropertyName, JSC::PropertyDescriptor&);
    static bool getOwnPropertySlotByIndex(JSC::JSCell*, JSC::ExecState*, unsigned propertyName, JSC::PropertySlot&);
    bool getOwnPropertySlotDelegate(JSC::ExecState*, JSC::PropertyName, JSC::PropertySlot&);
    bool getOwnPropertyDescriptorDelegate(JSC::ExecState*, JSC::PropertyName, JSC::PropertyDescriptor&);
    static void put(JSC::JSCell*, JSC::ExecState*, JSC::PropertyName, JSC::JSValue, JSC::PutPropertySlot&);
    static const JSC::ClassInfo s_info;

    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, globalObject, prototype, JSC::TypeInfo(JSC::GlobalObjectType, StructureFlags), &s_info);
    }

    static void visitChildren(JSCell*, JSC::SlotVisitor&);


    // Custom attributes
    JSC::JSValue messageChannel(JSC::ExecState*) const;
    JSC::JSValue eventSource(JSC::ExecState*) const;
    JSC::JSValue xmlHttpRequest(JSC::ExecState*) const;
#if ENABLE(WEB_SOCKETS)
    JSC::JSValue webSocket(JSC::ExecState*) const;
#endif

    // Custom functions
    JSC::JSValue importScripts(JSC::ExecState*);
    JSC::JSValue setTimeout(JSC::ExecState*);
    JSC::JSValue setInterval(JSC::ExecState*);
protected:
    JSWorkerContext(JSC::JSGlobalData&, JSC::Structure*, PassRefPtr<WorkerContext>);
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | JSC::InterceptsGetOwnPropertySlotByIndexEvenWhenLengthIsNotZero | JSC::OverridesVisitChildren | Base::StructureFlags;
};


class JSWorkerContextPrototype : public JSC::JSNonFinalObject {
public:
    typedef JSC::JSNonFinalObject Base;
    static JSWorkerContextPrototype* create(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::Structure* structure)
    {
        JSWorkerContextPrototype* ptr = new (NotNull, JSC::allocateCell<JSWorkerContextPrototype>(globalData.heap)) JSWorkerContextPrototype(globalData, globalObject, structure);
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
    JSWorkerContextPrototype(JSC::JSGlobalData& globalData, JSC::JSGlobalObject*, JSC::Structure* structure) : JSC::JSNonFinalObject(globalData, structure) { }
protected:
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | JSC::OverridesVisitChildren | Base::StructureFlags;
};

// Functions

JSC::EncodedJSValue JSC_HOST_CALL jsWorkerContextPrototypeFunctionClose(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsWorkerContextPrototypeFunctionImportScripts(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsWorkerContextPrototypeFunctionSetTimeout(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsWorkerContextPrototypeFunctionClearTimeout(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsWorkerContextPrototypeFunctionSetInterval(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsWorkerContextPrototypeFunctionClearInterval(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsWorkerContextPrototypeFunctionAddEventListener(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsWorkerContextPrototypeFunctionRemoveEventListener(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsWorkerContextPrototypeFunctionDispatchEvent(JSC::ExecState*);
#if ENABLE(SQL_DATABASE)
JSC::EncodedJSValue JSC_HOST_CALL jsWorkerContextPrototypeFunctionOpenDatabase(JSC::ExecState*);
#endif
#if ENABLE(SQL_DATABASE)
JSC::EncodedJSValue JSC_HOST_CALL jsWorkerContextPrototypeFunctionOpenDatabaseSync(JSC::ExecState*);
#endif
#if ENABLE(FILE_SYSTEM)
JSC::EncodedJSValue JSC_HOST_CALL jsWorkerContextPrototypeFunctionWebkitRequestFileSystem(JSC::ExecState*);
#endif
#if ENABLE(FILE_SYSTEM)
JSC::EncodedJSValue JSC_HOST_CALL jsWorkerContextPrototypeFunctionWebkitRequestFileSystemSync(JSC::ExecState*);
#endif
#if ENABLE(FILE_SYSTEM)
JSC::EncodedJSValue JSC_HOST_CALL jsWorkerContextPrototypeFunctionWebkitResolveLocalFileSystemURL(JSC::ExecState*);
#endif
#if ENABLE(FILE_SYSTEM)
JSC::EncodedJSValue JSC_HOST_CALL jsWorkerContextPrototypeFunctionWebkitResolveLocalFileSystemSyncURL(JSC::ExecState*);
#endif
// Attributes

JSC::JSValue jsWorkerContextSelf(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextSelf(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsWorkerContextLocation(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextLocation(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsWorkerContextOnerror(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextOnerror(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsWorkerContextNavigator(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextNavigator(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsWorkerContextMessageEventConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextMessageEventConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsWorkerContextWorkerLocationConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextWorkerLocationConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsWorkerContextMessageChannelConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextMessageChannelConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsWorkerContextEventSourceConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextEventSourceConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsWorkerContextXMLHttpRequestConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextXMLHttpRequestConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsWorkerContextBlobConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextBlobConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsWorkerContextFileReaderConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextFileReaderConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsWorkerContextFileReaderSyncConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextFileReaderSyncConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
#if ENABLE(BLOB)
JSC::JSValue jsWorkerContextURLConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextURLConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
#endif
#if ENABLE(BLOB)
JSC::JSValue jsWorkerContextWebkitURLConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextWebkitURLConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
#endif
JSC::JSValue jsWorkerContextArrayBufferConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextArrayBufferConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsWorkerContextInt8ArrayConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextInt8ArrayConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsWorkerContextUint8ArrayConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextUint8ArrayConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsWorkerContextUint8ClampedArrayConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextUint8ClampedArrayConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsWorkerContextInt16ArrayConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextInt16ArrayConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsWorkerContextUint16ArrayConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextUint16ArrayConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsWorkerContextInt32ArrayConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextInt32ArrayConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsWorkerContextUint32ArrayConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextUint32ArrayConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsWorkerContextFloat32ArrayConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextFloat32ArrayConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsWorkerContextFloat64ArrayConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextFloat64ArrayConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
JSC::JSValue jsWorkerContextDataViewConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextDataViewConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
#if ENABLE(LEGACY_NOTIFICATIONS) || ENABLE(NOTIFICATIONS)
JSC::JSValue jsWorkerContextWebkitNotifications(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
#endif
#if ENABLE(INDEXED_DATABASE)
JSC::JSValue jsWorkerContextWebkitIndexedDB(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
#endif
#if ENABLE(INDEXED_DATABASE)
JSC::JSValue jsWorkerContextWebkitIDBCursorConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextWebkitIDBCursorConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
#endif
#if ENABLE(INDEXED_DATABASE)
JSC::JSValue jsWorkerContextWebkitIDBDatabaseConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextWebkitIDBDatabaseConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
#endif
#if ENABLE(INDEXED_DATABASE)
JSC::JSValue jsWorkerContextWebkitIDBFactoryConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextWebkitIDBFactoryConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
#endif
#if ENABLE(INDEXED_DATABASE)
JSC::JSValue jsWorkerContextWebkitIDBIndexConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextWebkitIDBIndexConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
#endif
#if ENABLE(INDEXED_DATABASE)
JSC::JSValue jsWorkerContextWebkitIDBKeyRangeConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextWebkitIDBKeyRangeConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
#endif
#if ENABLE(INDEXED_DATABASE)
JSC::JSValue jsWorkerContextWebkitIDBObjectStoreConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextWebkitIDBObjectStoreConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
#endif
#if ENABLE(INDEXED_DATABASE)
JSC::JSValue jsWorkerContextWebkitIDBRequestConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextWebkitIDBRequestConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
#endif
#if ENABLE(INDEXED_DATABASE)
JSC::JSValue jsWorkerContextWebkitIDBTransactionConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextWebkitIDBTransactionConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
#endif
#if ENABLE(INDEXED_DATABASE)
JSC::JSValue jsWorkerContextIndexedDB(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
#endif
#if ENABLE(INDEXED_DATABASE)
JSC::JSValue jsWorkerContextIDBCursorConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextIDBCursorConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
#endif
#if ENABLE(INDEXED_DATABASE)
JSC::JSValue jsWorkerContextIDBCursorWithValueConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextIDBCursorWithValueConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
#endif
#if ENABLE(INDEXED_DATABASE)
JSC::JSValue jsWorkerContextIDBDatabaseConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextIDBDatabaseConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
#endif
#if ENABLE(INDEXED_DATABASE)
JSC::JSValue jsWorkerContextIDBFactoryConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextIDBFactoryConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
#endif
#if ENABLE(INDEXED_DATABASE)
JSC::JSValue jsWorkerContextIDBIndexConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextIDBIndexConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
#endif
#if ENABLE(INDEXED_DATABASE)
JSC::JSValue jsWorkerContextIDBKeyRangeConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextIDBKeyRangeConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
#endif
#if ENABLE(INDEXED_DATABASE)
JSC::JSValue jsWorkerContextIDBObjectStoreConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextIDBObjectStoreConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
#endif
#if ENABLE(INDEXED_DATABASE)
JSC::JSValue jsWorkerContextIDBOpenDBRequestConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextIDBOpenDBRequestConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
#endif
#if ENABLE(INDEXED_DATABASE)
JSC::JSValue jsWorkerContextIDBRequestConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextIDBRequestConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
#endif
#if ENABLE(INDEXED_DATABASE)
JSC::JSValue jsWorkerContextIDBTransactionConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextIDBTransactionConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
#endif
#if ENABLE(INDEXED_DATABASE)
JSC::JSValue jsWorkerContextIDBVersionChangeEventConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextIDBVersionChangeEventConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
#endif
#if ENABLE(FILE_SYSTEM)
JSC::JSValue jsWorkerContextFileErrorConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextFileErrorConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
#endif
#if ENABLE(FILE_SYSTEM)
JSC::JSValue jsWorkerContextFileExceptionConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextFileExceptionConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
#endif
#if ENABLE(WEB_SOCKETS)
JSC::JSValue jsWorkerContextWebSocketConstructor(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
void setJSWorkerContextWebSocketConstructor(JSC::ExecState*, JSC::JSObject*, JSC::JSValue);
#endif
// Constants

#if ENABLE(FILE_SYSTEM)
JSC::JSValue jsWorkerContextTEMPORARY(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
#endif
#if ENABLE(FILE_SYSTEM)
JSC::JSValue jsWorkerContextPERSISTENT(JSC::ExecState*, JSC::JSValue, JSC::PropertyName);
#endif

} // namespace WebCore

#endif // ENABLE(WORKERS)

#endif
