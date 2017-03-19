//
// Created by Jeremy on 3/18/2017.
//

#include <string>
#include "org_jjhartmann_jeremy_testopencv2_JNIBindings_IORBSEngineJNI.h"


// SLAM ENGINE

using namespace std;
using namespace cv;

JNIEXPORT void JNICALL
Java_org_jjhartmann_jeremy_testopencv2_JNIBindings_IORBSEngineJNI_InitSystem(JNIEnv *env,
                                                                             jobject instance,
                                                                             jstring vocDataBase_,
                                                                             jstring settingsFile_) {
    const char *vocDataBase = env->GetStringUTFChars(vocDataBase_, 0);
    const char *settingsFile = env->GetStringUTFChars(settingsFile_, 0);

    string vocFile(vocDataBase);
    string sfile(settingsFile);

    // Init the ORB SLAM System
    orbSlamEngine = new ORBSEngine(vocFile, sfile);

    env->ReleaseStringUTFChars(vocDataBase_, vocDataBase);
    env->ReleaseStringUTFChars(settingsFile_, settingsFile);
}

JNIEXPORT jlong JNICALL
Java_org_jjhartmann_jeremy_testopencv2_JNIBindings_IORBSEngineJNI_TrackMonocular(JNIEnv *env,
                                                                                 jobject instance,
                                                                                 jlong matAddr,
                                                                                 jlong matRgbaAddr) {
    // Convert images to mat
    Mat &cImg = *(Mat *)matAddr; // NOT OWNED
    Mat &cRgbaImg = *(Mat *)matRgbaAddr; // NOT OWNED

    // Call method to process image
    cRgbaImg = orbSlamEngine->ProcessImage(cImg);
}

JNIEXPORT void JNICALL
Java_org_jjhartmann_jeremy_testopencv2_JNIBindings_IORBSEngineJNI_Shutdown(JNIEnv *env,
                                                                           jobject instance) {
    // Delete Engine pointer
    if (orbSlamEngine != nullptr){
        delete orbSlamEngine;
    }
}

JNIEXPORT void JNICALL
Java_org_jjhartmann_jeremy_testopencv2_JNIBindings_IORBSEngineJNI_GLESInit(JNIEnv *env,
                                                                           jobject instance) {

    // TODO

}

JNIEXPORT void JNICALL
Java_org_jjhartmann_jeremy_testopencv2_JNIBindings_IORBSEngineJNI_GLESRender(JNIEnv *env,
                                                                             jobject instance) {

    // TODO

}

JNIEXPORT void JNICALL
Java_org_jjhartmann_jeremy_testopencv2_JNIBindings_IORBSEngineJNI_GLESResize(JNIEnv *env,
                                                                             jobject instance) {

    // TODO

}