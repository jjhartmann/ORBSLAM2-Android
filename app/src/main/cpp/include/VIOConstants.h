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

    static int MIN_FEATURES_TO_TRACK = 200;


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
        clock::time_point timePoint;
    };
}

#endif //VISUALINERTIALODOMETRY_ANDROID_UW_SAR_2_VIOCONSTANTS_H
