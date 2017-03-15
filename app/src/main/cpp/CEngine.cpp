//
// Created by Jeremy on 3/13/2017.
//

#include "org_jjhartmann_jeremy_testopencv2_IEngineJNI.h"

using namespace std;
using namespace cv;

JNIEXPORT void JNICALL
Java_org_jjhartmann_jeremy_testopencv2_IEngineJNI_FindFeatures(JNIEnv *env, jobject instance,
                                                               jlong matAddrGr, jlong matAddrRGB) {

    // TODO: Feature detection.
    Mat &rgbaMat = *(Mat*) matAddrRGB;
    Mat &grayMat = *(Mat*) matAddrGr;
    vector<KeyPoint> vKeys;

    Ptr<FeatureDetector> detector = FastFeatureDetector::create(50);
    detector->detect(grayMat, vKeys);

    for (unsigned int i = 0; i < vKeys.size(); ++i) {
        const KeyPoint &kp = vKeys[i];
        circle(rgbaMat, Point(kp.pt.x, kp.pt.y), 10, Scalar(255,0,0,255));
    }
}

JNIEXPORT void JNICALL
Java_org_jjhartmann_jeremy_testopencv2_IEngineJNI_VisualOdometry(JNIEnv *env, jobject instance,
                                                                 jlong matAddrCurrent) {
    // TODO: COnduct visual odometrry

    Mat &cImg = *(Mat *)matAddrCurrent;
    mVIOEngineAddr->ProcessImage(cImg);

    // Determine if engine has all required frames.
    if (!mVIOEngineAddr->IsReady())
    {
        mVIOEngineAddr->ShiftBuffers();
        return;
    }

    // Process the image and calculate visual odometry



    mVIOEngineAddr->ShiftBuffers();
}

JNIEXPORT void JNICALL
Java_org_jjhartmann_jeremy_testopencv2_IEngineJNI_Start(JNIEnv *env, jobject instance,
                                                        jint width,
                                                        jint height) {

    // TODO: Create initlilization of VIO
    int w = width;
    int h = height;
    mVIOEngineAddr = new VIOEngine();
    mVIOEngineAddr->Init(w, h);

}

JNIEXPORT void JNICALL
Java_org_jjhartmann_jeremy_testopencv2_IEngineJNI_Stop(JNIEnv *env, jobject instance) {

    // TODO: Clean the mVIOEngine
    if (mVIOEngineAddr != nullptr){
        delete mVIOEngineAddr;
    }

}