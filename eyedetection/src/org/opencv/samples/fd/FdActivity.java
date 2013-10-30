package org.opencv.samples.fd;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.RelativeLayout;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.ToggleButton;
import android.widget.SeekBar.OnSeekBarChangeListener;

public class FdActivity extends Activity {
	private static final String TAG = "Sample::Activity";

	private MenuItem mItemFace50;
	private MenuItem mItemFace40;
	private MenuItem mItemFace30;
	private MenuItem mItemFace20;
	private MenuItem mItemType;

	private FdView mView;
	private TextView matching_method;
	public static int method = 1;

	private BaseLoaderCallback mOpenCVCallBack = new BaseLoaderCallback(this) {
		@Override
		public void onManagerConnected(int status) {
			switch (status) {
			case LoaderCallbackInterface.SUCCESS: {
				Log.i(TAG, "OpenCV loaded successfully");

				// Load native libs after OpenCV initialization
				// System.loadLibrary("detection_based_tracker");

				// Create and set View
				mView = new FdView(mAppContext);
				mView.setDetectorType(mDetectorType);
				mView.setMinFaceSize(0.2f);

				VerticalSeekBar VerticalseekBar = new VerticalSeekBar(
						getApplicationContext());
				VerticalseekBar.setMax(5);
				VerticalseekBar.setPadding(20, 20, 20, 20);
				RelativeLayout.LayoutParams vsek = new RelativeLayout.LayoutParams(
						RelativeLayout.LayoutParams.WRAP_CONTENT, 400);
				vsek.addRule(RelativeLayout.ALIGN_PARENT_RIGHT);
				VerticalseekBar.setId(1);
				VerticalseekBar
						.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {

							public void onProgressChanged(SeekBar seekBar,
									int progress, boolean fromUser) {

								method = progress;
								switch (method) {
								case 0:
									matching_method.setText("TM_SQDIFF");
									break;
								case 1:
									matching_method.setText("TM_SQDIFF_NORMED");
									break;
								case 2:
									matching_method.setText("TM_CCOEFF");
									break;
								case 3:
									matching_method.setText("TM_CCOEFF_NORMED");
									break;
								case 4:
									matching_method.setText("TM_CCORR");
									break;
								case 5:
									matching_method.setText("TM_CCORR_NORMED");
									break;
								}

							}

							public void onStartTrackingTouch(SeekBar seekBar) {
							}

							public void onStopTrackingTouch(SeekBar seekBar) {
							}
						});

				matching_method = new TextView(getApplicationContext());
				matching_method.setText("TM_SQDIFF");
				matching_method.setTextColor(Color.YELLOW);
				RelativeLayout.LayoutParams matching_method_param = new RelativeLayout.LayoutParams(
						RelativeLayout.LayoutParams.WRAP_CONTENT,
						RelativeLayout.LayoutParams.WRAP_CONTENT);
				matching_method_param
						.addRule(RelativeLayout.ALIGN_PARENT_RIGHT);
				matching_method_param.addRule(RelativeLayout.BELOW,
						VerticalseekBar.getId());

				Button btn = new Button(getApplicationContext());
				btn.setText("Create template");
				RelativeLayout.LayoutParams btnp = new RelativeLayout.LayoutParams(
						RelativeLayout.LayoutParams.WRAP_CONTENT,
						RelativeLayout.LayoutParams.WRAP_CONTENT);
				btnp.addRule(RelativeLayout.ALIGN_PARENT_LEFT);
				btn.setId(2);

				btn.setOnClickListener(new OnClickListener() {
					public void onClick(View v) {
						mView.resetLearFramesCount();
					}
				});

				RelativeLayout frameLayout = new RelativeLayout(
						getApplicationContext());
				frameLayout.addView(mView, 0);
				frameLayout.addView(btn, btnp);

				frameLayout.addView(VerticalseekBar, vsek);
				frameLayout.addView(matching_method, matching_method_param);

				setContentView(frameLayout);

				// Check native OpenCV camera
				if (!mView.openCamera()) {
					AlertDialog ad = new AlertDialog.Builder(mAppContext)
							.create();
					ad.setCancelable(false); // This blocks the 'BACK' button
					ad.setMessage("Fatal error: can't open camera!");
					ad.setButton("OK", new DialogInterface.OnClickListener() {
						public void onClick(DialogInterface dialog, int which) {
							dialog.dismiss();
							finish();
						}
					});
					ad.show();
				}
			}
				break;
			default: {
				super.onManagerConnected(status);
			}
				break;
			}
		}
	};

	private int mDetectorType = 0;
	private String[] mDetectorName;

	public FdActivity() {
		Log.i(TAG, "Instantiated new " + this.getClass());
		mDetectorName = new String[2];
		mDetectorName[FdView.JAVA_DETECTOR] = "Java";
		mDetectorName[FdView.NATIVE_DETECTOR] = "Native (tracking)";
	}

	@Override
	protected void onPause() {
		Log.i(TAG, "onPause");
		super.onPause();
		if (mView != null)
			mView.releaseCamera();
	}

	@Override
	protected void onResume() {
		Log.i(TAG, "onResume");
		super.onResume();
		if (mView != null && !mView.openCamera()) {
			AlertDialog ad = new AlertDialog.Builder(this).create();
			ad.setCancelable(false); // This blocks the 'BACK' button
			ad.setMessage("Fatal error: can't open camera!");
			ad.setButton("OK", new DialogInterface.OnClickListener() {
				public void onClick(DialogInterface dialog, int which) {
					dialog.dismiss();
					finish();
				}
			});
			ad.show();
		}
	}

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		Log.i(TAG, "onCreate");
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);

		Log.i(TAG, "Trying to load OpenCV library");
		if (!OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_2, this,
				mOpenCVCallBack)) {
			Log.e(TAG, "Cannot connect to OpenCV Manager");
		}
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		Log.i(TAG, "onCreateOptionsMenu");
		mItemFace50 = menu.add("Face size 50%");
		mItemFace40 = menu.add("Face size 40%");
		mItemFace30 = menu.add("Face size 30%");
		mItemFace20 = menu.add("Face size 20%");
		mItemType = menu.add(mDetectorName[mDetectorType]);

		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		Log.i(TAG, "Menu Item selected " + item);
		if (item == mItemFace50)
			mView.setMinFaceSize(0.5f);
		else if (item == mItemFace40)
			mView.setMinFaceSize(0.4f);
		else if (item == mItemFace30)
			mView.setMinFaceSize(0.3f);
		else if (item == mItemFace20)
			mView.setMinFaceSize(0.2f);
		else if (item == mItemType) {
			mDetectorType = (mDetectorType + 1) % mDetectorName.length;
			item.setTitle(mDetectorName[mDetectorType]);
			mView.setDetectorType(mDetectorType);
		}
		return true;
	}
}
