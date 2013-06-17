// Automatically generated from /home/christian/projects/genode/genode/qt5/contrib/qt-everywhere-opensource-src-5.0.2/qtwebkit/Source/JavaScriptCore/runtime/JSONObject.cpp using /home/christian/projects/genode/genode/qt5/contrib/qt-everywhere-opensource-src-5.0.2/qtwebkit/Source/JavaScriptCore/create_hash_table. DO NOT EDIT!

#include "Lookup.h"

namespace JSC {

static const struct HashTableValue jsonTableValues[3] = {
   { "parse", DontEnum|Function, (intptr_t)static_cast<NativeFunction>(JSONProtoFuncParse), (intptr_t)2, NoIntrinsic },
   { "stringify", DontEnum|Function, (intptr_t)static_cast<NativeFunction>(JSONProtoFuncStringify), (intptr_t)3, NoIntrinsic },
   { 0, 0, 0, 0, NoIntrinsic }
};

extern const struct HashTable jsonTable =
    { 4, 3, jsonTableValues, 0 };
} // namespace
