package org.opencv.samples.facedetect;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;
import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.MatOfRect;
import org.opencv.core.Rect;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.objdetect.CascadeClassifier;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.WindowManager;
import edu.thu.xface.R;
import edu.thu.xface.libs.XFaceLibrary;
import edu.thu.xface.util.CommonUtil;
import edu.thu.xface.util.ToastUtil;

public class FdActivity extends Activity implements CvCameraViewListener2 {

	private static final String TAG = "OCVSample::Activity";
	private static final Scalar FACE_RECT_COLOR = new Scalar(0, 255, 0, 255);
	public static final int JAVA_DETECTOR = 0;
	public static final int NATIVE_DETECTOR = 1;

	//
	private boolean bInitFacerec = false;
	private boolean bFrameProcessing = false;
	private boolean bExitRecognition = false;
	private Handler handler;
	private int count = 0;
	//

	private MenuItem mItemFace50;
	private MenuItem mItemFace40;
	private MenuItem mItemFace30;
	private MenuItem mItemFace20;
	private MenuItem mItemType;

	private Mat mRgba;
	private Mat mGray;
	private File mCascadeFile;
	private CascadeClassifier mJavaDetector;
	private DetectionBasedTracker mNativeDetector;

	private int mDetectorType = JAVA_DETECTOR;
	private String[] mDetectorName;

	private float mRelativeFaceSize = 0.2f;
	private int mAbsoluteFaceSize = 0;

	private CameraBridgeViewBase mOpenCvCameraView;

	// static {
	// Log.i(TAG, "opencv library load!");
	// if (!OpenCVLoader.initDebug()) {
	// Log.i(TAG, "OpenCV load not successfully");
	// } else {
	// System.loadLibrary("detection_based_tracker");// load other libraries
	// }
	// }

	// private BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(this) {
	// @Override
	// public void onManagerConnected(int status) {
	// switch (status) {
	// case LoaderCallbackInterface.SUCCESS: {
	// Log.i(TAG, "OpenCV loaded successfully");
	//
	// // Load native library after(!) OpenCV initialization
	// System.loadLibrary("detection_based_tracker");
	//
	// try {
	// // load cascade file from application resources
	// InputStream is = getResources().openRawResource(R.raw.lbpcascade_frontalface);
	// File cascadeDir = getDir("cascade", Context.MODE_PRIVATE);
	// mCascadeFile = new File(cascadeDir, "lbpcascade_frontalface.xml");
	// FileOutputStream os = new FileOutputStream(mCascadeFile);
	//
	// byte[] buffer = new byte[4096];
	// int bytesRead;
	// while ((bytesRead = is.read(buffer)) != -1) {
	// os.write(buffer, 0, bytesRead);
	// }
	// is.close();
	// os.close();
	//
	// mJavaDetector = new CascadeClassifier(mCascadeFile.getAbsolutePath());
	// if (mJavaDetector.empty()) {
	// Log.e(TAG, "Failed to load cascade classifier");
	// mJavaDetector = null;
	// } else
	// Log.i(TAG, "Loaded cascade classifier from " + mCascadeFile.getAbsolutePath());
	//
	// mNativeDetector = new DetectionBasedTracker(mCascadeFile.getAbsolutePath(), 0);// hujiawei
	//
	// cascadeDir.delete();
	//
	// } catch (IOException e) {
	// e.printStackTrace();
	// Log.e(TAG, "Failed to load cascade. Exception thrown: " + e);
	// }
	//
	// mOpenCvCameraView.enableView();
	// }
	// break;
	// default: {
	// super.onManagerConnected(status);
	// }
	// break;
	// }
	// }
	// };

	public FdActivity() {
		mDetectorName = new String[2];
		mDetectorName[JAVA_DETECTOR] = "Java";
		mDetectorName[NATIVE_DETECTOR] = "Native (tracking)";

		Log.i(TAG, "Instantiated new " + this.getClass());

		// used to handle the message sent from the thread reporting the result
		handler = new Handler(new Handler.Callback() {
			public boolean handleMessage(Message msg) {
				int result = msg.arg1;
				if (msg.arg2 == 1) {
					if (result == -1) {
						Log.i(TAG, "unkown person!");
						ToastUtil.showShortToast(getApplicationContext(), "I do't know you !");
						// tv_facerec_result.setText("I don't know you! ");
					} else {
						String name = CommonUtil.userProps.getProperty(String.valueOf(result));
						Log.i(TAG, "user id=" + result + " ## name=" + name);
						ToastUtil.showShortToast(getApplicationContext(), "You are " + name + " !");
						count++;
						// tv_facerec_result.setText(count + ": You are " + name + " ! ");
					}
				} else {
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
						// tv_facerec_result.setText("Initialization Successfully!");
					}
				}

				return false;
			}
		});

