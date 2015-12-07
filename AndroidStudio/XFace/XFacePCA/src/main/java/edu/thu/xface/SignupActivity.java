package edu.thu.xface;

import java.util.ArrayList;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;

import edu.thu.xface.adapter.UserInfo;
import edu.thu.xface.util.CommonUtil;
import edu.thu.xface.util.ToastUtil;

/**
 * sign up
 *
 * @author hujiawei
 */
public class SignupActivity extends Activity {

    private EditText et_signup_name;

    private String name = "";
    private String[] names;
    private ArrayList<UserInfo> userInfos;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_signup);
        et_signup_name = (EditText) findViewById(R.id.et_signup_name);
    }

    // choose a registered name!
    public void tv_signup_name(View view) {
        userInfos = CommonUtil.getAllUsers(false);//no image count
        names = new String[userInfos.size()];
        for (int i = 0; i < userInfos.size(); i++) {
            names[i] = userInfos.get(i).getName();
        }
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
                        dialog.dismiss();
                    }
                }).show();
    }

    // go to camera
    public void btn_signup_gotocamera(View view) {
        name = et_signup_name.getText().toString();
        int userid = -1;
        // sharedPreferences.getStringSet("allnames", null);//android-11!!!
        if (null == name || "".equalsIgnoreCase(name)) {
            ToastUtil.showShortToast(getApplicationContext(), "请输入您的名字！");
            return;
        }
        if (CommonUtil.userProps.contains(name)) {// contains value?
            // ToastUtil.showShortToast(getApplicationContext(), "名字重复了哟亲！");
            for (int i = 0; i < userInfos.size(); i++) {
                if (userInfos.get(i).getName().equalsIgnoreCase(name)) {
                    userid = userInfos.get(i).getUserid();
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
