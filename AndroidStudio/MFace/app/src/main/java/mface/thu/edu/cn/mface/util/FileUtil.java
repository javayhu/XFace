package mface.thu.edu.cn.mface.util;

import android.content.Context;
import android.content.res.AssetManager;
import android.os.Environment;
import android.util.Log;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;

import mface.thu.edu.cn.mface.R;

/**
 * File util
 */
public class FileUtil {

    private final String TAG = FileUtil.class.getSimpleName();

    private Context context;
    private static FileUtil instance = null;

    // folder
    public static final String FOLDER_MFACE_NAME = "pointface";

    public static final String FOLDER_POINT_NAME = "point";

    private FileUtil(Context context) {
        this.context = context;
    }

    public static FileUtil getInstance(Context context) {
        if (instance == null) {
            instance = new FileUtil(context);
        }
        return instance;
    }

    // when init app, do some file stuff, this job does only once, when first init the app
    public void initAppFiles() throws Exception {
        Log.e(TAG, "init app files");

        copyAssets(FOLDER_POINT_NAME, getSDFolder(FOLDER_POINT_NAME));

        //these files can be copyed every time when app first launch
        //copyAssets(FOLDER_CASCADE_NAME, getSDFolder(FOLDER_CASCADE_NAME));
        //copyAssets(FOLDER_DATA_NAME, getSDFolder(FOLDER_DATA_NAME));
        //copyAssets(FOLDER_USER_NAME, getSDFolder(FOLDER_USER_NAME));
    }

    //////////////////  methonds can be called outside  ////////////////////

    public static File getFolderPoint() {
        return new File(getSDFolder(FOLDER_POINT_NAME));
    }

    //////////////////  methonds can be called only inside  ////////////////////

    // get some folder from the app sd root folder
    private static String getSDFolder(String folder) {
        String sdfolder = getSDRootFolder() + File.separator + folder;
        File file = new File(sdfolder);
        if (!file.exists()) {
            file.mkdirs();
        }
        return file.getAbsolutePath();
    }

    // get app sd root folder path
    private static String getSDRootFolder() {//TODO: sd should be mounted!!!
        String root = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator + FOLDER_MFACE_NAME;
        File file = new File(root);
        if (!file.exists()) {
            file.mkdirs();
        }
        return file.getAbsolutePath();
    }

    // copy assets files to sdcard
    private void copyAssets(String assetdir, String sddir) throws Exception {
        Log.e(TAG, "copy assets from " + assetdir + " -> " + sddir);
        AssetManager assetManager = context.getResources().getAssets();
        String[] files = assetManager.list(assetdir);
        File mWorkingPath = new File(sddir);
        if (!mWorkingPath.exists()) {// if this directory does not exists, make one.
            mWorkingPath.mkdirs();
        }
        for (int i = 0; i < files.length; i++) {
            String fileName = files[i];
            if (!fileName.contains(".")) {// we make sure file name not contains '.' to be a folder.
                if (0 == assetdir.length()) {//asset root folder
                    copyAssets(fileName, sddir + File.separator + fileName + File.separator);
                } else {
                    copyAssets(assetdir + File.separator + fileName, sddir + File.separator + fileName + File.separator);
                }
                continue;
            }
            File outFile = new File(mWorkingPath, fileName);
            if (outFile.exists()) outFile.delete();
            InputStream in = null;
            if (0 != assetdir.length())
                in = assetManager.open(assetdir + File.separator + fileName);
            else
                in = assetManager.open(fileName);
            OutputStream out = new FileOutputStream(outFile);
            int len;
            byte[] buf = new byte[1024 * 2];
            while ((len = in.read(buf)) > 0) {// Transfer bytes from in to out
                out.write(buf, 0, len);
            }
            in.close();
            out.close();
        }
    }

    // delete a directory
    private boolean deleteDir(File dir) {
        if (dir.isDirectory()) {
            String[] children = dir.list();
            for (int i = 0; i < children.length; i++) {
                boolean success = deleteDir(new File(dir, children[i]));
                if (!success) {
                    return false;
                }
            }
        }
        return dir.delete(); // The directory is empty now and can be deleted.
    }
}