		new Thread(new Runnable() {
			public void run() {
				Log.i(TAG, "bInitFacerec= " + bInitFacerec + " $$ bExitRecognition= " + bExitRecognition
						+ " $$ frameprocessing=" + bFrameProcessing);
				if (!bInitFacerec) {// facerec init?
					long result = XFaceLibrary.initFacerec();// it will take a lot of time!
					Message message = new Message();
					message.arg1 = (int) result;// 1/-1/-2
					message.arg2 = 0;
					handler.sendMessage(message);
					bInitFacerec = true;// no longer init!
				}
				while (!bExitRecognition) {// is recognition exits?
					if (!bFrameProcessing) {// is frame being processing?
						if (null == mGray || mGray.empty()) {
							Log.i(TAG, "gray mat is null");
							// return;// return when no data//can not return
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
						Thread.currentThread().sleep(1000);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
			}
		}).start();
	}

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		Log.i(TAG, "called onCreate");
		super.onCreate(savedInstanceState);
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

		setContentView(R.layout.face_detect_surface_view);

		// //
		try {
			// load cascade file from application resources
			InputStream is = getResources().openRawResource(R.raw.lbpcascade_frontalface);
			File cascadeDir = getDir("cascade", Context.MODE_PRIVATE);
			mCascadeFile = new File(cascadeDir, "lbpcascade_frontalface.xml");
			FileOutputStream os = new FileOutputStream(mCascadeFile);

			byte[] buffer = new byte[4096];
			int bytesRead;
			while ((bytesRead = is.read(buffer)) != -1) {
				os.write(buffer, 0, bytesRead);
			}
			is.close();
			os.close();

			mJavaDetector = new CascadeClassifier(mCascadeFile.getAbsolutePath());
			if (mJavaDetector.empty()) {
				Log.e(TAG, "Failed to load cascade classifier");
				mJavaDetector = null;
			} else
				Log.i(TAG, "Loaded cascade classifier from " + mCascadeFile.getAbsolutePath());

			// mNativeDetector = new DetectionBasedTracker(mCascadeFile.getAbsolutePath(), 0);// hujiawei

			cascadeDir.delete();

		} catch (IOException e) {
			e.printStackTrace();
			Log.e(TAG, "Failed to load cascade. Exception thrown: " + e);
		}

		mDetectorType = JAVA_DETECTOR;
		mOpenCvCameraView = (CameraBridgeViewBase) findViewById(R.id.fd_activity_surface_view);
		mOpenCvCameraView.enableView();//
		mOpenCvCameraView.setCvCameraViewListener(this);
	}

	@Override
	public void onPause() {
		super.onPause();
		if (mOpenCvCameraView != null)
			mOpenCvCameraView.disableView();
	}

	@Override
	public void onResume() {
		super.onResume();
		// OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_3, this, mLoaderCallback);//
	}

	public void onDestroy() {
		super.onDestroy();
		mOpenCvCameraView.disableView();
	}

	public void onCameraViewStarted(int width, int height) {
		mGray = new Mat();
		mRgba = new Mat();
	}

	public void onCameraViewStopped() {
		mGray.release();
		mRgba.release();
	}

	public Mat onCameraFrame(CvCameraViewFrame inputFrame) {

		mRgba = inputFrame.rgba();
		mGray = inputFrame.gray();

		if (mAbsoluteFaceSize == 0) {
			int height = mGray.rows();
			if (Math.round(height * mRelativeFaceSize) > 0) {
				mAbsoluteFaceSize = Math.round(height * mRelativeFaceSize);
			}
			// mNativeDetector.setMinFaceSize(mAbsoluteFaceSize);//
		}

		MatOfRect faces = new MatOfRect();

		if (mDetectorType == JAVA_DETECTOR) {
			if (mJavaDetector != null)
				mJavaDetector.detectMultiScale(mGray, faces, 1.1, 2, 2, // TODO: objdetect.CV_HAAR_SCALE_IMAGE
						new Size(mAbsoluteFaceSize, mAbsoluteFaceSize), new Size());
		} else if (mDetectorType == NATIVE_DETECTOR) {
			if (mNativeDetector != null)
				mNativeDetector.detect(mGray, faces);
		} else {
			Log.e(TAG, "Detection method is not selected!");
		}

		Rect[] facesArray = faces.toArray();
		for (int i = 0; i < facesArray.length; i++)
			Core.rectangle(mRgba, facesArray[i].tl(), facesArray[i].br(), FACE_RECT_COLOR, 3);

		return mRgba;
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		Log.i(TAG, "called onCreateOptionsMenu");
		mItemFace50 = menu.add("Face size 50%");
		mItemFace40 = menu.add("Face size 40%");
		mItemFace30 = menu.add("Face size 30%");
		mItemFace20 = menu.add("Face size 20%");
		mItemType = menu.add(mDetectorName[mDetectorType]);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		Log.i(TAG, "called onOptionsItemSelected; selected item: " + item);
		if (item == mItemFace50)
			setMinFaceSize(0.5f);
		else if (item == mItemFace40)
			setMinFaceSize(0.4f);
		else if (item == mItemFace30)
			setMinFaceSize(0.3f);
		else if (item == mItemFace20)
			setMinFaceSize(0.2f);
		else if (item == mItemType) {
			mDetectorType = (mDetectorType + 1) % mDetectorName.length;
			item.setTitle(mDetectorName[mDetectorType]);
			setDetectorType(mDetectorType);
		}
		return true;
	}

	private void setMinFaceSize(float faceSize) {
		mRelativeFaceSize = faceSize;
		mAbsoluteFaceSize = 0;
	}

	private void setDetectorType(int type) {
		if (mDetectorType != type) {
			mDetectorType = type;

			if (type == NATIVE_DETECTOR) {
				Log.i(TAG, "Detection Based Tracker enabled");
				mNativeDetector.start();
			} else {
				Log.i(TAG, "Cascade detector enabled");
				mNativeDetector.stop();
			}
		}
	}
}
