package edu.thu.xface.libs;

import org.opencv.core.Mat;
import org.opencv.core.MatOfRect;

import edu.thu.xface.util.CommonUtil;

/**
 * XFace library
 * 
 * @author hujiawei
 * 
 */
public class XFaceLibrary {

	private static final String TAG = "XFaceLibrary";

	// ------------------------------------------------------------------------------
	// face recognition part
	// ------------------------------------------------------------------------------

	public long xfacerec = 0;//xface will be inited when app starts 

	public long trainModel() {
		xfacerec = nativeTrainModel(xfacerec, CommonUtil.FACEDATA_FILEPATH, CommonUtil.ORLFACEDATA_FILEPATH,
				CommonUtil.FACERECMODEL_FILEPATH, CommonUtil.EIGEN_COMPONENT, CommonUtil.EIGEN_THRESHOLD);
		return xfacerec;
	}

	public void addImage(Mat image, int label) {
		nativeAddImage(xfacerec, CommonUtil.FACERECMODEL_FILEPATH, image.nativeObj, label);
	}

	public long initFacerec() {
		xfacerec = nativeInitFacerec(CommonUtil.FACERECMODEL_FILEPATH, CommonUtil.EIGEN_COMPONENT,
				CommonUtil.EIGEN_THRESHOLD);
		return xfacerec;
	}

	public int facerec(Mat mat) {
		return nativeFacerec(xfacerec, CommonUtil.FACERECMODEL_FILEPATH, mat.getNativeObjAddr(), CommonUtil.IMAGE_WIDTH,
				CommonUtil.IMAGE_HEIGHT);
	}

	public int destoryFacerec() {
		return nativeDestoryFacerec(xfacerec);
	}

	private static native long nativeTrainModel(long xfacrec, String datapath, String orlpath, String modelpath, int component,
			double threshold);

	private static native void nativeAddImage(long xfacerec, String modelpath, long addr, int label);

	private static native long nativeInitFacerec(String modelpath, int component, double threshold);

	private static native int nativeFacerec(long xfacerec, String modelpath, long addr, int width, int height);

	private static native int nativeDestoryFacerec(long xfacerec);

	// ------------------------------------------------------------------------------
	// face detection part
	// ------------------------------------------------------------------------------

	public long xfacedec = 0;

	public void initFacedetect(String cascadeName, int minFaceSize) {
		xfacedec = nativeInitFacedetect(cascadeName, minFaceSize);
	}

	public void startFacedetect() {
		nativeStartFacedetect(xfacedec);
	}

	public void stopFacedetect() {
		nativeStopFacedetect(xfacedec);
	}

	public void setMinFaceSize(int size) {
		nativeSetFaceSize(xfacedec, size);
	}

	public void facedetect(Mat imageGray, MatOfRect faces) {
		nativeFacedetect(xfacedec, imageGray.getNativeObjAddr(), faces.getNativeObjAddr());
	}

	public void destroryFacedetect() {
		nativeDestroyFacedetect(xfacedec);
		xfacedec = 0;
	}

	private static native long nativeInitFacedetect(String cascadeName, int minFaceSize);

	private static native void nativeDestroyFacedetect(long thiz);

	private static native void nativeStartFacedetect(long thiz);

	private static native void nativeStopFacedetect(long thiz);

	private static native void nativeSetFaceSize(long thiz, int size);

	private static native void nativeFacedetect(long thiz, long inputImage, long faces);

}
