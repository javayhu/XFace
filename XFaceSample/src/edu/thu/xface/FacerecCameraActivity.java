package edu.thu.xface;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;
import org.opencv.android.JavaCameraView;
import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.MatOfRect;
import org.opencv.core.Rect;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.objdetect.CascadeClassifier;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;
import edu.thu.xface.libs.XFaceLibrary;
import edu.thu.xface.util.CommonUtil;
import edu.thu.xface.util.ToastUtil;

/**
 * 
 * facerec camera
 * 
 * @author hujiawei
 * 
 */
public class FacerecCameraActivity extends Activity implements CvCameraViewListener2 {

	private static final String TAG = "FacerecCameraActivity";

	// private TextView tv_facerec_result;
	private JavaCameraView mOpenCvCameraView;
	private boolean bInitFacerec = false;
	private boolean bFrameProcessing = false;
	private boolean bExitRecognition = false;
	private long threadSleepTime = 1000;

	// face detection!!
//	public static final int JAVA_DETECTOR = 0;
//	public static final int NATIVE_DETECTOR = 1;
	private static final Scalar FACE_RECT_COLOR = new Scalar(0, 255, 0, 255);
	private Handler handler;
	private int count = 0;
	private Mat mRgba;
	private Mat mGray;

	private File mCascadeFile;
	private CascadeClassifier mJavaDetector;
	// private DetectionBasedTracker mNativeDetector;

//	private int mDetectorType = JAVA_DETECTOR;
	// private String[] mDetectorName;

	private float mRelativeFaceSize = 0.2f;
	private int mAbsoluteFaceSize = 0;

	// face detection!!

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_facereccamera);
		// setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_FULL_SENSOR);//hujiawei oritentation
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		// tv_facerec_result = (TextView) findViewById(R.id.tv_facerec_result);

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
			} else {
				Log.i(TAG, "Loaded cascade classifier from " + mCascadeFile.getAbsolutePath());
			}
			// mNativeDetector = new DetectionBasedTracker(mCascadeFile.getAbsolutePath(), 0);// hujiawei
			// cascadeDir.delete();//

		} catch (IOException e) {
			e.printStackTrace();
			Log.e(TAG, "Failed to load cascade. Exception thrown: " + e);
		}

