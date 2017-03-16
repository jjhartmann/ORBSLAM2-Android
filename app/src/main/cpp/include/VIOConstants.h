//
// Created by Jeremy on 3/14/2017.
//

#ifndef VISUALINERTIALODOMETRY_ANDROID_UW_SAR_2_VIOCONSTANTS_H
#define VISUALINERTIALODOMETRY_ANDROID_UW_SAR_2_VIOCONSTANTS_H

// Useful macros to use in code.
#define DEBUG_MODE 1

#include <chrono>
namespace SARVIOFusion {
    static enum ERROR_CODE {
        INTEGER_OUT_OF_RANGE = 0
    } VIOERROR;

    static int MIN_FEATURES_TO_TRACK = 400;


    typedef std::chrono::high_resolution_clock clock;
    typedef std::chrono::microseconds microseconds;
    typedef std::chrono::milliseconds milliseconds;

    static clock::time_point GetTimeStamp() {return clock::now(); }
    static microseconds GetIntervalUs(const clock::time_point &t0, const clock::time_point &t1){
        return std::chrono::duration_cast<microseconds>(t1 - t0);
    }
    static milliseconds GetIntervalMs(const clock::time_point &t0, const clock::time_point &t1) {
        return std::chrono::duration_cast<milliseconds>(t1 - t0);
    }

    struct TranslationVector {
        double dx;
        double dy;
        double dz;
        uint mRetrackIndex;
        clock::time_point timePoint;

        TranslationVector(): dx(0), dy(0), dz(0), mRetrackIndex(0) {}
        TranslationVector(double in_x, double in_y, double in_z){
            dx = in_x;
            dy = in_y;
            dz = in_z;
            mRetrackIndex = 0;
        }

        void reset() {
            dx = 0;
            dy = 0;
            dz = 0;
            timePoint = GetTimeStamp();
        }
    };
}

#endif //VISUALINERTIALODOMETRY_ANDROID_UW_SAR_2_VIOCONSTANTS_H
