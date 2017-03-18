#include <jni.h>
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/core/ocl.hpp>

#include <GLES/gl.h>
#include <System.h>

using namespace std;
using namespace cv;

static ORB_SLAM2::System *s;

extern "C"
JNIEXPORT jstring JNICALL
Java_org_jjhartmann_jeremy_testopencv2_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    cv::Mat test = cv::Mat::eye(5, 5, CV_64F);

    string voc_string = "some/path/";
    string cal_string = "To/some/calib/file";
//    s=new ORB_SLAM2::System(voc_string,cal_string,ORB_SLAM2::System::MONOCULAR,true);


    return env->NewStringUTF(hello.c_str());
}


