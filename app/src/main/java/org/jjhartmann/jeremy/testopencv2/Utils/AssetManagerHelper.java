/**
 * VisualInertialOdometry-Android-UW-SAR-2
 * <p>
 * Created by J. J. Hartmann on 3/19/2017.
 * Email: j3hartma@uwaterloo.ca
 * Copyright 2017
 */

package org.jjhartmann.jeremy.testopencv2.Utils;

import android.content.res.AssetManager;
import android.os.Environment;
import android.util.Log;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;


public class AssetManagerHelper
{
    private AssetManager            mAssestManager;

    public AssetManagerHelper(AssetManager in_amanager){
        mAssestManager = in_amanager;
    }

    // Copy Asset Files to External Storage.
    public void copyAsset(String filename) {
        InputStream in = null;
        OutputStream out = null;
        try {
            in = mAssestManager.open(filename);
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
}
