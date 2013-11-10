package edu.thu.xface;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import edu.thu.xface.util.CommonUtil;
import edu.thu.xface.util.ToastUtil;

/**
 * sign up
 * 
 * @author hujiawei
 * 
 */
public class SignupActivity extends Activity {

	// private SharedPreferences sharedPreferences;
	private EditText et_signup_name;
	private String name = "";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_signup);
		// sharedPreferences = getSharedPreferences("xface", MODE_PRIVATE);
		et_signup_name = (EditText) findViewById(R.id.et_signup_name);
	}

	public void btn_signup_gotocamera(View view) {
		name = et_signup_name.getText().toString();
		// sharedPreferences.getStringSet("allnames", null);//android-11!!!
		if (null == name || "".equalsIgnoreCase(name)) {
			ToastUtil.showShortToast(getApplicationContext(), "请输入您的名字！");
		} else if (CommonUtil.userProps.contains(name)) {// contains value?
			ToastUtil.showShortToast(getApplicationContext(), "名字重复了哟亲！");
		} else {
			// ToastUtil.showShortToast(getApplicationContext(), "您的名字是 " + name);
			Intent intent = new Intent(SignupActivity.this, SignupCameraActivity.class);
			intent.putExtra("name", name);
			startActivity(intent);
			SignupActivity.this.finish();
		}
	}
}
