//
// Created by Jeremy on 3/18/2017.
//

#ifndef VISUALINERTIALODOMETRY_ANDROID_UW_SAR_2_ORBSENGINE_H
#define VISUALINERTIALODOMETRY_ANDROID_UW_SAR_2_ORBSENGINE_H


#include <string>
#include <opencv2/core/mat.hpp>
#include <System.h>
#include "VIOConstants.h"

class ORBSEngine {
public:
    ORBSEngine(std::string vocFile, std::string sfile);
    ~ORBSEngine();

    cv::Mat ProcessImage(cv::Mat);
    void Shutdown();

private:
    bool mSystemRunning = false;
    uint mFrameCount = 0;
    SARVIOFusion::clock::time_point mCurrentTimeStamp;

    ORB_SLAM2::System *mSystem;

};


#endif //VISUALINERTIALODOMETRY_ANDROID_UW_SAR_2_ORBSENGINE_H
