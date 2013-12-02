package edu.thu.xface;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.RandomAccessFile;

import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;
import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.MatOfRect;
import org.opencv.core.Rect;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.highgui.Highgui;
import org.opencv.imgproc.Imgproc;
import org.opencv.objdetect.CascadeClassifier;
import org.opencv.samples.facedetect.DetectionBasedTracker;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
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

	//// / face detection!
	public static final int JAVA_DETECTOR = 0;
	public static final int NATIVE_DETECTOR = 1;
	private static final Scalar FACE_RECT_COLOR = new Scalar(0, 255, 0, 255);
	private Handler handler;
	private int count = 0;
	private Mat mRgba;
	private Mat mGray;

	private File mCascadeFile;
	private CascadeClassifier mJavaDetector;
	private DetectionBasedTracker mNativeDetector;

	private int mDetectorType = JAVA_DETECTOR;
	// private String[] mDetectorName;

	private float mRelativeFaceSize = 0.2f;
	private int mAbsoluteFaceSize = 0;

	//// / face detection!

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_signupcamera);
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		// sharedPreferences = getSharedPreferences("xface", MODE_PRIVATE);
		name = getIntent().getStringExtra("name");
		Log.i(TAG, "name is " + name);
		btn_camera_takepic = (Button) findViewById(R.id.btn_camera_takepic);

		// / face detection!
		try {
			// File cascadeDir = getDir("cascade", Context.MODE_PRIVATE);
			mCascadeFile = new File(CommonUtil.LBPCASCADE_FILEPATH);
			if (!mCascadeFile.exists()) {// if file not exist, load from raw, otherwise, just use it!
				// load cascade file from application resources
				InputStream is = getResources().openRawResource(R.raw.lbpcascade_frontalface);
				// mCascadeFile = new File(cascadeDir, "lbpcascade_frontalface.xml");
				FileOutputStream os = new FileOutputStream(mCascadeFile);

				byte[] buffer = new byte[4096];
				int bytesRead;
				while ((bytesRead = is.read(buffer)) != -1) {
					os.write(buffer, 0, bytesRead);
				}
				is.close();
				os.close();
			}

			mJavaDetector = new CascadeClassifier(mCascadeFile.getAbsolutePath());
			if (mJavaDetector.empty()) {
				Log.e(TAG, "Failed to load cascade classifier");
				mJavaDetector = null;
			} else
				Log.i(TAG, "Loaded cascade classifier from " + mCascadeFile.getAbsolutePath());

			// mNativeDetector = new DetectionBasedTracker(mCascadeFile.getAbsolutePath(), 0);// hujiawei
			// cascadeDir.delete();//

		} catch (IOException e) {
			e.printStackTrace();
			Log.e(TAG, "Failed to load cascade. Exception thrown: " + e);
		}

		mDetectorType = JAVA_DETECTOR;
		// / face detection!

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
			return;// do not forget!
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
			Core.flip(image.t(), image, 0);// counter-clock wise 90
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
		// save data to facedata.txt
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

		// go to logo activity
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
		// / face detection!
		// return inputFrame.rgba();//
		// Log.i(TAG, inputFrame.gray().width() + "" + inputFrame.gray().height());
		// landscape 640*480 || portrait [320*240]-> 240*320!
		// when portrait mode, inputframe is 320*240, so pic is rotated!
		mRgba = inputFrame.rgba();
		mGray = inputFrame.gray();

		Core.flip(mRgba.t(), mRgba, 0);// counter-clock wise 90
		Core.flip(mGray.t(), mGray, 0);

		if (mAbsoluteFaceSize == 0) {
			int height = mGray.rows();
			if (Math.round(height * mRelativeFaceSize) > 0) {
				mAbsoluteFaceSize = Math.round(height * mRelativeFaceSize);
			}
			// mNativeDetector.setMinFaceSize(mAbsoluteFaceSize);//
		}

		MatOfRect faces = new MatOfRect();
		if (mJavaDetector != null) {// use only java detector
			mJavaDetector.detectMultiScale(mGray, faces, 1.1, 2, 2, // TODO: objdetect.CV_HAAR_SCALE_IMAGE
					new Size(mAbsoluteFaceSize, mAbsoluteFaceSize), new Size());
		}

		// if (mDetectorType == JAVA_DETECTOR) {
		// if (mJavaDetector != null)
		// mJavaDetector.detectMultiScale(mGray, faces, 1.1, 2, 2, // TODO: objdetect.CV_HAAR_SCALE_IMAGE
		// new Size(mAbsoluteFaceSize, mAbsoluteFaceSize), new Size());
		// } else if (mDetectorType == NATIVE_DETECTOR) {
		// if (mNativeDetector != null)
		// mNativeDetector.detect(mGray, faces);
		// } else {
		// Log.e(TAG, "Detection method is not selected!");
		// }

		Rect[] facesArray = faces.toArray();
		for (int i = 0; i < facesArray.length; i++) {
			Core.rectangle(mRgba, facesArray[i].tl(), facesArray[i].br(), FACE_RECT_COLOR, 3);
		}
		// if (!bIsLandscape) {
		// // Core.flip(mRgba, mRgba, 0);
		// Core.flip(mRgba.t(), mRgba, 1);
		// Core.flip(mRgba.t(), mRgba, 1);
		// }

		Core.flip(mRgba.t(), mRgba, 1);// counter-clock wise 90
		Core.flip(mGray.t(), mGray, 1);

		return mRgba;
		// / face detection!
	}

}
