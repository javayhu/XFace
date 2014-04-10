package edu.thu.xface;

import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;
import org.opencv.android.JavaCameraView;
import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.MatOfRect;
import org.opencv.core.Rect;
import org.opencv.core.Scalar;
import org.opencv.core.Size;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
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
	private boolean bFrameRecing = false;
	private boolean bFrameDetecting = false;
	private boolean bExitRecognition = false;
	private long threadSleepTime = 1000;

	// face recognition!!
	private Mat mRec;
	private byte[] lock = new byte[0];
	// face recognition!!

	// face detection!!
	private Mat mRgba;
	private Mat mGray;
	private int count = 0;
	private Handler handler;
	private XFaceLibrary xface;
	private int mAbsoluteFaceSize = 0;
	private float mRelativeFaceSize = 0.2f;
	private static final Scalar FACE_RECT_COLOR = new Scalar(0, 255, 0, 255);

	// face detection!!

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_facereccamera);
		// setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_FULL_SENSOR);//hujiawei oritentation
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		// tv_facerec_result = (TextView) findViewById(R.id.tv_facerec_result);
		mOpenCvCameraView = (JavaCameraView) findViewById(R.id.cv_facerec_camera);
		mOpenCvCameraView.setCvCameraViewListener(this);
		mOpenCvCameraView.enableView();

		// / face detection!
		// xface = new XFaceLibrary();// hujiawei
		xface = CommonUtil.xFaceLibrary;
		xface.initFacedetect(CommonUtil.LBPCASCADE_FILEPATH, 0);
		// / face detection!

		// used to handle the message sent from the thread reporting the result
		handler = new Handler(new Handler.Callback() {
			public boolean handleMessage(Message msg) {
				int result = msg.arg1;// arg1 = result, arg2 = [state]
				if (msg.arg2 == 1) {// when facerec! arg2=1
					if (result == -1) {
						Log.i(TAG, "unkown person!");
						ToastUtil.showShortToast(getApplicationContext(), "I do't know you !");
					} else {
						String name = CommonUtil.userProps.getProperty(String.valueOf(result));
						Log.i(TAG, "user id=" + result + " ## name=" + name);
						ToastUtil.showShortToast(getApplicationContext(), count + ": You are " + name + " !");
						count++;
					}
				} else {// when init! arg2=0 !!! need!!!
					if (result == 0) {
						Log.i(TAG, "init facerec fail, result=" + result);
						ToastUtil.showShortToast(getApplicationContext(), "Initialization Fail!");
					} else {
						Log.i(TAG, "init facerec ok, result=" + result);
						ToastUtil.showShortToast(getApplicationContext(), "Initialization Successfully!");
					}
				}

				return false;
			}
		});

		// thread used for recogniton!
		new Thread(new Runnable() {
			public void run() {
				Log.i(TAG, "bInitFacerec= " + bInitFacerec + " $$ bExitRecognition= " + bExitRecognition
						+ " $$ frameprocessing=" + bFrameRecing);
				if (!bInitFacerec) {// facerec init?
					long result = xface.xfacerec;
					if (result == 0) {
						result = xface.initFacerec();// it will take a lot of time!
					}
					Message message = new Message();
					message.arg1 = (int) result;// 0/(very large value)
					message.arg2 = 0;// doing init!
					handler.sendMessage(message);
					// what if initialize failed?!
					if (result > 0) {
						bInitFacerec = true;// no longer init!
					} else {
						return;// if fail, stop the thread!
					}
				}// problem exist! if facerec not initialized, face rec action will still do! solution is above!
				while (!bExitRecognition) {// is recognition exits?
					if (!bFrameRecing) {// is frame being processing?
						if (null == mRec || mRec.empty()) {// it's hard to say when it is called!
							Log.i(TAG, "rec image mat is null");
							// return;// return when no data//can not return!!!!
						} else {
							bFrameRecing = true;
							Log.i(TAG, "runFacerec! mat addr = " + mRec.getNativeObjAddr());// 2103032
							// Log.i(TAG, "data addr=" + mGray.dataAddr() + " $$ native addr=" +
							// mGray.getNativeObjAddr()
							// + " $$ native object=" + mGray.nativeObj);// $1 not equal $2,but $2=$3
							int result = xface.facerec(mRec);
							// int result = -1;
							Message message = new Message();
							message.arg1 = result;
							message.arg2 = 1;
							handler.sendMessage(message);
							bFrameRecing = false;
						}
						try {
							Thread.currentThread().sleep(threadSleepTime);// TODO!
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
					}
				}
			}
		}).start();

	}

	public void btn_facerec_back(View view) {
		Log.i(TAG, "btn_facerec_back");
		FacerecCameraActivity.this.finish();//
	}

	// onpause, onresume has problems!!!TODO!
	@Override
	public void onPause() {
		super.onPause();
		Log.i(TAG, "on pause and finish!");
		if (mOpenCvCameraView != null) {
			mOpenCvCameraView.disableView();
		}
		xface.destroryFacedetect();
		bExitRecognition = true;//
		FacerecCameraActivity.this.finish();
	}

	@Override
	public void onResume() {
		super.onResume();
		Log.i(TAG, "on resume");
		if (mOpenCvCameraView != null && !mOpenCvCameraView.isEnabled()) {
			mOpenCvCameraView.enableView();
		}
	}

	public void onDestroy() {
		super.onDestroy();
		Log.i(TAG, "on destroy");
		if (mOpenCvCameraView != null) {
			mOpenCvCameraView.disableView();
		}
		xface.destroryFacedetect();
		bExitRecognition = true;//
	}

	public void onCameraViewStarted(int width, int height) {
		Log.i(TAG, "camera view start");
		mGray = new Mat();
		mRgba = new Mat();
		mRec = new Mat();
		xface.startFacedetect();
	}

	public void onCameraViewStopped() {
		Log.i(TAG, "camera view stop");
		mGray.release();
		mRgba.release();
		mRec.release();
		// xface.destoryFacerec();//can not be destoryed
		xface.stopFacedetect();
	}

	// preview frame!!!
	public Mat onCameraFrame(CvCameraViewFrame inputFrame) {
		Log.i(TAG, inputFrame.gray().width() + "*" + inputFrame.gray().height());// 720*480
		// image frame size is 480*720!!! here image is rotated when showing!
		mRgba = inputFrame.rgba();
		mGray = inputFrame.gray();
		mGray.copyTo(mRec);
		// Log.i(TAG, "$$ gray address is " + mGray.nativeObj + " rec address is " + mRec.nativeObj);// different

		// face detection!!
		// Core.flip(mRgba.t(), mRgba, 0);// counter-clock wise 90
		// Core.flip(mGray.t(), mGray, 0);
		//
		// if (mAbsoluteFaceSize == 0) {
		// int height = mGray.rows();
		// if (Math.round(height * mRelativeFaceSize) > 0) {
		// mAbsoluteFaceSize = Math.round(height * mRelativeFaceSize);
		// }
		// xface.setMinFaceSize(mAbsoluteFaceSize);//
		// }
		//
		// MatOfRect faces = new MatOfRect();
		//
		// xface.facedetect(mGray, faces);
		//
		// Rect[] facesArray = faces.toArray();
		// for (int i = 0; i < facesArray.length; i++)
		// Core.rectangle(mRgba, facesArray[i].tl(), facesArray[i].br(), FACE_RECT_COLOR, 3);
		//
		// Core.flip(mRgba.t(), mRgba, 1);// clock wise 90
		// Core.flip(mGray.t(), mGray, 1);
		// face detection!!

		return mRgba;
	}

}
