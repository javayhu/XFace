package edu.thu.xface;

import java.util.ArrayList;
import java.util.List;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Toast;
import edu.thu.xface.adapter.SwipeAdapter;
import edu.thu.xface.adapter.UserInfo;
import edu.thu.xface.util.CommonUtil;

/**
 * 
 * user list
 * 
 * @author hujiawei
 * 
 */
public class UsersActivity extends Activity {

	private List<UserInfo> data = new ArrayList<UserInfo>();
	private SwipeListView mListView;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_users);

		data = CommonUtil.getAllUsers();

		mListView = (SwipeListView) findViewById(R.id.lv_users_userlist);
		SwipeAdapter mAdapter = new SwipeAdapter(this, data, mListView.getRightViewWidth());

		mAdapter.setOnRightItemClickListener(new SwipeAdapter.onRightItemClickListener() {
			@Override
			public void onRightItemClick(View v, int position) {
				Toast.makeText(UsersActivity.this, "delete " + (position + 1) + " record ", Toast.LENGTH_SHORT).show();
			}
		});

		mListView.setAdapter(mAdapter);

		mListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
			@Override
			public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
				Toast.makeText(UsersActivity.this, "item onclick " + position, Toast.LENGTH_SHORT).show();
			}
		});
	}

	public void btn_users_back(View view) {
		UsersActivity.this.finish();
	}

}
