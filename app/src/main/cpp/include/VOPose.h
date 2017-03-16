//
// Created by Jeremy on 3/15/2017.
//

#ifndef VISUALINERTIALODOMETRY_ANDROID_UW_SAR_2_VOPOSE_H
#define VISUALINERTIALODOMETRY_ANDROID_UW_SAR_2_VOPOSE_H

#include "VIOConstants.h"

#include <opencv2/core/mat.hpp>
#include <chrono>

class VOPose {
public:
    VOPose();
    ~VOPose();

    void AddTranslationVector(cv::Mat in_t);
    SARVIOFusion::TranslationVector GetIntegratedTranslation();
private:

    // Data structure that houses translation and timestamp

    std::vector<SARVIOFusion::TranslationVector> mTranslationArray;

    // Current instance of R and t.
    SARVIOFusion::TranslationVector mIntegratedT;
    SARVIOFusion::TranslationVector mCurrentT;
    cv::Mat mCurrentR;
};


#endif //VISUALINERTIALODOMETRY_ANDROID_UW_SAR_2_VOPOSE_H
