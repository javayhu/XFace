package edu.thu.xface;

import org.opencv.android.OpenCVLoader;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

public class LogoActivity extends Activity {

	private static final String TAG = "LogoActivity";

	static {
		// System.loadLibrary("opencv_java");
		Log.i(TAG, "opencv library load!");
		if (!OpenCVLoader.initDebug()) {
			// Handle initialization error
			Log.i(TAG, "OpenCV load not successfully");
		} else {
			System.loadLibrary("xface");
		}
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_logo);
	}

	public void btn_logo_signup(View view) {
		Intent intent = new Intent(LogoActivity.this, SignupActivity.class);
		startActivity(intent);
	}

	public void btn_logo_signin(View view) {
		// Intent intent = new Intent(LogoActivity.this, DemoActivity.class);
		Intent intent = new Intent(LogoActivity.this, FacerecCameraActivity.class);
		startActivity(intent);
	}

}
