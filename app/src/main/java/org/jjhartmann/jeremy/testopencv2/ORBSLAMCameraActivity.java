package org.jjhartmann.jeremy.testopencv2;

import android.annotation.SuppressLint;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.os.Environment;
import android.support.v7.app.ActionBar;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.SurfaceView;
import android.view.View;
import android.widget.ImageView;

import org.jjhartmann.jeremy.testopencv2.JNIBindings.IORBSEngineJNI;
import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;
import org.opencv.android.CameraGLSurfaceView;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.android.Utils;
import org.opencv.core.Mat;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/**
 * An example full-screen activity that shows and hides the system UI (i.e.
 * status bar and navigation/system bar) with user interaction.
 */
public class ORBSLAMCameraActivity
        extends AppCompatActivity
        implements CvCameraViewListener2
{

    // Full Screen Activity Default Code
    private static final long UI_ANIMATION_DELAY = 200;
    private final Handler mHideHandler = new Handler();
    private final Runnable mHidePart2Runnable = new Runnable()
    {
        @SuppressLint("InlinedApi")
        @Override
        public void run()
        {
            // Delayed removal of status and navigation bar

            // Note that some of these constants are new as of API 16 (Jelly Bean)
            // and API 19 (KitKat). It is safe to use them, as they are inlined
            // at compile-time and do nothing on earlier devices.
            mOpenCameraView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_LOW_PROFILE
                    | View.SYSTEM_UI_FLAG_FULLSCREEN
                    | View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                    | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                    | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                    | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);
        }
    };
    private View mControlsView;
    private final Runnable mShowPart2Runnable = new Runnable()
    {
        @Override
        public void run()
        {
            // Delayed display of UI elements
            ActionBar actionBar = getSupportActionBar();
            if (actionBar != null)
            {
                actionBar.show();
            }
            mControlsView.setVisibility(View.VISIBLE);
        }
    };
    private boolean mVisible;
    private final Runnable mHideRunnable = new Runnable()
    {
        @Override
        public void run()
        {
            hide();
        }
    };

      private void toggle()
    {
        if (mVisible)
        {
            hide();
        }
        else
        {
            show();
        }
    }

    private void hide()
    {
        // Hide UI first
        ActionBar actionBar = getSupportActionBar();
        if (actionBar != null)
        {
            actionBar.hide();
        }
        mControlsView.setVisibility(View.GONE);
        mVisible = false;

        // Schedule a runnable to remove the status and navigation bar after a delay
        mHideHandler.removeCallbacks(mShowPart2Runnable);
        mHideHandler.postDelayed(mHidePart2Runnable, UI_ANIMATION_DELAY);
    }

    @SuppressLint("InlinedApi")
    private void show()
    {
        // Show the system bar
        mOpenCameraView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION);
        mVisible = true;

        // Schedule a runnable to display UI elements after a delay
        mHideHandler.removeCallbacks(mHidePart2Runnable);
        mHideHandler.postDelayed(mShowPart2Runnable, UI_ANIMATION_DELAY);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Build Start
    //
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    private static final String     TAG = "VIO::ORBSLAMActivity";
    private CameraBridgeViewBase    mOpenCameraView;
    private CameraGLSurfaceView     mGLSurfaceView;
    private Object                  mChangeState;
    private boolean                 mIsGoing = false;

    private Mat                     mRgbaImg;
    private Mat                     mGrayImag;
    private Mat                     mFrameDraw;

    private IORBSEngineJNI          mORBEngine;
    private Object                  mEngineLock;

    private String                  mCamCalibFileName = "CameraCalibration.yaml";
    private String                  mORBBowVocabularyName = "ORBBoWVocabulary.txt";

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
                } break;
                default:
                {
                    Log.i(TAG, "OpenCV Loading error!! - mLoaderCallback");
                    super.onManagerConnected(status);
                } break;
            }
        }
    };



    ////////////////////////////////////////////////////////////////////////////////////////////////
    // OnCreate Method
    ////////////////////////////////////////////////////////////////////////////////////////////////
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_orbslamcamera);

        mVisible = true;
        mControlsView = findViewById(R.id.fullscreen_content_controls);


        // Configure the OpenCV Camera View
        mOpenCameraView = (CameraBridgeViewBase) findViewById(R.id.tutorial1_activity_java_surface_view);
        mOpenCameraView.setVisibility(SurfaceView.VISIBLE);
        mOpenCameraView.setCvCameraViewListener(this);

        // Set up the user interaction to manually show or hide the system UI.
        mOpenCameraView.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View view)
            {
                toggle();
            }
        });


        // Initialize the ORB SLAM System
        mORBEngine = new IORBSEngineJNI();


        // Copy Assets to external file directory
        final File calibFile = new File(Environment.getExternalStorageDirectory().getAbsolutePath(), mCamCalibFileName);
        if (true || !calibFile.exists()){
            copyAsset(mCamCalibFileName);
        }

        double t0 = System.currentTimeMillis();
        final File vocFile = new File(Environment.getExternalStorageDirectory().getAbsolutePath(), mORBBowVocabularyName);
        if (!vocFile.exists()) {
            copyAsset(mORBBowVocabularyName);
        }
        double t1 = System.currentTimeMillis();
        double seconds = (t1 - t0)/1000.0;
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

        if (mORBEngine != null){
            mORBEngine.Shutdown();
            mORBEngine.isRunning = false;
        }

    }

    // Copy Asset Files to External Storage.
    // TODO: Extract into own class and process in separate thread.
    private void copyAsset(String filename) {
        AssetManager assetManager = getAssets();
        InputStream in = null;
        OutputStream out = null;
        try {
            in = assetManager.open(filename);
            File outFile = new File(Environment.getExternalStorageDirectory().getAbsolutePath(), filename);
            out = new FileOutputStream(outFile);
            copyFile(in, out);
        } catch(IOException e) {
            Log.e("tag", "Failed to copy asset file: " + filename, e);
        }
        finally {
            if (in != null) {
                try {
                    in.close();
                } catch (IOException e) {
                    // NOOP
                }
            }
            if (out != null) {
                try {
                    out.close();
                } catch (IOException e) {
                    // NOOP
                }
            }
        }
    }

    private void copyFile(InputStream in, OutputStream out) throws IOException {
        byte[] buffer = new byte[1024];
        int read;
        while((read = in.read(buffer)) != -1){
            out.write(buffer, 0, read);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // OpenCV 3.2 CvCameraViewListener2 Implementation Overrides
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
        if (!mORBEngine.isRunning) {
            // TODO: Get teh Voc DB and settingsFile.
            String vocDBFile = Environment.getExternalStorageDirectory().getAbsolutePath() + "/" + mORBBowVocabularyName;
            String settingsFile = Environment.getExternalStorageDirectory().getAbsolutePath() + "/" + mCamCalibFileName;
            mORBEngine.InitSystem(vocDBFile, settingsFile);
            mORBEngine.isRunning = true;
        }
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
        mRgbaImg = inputFrame.rgba();
        mGrayImag = inputFrame.gray();
        mFrameDraw = new Mat();

        mORBEngine.TrackMonocular(mGrayImag.getNativeObjAddr(), mRgbaImg.getNativeObjAddr(), mFrameDraw.getNativeObjAddr());

        final Bitmap bm = Bitmap.createBitmap(mFrameDraw.cols(), mFrameDraw.rows(), Bitmap.Config.ARGB_8888);
        Utils.matToBitmap(mFrameDraw, bm);

        // TODO: refactor this into thread
        runOnUiThread(new Runnable()
        {
            @Override
            public void run()
            {
                ImageView iv = (ImageView) findViewById(R.id.status_imageView);
                iv.setImageBitmap(bm);
            }
        });


        return mRgbaImg;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // OpenGL ES 1 CM Renderer Implementation Overrides
    //
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////


}
