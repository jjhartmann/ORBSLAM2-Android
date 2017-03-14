//
// Created by Jeremy on 3/14/2017.
//

#ifndef VISUALINERTIALODOMETRY_ANDROID_UW_SAR_2_VIOENGINE_H
#define VISUALINERTIALODOMETRY_ANDROID_UW_SAR_2_VIOENGINE_H


#include "VIOImage.h"

class VIOEngine {
public:
    VIOEngine();
    ~VIOEngine();

    void Init(int width, int height);
    void ProcessImage(cv::Mat inputImg);

private:
    VIOImage *mCurrentImage;  // OWN
    VIOImage *mPreviousImage; // OWN
    VIOImage *mPendingImage;  // OWN




    // Static Methods
public:
    static void inline Swap(VIOImage *img1, VIOImage *img2){
        VIOImage *tmp = img1;
        img1 = img2;
        img2 = tmp;
    }


};


#endif //VISUALINERTIALODOMETRY_ANDROID_UW_SAR_2_VIOENGINE_H
