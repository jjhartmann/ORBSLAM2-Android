//
// Created by Jeremy on 3/14/2017.
//

#include "VIOEngine.h"

#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/video/tracking.hpp>

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
    // Create Octaves and Gray Images
    mCurrentImage->CreateOctaves(inputImg);

    // Detect fast features and store them in array.
    DetectFeatures(mCurrentImage);

    // Check if buffers are filled.
    if (!IsReady()){
        return;
    }

    // Track and store rotation and translation vectors
    TrackFeatures(mCurrentImage, mPreviousImage);
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
    Ptr<FeatureDetector> detector = FastFeatureDetector::create(THRESHOLD); // TODO: Move this ourside function
    vector<KeyPoint> &kp = in_vioImg->GetKPAt(0);

    // TODO: Heuristics using an image pyramid, use low resolution and work up the pyrimad when accuracy fails.
    detector->detect(in_vioImg->GetGrayAt(0), kp);
}

void VIOEngine::TrackFeatures(VIOImage *img_1, VIOImage *img_2) {

    // Set Params
    Size winSize = Size(21 ,21);
    TermCriteria termcrit = TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 30, 0.01);

    // Calc optical flow from FAST points
    calcOpticalFlowPyrLK(img_2->GetGrayAt(0),
                         img_1->GetGrayAt(0),
                         img_2->GetKPAt(0),
                         img_1->GetKPAt(0),
                         mTrackingStatus,
                         mTrackingError,
                         winSize,
                         3,
                         termcrit);

    // TODO: Obtain these from camera calibration once implemented.
    double focalLength = 467;
    cv::Point2d pp(img_1->GetWidth()/2, img_1->GetHeight()/2);

    // Get the essential matrix and recover pose
    Mat E, R, t, mask;
    E = findEssentialMat(img_1->GetKPAt(0), img_2->GetKPAt(0), focalLength, pp, RANSAC, 0.999, 1.0, mask);
    recoverPose(E, img_1->GetKPAt(0), img_2->GetKPAt(0), R, t, focalLength, pp, mask);

    // TODO: Calculate the absolute scale
    double scale = 1.0f;
    Mat t_f = t.clone();
    Mat R_f = R.clone();

    // TODO: Investigate Not entirely sure that this does
    // if ((scale > 0.01) && (t.at<double>(2) > t.at<double>(0)) && (t.at<double>(2) > t.at<))

    // Recover camera pose
    t_f = t_f + scale*(R_f*t);
    R_f = R*R_f;

    // Set current camera pose
    mCurrentPose_R_f = R_f.clone();
    mCurrentPose_t_f = t_f.clone();

}