//		mDetectorType = JAVA_DETECTOR;
		// / face detection!

		mOpenCvCameraView = (JavaCameraView) findViewById(R.id.cv_facerec_camera);
		mOpenCvCameraView.setVisibility(SurfaceView.VISIBLE);
		mOpenCvCameraView.setCvCameraViewListener(this);
		mOpenCvCameraView.enableView();

		// used to handle the message sent from the thread reporting the result
		handler = new Handler(new Handler.Callback() {
			public boolean handleMessage(Message msg) {
				int result = msg.arg1;//arg1 = result, arg2 = 
				if (msg.arg2 == 1) {//when facerec!
					if (result == -1) {
						Log.i(TAG, "unkown person!");
						ToastUtil.showShortToast(getApplicationContext(), "I do't know you !");
						// tv_facerec_result.setText("I don't know you! ");
					} else {
						String name = CommonUtil.userProps.getProperty(String.valueOf(result));
						Log.i(TAG, "user id=" + result + " ## name=" + name);
						ToastUtil.showShortToast(getApplicationContext(), count + ": You are " + name + " !");
						count++;
						// tv_facerec_result.setText(count + ": You are " + name + " ! ");
					}
				} else {//when init!
					if (result == -2) {
						Log.i(TAG, "less than 2 images");
						ToastUtil.showShortToast(getApplicationContext(), "less than 2 images");
						// tv_facerec_result.setText("Less than 2 images");
					} else if (result == -1) {
						Log.i(TAG, "invalid file");
						ToastUtil.showShortToast(getApplicationContext(), "invalid file");
						// tv_facerec_result.setText("Invalid data file");
					} else {
						Log.i(TAG, "init facerec ok, result=" + result);
						ToastUtil.showShortToast(getApplicationContext(), "Initialization Successfully!");
						// tv_facerec_result.setText("Initialization Successfully!");
					}
				}

				return false;
			}
		});

		// thread used for recogniton!
		new Thread(new Runnable() {
			public void run() {
				Log.i(TAG, "bInitFacerec= " + bInitFacerec + " $$ bExitRecognition= " + bExitRecognition
						+ " $$ frameprocessing=" + bFrameProcessing);
				if (!bInitFacerec) {// facerec init?
					long result = XFaceLibrary.initFacerec();// it will take a lot of time!
					Message message = new Message();
					message.arg1 = (int) result;// 1/-1/-2
					message.arg2 = 0;//doing init!
					handler.sendMessage(message);
					bInitFacerec = true;// no longer init!
				}
				while (!bExitRecognition) {// is recognition exits?
					if (!bFrameProcessing) {// is frame being processing?
						if (null == mGray || mGray.empty()) {// it's hard to say when it is called!
							Log.i(TAG, "gray mat is null");
							// return;// return when no data//can not return!!!!
						} else {
							bFrameProcessing = true;
							Log.i(TAG, "runFacerec! addr = " + mGray.getNativeObjAddr());// 2103032
							// Log.i(TAG, "data addr=" + mGray.dataAddr() + " $$ native addr=" +
							// mGray.getNativeObjAddr()
							// + " $$ native object=" + mGray.nativeObj);// $1 not equal $2,but $2=$3
							int result = XFaceLibrary.facerec(mGray);
							Message message = new Message();
							message.arg1 = result;
							message.arg2 = 1;
							handler.sendMessage(message);
							bFrameProcessing = false;
						}
					}
					try {
						Thread.currentThread().sleep(threadSleepTime);//TODO!
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
			}
		}).start();

	}

	public FacerecCameraActivity() {
		Log.i(TAG, "Instantiated new " + this.getClass());
	}

	public void btn_facerec_back(View view) {
		Log.i(TAG, "btn_facerec_back");
		FacerecCameraActivity.this.finish();//
		//should the activity finishs?yes!
	}

	// onpause, onresume has problems!!!TODO!
	@Override
	public void onPause() {
		super.onPause();
		if (mOpenCvCameraView != null)
			mOpenCvCameraView.disableView();
	}

	@Override
	public void onResume() {
		super.onResume();
		if (mOpenCvCameraView != null && !mOpenCvCameraView.isEnabled())
			mOpenCvCameraView.enableView();
	}

	public void onDestroy() {
		super.onDestroy();
		if (mOpenCvCameraView != null)
			mOpenCvCameraView.disableView();
		bExitRecognition = true;//
	}

	// @Override
	// public void onConfigurationChanged(Configuration newConfig) {
	// super.onConfigurationChanged(newConfig);//
	// if (newConfig.orientation == Configuration.ORIENTATION_PORTRAIT) {
	// bIsLandscape = false;
	// } else {
	// bIsLandscape = true;
	// }
	// }

	public void onCameraViewStarted(int width, int height) {
		mGray = new Mat();
		mRgba = new Mat();
	}

	public void onCameraViewStopped() {
		// destory camera -> release
		int result = XFaceLibrary.destoryFacerec();
		mGray.release();
		mRgba.release();
	}

	// preview frame!!!
	public Mat onCameraFrame(CvCameraViewFrame inputFrame) {
		// Log.i(TAG, inputFrame.gray().width() + "" + inputFrame.gray().height());
		// landscape 640*480 || portrait [320*240]-> 240*320!
		// when portrait mode, inputframe is 320*240, so pic is rotated!
		mRgba = inputFrame.rgba();
		mGray = inputFrame.gray();

		// face detection!!
//		Core.flip(mRgba.t(), mRgba, 0);// counter-clock wise 90
//		Core.flip(mGray.t(), mGray, 0);
//
//		if (mAbsoluteFaceSize == 0) {
//			int height = mGray.rows();
//			if (Math.round(height * mRelativeFaceSize) > 0) {
//				mAbsoluteFaceSize = Math.round(height * mRelativeFaceSize);
//			}
//			// mNativeDetector.setMinFaceSize(mAbsoluteFaceSize);//
//		}
//
//		MatOfRect faces = new MatOfRect();
//		if (mJavaDetector != null) {// use only java detector
//			mJavaDetector.detectMultiScale(mGray, faces, 1.1, 2, 2, // TODO: objdetect.CV_HAAR_SCALE_IMAGE
//					new Size(mAbsoluteFaceSize, mAbsoluteFaceSize), new Size());
//		}
//
//		Rect[] facesArray = faces.toArray();
//		for (int i = 0; i < facesArray.length; i++) {
//			Core.rectangle(mRgba, facesArray[i].tl(), facesArray[i].br(), FACE_RECT_COLOR, 3);
//		}
//
//		Core.flip(mRgba.t(), mRgba, 1);// counter-clock wise 90
//		Core.flip(mGray.t(), mGray, 1);
		// face detection!!
		
		return mRgba;
	}

}
