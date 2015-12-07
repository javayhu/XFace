package mface.thu.edu.cn.mface;

import android.util.Log;

import org.opencv.core.Mat;

import java.io.File;

import mface.thu.edu.cn.mface.util.FileUtil;

/**
 * @author hujiawei
 * @date 15/6/2 11:26
 */
public class PointFaceLib {

    private static final String TAG = "PointFaceLib";

    private static PointFaceLib lib;

    private static final String FILE_W2C = "w2c.txt";
    private static final String FILE_MALE = "codebook_male.txt";
    private static final String FILE_FEMALE = "codebook_female.txt";

    private PointFaceLib() {

    }

    public static PointFaceLib getInstance() {
        if (lib == null) {
            lib = new PointFaceLib();
        }
        return lib;
    }

    public double point(Mat mat, int flag) {
        Log.e("point", "address = " + String.valueOf(mat.getNativeObjAddr()));//3547552

        String s1 = FileUtil.getFolderPoint().getAbsolutePath() + File.separator + FILE_W2C;
        String s2 = FileUtil.getFolderPoint().getAbsolutePath() + File.separator + FILE_MALE;
        String s3 = FileUtil.getFolderPoint().getAbsolutePath() + File.separator + FILE_FEMALE;

        Log.e(TAG, s1 + " \n " + s2 + " \n" + s3);

        return nativePoint(mat.getNativeObjAddr(), flag, s1, s2, s3);
    }

    private native double nativePoint(long nativeObj, int flag, String s1, String s2, String s3);

}
