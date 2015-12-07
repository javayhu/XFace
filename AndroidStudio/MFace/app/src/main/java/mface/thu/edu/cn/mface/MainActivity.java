package mface.thu.edu.cn.mface;

import android.app.ProgressDialog;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;

import org.opencv.android.CameraBridgeViewBase;
import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.Point;
import org.opencv.core.Scalar;

import mface.thu.edu.cn.mface.util.ToastUtil;
import mface.thu.edu.cn.mface.view.MFaceCameraView;

/**
 * 给脸评分的操作界面
 */
public class MainActivity extends ActionBarActivity implements CameraBridgeViewBase.CvCameraViewListener2 {

    private static final String TAG = MainActivity.class.getSimpleName();

    private int gender = 1;
    private Mat mRgba;
    private Mat mGray;
    //private Mat mPoint;
    private MFaceCameraView mOpenCvCameraView;

    private static final Scalar FACE_RECT_COLOR = new Scalar(0, 255, 0, 255);

    private ProgressDialog progressDialog;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        gender = getIntent().getIntExtra("gender", 1);//gender

        mOpenCvCameraView = (MFaceCameraView) findViewById(R.id.cv_main_camera);
        mOpenCvCameraView.enableView();
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
        mOpenCvCameraView.enableView();
        mOpenCvCameraView.setCameraIndex(CameraBridgeViewBase.CAMERA_ID_FRONT);
    }

    public void onDestroy() {
        super.onDestroy();
        if (mOpenCvCameraView != null)
            mOpenCvCameraView.disableView();
    }

    public void onCameraViewStarted(int width, int height) {
        mGray = new Mat();
        mRgba = new Mat();
        //mPoint = new Mat();
    }

    public void onCameraViewStopped() {
        mGray.release();
        mRgba.release();
        //mPoint.release();
    }

    // when a camera frame comes in!
    public Mat onCameraFrame(CameraBridgeViewBase.CvCameraViewFrame inputFrame) {
        mRgba = inputFrame.rgba();
        mGray = inputFrame.gray();

        int y = mRgba.rows();
        int x = mRgba.cols();
        Point tl = new Point(0.3 * x, 0.2 * y);
        Point br = new Point(0.7 * x, 0.8 * y);
        Log.e(TAG, "mRgba rows = " + y + " cols=" + x);

        Core.rectangle(mRgba, tl, br, FACE_RECT_COLOR, 3);

        return mRgba;
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId() == R.id.action_point) {
            if (mRgba != null && !mRgba.empty()) {
                final Mat mPoint = new Mat();
                mRgba.copyTo(mPoint);

                new AsyncTask<Void, Void, Double>() {
                    @Override
                    protected void onPreExecute() {
                        mOpenCvCameraView.disableView();
                        //Unable to add window -- token null is not for an application
                        progressDialog = ProgressDialog.show(MainActivity.this, getString(R.string.dialog_wait),
                                getString(R.string.dialog_pointing_face), false, false);
                        super.onPreExecute();
                    }

                    @Override
                    protected Double doInBackground(Void... params) {
                        Log.e(TAG, "pointing face");
                        double result = 0.0;
                        try {
                            int y = mPoint.rows();
                            int x = mPoint.cols();
                            Log.e(TAG, "mPoint rows = " + y + " cols=" + x);//rows = 0 cols=640
                            //Point tl = new Point(0.3 * x, 0.2 * y);
                            //Point br = new Point(0.7 * x, 0.8 * y);

                            //Mat mPoint2 = new Mat();
                            Mat mPoint2 = mPoint.submat((int) (0.2 * y), (int) (0.8 * y), (int) (0.3 * x), (int) (0.7 * x)).clone();//submat(int rowStart, int rowEnd, int colStart, int colEnd)
                            y = mPoint2.rows();
                            x = mPoint2.cols();
                            Log.e(TAG, "mPoint2 rows = " + y + " cols=" + x);//rows = 0 cols=640


                            if (!mPoint2.empty()){
                                result = PointFaceLib.getInstance().point(mPoint2, gender);
                            }

                            //result = PointFaceLib.getInstance().point(mRgba, gender);

                        } catch (Exception e) {
                            Log.e(TAG, "pointing face exception");
                            e.printStackTrace();
                        }
                        return result;
                    }

                    @Override
                    protected void onPostExecute(Double result) {
                        ToastUtil.showLongToast(MainActivity.this, "Your point is " + result);//show point

                        mOpenCvCameraView.enableView();
                        progressDialog.dismiss();
                        super.onPostExecute(result);
                    }
                }.execute();

            } else {
                ToastUtil.showShortToast(this, "Retry");
            }
        }
        return super.onOptionsItemSelected(item);
    }
}
