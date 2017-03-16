//
// Created by Jeremy on 3/15/2017.
//

#include "VOPose.h"
using namespace SARVIOFusion;

VOPose::VOPose() {

}

VOPose::~VOPose() {

}

void VOPose::AddTranslationVector(cv::Mat in_t) {
    TranslationVector tmp;
    tmp.dx = in_t.at<double>(0);
    tmp.dy = in_t.at<double>(1);
    tmp.dz = in_t.at<double>(2);

    mIntegratedT.dx += tmp.dx;
    mIntegratedT.dy += tmp.dy;
    mIntegratedT.dz += tmp.dz;

    mTranslationArray.push_back(tmp);
}

TranslationVector VOPose::GetIntegratedTranslation() {
    return mIntegratedT;
}

