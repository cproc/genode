/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class java_lang_ClassLoader_NativeLibrary */

#ifndef _Included_java_lang_ClassLoader_NativeLibrary
#define _Included_java_lang_ClassLoader_NativeLibrary
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     java_lang_ClassLoader_NativeLibrary
 * Method:    load
 * Signature: (Ljava/lang/String;Z)V
 */
JNIEXPORT void JNICALL Java_java_lang_ClassLoader_00024NativeLibrary_load
  (JNIEnv *, jobject, jstring, jboolean);

/*
 * Class:     java_lang_ClassLoader_NativeLibrary
 * Method:    find
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_java_lang_ClassLoader_00024NativeLibrary_find
  (JNIEnv *, jobject, jstring);

/*
 * Class:     java_lang_ClassLoader_NativeLibrary
 * Method:    unload
 * Signature: (Ljava/lang/String;Z)V
 */
JNIEXPORT void JNICALL Java_java_lang_ClassLoader_00024NativeLibrary_unload
  (JNIEnv *, jobject, jstring, jboolean);

#ifdef __cplusplus
}
#endif
#endif
