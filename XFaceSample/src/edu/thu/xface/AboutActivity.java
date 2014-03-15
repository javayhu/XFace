package edu.thu.xface;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;

/**
 * 
 * about
 * @author hujiawei
 *
 */
public class AboutActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_about);
	}

	public void btn_about_back(View view) {
		AboutActivity.this.finish();
	}

}
