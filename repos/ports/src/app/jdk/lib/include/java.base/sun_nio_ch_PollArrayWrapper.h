/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class sun_nio_ch_PollArrayWrapper */

#ifndef _Included_sun_nio_ch_PollArrayWrapper
#define _Included_sun_nio_ch_PollArrayWrapper
#ifdef __cplusplus
extern "C" {
#endif
#undef sun_nio_ch_PollArrayWrapper_SIZE_POLLFD
#define sun_nio_ch_PollArrayWrapper_SIZE_POLLFD 8L
#undef sun_nio_ch_PollArrayWrapper_FD_OFFSET
#define sun_nio_ch_PollArrayWrapper_FD_OFFSET 0L
#undef sun_nio_ch_PollArrayWrapper_EVENT_OFFSET
#define sun_nio_ch_PollArrayWrapper_EVENT_OFFSET 4L
#undef sun_nio_ch_PollArrayWrapper_REVENT_OFFSET
#define sun_nio_ch_PollArrayWrapper_REVENT_OFFSET 6L
/*
 * Class:     sun_nio_ch_PollArrayWrapper
 * Method:    poll0
 * Signature: (JIJ)I
 */
JNIEXPORT jint JNICALL Java_sun_nio_ch_PollArrayWrapper_poll0
  (JNIEnv *, jobject, jlong, jint, jlong);

/*
 * Class:     sun_nio_ch_PollArrayWrapper
 * Method:    interrupt
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_sun_nio_ch_PollArrayWrapper_interrupt
  (JNIEnv *, jclass, jint);

#ifdef __cplusplus
}
#endif
#endif
