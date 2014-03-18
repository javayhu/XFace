package edu.thu.xface;

import java.util.ArrayList;
import java.util.List;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import edu.thu.xface.adapter.UserInfo;
import edu.thu.xface.adapter.UserSwipeAdapter;
import edu.thu.xface.util.CommonUtil;
import edu.thu.xface.util.ToastUtil;

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

		data = CommonUtil.getAllUsers(true);

		mListView = (SwipeListView) findViewById(R.id.lv_users_userlist);
		UserSwipeAdapter mAdapter = new UserSwipeAdapter(this, data, mListView.getRightViewWidth());

		mAdapter.setOnRightItemClickListener(new UserSwipeAdapter.onRightItemClickListener() {
			@Override
			public void onRightItemClick(View v, int position) {
				ToastUtil.showShortToast(getApplicationContext(), "暂不支持删除用户操作! ");
			}
		});

		mListView.setAdapter(mAdapter);

		mListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
			@Override
			public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
//				Toast.makeText(UsersActivity.this, "item onclick " + position, Toast.LENGTH_SHORT).show();
				Intent intent = new Intent(UsersActivity.this, UserpicsActivity.class);
				intent.putExtra("userid", data.get(position).getUserid());
				intent.putExtra("username", data.get(position).getName());
				startActivity(intent);
			}
		});
	}

	public void btn_users_back(View view) {
		UsersActivity.this.finish();
	}

}
