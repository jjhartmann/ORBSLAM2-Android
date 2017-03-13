#include <jni.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/core/ocl.hpp>




extern "C"
JNIEXPORT jstring JNICALL
Java_org_jjhartmann_jeremy_testopencv2_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    cv::Mat test = cv::Mat::eye(5, 5, CV_64F);
    cv::VideoCapture camera(0);
    bool open = camera.isOpened();
    if (open) {
        cv::Mat img;
        camera >> img;
        cv::Mat frame;
        cv::cvtColor(img, frame, CV_BGR2GRAY);
        camera.release();
    }

    return env->NewStringUTF(hello.c_str());
}


JNIEXPORT void JNICALL
Java_org_jjhartmann_jeremy_testopencv2_IEngineJNI_FindFeatures(JNIEnv *env, jobject instance,
                                                               jlong matAddrGr, jlong matAddrRGB) {

    // TODO

}