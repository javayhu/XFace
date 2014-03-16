package edu.thu.xface;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.RadioButton;
import android.widget.RelativeLayout;
import edu.thu.xface.util.CommonUtil;

/**
 * setting
 * 
 * @author hujiawei
 * 
 */
public class SettingActivity extends Activity {

	private RelativeLayout rl_settings_about;
	private RelativeLayout rl_settings_users;
	private RelativeLayout rl_settings_suggestion;
	private RadioButton rb_face_eigen;
	private RadioButton rb_face_fisher;
	private RadioButton rb_face_lbph;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_setting);

		// used to test!
		rl_settings_suggestion = (RelativeLayout) findViewById(R.id.rl_settings_suggestion);
		rl_settings_suggestion.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				// Intent intent = new Intent(SettingActivity.this, FdActivity.class);
				// startActivity(intent);
			}
		});

		rl_settings_about = (RelativeLayout) findViewById(R.id.rl_settings_about);
		rl_settings_about.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				Intent intent = new Intent(SettingActivity.this, AboutActivity.class);
				startActivity(intent);
			}
		});

		rl_settings_users = (RelativeLayout) findViewById(R.id.rl_settings_users);
		rl_settings_users.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				Intent intent = new Intent(SettingActivity.this, UsersActivity.class);
				startActivity(intent);
			}
		});

		rb_face_eigen = (RadioButton) findViewById(R.id.rb_face_eigen);
		rb_face_fisher = (RadioButton) findViewById(R.id.rb_face_fisher);
		rb_face_lbph = (RadioButton) findViewById(R.id.rb_face_lbph);

		if (CommonUtil.FACERECOGNIZER.equalsIgnoreCase(CommonUtil.FACE_EIGEN)) {
			rb_face_eigen.setChecked(true);
		} else if (CommonUtil.FACERECOGNIZER.equalsIgnoreCase(CommonUtil.FACE_FISHER)) {
			rb_face_fisher.setChecked(true);
		} else if (CommonUtil.FACERECOGNIZER.equalsIgnoreCase(CommonUtil.FACE_LBPH)) {
			rb_face_lbph.setChecked(true);
		}

	}

	private void resetFacerec(String face) {
		CommonUtil.userProps.setProperty(CommonUtil.FACE_ALGORITHM, face);
		try {
			CommonUtil.saveUserProperties(CommonUtil.userProps);
		} catch (Exception e) {
			e.printStackTrace();
		}
		CommonUtil.FACERECOGNIZER = face;
	}

	public void rb_face_fisher(View view) {
		resetFacerec(CommonUtil.FACE_FISHER);
	}

	public void rb_face_eigen(View view) {
		resetFacerec(CommonUtil.FACE_EIGEN);
	}

	public void rb_face_lbph(View view) {
		resetFacerec(CommonUtil.FACE_LBPH);
	}

	public void btn_setting_back(View view) {
		SettingActivity.this.finish();
	}

}
