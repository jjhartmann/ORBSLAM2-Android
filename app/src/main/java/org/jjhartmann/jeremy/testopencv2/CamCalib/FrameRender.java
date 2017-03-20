package org.jjhartmann.jeremy.testopencv2.CamCalib;

import org.opencv.android.CameraBridgeViewBase;
import org.opencv.core.Mat;

abstract public class FrameRender {
    protected CameraCalibrator mCalibrator;

    public abstract Mat render(CameraBridgeViewBase.CvCameraViewFrame inputFrame);
}
