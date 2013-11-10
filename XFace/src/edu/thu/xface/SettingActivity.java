package edu.thu.xface;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.RelativeLayout;

public class SettingActivity extends Activity {

	private static final String TAG = "SettingActivity";

	private RelativeLayout rl_settings_about;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_setting);

		rl_settings_about = (RelativeLayout) findViewById(R.id.rl_settings_about);
		rl_settings_about.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				Intent intent = new Intent(SettingActivity.this, AboutActivity.class);
				startActivity(intent);
			}
		});
	}

	public void btn_setting_back(View view) {
		SettingActivity.this.finish();
	}

}
