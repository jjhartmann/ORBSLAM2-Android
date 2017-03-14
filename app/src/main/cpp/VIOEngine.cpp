//
// Created by Jeremy on 3/14/2017.
//

#include "VIOEngine.h"

#include <opencv2/core.hpp>

VIOEngine::VIOEngine() :
        mCurrentImage(nullptr),
        mPreviousImage(nullptr),
        mPendingImage(nullptr)
{
    ; // TODO: Anything else?
}

VIOEngine::~VIOEngine()
{
    // Clean up VIOImages
    if (mCurrentImage != nullptr){
        delete mCurrentImage;
    }
    if (mPreviousImage != nullptr){
        delete mPreviousImage;
    }
    if (mPendingImage != nullptr){
        delete mPendingImage;
    }
}

void VIOEngine::Init(int width, int height) {
    mCurrentImage = new VIOImage(4, 0.5, width, height);
    mPreviousImage = new VIOImage(4, 0.5, width, height);
    mPendingImage = new VIOImage(4, 0.5, width, height);
}

void VIOEngine::ProcessImage(cv::Mat inputImg) {

}


