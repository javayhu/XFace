package edu.thu.xface;

import java.io.FileOutputStream;

import org.opencv.android.JavaCameraView;

import android.content.Context;
import android.hardware.Camera;
import android.hardware.Camera.PictureCallback;
import android.util.AttributeSet;
import android.util.Log;

public class FacerecCameraView extends JavaCameraView implements PictureCallback {

	private static final String TAG = "FacerecCameraView";
	private String mPictureFileName;

	public FacerecCameraView(Context context, AttributeSet attrs) {
		super(context, attrs);
		// hujiawei can not put it here! otherwise run into error!
		// Camera.Parameters params = mCamera.getParameters();
		// params.setPreviewSize(640, 480);
		// mCamera.setParameters(params);
	}

	public void takePicture(final String fileName) {
		Log.i(TAG, "Taking picture");
		this.mPictureFileName = fileName;
		// Postview and jpeg are sent in the same buffers if the queue is not empty when performing a capture.
		// Clear up buffers to avoid mCamera.takePicture to be stuck because of a memory issue
		mCamera.setPreviewCallback(null);

		// PictureCallback is implemented by the current class
		mCamera.takePicture(null, null, this);
	}

	@Override
	public void onPictureTaken(byte[] data, Camera camera) {
		Log.i(TAG, "Saving a bitmap to file");
		// The camera preview was automatically stopped. Start it again.
		mCamera.startPreview();
		mCamera.setPreviewCallback(this);
		// Write the image in a file (in jpeg format)
		try {
			FileOutputStream fos = new FileOutputStream(mPictureFileName);
			fos.write(data);
			fos.close();
		} catch (java.io.IOException e) {
			Log.e(TAG, "Exception in photoCallback", e);
		}
	}
}