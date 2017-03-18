//
// Created by Jeremy on 3/14/2017.
//

#ifndef VISUALINERTIALODOMETRY_ANDROID_UW_SAR_2_VIOIMAGE_H
#define VISUALINERTIALODOMETRY_ANDROID_UW_SAR_2_VIOIMAGE_H

#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include "VIOConstants.h"

class VIOImage {

public:
    VIOImage(const unsigned int oc,
             double sr,
             unsigned int width,
             unsigned int height);
    ~VIOImage();

    void CreateOctaves(cv::Mat &imgAddr);
    void CleanOctaves();
    bool isImageLoaded();

    // Inline functions for fast reterival.
    inline cv::Mat& operator[](int i) {
        if (i < 0 || i >= OCTAVE_COUNT)
            throw SARVIOFusion::INTEGER_OUT_OF_RANGE;

        return mImages[i];
    }

    inline cv::Mat& GetGrayAt(int i = 0) {
        if (i < 0 || i >= OCTAVE_COUNT)
            throw SARVIOFusion::INTEGER_OUT_OF_RANGE;

        return mGrayImgs[i];
    }

    // Get
    unsigned int GetOctaveCount();
    unsigned int GetWidth(int i = 0);
    unsigned int GetHeight(int i = 0);
    unsigned int GetOriginalHeight();
    unsigned int GetOriginalWidth();
    double GetScale(int i = 0);
    // Set
    std::vector<cv::KeyPoint>& GetKPAt(int i = 0);
    std::vector<cv::Point2f>& GetKPP2FAt(int i = 0);

private:
    // Member Variables
    unsigned int OCTAVE_COUNT = 4;
    double SIZE_REDUCED = 0.5f;
    unsigned int ORIGINAL_WIDTH = 0;
    unsigned int ORIGINAL_HEIGHT = 0;
    std::vector<cv::Mat> mGrayImgs;
    std::vector<cv::Mat> mImages; // Ref mOriginImage
    std::vector<std::vector<cv::KeyPoint>> mKPArray; // Ref mOriginalImage
    std::vector<std::vector<cv::Point2f>> mKPPointFArray;
    cv::Mat mOriginalImage; // OWN
};


#endif //VISUALINERTIALODOMETRY_ANDROID_UW_SAR_2_VIOIMAGE_H
