//
// Created by Jeremy on 3/14/2017.
//

#include "VIOImage.h"
using namespace cv;
using namespace std;
using namespace SARVIOFusion;

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
    CleanOctaves();
}

void VIOImage::CreateOctaves(Mat &imgAddr) {

    // Clean memory from previous image
    if (isImageLoaded()){
        mOriginalImage.release();
    }

    // TODO: reduce the scale right here?
    imgAddr.copyTo(mOriginalImage);
    int w = round(ORIGINAL_WIDTH * SIZE_REDUCED);
    int h = round(ORIGINAL_HEIGHT * SIZE_REDUCED);

    // Resize image
    Mat newImg;
    resize(mOriginalImage, newImg, Size(w, h));
    mImages.push_back(newImg);

    // Convert Color
    cvtColor(newImg, newImg, COLOR_RGBA2GRAY);
    mGrayImgs.push_back(newImg);

    // Push empty array
    mKPArray.push_back(vector<KeyPoint>());

    for (int i = 0; i < OCTAVE_COUNT - 1; ++i){
        Mat dstImg;
        pyrDown(mImages[i], dstImg);
        mImages.push_back(dstImg);

        cvtColor(dstImg, dstImg, COLOR_RGBA2GRAY);
        mGrayImgs.push_back(newImg);

        mKPArray.push_back(vector<KeyPoint>());
        mKPPointFArray.push_back(vector<Point2f>());
    }
}



void VIOImage::CleanOctaves() {
    // Mat owns data?? Will auto release when unreferenced. Vecotrs?
    mOriginalImage.release();

    // Release Containers in vector.
    // TODO: Optimise
    mImages.clear();
    mGrayImgs.clear();
    mKPArray.clear();
    mKPPointFArray.clear();
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

unsigned int VIOImage::GetHeight() {
    if (mGrayImgs.size() > 0)
        return mGrayImgs[0].rows;
    return 0;
}

unsigned int VIOImage::GetWidth() {
    if (mGrayImgs.size() > 0)
        return mGrayImgs[0].cols;
    return 0;
}

double VIOImage::GetScale(int i) {
    return 1 / SIZE_REDUCED ;
}

std::vector<cv::Point2f> &VIOImage::GetKPP2FAt(int i) {
    if (i < 0 || i >= OCTAVE_COUNT)
        throw INTEGER_OUT_OF_RANGE;
    return mKPPointFArray[i];
}

