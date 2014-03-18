package edu.thu.xface;

import java.io.File;

import android.app.Activity;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import edu.thu.xface.libs.XFaceLibrary;
import edu.thu.xface.util.CommonUtil;
import edu.thu.xface.util.ToastUtil;

/**
 * 
 * logo index
 * 
 * @author hujiawei
 * 
 */
public class LogoActivity extends Activity {

	private static final String TAG = "LogoActivity";

	private Button btn_logo_signin;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_logo);
		btn_logo_signin = (Button) findViewById(R.id.btn_logo_signin);

		if (!CommonUtil.isAppInit) {
			// async do init app
			new AsyncTask<Void, Void, Boolean>() {

				@Override
				protected Boolean doInBackground(Void... params) {
					CommonUtil.initApp(getApplicationContext());
					// whether or not to re-calculate the model?
					File file = new File(CommonUtil.FACERECMODEL_FILEPATH);
					if (!file.exists()) {
						new XFaceLibrary().trainModel();// takes time! 44 seconds
					}
					return true;
				}

				@Override
				protected void onPostExecute(Boolean result) {
					if (result) {
						ToastUtil.showShortToast(getApplicationContext(), "应用程序初始化成功咯!");
						btn_logo_signin.setEnabled(true);
					} else {
						ToastUtil.showShortToast(getApplicationContext(), "应用程序初始化失败啦!");
					}
				}

				@Override
				protected void onPreExecute() {
					ToastUtil.showShortToast(getApplicationContext(), "应用程序初始化中...请稍候...");
					btn_logo_signin.setEnabled(false);
				}

			}.execute();
		}
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		menu.add("Setting");
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		Log.i(TAG, "called onOptionsItemSelected; selected item: " + item);
		Intent intent = new Intent(LogoActivity.this, SettingActivity.class);
		startActivity(intent);
		return true;
	}

	public void btn_logo_signup(View view) {
		Intent intent = new Intent(LogoActivity.this, SignupActivity.class);
		startActivity(intent);
	}

	public void btn_logo_signin(View view) {
		Intent intent = new Intent(LogoActivity.this, FacerecCameraActivity.class);
		startActivity(intent);
	}

}
