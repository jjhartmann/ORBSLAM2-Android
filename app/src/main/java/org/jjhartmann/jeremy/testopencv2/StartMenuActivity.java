package org.jjhartmann.jeremy.testopencv2;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class StartMenuActivity extends Activity
{

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_start_menu);


        // Init camera calibration button
        final Button bCameraCalibration = (Button) findViewById(R.id.button_calibrate_camera);
        bCameraCalibration.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                //  Init Camera Calib
                Intent intent = new Intent(StartMenuActivity.this, CameraCalibrationActivity.class);
                startActivity(intent);
            }
        });
    }
}
