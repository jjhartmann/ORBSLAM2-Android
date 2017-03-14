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

VIOImage::~VIOImage(){
    ; // TODO: Clean up owned data
}

void VIOImage::CreateOctaves(long imgAddr) {
    Mat &currImg = *(Mat*) imgAddr;
    int w = round(ORIGINAL_WIDTH * SIZE_REDUCED);
    int h = round(ORIGINAL_HEIGHT * SIZE_REDUCED);

    // Resize image
    Mat newImg;
    resize(currImg, newImg, Size(w, h));
    mImages.push_back(newImg);
    for (int i = 0; i < OCTAVE_COUNT - 1; ++i){
        Mat dstImg;
        pyrDown(mImages[i], dstImg);
        mImages.push_back(dstImg);
    }
}



void VIOImage::CleanOctaves() {
    // Mat owns data?? Will auto release when unreferenced.
}

unsigned int VIOImage::GetOctaveCount() {
    return OCTAVE_COUNT;
}
