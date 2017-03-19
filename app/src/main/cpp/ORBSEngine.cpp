//
// Created by Jeremy on 3/18/2017.
//

#include <org_jjhartmann_jeremy_testopencv2_JNIBindings_IORBSEngineJNI.h>
#include "ORBSEngine.h"


ORBSEngine::ORBSEngine(std::string vocFile, std::string sfile) :
    mSystemRunning(false),
    mFrameCount(0)
{

    // Init the ORB SLAM System
    mSystem = new ORB_SLAM2::System(vocFile, sfile, ORB_SLAM2::System::MONOCULAR, true);
    mSystemRunning = (mSystem != nullptr);
}

ORBSEngine::~ORBSEngine() {

}

cv::Mat ORBSEngine::ProcessImage(cv::Mat grayImg) {

    SARVIOFusion::clock::time_point t0 = SARVIOFusion::GetTimeStamp();
    mCurrentTimeStamp = t0;
    double seconds = std::chrono::duration_cast<std::chrono::milliseconds>(t0.time_since_epoch()).count();
    return mSystem->TrackMonocular(grayImg, seconds);
}

void ORBSEngine::Shutdown() {
    mSystem->Shutdown();
    mSystemRunning = false;
    if (mSystem != nullptr){
        delete mSystem;
    }
}
