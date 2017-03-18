package org.jjhartmann.jeremy.testopencv2.JNIBindings;

/**
 * VisualInertialOdometry-Android-UW-SAR-2
 * <p>
 * Created by J. J. Hartmann on 3/18/2017.
 * Email: j3hartma@uwaterloo.ca
 * Copyright 2017
 */

public class IORBSEngineJNI
{
    public native void GLESInit();
    public native void GLESRender();
    public native void GLESResize();
}
