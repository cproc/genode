/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class java_io_ObjectInputStream */

#ifndef _Included_java_io_ObjectInputStream
#define _Included_java_io_ObjectInputStream
#ifdef __cplusplus
extern "C" {
#endif
#undef java_io_ObjectInputStream_MAX_SKIP_BUFFER_SIZE
#define java_io_ObjectInputStream_MAX_SKIP_BUFFER_SIZE 2048L
#undef java_io_ObjectInputStream_DEFAULT_BUFFER_SIZE
#define java_io_ObjectInputStream_DEFAULT_BUFFER_SIZE 8192L
#undef java_io_ObjectInputStream_MAX_BUFFER_SIZE
#define java_io_ObjectInputStream_MAX_BUFFER_SIZE 2147483639L
#undef java_io_ObjectInputStream_NULL_HANDLE
#define java_io_ObjectInputStream_NULL_HANDLE -1L
/*
 * Class:     java_io_ObjectInputStream
 * Method:    bytesToFloats
 * Signature: ([BI[FII)V
 */
JNIEXPORT void JNICALL Java_java_io_ObjectInputStream_bytesToFloats
  (JNIEnv *, jclass, jbyteArray, jint, jfloatArray, jint, jint);

/*
 * Class:     java_io_ObjectInputStream
 * Method:    bytesToDoubles
 * Signature: ([BI[DII)V
 */
JNIEXPORT void JNICALL Java_java_io_ObjectInputStream_bytesToDoubles
  (JNIEnv *, jclass, jbyteArray, jint, jdoubleArray, jint, jint);

#ifdef __cplusplus
}
#endif
#endif
