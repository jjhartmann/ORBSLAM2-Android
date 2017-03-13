#include <jni.h>
#include <string>
#include <opencv2/opencv.hpp>

extern "C"
JNIEXPORT jstring JNICALL
Java_org_jjhartmann_jeremy_testopencv2_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    cv::Mat test = cv::Mat::eye(5, 5, CV_64F);
    cv::VideoCapture camera;
    camera.open(0);
    cv::Mat img;
    camera >> img;
    camera.release();


    return env->NewStringUTF(hello.c_str());
}
