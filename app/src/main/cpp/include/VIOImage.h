//
// Created by Jeremy on 3/14/2017.
//

#ifndef VISUALINERTIALODOMETRY_ANDROID_UW_SAR_2_VIOIMAGE_H
#define VISUALINERTIALODOMETRY_ANDROID_UW_SAR_2_VIOIMAGE_H

#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

class VIOImage {

public:
    VIOImage(const unsigned int oc,
             double sr,
             unsigned int width,
             unsigned int height);
    ~VIOImage();

    void CreateOctaves(long imgAddr);
    void CleanOctaves();
    bool isImageLoaded();

    // Get
    unsigned int GetOctaveCount();
private:
    // Member Variables
    unsigned int OCTAVE_COUNT = 4;
    double SIZE_REDUCED = 0.3f;
    unsigned int ORIGINAL_WIDTH = 0;
    unsigned int ORIGINAL_HEIGHT = 0;
    std::vector<cv::Mat> mImages; // Own
    std::vector<std::vector<cv::KeyPoint>> mKPArray; //Own
};


#endif //VISUALINERTIALODOMETRY_ANDROID_UW_SAR_2_VIOIMAGE_H
