package edu.thu.xface;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.TextView;
import edu.thu.xface.adapter.UserpicInfo;
import edu.thu.xface.adapter.UserpicSwipeAdapter;
import edu.thu.xface.util.CommonUtil;
import edu.thu.xface.util.ToastUtil;

/**
 * 
 * user pictures list
 * 
 * @author hujiawei
 * 
 */
public class UserpicsActivity extends Activity {

	private List<UserpicInfo> data = new ArrayList<UserpicInfo>();

	private SwipeListView mListView;
	private TextView tv_userlist_title;

	private UserpicSwipeAdapter mAdapter;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_userpics);
		tv_userlist_title = (TextView) findViewById(R.id.tv_userlist_title);

		int userid = getIntent().getIntExtra("userid", 0);
		String username = getIntent().getStringExtra("username");
		tv_userlist_title.setText("[" + userid + "] " + username);

		data = CommonUtil.getAllUserpics(userid);
		mListView = (SwipeListView) findViewById(R.id.lv_userpics_userlist);
		mAdapter = new UserpicSwipeAdapter(this, data, mListView.getRightViewWidth());

		mAdapter.setOnRightItemClickListener(new UserpicSwipeAdapter.onRightItemClickListener() {
			@Override
			public void onRightItemClick(View v, int position) {
				// ToastUtil.showShortToast(getApplicationContext(), "delete " + (position + 1) + "! 暂不支持删除操作! ");
				deleteUserpic(data.get(position));
				data.remove(position);
				mAdapter.notifyDataSetChanged();
				// java.lang.UnsupportedOperationException: removeViewAt(int) is not supported in AdapterView
				// mListView.removeViewAt(position);//
				// mListView.invalidate();
			}
		});

		mListView.setAdapter(mAdapter);

		mListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
			@Override
			public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
				// Toast.makeText(UserpicsActivity.this, "item onclick " + position, Toast.LENGTH_SHORT).show();
			}
		});
	}

	// delete user picture
	protected void deleteUserpic(UserpicInfo userpicInfo) {
		File file = new File(CommonUtil.getUserpicFullpath(userpicInfo.getUserid(), userpicInfo.getFilename()));
		if (file.exists()) {
			file.delete();// delete pic file
		}
		// delete one line face data!
		// current method is deleting old one, and create a new one!
		// if using this method, projections have to rebuild, for labels disorders!
		File file2 = new File(CommonUtil.FACEDATA_FILEPATH);
		if (file2.exists()) {
			file2.delete();// delete pic file
		}
		try {
			file2.createNewFile();// new on!
			BufferedWriter writer = new BufferedWriter(new FileWriter(file2));
			File demofolder = CommonUtil.DEMOFOLDER;
			File[] folders = demofolder.listFiles();
			File folder;
			for (int i = 0; i < folders.length; i++) {
				folder = folders[i];
				File[] images = folder.listFiles();
				for (int j = 0; j < images.length; j++) {
					writer.write(images[j].getAbsolutePath() + ";" + (i + 1) + "\n");
				}
			}
			writer.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		// delete one pic projection!
		//TODO! wait for another rebuild model!
	}

	public void btn_userpics_back(View view) {
		UserpicsActivity.this.finish();
	}

}
