//
// Created by Jeremy on 3/14/2017.
//

#include "VIOEngine.h"

#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/video/tracking.hpp>

using namespace SARVIOFusion;
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
    if (mPoseEstimation.IsReset()) {
        DetectFeatures(mCurrentImage);
        mPoseEstimation.AddTranslationVector(TranslationVector(0, 0, 0));
        return;
    }

    // Check if buffers are filled.
//    if (!IsReady()){
//        return;
//    }

    // Track and store rotation and translation vectors
    TrackFeatures(mCurrentImage, mPreviousImage);
}

bool VIOEngine::IsReady() {
    return mCurrentImage->isImageLoaded() && mPreviousImage->isImageLoaded();
}

void VIOEngine::ShiftBuffers() {
    // Clean and swap buffers.
    mPreviousImage->CleanOctaves();
    std::swap(mCurrentImage, mPreviousImage);
    //VIOEngine::Swap(mCurrentImage, mPreviousImage);
}

void VIOEngine::PrintPoint(cv::Mat &in_img, int xOffset, int yOffSet) {
    if (mCurrentPose_t_f.empty() || mCurrentPose_R_f.empty())
        return;

    TranslationVector integratedPose = mPoseEstimation.GetIntegratedTranslation();
    int x = int(integratedPose.dx) + 640; // X
    int y = int(integratedPose.dy) + 360; // Y
    circle(in_img, Point(x, y) ,1, CV_RGB(255,0,0), 2);

    if (DEBUG_MODE) {
        char text[100];
        rectangle(in_img, Point(10, 30), Point(590, 50), CV_RGB(0, 0, 0), CV_FILLED);
        sprintf(text, "Coordinates: x = %02fm y = %02fm z = %02fm", mCurrentPose_t_f.at<double>(0),
                mCurrentPose_t_f.at<double>(1), mCurrentPose_t_f.at<double>(2));
        putText(in_img, text, Point(10, 50), FONT_HERSHEY_PLAIN, 1, Scalar::all(255), 1, 8);

        rectangle(in_img, Point(10, 60), Point(590, 100), CV_RGB(0, 0, 0), CV_FILLED);
        sprintf(text, "Coordinates: x = %02fm y = %02fm z = %02fm",integratedPose.dx,
                integratedPose.dy, integratedPose.dz);
        putText(in_img, text, Point(10, 80), FONT_HERSHEY_PLAIN, 1, Scalar::all(255), 1, 8);
    }


}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// Private Methods
/// : TODO: Refactor this into own class?
////////////////////////////////////////////////////////////////////////////////////////////////////
void VIOEngine::DetectFeatures(VIOImage *in_vioImg) {


    // Params
    int THRESHOLD = 50;
    int index = 0;
    Ptr<FeatureDetector> detector = FastFeatureDetector::create(THRESHOLD); // TODO: Move this ourside function
    vector<KeyPoint> &kp = in_vioImg->GetKPAt(index);

    // TODO: Heuristics using an image pyramid, use low resolution and work up the pyrimad when accuracy fails.
    detector->detect(in_vioImg->GetGrayAt(index), kp);

    // Convert to Point2F array
    KeyPoint::convert(kp, in_vioImg->GetKPP2FAt(index));
}

void VIOEngine::TrackFeatures(VIOImage *img_1, VIOImage *img_2) {

    // Set Params
    Size winSize = Size(21 ,21);
    TermCriteria termcrit = TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 30, 0.01);

    // Calc optical flow from FAST points
    Mat &grayImg_1 = img_1->GetGrayAt(0);
    uchar type_1 = grayImg_1.type() & CV_MAT_DEPTH_MASK;

    Mat &grayImg_2 = img_2->GetGrayAt(0);
    uchar type_2 = grayImg_1.type() & CV_MAT_DEPTH_MASK;

    vector<Point2f> &kp_1 = img_1->GetKPP2FAt(0);
    vector<Point2f> &kp_2 = img_2->GetKPP2FAt(0);

    // Check for for  features to track
    CalcOpticalFlow(grayImg_1, grayImg_2, kp_1, kp_2, mTrackingStatus, mTrackingError);

    // If features being tracked fall below some threshold, Restart Flow
    if (mPreviousImage->GetKPAt(0).size() < MIN_FEATURES_TO_TRACK) {
        DetectFeatures(mPreviousImage);
        CalcOpticalFlow(grayImg_1, grayImg_2, kp_1, kp_2, mTrackingStatus, mTrackingError);
    }

    // TODO: Obtain these from camera calibration once implemented.
    double focalLength = 467;
    cv::Point2d pp(img_1->GetWidth()/2, img_1->GetHeight()/2);

    // Get the essential matrix and recover pose
    Mat E, R, t, mask;
    E = findEssentialMat(kp_1, kp_2, focalLength, pp, RANSAC, 0.999, 1.0, mask);
    recoverPose(E,kp_1, kp_2, R, t, focalLength, pp, mask);

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

    mPoseEstimation.AddTranslationVector(mCurrentPose_t_f);
}

void VIOEngine::CalcOpticalFlow(cv::Mat img_1,
                                cv::Mat img_2,
                                std::vector<cv::Point2f> &points_1,
                                std::vector<cv::Point2f> &points_2,
                                std::vector<uchar> &status,
                                std::vector<float> &err) {

    // Set Params: TODO: Pull these from a calibration class?
    Size winSize = Size(21 ,21);
    TermCriteria termcrit = TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 30, 0.01);
    int maxLevel = 3;
    int oFlags = 0;
    double minEgienValue = 0.001;

    calcOpticalFlowPyrLK(img_2,
                         img_1,
                         points_2,
                         points_1,
                         status,
                         err,
                         winSize,
                         maxLevel,
                         termcrit,
                         oFlags,
                         minEgienValue);

    // Remove bad or untracked features.
    int newSize = 0;
    for (int i = 0; i < status.size(); ++i) {
        Point2f pt = points_1[i];
        if (status[i] == 0 || pt.x < 0 || pt.y < 0) {
            if (pt.x < 0 || pt.y < 0){
                status[i] = 0;
            }
        } else {
            newSize++;
        }
    }

    vector<cv::Point2f> newPoints_1(newSize, Point2f(0.0, 0.0));
    vector<cv::Point2f> newPoints_2(newSize, Point2f(0.0, 0.0));

    int nIndex = 0;
    for (int i = 0; i < status.size(); ++i) {
        if (status[i] > 0) {
            newPoints_1[nIndex] = points_1[i];
            newPoints_2[nIndex] = points_2[i];
            nIndex++;
        }
    }

    // Copy to in out vectors.
    points_1 = newPoints_1;
    points_2 = newPoints_2;
}




