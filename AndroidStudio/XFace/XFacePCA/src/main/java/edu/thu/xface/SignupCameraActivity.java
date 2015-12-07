package edu.thu.xface;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;

import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;
import org.opencv.android.JavaCameraView;
import org.opencv.core.Mat;

import java.io.File;

import edu.thu.xface.libs.XFaceLibrary;
import edu.thu.xface.util.CommonUtil;
import edu.thu.xface.util.ToastUtil;

/**
 * camera activity for sign up
 *
 * @author hujiawei
 */
public class SignupCameraActivity extends Activity implements CvCameraViewListener2 {

    // private static final String DEFAULT_HINT = "Saving your face! Waiting...";//

    private static final String TAG = "SignupCameraActivity";

    private final int MAX_COUNT = 10;// how many faces can be added once
    private int count = 1;// how many faces current added

    private int userid;
    private String name;
    private String filePath;
    private boolean bPicSave = false;
    private boolean bInitFacerec = false;
    private boolean bFrameDetecting = false;
    private boolean bExitDetection = false;
    private long threadSleepTime = 1000;

    private JavaCameraView mOpenCvCameraView;

    // face detection!!! hujiawei
    private Mat mRgba;
    private Mat mGray;
    private Mat mDet;
    private Handler handler;
    private XFaceLibrary xface;
    // face detection!!! hujiawei

    // // / face detection!
    // private static final Scalar FACE_RECT_COLOR = new Scalar(0, 255, 0, 255);// green
    // private float mRelativeFaceSize = 0.2f;
    // private int mAbsoluteFaceSize = 0;
    // // / face detection!

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_signupcamera);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
//		btn_camera_ok = (Button) findViewById(R.id.btn_camera_ok);

        name = getIntent().getStringExtra("name");
        userid = getIntent().getIntExtra("userid", -1);
        Log.e(TAG, "name = " + name + " && userid = " + userid);
        int total = 0;
        String stotal = CommonUtil.userProps.getProperty("total");
        if (null != stotal) {// have some users!
            total = Integer.parseInt(stotal);
        }
        if (userid <= 0) {// not have this one!
            userid = total + 1;
            try {// save new data!
                CommonUtil.userProps.setProperty("total", String.valueOf(userid));
                CommonUtil.userProps.setProperty(String.valueOf(userid), name);
                CommonUtil.saveUserProperties(CommonUtil.userProps);
            } catch (Exception e) {
                e.printStackTrace();
            }
            // creat folder for this user!
            File userfolder = new File(CommonUtil.USERFOLDER.getAbsolutePath() + File.separator
                    + String.valueOf(userid));
            if (!userfolder.exists()) {
                userfolder.mkdir();
            }

            //2015-4-14 another folder for user in demo -> now is lfw
            File demouserfolder = new File(CommonUtil.CAMERAFOLDER.getAbsolutePath() + File.separator
                    + String.valueOf(userid));
            if (!demouserfolder.exists()) {
                demouserfolder.mkdir();
            }

        }
        // attention for this version!! if the user no pic, save the user's name anyway!!!

        // / face detection!
        xface = CommonUtil.xFaceLibrary;
        // / face detection!

        mOpenCvCameraView = (JavaCameraView) findViewById(R.id.cv_camera_signup);
        mOpenCvCameraView.setVisibility(SurfaceView.VISIBLE);
        mOpenCvCameraView.setCvCameraViewListener(this);
        mOpenCvCameraView.enableView();

        // used to handle the message sent from the thread reporting the result
        handler = new Handler(new Handler.Callback() {
            public boolean handleMessage(Message msg) {
                int result = msg.arg1;// arg1 = result, arg2 = [state]
                if (msg.arg2 == 1) {// when facerec! arg2=1
                    if (result == 0) {
                        Log.e(TAG, "add image fail!");
//						ToastUtil.showShortToast(getApplicationContext(), "add image fail!");
                    } else {
                        Log.e(TAG, "add image ok!");
                        ToastUtil.showShortToast(getApplicationContext(), "add image count=" + count);
                        count++;
                    }
                } else {// when init! arg2=0 !!! need!!!
                    if (result == 0) {
                        Log.e(TAG, "init facerec fail, result=" + result);
                        ToastUtil.showShortToast(getApplicationContext(), "Initialization Fail!");
                    } else {
                        Log.e(TAG, "init facerec ok, result=" + result);
                        ToastUtil.showShortToast(getApplicationContext(), "Initialization Successfully!");
                    }
                }

                return false;
            }
        });

        // [hujiawei] face detect!! thread used for detection!!
        new Thread(new Runnable() {
            public void run() {
                Log.e(TAG, "bInitFacerec= " + bInitFacerec + " $$ bExitDetection= " + bExitDetection
                        + " $$ bFrameDetecting=" + bFrameDetecting);
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
                // [hujiawei] [here!! change all recognition to detection!!!]
                while (!bExitDetection) {// is recognition exits?
                    if (!bFrameDetecting) {// is frame being processing?
                        if (null == mDet || mDet.empty()) {// it's hard to say when it is called!
                            Log.e(TAG, "detection image mat is null");
                            // return;// return when no data//can not return!!!!
                        } else {
                            bFrameDetecting = true;
                            Log.e(TAG, "runFaceDect! mat addr = " + mDet.getNativeObjAddr());// 2103032

                            String subpath = File.separator + String.valueOf(userid) + File.separator + System.currentTimeMillis() + ".jpg";
                            filePath = CommonUtil.USERFOLDER.getAbsolutePath() + subpath; //
                            String demopath = CommonUtil.CAMERAFOLDER.getAbsolutePath() + subpath;//

                            int result = xface.addImage(mDet, userid, filePath, demopath);
                            Message message = new Message();
                            message.arg1 = result;// add image result!
                            message.arg2 = 1;// doing detection!
                            if (result > 0) {// save data to facedata.txt
                                String data = filePath + ";" + userid + "\n";// user image file path;user id
                                CommonUtil.saveUserFaceData(data);
                            }
                            handler.sendMessage(message);
                            bFrameDetecting = false;
                        }
                        try {
                            Thread.currentThread().sleep(threadSleepTime);//
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                    }
                }
            }
        }).start();

    }

    public void btn_camera_back(View view) {
        Log.e(TAG, "btn_camera_back");
        bExitDetection = true;
        finish();
    }

