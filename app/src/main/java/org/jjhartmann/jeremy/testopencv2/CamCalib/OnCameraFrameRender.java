package org.jjhartmann.jeremy.testopencv2.CamCalib;

/**
 * VisualInertialOdometry-Android-UW-SAR-2
 * <p>
 * Created by J. J. Hartmann on 3/19/2017.
 * Email: j3hartma@uwaterloo.ca
 * Copyright 2017
 */

import java.util.ArrayList;
import java.util.List;

import org.jjhartmann.jeremy.testopencv2.R;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.MatOfPoint;
import org.opencv.core.Point;
import org.opencv.core.Range;
import org.opencv.core.Scalar;
import org.opencv.imgproc.Imgproc;

import android.content.res.Resources;

class UndistortionFrameRender extends FrameRender {
    public UndistortionFrameRender(CameraCalibrator calibrator) {
        mCalibrator = calibrator;
    }

    @Override
    public Mat render(CvCameraViewFrame inputFrame) {
        Mat renderedFrame = new Mat(inputFrame.rgba().size(), inputFrame.rgba().type());
        Imgproc.undistort(inputFrame.rgba(), renderedFrame,
                mCalibrator.getCameraMatrix(), mCalibrator.getDistortionCoefficients());

        return renderedFrame;
    }
}

class ComparisonFrameRender extends FrameRender {
    private int mWidth;
    private int mHeight;
    private Resources mResources;
    public ComparisonFrameRender(CameraCalibrator calibrator, int width, int height, Resources resources) {
        mCalibrator = calibrator;
        mWidth = width;
        mHeight = height;
        mResources = resources;
    }

    @Override
    public Mat render(CvCameraViewFrame inputFrame) {
        Mat undistortedFrame = new Mat(inputFrame.rgba().size(), inputFrame.rgba().type());
        Imgproc.undistort(inputFrame.rgba(), undistortedFrame,
                mCalibrator.getCameraMatrix(), mCalibrator.getDistortionCoefficients());

        Mat comparisonFrame = inputFrame.rgba();
        undistortedFrame.colRange(new Range(0, mWidth / 2)).copyTo(comparisonFrame.colRange(new Range(mWidth / 2, mWidth)));
        List<MatOfPoint> border = new ArrayList<MatOfPoint>();
        final int shift = (int)(mWidth * 0.005);
        border.add(new MatOfPoint(new Point(mWidth / 2 - shift, 0), new Point(mWidth / 2 + shift, 0),
                new Point(mWidth / 2 + shift, mHeight), new Point(mWidth / 2 - shift, mHeight)));
        Imgproc.fillPoly(comparisonFrame, border, new Scalar(255, 255, 255));

        Imgproc.putText(comparisonFrame, mResources.getString(R.string.original), new Point(mWidth * 0.1, mHeight * 0.1),
                Core.FONT_HERSHEY_SIMPLEX, 1.0, new Scalar(255, 255, 0));
        Imgproc.putText(comparisonFrame, mResources.getString(R.string.undistorted), new Point(mWidth * 0.6, mHeight * 0.1),
                Core.FONT_HERSHEY_SIMPLEX, 1.0, new Scalar(255, 255, 0));

        return comparisonFrame;
    }
}

public class OnCameraFrameRender {
    private FrameRender mFrameRender;
    public OnCameraFrameRender(FrameRender frameRender) {
        mFrameRender = frameRender;
    }
    public Mat render(CvCameraViewFrame inputFrame) {
        return mFrameRender.render(inputFrame);
    }
}