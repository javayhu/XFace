package mface.thu.edu.cn.mface.util;

import android.content.Context;
import android.widget.Toast;

/**
 * Toast message tool
 */
public class ToastUtil {

    public static void showLongToast(Context context, String message) {
        Toast.makeText(context, message, Toast.LENGTH_LONG).show();
    }

    public static void showShortToast(Context context, String message) {
        Toast.makeText(context, message, Toast.LENGTH_SHORT).show();
    }

}
