package edu.thu.xface;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;

import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;
import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.Size;
import org.opencv.highgui.Highgui;
import org.opencv.imgproc.Imgproc;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import edu.thu.xface.util.CommonUtil;
import edu.thu.xface.util.ToastUtil;

/**
 * camera view for sign up
 * 
 * @author hujiawei
 * 
 */
public class SignupCameraActivity extends Activity implements CvCameraViewListener2 {

	private static final String TAG = "SignupCameraActivity";

	// private SharedPreferences sharedPreferences;
	private SignupCameraView mOpenCvCameraView;
	private Button btn_camera_takepic;
	private String name;
	private String filePath;
	private boolean bPictaken = false;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_signupcamera);
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		// sharedPreferences = getSharedPreferences("xface", MODE_PRIVATE);
		name = getIntent().getStringExtra("name");
		Log.i(TAG, "name is " + name);
		btn_camera_takepic = (Button) findViewById(R.id.btn_camera_takepic);
		mOpenCvCameraView = (SignupCameraView) findViewById(R.id.cv_camera_signup);
		mOpenCvCameraView.setVisibility(SurfaceView.VISIBLE);
		mOpenCvCameraView.setCvCameraViewListener(this);
		mOpenCvCameraView.enableView();
	}

	public SignupCameraActivity() {
		Log.i(TAG, "Instantiated new " + this.getClass());
	}

	public void btn_camera_back(View view) {
		Log.i(TAG, "btn_camera_back");
		SignupCameraActivity.this.finish();
	}

	public void btn_camera_ok(View view) {
		Log.i(TAG, "btn_camera_ok");
		if (!bPictaken) {
			ToastUtil.showShortToast(getApplicationContext(), "未拍照所以注册失败！");
			SignupCameraActivity.this.finish();
			return;//do not forget!
		}
		// async do -> runtime exception: method called after release()
		// maybe the activity releases before the thread returns!
		// new AsyncTask<Void, Void, Void>() {
		// protected Void doInBackground(Void... params) {
		// process image
		Mat image = Highgui.imread(filePath);
		int width = image.width();
		int height = image.height();
		if (width > height) {// portrait should be rotated! direction? TODO!
			Core.flip(image.t(), image, 0);
		}
		Imgproc.cvtColor(image, image, Imgproc.COLOR_BGR2GRAY);// gray
		Imgproc.resize(image, image, new Size(CommonUtil.IMAGE_WIDTH, CommonUtil.IMAGE_HEIGHT));//
		filePath = CommonUtil.USERFOLDER.getAbsolutePath() + File.separator + name + ".jpg";
		Highgui.imwrite(filePath, image);
		// image = Highgui.imread(filePath);
		// Log.i(TAG, "width=" + width + ";height=" + height);
		// users.properties
		String stotal = CommonUtil.userProps.getProperty("total");
		int total = 0;
		if (null != stotal) {// have some!
			total = Integer.parseInt(stotal);
		}
		int id = total + 1;// user id!
		try {
			CommonUtil.userProps.setProperty("total", String.valueOf(id));
			CommonUtil.userProps.setProperty(String.valueOf(id), name);
			CommonUtil.saveUserProperties(CommonUtil.userProps);
		} catch (Exception e) {
			e.printStackTrace();
		}
		// facedata.txt
		String data = filePath + ";" + id + "\n";// user image file path;user id
		try {
			RandomAccessFile facedataFile = new RandomAccessFile(CommonUtil.SDFOLDER + File.separator
					+ CommonUtil.FACEDATA_FILENAME, "rw");
			facedataFile.seek(facedataFile.length());
			facedataFile.write(data.getBytes());
			facedataFile.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		Log.i(TAG, "image process ok");
		// return null;
		// }
		// }.execute();

		Intent intent = new Intent(SignupCameraActivity.this, LogoActivity.class);
		startActivity(intent);
		SignupCameraActivity.this.finish();
	}

	public void btn_camera_takepic(View view) {
		Log.i(TAG, "btn_camera_takepic");
		filePath = CommonUtil.CAMERAFOLDER.getAbsolutePath() + File.separator + name + ".jpg";
		mOpenCvCameraView.takePicture(filePath);
		btn_camera_takepic.setText("Take this to replace!");
		// Toast.makeText(this, filePath + " saved", Toast.LENGTH_SHORT).show();
		ToastUtil.showShortToast(getApplicationContext(), filePath + " saved");
		bPictaken = true;
	}

	@Override
	public void onPause() {
		super.onPause();
		Log.i(TAG, "on pause");
		if (mOpenCvCameraView != null)
			mOpenCvCameraView.disableView();
	}

	@Override
	public void onResume() {
		super.onResume();
		Log.i(TAG, "on resume");
		if (mOpenCvCameraView != null && !mOpenCvCameraView.isEnabled())
			mOpenCvCameraView.enableView();
	}

	public void onDestroy() {
		super.onDestroy();
		Log.i(TAG, "on destory");
		if (mOpenCvCameraView != null)
			mOpenCvCameraView.disableView();
	}

	public void onCameraViewStarted(int width, int height) {
	}

	public void onCameraViewStopped() {
	}

	public Mat onCameraFrame(CvCameraViewFrame inputFrame) {
		return inputFrame.rgba();
	}

}
