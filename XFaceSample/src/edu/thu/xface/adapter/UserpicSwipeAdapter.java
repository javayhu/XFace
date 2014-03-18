package edu.thu.xface.adapter;

import java.io.File;
import java.util.List;

import android.annotation.TargetApi;
import android.content.Context;
import android.graphics.drawable.Drawable;
import android.os.Build;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.LinearLayout.LayoutParams;
import android.widget.RelativeLayout;
import android.widget.TextView;
import edu.thu.xface.R;
import edu.thu.xface.util.CommonUtil;

/**
 * 
 * list view adapter
 * 
 * @author hujiawei
 * 
 */
public class UserpicSwipeAdapter extends BaseAdapter {

	private Context mContext = null;
	private List<UserpicInfo> data;
	private int mRightWidth = 0;

	public UserpicSwipeAdapter(Context ctx, List<UserpicInfo> data, int rightWidth) {
		mContext = ctx;
		this.data = data;
		mRightWidth = rightWidth;
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

	@TargetApi(Build.VERSION_CODES.JELLY_BEAN)
	@Override
	public View getView(final int position, View convertView, ViewGroup parent) {
		ViewHolder holder;
		if (convertView == null) {
			convertView = LayoutInflater.from(mContext).inflate(R.layout.list_item_userpic, parent, false);
			holder = new ViewHolder();
			holder.userpics_item_left = (RelativeLayout) convertView.findViewById(R.id.userpics_item_left);
			holder.userpics_item_right = (RelativeLayout) convertView.findViewById(R.id.userpics_item_right);

			// holder.tv_list_userpic = (TextView) convertView.findViewById(R.id.tv_list_userpic);
			holder.iv_userpics_pic = (ImageView) convertView.findViewById(R.id.iv_userpics_pic);
			holder.tv_list_filename = (TextView) convertView.findViewById(R.id.tv_list_filename);

			convertView.setTag(holder);
		} else {
			holder = (ViewHolder) convertView.getTag();
		}

		LinearLayout.LayoutParams lp1 = new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
		holder.userpics_item_left.setLayoutParams(lp1);
		LinearLayout.LayoutParams lp2 = new LayoutParams(mRightWidth, LayoutParams.MATCH_PARENT);
		holder.userpics_item_right.setLayoutParams(lp2);

		UserpicInfo userpicInfo = data.get(position);

		// holder.tv_list_userpic.setText(String.valueOf(UserpicInfo.getUserid()));
		String path = CommonUtil.getUserpicFullpath(userpicInfo.getUserid(), userpicInfo.getFilename());
		holder.iv_userpics_pic.setImageDrawable(Drawable.createFromPath(path));
		holder.tv_list_filename.setText(userpicInfo.getFilename());

		holder.userpics_item_right.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				if (mListener != null) {
					mListener.onRightItemClick(v, position);
				}
			}
		});
		return convertView;
	}

	class ViewHolder {
		RelativeLayout userpics_item_left;
		RelativeLayout userpics_item_right;

		// TextView tv_list_userpic;
		ImageView iv_userpics_pic;
		TextView tv_list_filename;
	}

	private onRightItemClickListener mListener = null;

	public void setOnRightItemClickListener(onRightItemClickListener listener) {
		mListener = listener;
	}

	public interface onRightItemClickListener {
		void onRightItemClick(View v, int position);
	}
}
