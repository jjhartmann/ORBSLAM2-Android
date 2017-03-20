package org.jjhartmann.jeremy.testopencv2;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.res.Resources;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Button;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;

import org.jjhartmann.jeremy.testopencv2.Utils.AssetManagerHelper;
import org.jjhartmann.jeremy.testopencv2.CamCalib.*;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.utils.*;

import org.yaml.snakeyaml.Yaml;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.util.Map;

public class CameraCalibrationActivity
        extends Activity
        implements CvCameraViewListener2
{
    private Mat                     mRgbaImage;
    private Yaml                    mYamlParser;
    private Map<String, Object>     mSettingsFile;
    private String                  mStringOutputtoConsole;
    private ScrollView              mScrollDebugView;
    private TextView                mDebugTextView;
    private CameraBridgeViewBase    mOpenCameraView;
    private OnCameraFrameRender     mOnCameraFrameRender;

    private int                     mWidth;
    private int                     mHeight;
    private CameraCalibrator        mCalibrator;

    private String TAG = "CAM_CALIB";

    // Call back for async opencv init
    private BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(this) {
        @Override
        public void onManagerConnected(int status) {
            switch (status) {
                case LoaderCallbackInterface.SUCCESS:
                {
                    Log.i(TAG, "OpenCV loaded successfully");
                    mOpenCameraView.enableView();
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
        setContentView(R.layout.activity_camera_calibration);

        Button bCaptureImage = (Button) findViewById(R.id.button_capture_image);
        bCaptureImage.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                // Capture current image.
                mCalibrator.addCorners();
            }
        });

        Button bCalibrate = (Button) findViewById(R.id.button_calibrate);
        bCalibrate.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                final Resources res = getResources();
                if (mCalibrator.getCornersBufferSize() < 2) {
                    (Toast.makeText(CameraCalibrationActivity.this, res.getString(R.string.more_samples), Toast.LENGTH_SHORT)).show();
                    return;
                }
                mOnCameraFrameRender = new OnCameraFrameRender(new PreviewFrameRender());
                new AsyncTask<Void, Void, Void>() {
                    private ProgressDialog calibrationProgress;

                    @Override
                    protected void onPreExecute() {
                        calibrationProgress = new ProgressDialog(CameraCalibrationActivity.this);
                        calibrationProgress.setTitle(res.getString(R.string.calibrating));
                        calibrationProgress.setMessage(res.getString(R.string.please_wait));
                        calibrationProgress.setCancelable(false);
                        calibrationProgress.setIndeterminate(true);
                        calibrationProgress.show();
                    }

                    @Override
                    protected Void doInBackground(Void... arg0) {
                        mCalibrator.calibrate();
                        return null;
                    }

                    @Override
                    protected void onPostExecute(Void result) {
                        calibrationProgress.dismiss();
                        mCalibrator.clearCorners();
                        mOnCameraFrameRender = new OnCameraFrameRender(new CalibrationFrameRender(mCalibrator));
                        String resultMessage = (mCalibrator.isCalibrated()) ?
                                res.getString(R.string.calibration_successful)  + " " + mCalibrator.getAvgReprojectionError() :
                                res.getString(R.string.calibration_unsuccessful);
                        (Toast.makeText(CameraCalibrationActivity.this, resultMessage, Toast.LENGTH_SHORT)).show();

                        if (mCalibrator.isCalibrated()) {
                            // TODO: Save calibration to external storage.
                        }
                    }
                }.execute();
            }
        });


        mScrollDebugView = (ScrollView) findViewById(R.id.scrollView_debug);
        mDebugTextView = (TextView) findViewById(R.id.textView_DebugConsole);

        File sFile = new File(Environment.getExternalStorageDirectory().getAbsolutePath(), Constants.CAMERA_CALIBRATION_FILE_NAME);
        if (true || !sFile.exists()){
            // TODO: Load from assests
            AssetManagerHelper amanger = new AssetManagerHelper(getAssets());
            amanger.copyAsset(Constants.CAMERA_CALIBRATION_FILE_NAME);
        }

        try
        {
            InputStream sStream = new FileInputStream(sFile);
            mYamlParser = new Yaml();
            mSettingsFile = (Map<String, Object>) mYamlParser.load(sStream);
        } catch (Exception e){
            Log.e(TAG, "Exception Camera Calibration class: " + e.toString());
        }

        // Init the camera view
        mOpenCameraView = (CameraBridgeViewBase) findViewById(R.id.java_surface_view_1);
        mOpenCameraView.setVisibility(SurfaceView.VISIBLE);
        mOpenCameraView.setCvCameraViewListener(this);

        mStringOutputtoConsole = new String();

        // Print current config file
        for (Map.Entry<String, Object> entry : mSettingsFile.entrySet()) {
            String key = entry.getKey();
            String val = "";
            if (entry.getValue().getClass() == Double.class){
                val = Double.toString((Double) entry.getValue());
            } else if(entry.getValue().getClass() == Integer.class) {
                val = Integer.toString((Integer) entry.getValue());
            }
            mStringOutputtoConsole = mStringOutputtoConsole + key + ": " + val + "\n" ;

        }

        mDebugTextView.setText(mStringOutputtoConsole);
        mScrollDebugView.fullScroll(ScrollView.FOCUS_DOWN);




    }

    @Override
    public void onPause()
    {
        super.onPause();
        if (mOpenCameraView != null)
            mOpenCameraView.disableView();
    }

    @Override
    public void onResume()
    {
        super.onResume();
        if (!OpenCVLoader.initDebug()) {
            Log.d(TAG, "Internal OpenCV library not found. Using OpenCV Manager for initialization");
            OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_3_2_0, this, mLoaderCallback);
        } else {
            Log.d(TAG, "OpenCV library found inside package. Using it!");
            mLoaderCallback.onManagerConnected(LoaderCallbackInterface.SUCCESS);
        }
    }

    public void onDestroy() {
        super.onDestroy();
        if (mOpenCameraView != null)
            mOpenCameraView.disableView();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // OpenCV 3.2
    //
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////

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
        if (mWidth != width || mHeight != height)
        {
            mWidth = width;
            mHeight = height;
            mCalibrator = new CameraCalibrator(mWidth, mHeight);
        }

        // set up frame renderer
        mOnCameraFrameRender = new OnCameraFrameRender(new CalibrationFrameRender(mCalibrator));
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
    public Mat onCameraFrame(CameraBridgeViewBase.CvCameraViewFrame inputFrame)
    {
        mRgbaImage = inputFrame.rgba();
        return mOnCameraFrameRender.render(inputFrame);
    }
}
