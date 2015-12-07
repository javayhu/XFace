package mface.thu.edu.cn.mface.util;

import android.content.Context;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.util.Log;

/**
 * Common util
 */
public class CommonUtil {

    private final String TAG = CommonUtil.class.getSimpleName();

    private Context context;
    private static SharedPreferences preferences;
    private static CommonUtil instance = null;

    public static final int IMAGE_WIDTH = 250;//how to choose the best width and height?!
    public static final int IMAGE_HEIGHT = 250;

    public static final String APPNAME = "mface";
    private static final String INITAPP_KEY = "initapp";

    private CommonUtil(Context context) {
        this.context = context;
        this.preferences = PreferenceManager.getDefaultSharedPreferences(context);
        //this.preferences = context.getSharedPreferences(APPNAME, Context.MODE_PRIVATE);//
    }

    public static CommonUtil getInstance(Context context) {
        if (instance == null) {
            instance = new CommonUtil(context);
            return instance;
        }
        return instance;
    }

    public static SharedPreferences getAppPreferences() {
        return preferences;
    }

    // do something to init app
    public void initApp() throws Exception {
        Log.e(TAG, "init app");
        // things should be done only once when first launching the app
        FileUtil.getInstance(context).initAppFiles();

        // things should be done every time launching the app
        System.loadLibrary("opencv_java");//load libs when app starts
        System.loadLibrary("pointface");
        //UserUtil.getInstance().importUsers();//everytime import users first
    }

}
