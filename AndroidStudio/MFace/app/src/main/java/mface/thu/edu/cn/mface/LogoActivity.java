package mface.thu.edu.cn.mface;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.res.Configuration;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.util.Log;
import android.view.View;

import mface.thu.edu.cn.mface.util.CommonUtil;

/**
 * 进入之后的首页
 */
public class LogoActivity extends ActionBarActivity {

    private static final String TAG = LogoActivity.class.getSimpleName();

    private ProgressDialog progressDialog;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_logo);

        initApp();
    }

    @Override
    protected void onResume() {//move the code from onCreate to onResume, for 'clean data' in setttings
        //it may take quite a long time to copy needed files, so make async task to do it
        super.onResume();//**NOTE** when rotate the screen orientation, this may trigger again
    }

    private void initApp() {
        // the async task to train a model
        new AsyncTask<Void, Void, Void>() {
            @Override
            protected void onPreExecute() {
                //Unable to add window -- token null is not for an application
                progressDialog = ProgressDialog.show(LogoActivity.this, getString(R.string.dialog_wait),
                        getString(R.string.dialog_app_initialization), false, false);
                super.onPreExecute();
            }

            @Override
            protected Void doInBackground(Void... params) {
                Log.e(TAG, "app is initing");
                try {
                    CommonUtil.getInstance(getApplicationContext()).initApp();
                } catch (Exception e) {
                    Log.e(TAG, "init app exception");
                    e.printStackTrace();
                }
                return null;
            }

            @Override
            protected void onPostExecute(Void result) {
                progressDialog.dismiss();
                super.onPostExecute(result);
            }
        }.execute();

    }

    // go to point
    public void btn_logo_point(View view) {
        //query gender
        final Intent intent = new Intent(this, MainActivity.class);

        final AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setCancelable(false);
        builder.setIcon(R.drawable.ic_logo);
        builder.setTitle("Gender");
        builder.setMessage("Are you a boy?");
        builder.setPositiveButton("YES",
                new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int whichButton) {
                        intent.putExtra("gender", 1);
                        startActivity(intent);
                    }
                });
        builder.setNegativeButton("NO",
                new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int whichButton) {
                        intent.putExtra("gender", 0);
                        startActivity(intent);
                    }
                });
        builder.show();
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        Log.e(TAG, "onConfigurationChanged");
    }
}
