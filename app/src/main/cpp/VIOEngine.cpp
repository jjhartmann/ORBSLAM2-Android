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
    mCurrentImage = new VIOImage(4, 0.2, width, height);
    mPreviousImage = new VIOImage(4, 0.2, width, height);
    mPendingImage = new VIOImage(4, 0.2, width, height);
}

void VIOEngine::ProcessImage(cv::Mat &inputImg) {
    // Create Octaves and Gray Images
    mCurrentImage->CreateOctaves(inputImg);

    // Detect fast features and store them in array.
    if (!IsReady()) {
        // Detect features in current image
        DetectFeatures(mCurrentImage);

        // Add origin to pose estimations.
        mPoseEstimation.AddTranslationVector(TranslationVector(0, 0, 0));
        return;
    }

    // Track and store rotation and translation vectors
    TrackFeatures(mCurrentImage, mPreviousImage);
}

bool VIOEngine::IsReady() {
    return !mPoseEstimation.IsReset() &&
            mPreviousImage->isImageLoaded() &&
            !mPreviousImage->GetKPP2FAt().empty() &&
            mPreviousImage->GetKPP2FAt().size() > MIN_FEATURES_TO_TRACK;
}

void VIOEngine::ShiftBuffers() {
    // Clean and swap buffers.
    mPreviousImage->CleanOctaves();
    std::swap(mCurrentImage, mPreviousImage);
}
void VIOEngine::PrintPoint(cv::Mat &in_img, int xOffset, int yOffSet) {
    if (!IsReady())
        return;

    TranslationVector integratedPose = mPoseEstimation.GetIntegratedTranslation();
    vector<TranslationVector> &mWalk = mPoseEstimation.GetRefToTranslationArray();

    double ix = xOffset, iy = yOffSet, iz = 0;
    int current_track_count = 0;
    int r = 255, g = 255, b = 0;
    for (TranslationVector v : mWalk) {
        if (v.mRetrackIndex != current_track_count){
            current_track_count = v.mRetrackIndex;
            r = (int)((13 % r) * ix) % 255;
            g = (int)((13 % g) * iy) % 255;
            ix = xOffset;
            iy = yOffSet;
            iz = 0;
        }
        ix += v.dx;
        iy += v.dy;
        iz += v.dz;
        circle(in_img, Point(ix, iy) ,1, CV_RGB(r,g,b), 2);
    }

    int x = int(integratedPose.dx) + xOffset; // X
    int y = int(integratedPose.dy) + yOffSet; // Y
    circle(in_img, Point(x, y) ,1, CV_RGB(255,255,0), 2);

    // Print Text to screen
    char text[100];
    rectangle(in_img, Point(10, 30), Point(590, 50), CV_RGB(0, 0, 0), CV_FILLED);
    sprintf(text, "Coordinates: x = %02fm y = %02fm z = %02fm", mCurrentPose_t_f.at<double>(0),
            mCurrentPose_t_f.at<double>(1), mCurrentPose_t_f.at<double>(2));
    putText(in_img, text, Point(10, 50), FONT_HERSHEY_PLAIN, 1, Scalar::all(255), 1, 8);

    rectangle(in_img, Point(10, 60), Point(590, 80), CV_RGB(0, 0, 0), CV_FILLED);
    sprintf(text, "Coordinates: x = %02fm y = %02fm z = %02fm",integratedPose.dx,
            integratedPose.dy, integratedPose.dz);
    putText(in_img, text, Point(10, 80), FONT_HERSHEY_PLAIN, 1, Scalar::all(255), 1, 8);

    rectangle(in_img, Point(10, 90), Point(590, 110), CV_RGB(0, 0, 0), CV_FILLED);
    sprintf(text, "Coordinates: x = %02fm y = %02fm z = %02fm",ix, iy, iz);
    putText(in_img, text, Point(10, 110), FONT_HERSHEY_PLAIN, 1, Scalar::all(255), 1, 8);


    // Print Features
    double scale = mCurrentImage->GetScale();
    vector<Point2f> kp_1 = mCurrentImage->GetKPP2FAt();
    vector<Point2f> kp_2 = mPreviousImage->GetKPP2FAt();

    bool ecept = false;
    for (int i = 0; i < kp_1.size(); ++i) {
        int prev_x = (int)scale * kp_2[i].x;
        int prev_y = (int)scale * kp_2[i].y;
        int curr_x = (int)scale * kp_1[i].x;
        int curr_y = (int)scale * kp_1[i].y;

        if (prev_x < 0 || prev_y < 0 ||  prev_x >= mCurrentImage->GetOriginalWidth() || prev_y >= mCurrentImage->GetOriginalHeight()){
            ecept = true;
        }

        circle(in_img, Point(prev_x, prev_y), 10, Scalar(255,0,0,255));
        circle(in_img, Point(curr_x, curr_y), 10, Scalar(0,255,0,255));
        arrowedLine(in_img, Point(prev_x, prev_y), Point(curr_x, curr_y), Scalar(255,255,255), 1);
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

    // Calc optical flow from FAST points
    Mat &grayImg_1 = img_1->GetGrayAt(0);
    uchar type_1 = grayImg_1.type() & CV_MAT_DEPTH_MASK;

    Mat &grayImg_2 = img_2->GetGrayAt(0);
    uchar type_2 = grayImg_1.type() & CV_MAT_DEPTH_MASK;

    vector<Point2f> &kp_1 = img_1->GetKPP2FAt(0);
    vector<Point2f> &kp_2 = img_2->GetKPP2FAt(0);
    kp_1.clear();

    // TODO: Optimise based on where furthes feature is.
    if (kp_2.size() < AVG_FEATURES_TO_TRACK) {
        DetectFeatures(mPreviousImage);
    }

    // Check for for  features to track
    CalcOpticalFlow(grayImg_1, grayImg_2, kp_1, kp_2, mTrackingStatus, mTrackingError);

    // TODO: have tracking fail only after a number of set failures. This can be accomplished thorugh a member var that holds the number of dropped frames.
    // If features being tracked fall below some threshold, Restart Flow
    if (kp_2.size() < MIN_FEATURES_TO_TRACK || kp_1.size() < MIN_FEATURES_TO_TRACK) {
        DetectFeatures(mPreviousImage);
        kp_1.clear();
        CalcOpticalFlow(grayImg_1, grayImg_2, kp_1, kp_2, mTrackingStatus, mTrackingError);
        if (kp_2.size() < MIN_FEATURES_TO_TRACK || kp_2.size() < MIN_FEATURES_TO_TRACK) {
            mCurrentImage->CleanOctaves();
            mPoseEstimation.Reset();
            return;
        }
    }

    // TODO: Obtain these from camera calibration once implemented.
    double focalLength = 23;
    cv::Point2d pp(img_1->GetWidth()/2, img_1->GetHeight()/2);

    // Get the essential matrix and recover pose
    Mat E, R, t, mask;
    E = findEssentialMat(kp_1, kp_2, focalLength, pp, RANSAC, 0.999, 1.0, mask);
    if (E.cols != 3 && E.rows != 3){return; }
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
    int w = mCurrentImage->GetWidth();
    int h = mCurrentImage->GetHeight();
    int newSize = 0;
    for (int i = 0; i < status.size(); ++i) {
        Point2f pt = points_1[i];
        if (status[i] == 0 || pt.x < 0 || pt.y < 0 || pt.x >= w || pt.y >= h) {
            if (pt.x < 0 || pt.y < 0 ||  pt.x >= w || pt.y >= h){
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




