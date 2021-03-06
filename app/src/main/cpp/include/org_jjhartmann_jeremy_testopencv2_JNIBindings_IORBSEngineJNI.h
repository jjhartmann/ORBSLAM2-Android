/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
#include "ORBSEngine.h"
/* Header for class org_jjhartmann_jeremy_testopencv2_JNIBindings_IORBSEngineJNI */

#ifndef _Included_org_jjhartmann_jeremy_testopencv2_JNIBindings_IORBSEngineJNI
#define _Included_org_jjhartmann_jeremy_testopencv2_JNIBindings_IORBSEngineJNI
#ifdef __cplusplus
extern "C" {
#endif


static ORBSEngine *orbSlamEngine = nullptr;


/*
 * Class:     org_jjhartmann_jeremy_testopencv2_JNIBindings_IORBSEngineJNI
 * Method:    InitSystem
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_jjhartmann_jeremy_testopencv2_JNIBindings_IORBSEngineJNI_InitSystem
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     org_jjhartmann_jeremy_testopencv2_JNIBindings_IORBSEngineJNI
 * Method:    TrackMonocular
 * Signature: (J)J
 */
JNIEXPORT void JNICALL Java_org_jjhartmann_jeremy_testopencv2_JNIBindings_IORBSEngineJNI_TrackMonocular
  (JNIEnv *, jobject, jlong, jlong, jlong);

/*
 * Class:     org_jjhartmann_jeremy_testopencv2_JNIBindings_IORBSEngineJNI
 * Method:    Shutdown
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_jjhartmann_jeremy_testopencv2_JNIBindings_IORBSEngineJNI_Shutdown
  (JNIEnv *, jobject);

/*
 * Class:     org_jjhartmann_jeremy_testopencv2_JNIBindings_IORBSEngineJNI
 * Method:    GLESInit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_jjhartmann_jeremy_testopencv2_JNIBindings_IORBSEngineJNI_GLESInit
  (JNIEnv *, jobject);

/*
 * Class:     org_jjhartmann_jeremy_testopencv2_JNIBindings_IORBSEngineJNI
 * Method:    GLESRender
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_jjhartmann_jeremy_testopencv2_JNIBindings_IORBSEngineJNI_GLESRender
  (JNIEnv *, jobject);

/*
 * Class:     org_jjhartmann_jeremy_testopencv2_JNIBindings_IORBSEngineJNI
 * Method:    GLESResize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_jjhartmann_jeremy_testopencv2_JNIBindings_IORBSEngineJNI_GLESResize
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
