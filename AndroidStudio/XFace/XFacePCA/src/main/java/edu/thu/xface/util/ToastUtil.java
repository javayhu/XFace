package edu.thu.xface.util;

import android.content.Context;
import android.widget.Toast;

/**
 * 显示Toast信息工具类
 * 
 * @author hujiawei
 */
public class ToastUtil {

	public static void showLongToast(Context context, String message) {
		Toast.makeText(context, message, Toast.LENGTH_LONG).show();
	}

	public static void showShortToast(Context context, String message) {
		Toast.makeText(context, message, Toast.LENGTH_SHORT).show();
	}

}
