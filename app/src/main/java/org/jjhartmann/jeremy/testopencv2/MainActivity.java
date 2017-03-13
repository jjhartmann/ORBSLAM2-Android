package org.jjhartmann.jeremy.testopencv2;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.SurfaceView;
import android.view.WindowManager;
import android.widget.TextView;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.Size;
import org.opencv.imgproc.Imgproc;

public class MainActivity extends AppCompatActivity implements CvCameraViewListener2
{
    private static final String     TAG = "VIO_UW_SAR::Activity";
    private CameraBridgeViewBase    mOpenCameraView;
    private Object                  mChangeState;
    private boolean                 mIsGoing = false;

    private Mat                     mRgbaImg;
    private Mat                     mGrayImag;

    private IEngineJNI              mEngine;

    // Used to load the 'native-lib' library on application startup.
    static
    {
        System.loadLibrary("native-lib");
    }

    // Call back for async opencv init
    private BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(this) {
        @Override
        public void onManagerConnected(int status) {
            switch (status) {
                case LoaderCallbackInterface.SUCCESS:
                {
                    Log.i(TAG, "OpenCV loaded successfully");
                    mOpenCameraView.enableView();
                    mEngine = new IEngineJNI();
                } break;
                default:
                {
                    Log.i(TAG, "OpenCV Loading error!! - mLoaderCallback");
                    super.onManagerConnected(status);
                } break;
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        setContentView(R.layout.activity_main);

        // Init the camera view
        mOpenCameraView = (CameraBridgeViewBase) findViewById(R.id.tutorial1_activity_java_surface_view);
        mOpenCameraView.setVisibility(SurfaceView.VISIBLE);
        mOpenCameraView.setCvCameraViewListener(this);


        //TextView textView = (TextView) findViewById(R.id.textView);



        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());
    }

    @Override
    public void onResume() {
        super.onResume();
        OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_3_2_0, this, mLoaderCallback);
    }

    /**
     * Dispatch onPause() to fragments.
     */
    @Override
    protected void onPause()
    {
        super.onPause();
        if (mOpenCameraView != null) {
            mOpenCameraView.disableView();
        }
    }

    @Override
    protected void onDestroy()
    {
        super.onDestroy();
        if (mOpenCameraView != null) {
            mOpenCameraView.disableView();
        }
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();



    /**********************************************************************************************
     *  Override methods that implement the CvCameraViewListener2
     **********************************************************************************************/

    /**
     * This method is invoked when camera preview has started. After this method is invoked
     * the frames will start to be delivered to client via the onCameraFrame() callback.
     *
     * @param width  -  the width of the frames that will be delivered
     * @param height - the height of the frames that will be delivered
     */
    @Override
    public void onCameraViewStarted(int width, int height)
    {

    }

    /**
     * This method is invoked when camera preview has been stopped for some reason.
     * No frames will be delivered via onCameraFrame() callback after this method is called.
     */
    @Override
    public void onCameraViewStopped()
    {

    }

    /**
     * This method is invoked when delivery of the frame needs to be done.
     * The returned values - is a modified frame which needs to be displayed on the screen.
     * TODO: pass the parameters specifying the format of the frame (BPP, YUV or RGB and etc)
     *
     * @param inputFrame
     */
    @Override
    public Mat onCameraFrame(CvCameraViewFrame inputFrame)
    {

//        Mat im = inputFrame.rgba();
//        Imgproc.resize(im, im, new Size(640.0, 480.0));
//        long add = im.getNativeObjAddr();

        // Feature detection
        mRgbaImg = inputFrame.rgba();
        mGrayImag = inputFrame.gray();
        mEngine.FindFeatures(mGrayImag.getNativeObjAddr(), mRgbaImg.getNativeObjAddr());


        return mRgbaImg;
    }
}
