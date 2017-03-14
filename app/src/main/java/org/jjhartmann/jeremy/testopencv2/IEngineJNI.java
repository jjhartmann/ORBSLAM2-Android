package org.jjhartmann.jeremy.testopencv2;

import org.opencv.core.Point3;

/**
 * VisualInertialOdometry-Android-UW-SAR-2
 * <p>
 * Created by J. J. Hartmann on 3/13/2017.
 * Copyright 2017
 */

public class IEngineJNI
{

    public native void FindFeatures(long matAddrGr, long matAddrRGB);
    public native Point3 VisualOdometry(long matAddrCurrent);
//    public native void Start();
//    public native void Clean();
}
