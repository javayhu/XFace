package edu.thu.xface;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import android.app.Activity;
import android.os.Bundle;
import android.view.ContextMenu;
import android.view.ContextMenu.ContextMenuInfo;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnCreateContextMenuListener;
import android.widget.AdapterView.AdapterContextMenuInfo;
import android.widget.ListView;
import android.widget.TextView;
import edu.thu.xface.adapter.UserpicInfo;
import edu.thu.xface.adapter.UserpicSwipeAdapter;
import edu.thu.xface.util.CommonUtil;

/**
 * 
 * user pictures list
 * 
 * @author hujiawei
 * 
 */
public class UserpicsActivity extends Activity {

	private List<UserpicInfo> data = new ArrayList<UserpicInfo>();

	private ListView mListView;
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
		mListView = (ListView) findViewById(R.id.lv_userpics_userlist);
		mAdapter = new UserpicSwipeAdapter(getApplicationContext(), data);
		mListView.setAdapter(mAdapter);

		mListView.setOnCreateContextMenuListener(new OnCreateContextMenuListener() {
			@Override
			public void onCreateContextMenu(ContextMenu menu, View v, ContextMenuInfo menuInfo) {
				menu.add(0, 0, 0, "删除");
			}
		});

	}

	@Override
	public boolean onContextItemSelected(MenuItem item) {
		AdapterContextMenuInfo menuInfo = (AdapterContextMenuInfo) item.getMenuInfo();
		int position = menuInfo.position;
//		ToastUtil.showShortToast(getApplicationContext(), "delete " + (position) + "!");
		deleteUserpic(data.get(position));
		data.remove(position);
		mAdapter.notifyDataSetChanged();
		return super.onContextItemSelected(item);
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
			file2.createNewFile();// new one!
			BufferedWriter writer = new BufferedWriter(new FileWriter(file2));
			File demofolder = CommonUtil.USERFOLDER;
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
		// TODO! wait for another rebuild model!
	}

	public void btn_userpics_back(View view) {
		UserpicsActivity.this.finish();
	}

}
