package edu.thu.xface.libs;

import android.util.Log;

import org.opencv.core.Mat;

import edu.thu.xface.util.CommonUtil;

/**
 * XFace library
 *
 * @author hujiawei
 */
@SuppressWarnings("ALL")
public class XFaceLibrary {

    private static final String TAG = "XFaceLibrary";

    // ------------------------------------------------------------------------------
    // face recognition part
    // ------------------------------------------------------------------------------

    public long xfacerec = 0;// xface will be inited when app starts
    public long facecas = 0;
    public long eyecas = 0;
    public long eyeglasscas = 0;

    public long trainModel() {
        xfacerec = nativeTrainModel(xfacerec, CommonUtil.FACEDATA_FILEPATH, CommonUtil.TRAINFACEDATA_FILEPATH,
                CommonUtil.FACERECMODEL_FILEPATH, CommonUtil.EIGEN_COMPONENT, CommonUtil.EIGEN_THRESHOLD);
        return xfacerec;
    }

    public int addImage(Mat image, int label, String path, String demopath) {
        return nativeAddImage(xfacerec, facecas, eyecas, eyeglasscas, CommonUtil.FACERECMODEL_FILEPATH,
                image.nativeObj, label, path, demopath);
    }

    public long initFacerec() {
        Log.e(TAG, "init face rec");
        xfacerec = nativeInitFacerec(CommonUtil.FACERECMODEL_FILEPATH, CommonUtil.EIGEN_COMPONENT,
                CommonUtil.EIGEN_THRESHOLD);
        return xfacerec;
    }

    public long initCascade(String path) {
        Log.e(TAG, "init cascade");
        return nativeInitCascade(path);
    }

    public int facedet(Mat rgba, Mat gray) {
        Log.e(TAG, "face detection");
        return nativeFacedet(xfacerec, facecas, eyecas, eyeglasscas, rgba.getNativeObjAddr(),
                gray.getNativeObjAddr());
    }

    public int facerec(Mat mat) {
        return nativeFacerec(xfacerec, facecas, eyecas, eyeglasscas, CommonUtil.FACERECMODEL_FILEPATH,
                mat.getNativeObjAddr());
    }

    public void destoryFacerec() {
        nativeDestoryFacerec(xfacerec, facecas, eyecas, eyeglasscas);
        xfacerec = 0;
    }

    //JNI
    private static native long nativeTrainModel(long xfacrec, String datapath, String orlpath, String modelpath,
                                                int component, double threshold);

    private static native int nativeAddImage(long xfacerec, long facecas, long eyecas, long eyeglasscad,
                                             String modelpath, long addr, int label, String path, String demopath);

    private static native long nativeInitFacerec(String modelpath, int component, double threshold);

    private static native int nativeFacerec(long xfacerec, long facecas, long eyecas, long eyeglasscad,
                                            String modelpath, long addr);///* , int width, int height */

    private static native void nativeDestoryFacerec(long xfacerec, long facecas, long eyecas, long eyeglasscad);

    private static native long nativeInitCascade(String path);

    private static native int nativeFacedet(long xfacerec, long facecas, long eyecas, long eyeglasscad,
                                            long rgba, long gray);

    // private static native long nativeInitEyeCascade(String path);
    //
    // private static native long nativeInitEyeGlassCascade(String path);

    // ------------------------------------------------------------------------------
    // face detection part
    // ------------------------------------------------------------------------------

    // public long xfacedec = 0;
    //
    // public void initFacedetect(String cascadeName, int minFaceSize) {
    // xfacedec = nativeInitFacedetect(cascadeName, minFaceSize);
    // }
    //
    // public void startFacedetect() {
    // nativeStartFacedetect(xfacedec);
    // }
    //
    // public void stopFacedetect() {
    // nativeStopFacedetect(xfacedec);
    // }
    //
    // public void setMinFaceSize(int size) {
    // nativeSetFaceSize(xfacedec, size);
    // }
    //
    // public void facedetect(Mat imageGray, MatOfRect faces) {
    // nativeFacedetect(xfacedec, imageGray.getNativeObjAddr(), faces.getNativeObjAddr());
    // }
    //
    // public void destroryFacedetect() {
    // nativeDestroyFacedetect(xfacedec);
    // xfacedec = 0;
    // }
    //
    // private static native long nativeInitFacedetect(String cascadeName, int minFaceSize);
    //
    // private static native void nativeDestroyFacedetect(long thiz);
    //
    // private static native void nativeStartFacedetect(long thiz);
    //
    // private static native void nativeStopFacedetect(long thiz);
    //
    // private static native void nativeSetFaceSize(long thiz, int size);
    //
    // private static native void nativeFacedetect(long thiz, long inputImage, long faces);

}