//	public void btn_camera_ok(View view) {
//		Log.e(TAG, "btn_camera_ok");
//		// do what?
//		bExitDetection = true;
//		finish();
//	}

    @Override
    public void onPause() {
        super.onPause();
        Log.e(TAG, "on pause and finish!");
        if (mOpenCvCameraView != null) {
            mOpenCvCameraView.disableView();
        }
        bExitDetection = true;
        finish();//
    }

    @Override
    public void onResume() {
        super.onResume();
        Log.e(TAG, "on resume");
        if (mOpenCvCameraView != null && !mOpenCvCameraView.isEnabled()) {
            mOpenCvCameraView.enableView();
        }
    }

    public void onDestroy() {
        super.onDestroy();
        Log.e(TAG, "on destory");
        if (mOpenCvCameraView != null) {
            mOpenCvCameraView.disableView();
        }
        bExitDetection = true;
    }

    public void onCameraViewStarted(int width, int height) {
        Log.e(TAG, "camera view start");
        mGray = new Mat();
        mRgba = new Mat();
        mDet = new Mat();
    }

    public void onCameraViewStopped() {
        Log.e(TAG, "camera view stop");
        mGray.release();
        mRgba.release();
        mDet.release();
    }

    public Mat onCameraFrame(CvCameraViewFrame inputFrame) {
        // landscape 640*480 || portrait [320*240]-> 240*320!
        // when portrait mode, inputframe is 320*240, so pic is rotated!
        mRgba = inputFrame.rgba();
        mGray = inputFrame.gray();
        mGray.copyTo(mDet);

        return mRgba;
    }

}
