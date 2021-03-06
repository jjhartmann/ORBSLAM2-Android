//
// Created by Jeremy on 3/15/2017.
//

#include "VOPose.h"
using namespace SARVIOFusion;

VOPose::VOPose() :
    mIsReset(true),
    mRetrackCount(0)
{

}

VOPose::~VOPose() {

}

void VOPose::AddTranslationVector(cv::Mat in_t) {
    mIsReset = false;

    TranslationVector tmp;
    tmp.dx = in_t.at<double>(0);
    tmp.dy = in_t.at<double>(1);
    tmp.dz = in_t.at<double>(2);
    tmp.timePoint = GetTimeStamp();
    tmp.mRetrackIndex = mRetrackCount;

    mIntegratedT.dx += tmp.dx;
    mIntegratedT.dy += tmp.dy;
    mIntegratedT.dz += tmp.dz;
    mIntegratedT.timePoint = GetTimeStamp();
    mIntegratedT.mRetrackIndex = mRetrackCount;

    mTranslationArray.push_back(tmp);
}

void VOPose::AddTranslationVector(SARVIOFusion::TranslationVector in_t) {
    mIsReset = false;
    mIntegratedT.dx += in_t.dx;
    mIntegratedT.dy += in_t.dy;
    mIntegratedT.dz += in_t.dz;

    in_t.timePoint = GetTimeStamp();
    in_t.mRetrackIndex = mRetrackCount;
    mTranslationArray.push_back(in_t);
}

TranslationVector VOPose::GetIntegratedTranslation() {
    return mIntegratedT;
}


void VOPose::Reset() {
    mRetrackCount++;
    mIntegratedT.reset();
    mCurrentT.reset();
    mIsReset = true;
}

bool VOPose::IsReset() {
    return mIsReset;
}

std::vector<SARVIOFusion::TranslationVector> &VOPose::GetRefToTranslationArray() {
    return mTranslationArray;
}

