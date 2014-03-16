package edu.thu.xface;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;
import java.util.Set;
import java.util.regex.Pattern;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import edu.thu.xface.util.CommonUtil;
import edu.thu.xface.util.ToastUtil;

/**
 * 
 * sign up
 * 
 * @author hujiawei
 * 
 */
public class SignupActivity extends Activity {

	private EditText et_signup_name;
	private String name = "";
	private String[] names;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_signup);
		et_signup_name = (EditText) findViewById(R.id.et_signup_name);
	}

	// choose a registered name!
	public void tv_signup_name(View view) {
		Pattern pattern = Pattern.compile("^[-\\+]?[\\d]*$");
		ArrayList<String> nameList = new ArrayList<String>();
		Set<String> keys = CommonUtil.userProps.stringPropertyNames();
		// System.out.println(keys);
		for (Iterator iterator = keys.iterator(); iterator.hasNext();) {
			String key = (String) iterator.next();
			if (pattern.matcher(key).matches()) {// key is number, then it's a name
				nameList.add(CommonUtil.userProps.getProperty(key));
			}
		}
		names = new String[nameList.size()];
		nameList.toArray(names);
		System.out.println(Arrays.toString(names));
		if (names.length <= 0) {
			ToastUtil.showShortToast(getApplicationContext(), "No one registered!");
			return;
		}
		// **SignupActivity.this** important!
		new AlertDialog.Builder(SignupActivity.this).setTitle("请选择你的名字:")
				.setItems(names, new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int which) {
						if (which >= 0) {
							et_signup_name.setText(names[which]);
						}
						// ToastUtil.showShortToast(getApplicationContext(), "you choose " + names[which]);
						dialog.dismiss();
					}
				}).show();
	}

	// go to camera
	public void btn_signup_gotocamera(View view) {
		Pattern pattern = Pattern.compile("^[-\\+]?[\\d]*$");
		name = et_signup_name.getText().toString();
		int userid = -1;
		// sharedPreferences.getStringSet("allnames", null);//android-11!!!
		if (null == name || "".equalsIgnoreCase(name)) {
			ToastUtil.showShortToast(getApplicationContext(), "请输入您的名字！");
			return;
		}
		if (CommonUtil.userProps.contains(name)) {// contains value?
			// ToastUtil.showShortToast(getApplicationContext(), "名字重复了哟亲！");
			Set<String> keys = CommonUtil.userProps.stringPropertyNames();
			// System.out.println(keys);
			for (Iterator iterator = keys.iterator(); iterator.hasNext();) {
				String key = (String) iterator.next();
				if (pattern.matcher(key).matches() && CommonUtil.userProps.get(key).equals(name)) {// key is number,
					userid = Integer.parseInt(key);
					break;
				}
			}
		}
		Intent intent = new Intent(SignupActivity.this, SignupCameraActivity.class);
		intent.putExtra("name", name);
		intent.putExtra("userid", userid);
		startActivity(intent);
		SignupActivity.this.finish();
	}
}
