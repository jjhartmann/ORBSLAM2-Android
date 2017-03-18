/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class org_jjhartmann_jeremy_testopencv2_IEngineJNI */
#ifndef _Included_org_jjhartmann_jeremy_testopencv2_IEngineJNI
#define _Included_org_jjhartmann_jeremy_testopencv2_IEngineJNI

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/core/ocl.hpp>

#include <vector>

#include "VIOImage.h"
#include "VIOEngine.h"

#ifdef __cplusplus
extern "C" {
#endif


VIOEngine *mVIOEngineAddr = nullptr;

/*
 * Class:     org_jjhartmann_jeremy_testopencv2_IEngineJNI
 * Method:    FindFeatures
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_org_jjhartmann_jeremy_testopencv2_IEngineJNI_FindFeatures
  (JNIEnv *, jobject, jlong, jlong);

JNIEXPORT void JNICALL Java_org_jjhartmann_jeremy_testopencv2_IEngineJNI_VisualOdometry
        (JNIEnv *env, jobject instance, jlong matAddrCurrent);

JNIEXPORT void JNICALL Java_org_jjhartmann_jeremy_testopencv2_IEngineJNI_Start
        (JNIEnv *env, jobject instance, jint width, jint height);

JNIEXPORT void JNICALL Java_org_jjhartmann_jeremy_testopencv2_IEngineJNI_Stop
        (JNIEnv *env, jobject instance);

#ifdef __cplusplus
}
#endif
#endif
