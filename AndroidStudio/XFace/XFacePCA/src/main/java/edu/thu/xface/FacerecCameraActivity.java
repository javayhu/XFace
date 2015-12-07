package edu.thu.xface;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;

import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;
import org.opencv.android.JavaCameraView;
import org.opencv.core.Mat;

import edu.thu.xface.libs.XFaceLibrary;
import edu.thu.xface.util.CommonUtil;
import edu.thu.xface.util.ToastUtil;

/**
 * facerec camera
 *
 * @author hujiawei
 */
public class FacerecCameraActivity extends Activity implements CvCameraViewListener2 {

    private static final String TAG = "FacerecCameraActivity";

    private JavaCameraView mOpenCvCameraView;
    private boolean bInitFacerec = false;
    private boolean bFrameRecing = false;
    private boolean bExitRecognition = false;
    private long threadSleepTime = 1000;

    // face recognition!!
    private Mat mRec;
    private Mat mRgba;
    private Mat mGray;
    private int count = 0;
    private Handler handler;
    private XFaceLibrary xface;
    // face recognition!!

    // face detection!!
    // private int mAbsoluteFaceSize = 0;
    // private float mRelativeFaceSize = 0.2f;
    // private static final Scalar FACE_RECT_COLOR = new Scalar(0, 255, 0, 255);
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
        xface = CommonUtil.xFaceLibrary;
        // / face detection!

        // used to handle the message sent from the thread reporting the result
        handler = new Handler(new Handler.Callback() {
            public boolean handleMessage(Message msg) {
                int result = msg.arg1;// arg1 = result, arg2 = [state]
                if (msg.arg2 == 1) {// when facerec! arg2=1
                    if (result == -1) {
                        Log.e(TAG, "unkown person!");
                        ToastUtil.showShortToast(getApplicationContext(), "I do't know you !");//
                    } else {
                        String name = CommonUtil.userProps.getProperty(String.valueOf(result));
                        Log.e(TAG, "user id=" + result + " ## name=" + name);
                        ToastUtil.showShortToast(getApplicationContext(), count + ": You are " + name + " !");
                        count++;
                        //finish();
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

    }

    public void btn_facerec_back(View view) {
        Log.e(TAG, "btn_facerec_back");
        bExitRecognition = true;// bug!!! for earlier versions!!!
        finish();//
    }

    // onpause, onresume has problems!!!
    @Override
    public void onPause() {
        super.onPause();
        Log.e(TAG, "on pause and finish!");
        if (mOpenCvCameraView != null) {
            mOpenCvCameraView.disableView();
        }
        bExitRecognition = true;//
        finish();
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
        Log.e(TAG, "on destroy");
        if (mOpenCvCameraView != null) {
            mOpenCvCameraView.disableView();
        }
        bExitRecognition = true;//
    }

    public void onCameraViewStarted(int width, int height) {
        Log.e(TAG, "camera view start");
        mGray = new Mat();
        mRgba = new Mat();
        mRec = new Mat();

        // thread used for recogniton!
        new Thread(new Runnable() {
            public void run() {
                Log.e(TAG, "bInitFacerec= " + bInitFacerec + " $$ bExitRecognition= " + bExitRecognition
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
                    if (result != 0) {
                        bInitFacerec = true;// no longer init!
                    } else {
                        return;// if fail, stop the thread!
                    }
                }// problem exist! if facerec not initialized, face rec action will still do! solution is above!

                Log.e(TAG, "bInitFacerec= " + bInitFacerec + " $$ bExitRecognition= " + bExitRecognition
                        + " $$ frameprocessing=" + bFrameRecing);
                while (true/*!bExitRecognition*/) {// is recognition exits?
                    if (true/*!bFrameRecing*/) {// is frame being processing?
                        if (null == mRec || mRec.empty()) {// it's hard to say when it is called!
                            Log.e(TAG, "rec image mat is null");
                            // return;// return when no data//can not return!!!!
                        } else {
                            bFrameRecing = true;
                            Log.e(TAG, "runFacerec! mat addr = " + mRec.getNativeObjAddr());// 2103032
                            int result = xface.facerec(mRec);
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

    public void onCameraViewStopped() {
        Log.e(TAG, "camera view stop");
        mGray.release();
        mRgba.release();
        mRec.release();
    }

    // preview frame!!!
    public Mat onCameraFrame(CvCameraViewFrame inputFrame) {
        Log.e(TAG, "inputFrame size = "+inputFrame.gray().width() + "*" + inputFrame.gray().height());// 720*480, 800*600
        // image frame size is 480*720!!! here image is rotated when showing!
        mRgba = inputFrame.rgba();
        mGray = inputFrame.gray();
        mGray.copyTo(mRec);

//		xface.facedet(mRgba, mGray);//

        return mRgba;
    }

}
