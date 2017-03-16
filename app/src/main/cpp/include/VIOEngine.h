//
// Created by Jeremy on 3/14/2017.
//

#ifndef VISUALINERTIALODOMETRY_ANDROID_UW_SAR_2_VIOENGINE_H
#define VISUALINERTIALODOMETRY_ANDROID_UW_SAR_2_VIOENGINE_H


#include "VIOImage.h"
#include "VOPose.h"
#include "VIOConstants.h"

class VIOEngine {
public:
    VIOEngine();
    ~VIOEngine();

    void Init(int width, int height);
    bool IsReady();
    void ProcessImage(cv::Mat &inputImg);
    void ShiftBuffers();
    void PrintPoint(cv::Mat &in_img, int xOffset = 0, int yOffSet = 0); // Don't Own
private:
    // TODO: Refactor image and status information into internal class
    VIOImage *mCurrentImage;  // OWN
    VIOImage *mPreviousImage; // OWN
    VIOImage *mPendingImage;  // OWN
    std::vector<uchar> mTrackingStatus;
    std::vector<float> mTrackingError;

    // TODO: Refactor camera pose information from optical flow
    cv::Mat mCurrentPose_R_f;
    cv::Mat mCurrentPose_t_f;
    VOPose mPoseEstimation;

    // Private Methods
    void DetectFeatures(VIOImage *in_vioImg);
    void TrackFeatures(VIOImage *img_1, VIOImage *img_2);
    void CalcOpticalFlow(cv::Mat img_1,
                         cv::Mat img_2,
                         std::vector<cv::Point2d> &points_1,
                         std::vector<cv::Point2f> &points_2,
                         std::vector<uchar> &status);


    // Static Methods
public:
    static void inline Swap(VIOImage *img1, VIOImage *img2){
//        VIOImage **tmp = img2;
//        *img2 = *img1;
//        *img1 = tmp;
        std::swap(img1, img2);
    }


};


#endif //VISUALINERTIALODOMETRY_ANDROID_UW_SAR_2_VIOENGINE_H
