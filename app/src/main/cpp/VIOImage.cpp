//
// Created by Jeremy on 3/14/2017.
//

#include "VIOImage.h"
using namespace cv;
using namespace std;

VIOImage::VIOImage(const unsigned int oc,
                   double sr,
                   unsigned int width,
                   unsigned int height) {
    OCTAVE_COUNT = oc;
    SIZE_REDUCED = sr;
    ORIGINAL_WIDTH = width;
    ORIGINAL_HEIGHT = height;
}

void VIOImage::CreateOctaves(long imgAddr) {
    Mat &currImg = *(Mat*) imgAddr;
    double currReducer = SIZE_REDUCED;
    for (int i = 0; i < OCTAVE_COUNT; ++i){
        Mat dstImg;
        int w = round(ORIGINAL_WIDTH * currReducer);
        int h = round(ORIGINAL_HEIGHT * currReducer);
        resize(currImg, dstImg, Size(w, h));
        mImages.push_back(dstImg);
    }
}



void VIOImage::CleanOctaves() {
    // Mat owns data?? Will auto release when unreferenced.
}

unsigned int VIOImage::GetOctaveCount() {
    return OCTAVE_COUNT;
}
