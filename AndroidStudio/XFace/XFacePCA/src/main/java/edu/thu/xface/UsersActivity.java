package edu.thu.xface;

import java.util.ArrayList;
import java.util.List;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;

import edu.thu.xface.adapter.UserInfo;
import edu.thu.xface.adapter.UserSwipeAdapter;
import edu.thu.xface.util.CommonUtil;

/**
 * user list
 *
 * @author hujiawei
 */
public class UsersActivity extends Activity {

    private List<UserInfo> data = new ArrayList<UserInfo>();
    // private SwipeListView mListView;
    private ListView mListView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_users);

        data = CommonUtil.getAllUsers(true);

        mListView = (ListView) findViewById(R.id.lv_users_userlist);//

        UserSwipeAdapter mAdapter = new UserSwipeAdapter(getApplicationContext(), data);
        mListView.setAdapter(mAdapter);

        mListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                // Toast.makeText(UsersActivity.this, "item onclick " + position, Toast.LENGTH_SHORT).show();
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
