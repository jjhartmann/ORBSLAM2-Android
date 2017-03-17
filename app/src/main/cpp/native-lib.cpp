#include <jni.h>
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/core/ocl.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>

using namespace std;
using namespace cv;

extern "C"
JNIEXPORT jstring JNICALL
Java_org_jjhartmann_jeremy_testopencv2_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    cv::Mat test = cv::Mat::eye(5, 5, CV_64F);

    typedef boost::tuple<std::string, int> animal;
    animal a{"cat", 4};


    return env->NewStringUTF(hello.c_str());
}


