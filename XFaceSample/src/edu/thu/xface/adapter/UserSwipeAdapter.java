package edu.thu.xface.adapter;

import java.util.List;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.RelativeLayout;
import android.widget.TextView;
import edu.thu.xface.R;

/**
 * 
 * list view adapter
 * 
 * @author hujiawei
 * 
 */
public class UserSwipeAdapter extends BaseAdapter {

	private Context mContext = null;
	private List<UserInfo> data;

	public UserSwipeAdapter(Context ctx, List<UserInfo> data) {
		this.mContext = ctx;
		this.data = data;
	}

	@Override
	public int getCount() {
		return data.size();
	}

	@Override
	public Object getItem(int position) {
		return data.get(position);
	}

	@Override
	public long getItemId(int position) {
		return position;
	}

	@Override
	public View getView(final int position, View convertView, ViewGroup parent) {
		ViewHolder holder;
		if (convertView == null) {
			convertView = LayoutInflater.from(mContext).inflate(R.layout.list_item_user, parent, false);
			holder = new ViewHolder();
			holder.item_left = (RelativeLayout) convertView.findViewById(R.id.item_left);

			holder.tv_list_name = (TextView) convertView.findViewById(R.id.tv_list_name);
			holder.tv_list_info = (TextView) convertView.findViewById(R.id.tv_list_info);

			convertView.setTag(holder);
		} else {
			holder = (ViewHolder) convertView.getTag();
		}

		UserInfo userinfo = data.get(position);

		holder.tv_list_name.setText("[" + userinfo.getUserid() + "] " + userinfo.getName());
		holder.tv_list_info.setText("pictures: " + userinfo.getCount());

		return convertView;
	}

	class ViewHolder {
		RelativeLayout item_left;

		TextView tv_list_name;
		TextView tv_list_info;
	}

}
