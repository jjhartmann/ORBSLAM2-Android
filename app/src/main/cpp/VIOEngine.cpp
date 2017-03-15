//
// Created by Jeremy on 3/14/2017.
//

#include "VIOEngine.h"

#include <opencv2/core.hpp>

using namespace std;
using namespace cv;

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

void VIOEngine::ProcessImage(cv::Mat &inputImg) {

}

bool VIOEngine::IsReady() {
    return mCurrentImage->isImageLoaded() && mPreviousImage->isImageLoaded();
}

void VIOEngine::ShiftBuffers() {
    // Clean and swap buffers.
    mPreviousImage->CleanOctaves();
    VIOEngine::Swap(mCurrentImage, mPreviousImage);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Private Methods
/// : TODO: Refactor this into own class?
////////////////////////////////////////////////////////////////////////////////////////////////////
void VIOEngine::DetectFeatures(VIOImage *in_vioImg) {


    // Params
    int THRESHOLD = 50;
    Ptr<FeatureDetector> detector = FastFeatureDetector::create(THRESHOLD);
    vector<KeyPoint> &kp = mCurrentImage->GetKPAt(0);

    // TODO: Heuristics using an image pyramid, use low resolution and work up the pyrimad when accuracy fails.
    detector->detect(mCurrentImage->GetGrayAt(0), kp);
}



