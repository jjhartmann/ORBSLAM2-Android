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
    mOriginalImage.release();
}

void VIOImage::CreateOctaves(Mat &imgAddr) {

    // Clean memory from previous image
    if (isImageLoaded()){
        mOriginalImage.release();
    }

    imgAddr.copyTo(mOriginalImage);
    int w = round(ORIGINAL_WIDTH * SIZE_REDUCED);
    int h = round(ORIGINAL_HEIGHT * SIZE_REDUCED);

    // Resize image
    Mat newImg;
    resize(mOriginalImage, newImg, Size(w, h));
    mImages.push_back(newImg);

    // Convert Color
    cvtColor(newImg, newImg, COLOR_BGR2GRAY);
    mGrayImgs.push_back(newImg);

    // Push empty array
    mKPArray.push_back(vector<KeyPoint>());

    for (int i = 0; i < OCTAVE_COUNT - 1; ++i){
        Mat dstImg;
        pyrDown(mImages[i], dstImg);
        mImages.push_back(dstImg);

        cvtColor(dstImg, dstImg, COLOR_BGR2GRAY);
        mGrayImgs.push_back(newImg);

        mKPArray.push_back(vector<KeyPoint>());
    }
}



void VIOImage::CleanOctaves() {
    // Mat owns data?? Will auto release when unreferenced. Vecotrs?
    mOriginalImage.release();
}

unsigned int VIOImage::GetOctaveCount() {
    return OCTAVE_COUNT;
}

bool VIOImage::isImageLoaded() {
   return !mOriginalImage.empty();
}

vector<KeyPoint>& VIOImage::GetKPAt(int i){
    if (i < 0 || i >= OCTAVE_COUNT)
        throw INTEGER_OUT_OF_RANGE;
    return mKPArray[i];
}
