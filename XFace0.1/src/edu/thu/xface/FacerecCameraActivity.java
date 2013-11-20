package edu.thu.xface;

import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;
import org.opencv.core.Mat;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;
import android.widget.TextView;
import edu.thu.xface.libs.XFaceLibrary;
import edu.thu.xface.util.CommonUtil;

public class FacerecCameraActivity extends Activity implements CvCameraViewListener2 {

	private static final String TAG = "FacerecCameraActivity";

	private TextView tv_facerec_result;
	private FacerecCameraView mOpenCvCameraView;
	private boolean bInitFacerec = false;
	private boolean bFrameProcessing = false;
	private boolean bExitRecognition = false;
	private Handler handler;
	private Mat mRgba;
	private Mat mGray;
	private int count = 0;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_facereccamera);
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		tv_facerec_result = (TextView) findViewById(R.id.tv_facerec_result);
		mOpenCvCameraView = (FacerecCameraView) findViewById(R.id.cv_facerec_camera);
		mOpenCvCameraView.setVisibility(SurfaceView.VISIBLE);
		mOpenCvCameraView.setCvCameraViewListener(this);
		mOpenCvCameraView.enableView();

		// used to handle the message sent from the thread reporting the result
		handler = new Handler(new Handler.Callback() {
			public boolean handleMessage(Message msg) {
				int result = msg.arg1;
				if (msg.arg2 == 1) {
					if (result == -1) {
						Log.i(TAG, "unkown person!");
						// ToastUtil.showShortToast(getApplicationContext(), "I do't know you !");
						tv_facerec_result.setText("I don't know you! ");
					} else {
						String name = CommonUtil.userProps.getProperty(String.valueOf(result));
						Log.i(TAG, "user id=" + result + " ## name=" + name);
						// ToastUtil.showShortToast(getApplicationContext(), "You are " + name + " !");
						count++;
						tv_facerec_result.setText(count + ": You are " + name + " ! ");
					}
				} else {
					if (result == -2) {
						Log.i(TAG, "less than 2 images");
						// ToastUtil.showShortToast(getApplicationContext(), "less than 2 images");
						tv_facerec_result.setText("Less than 2 images");
					} else if (result == -1) {
						Log.i(TAG, "invalid file");
						// ToastUtil.showShortToast(getApplicationContext(), "invalid file");
						tv_facerec_result.setText("Invalid data file");
					} else {
						Log.i(TAG, "init facerec ok, result=" + result);
						tv_facerec_result.setText("Initialization Successfully!");
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

	public FacerecCameraActivity() {
		Log.i(TAG, "Instantiated new " + this.getClass());
	}

	public void btn_facerec_back(View view) {
		Log.i(TAG, "btn_facerec_back");
		FacerecCameraActivity.this.finish();
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
		if (mOpenCvCameraView != null && !mOpenCvCameraView.isEnabled())
			mOpenCvCameraView.enableView();
	}

	public void onDestroy() {
		super.onDestroy();
		if (mOpenCvCameraView != null)
			mOpenCvCameraView.disableView();
		bExitRecognition = true;//
	}

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
		// Log.i(TAG, inputFrame.gray().width()+"");//320
		mRgba = inputFrame.rgba();
		mGray = inputFrame.gray();
		return mRgba;
	}

}
