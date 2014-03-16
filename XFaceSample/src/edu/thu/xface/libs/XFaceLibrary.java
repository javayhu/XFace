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

	// face recognition!!
	private long xfacerec = 0;

	public long initFacerec() {
		int face = 1;
		if (CommonUtil.FACERECOGNIZER.equalsIgnoreCase(CommonUtil.FACE_FISHER)) {
			face = 2;
		} else if (CommonUtil.FACERECOGNIZER.equalsIgnoreCase(CommonUtil.FACE_LBPH)) {
			face = 3;
		}
		xfacerec = nativeInitFacerec(CommonUtil.FACEDATA_FILEPATH, CommonUtil.FACERECMODEL_FILEPATH,
				CommonUtil.EIGEN_COMPONENT, CommonUtil.EIGEN_THRESHOLD, face);
		return xfacerec;
	}

	public int facerec(Mat mat) {
		return nativeFacerec(xfacerec, CommonUtil.FACERECMODEL_FILEPATH, mat.getNativeObjAddr(),
				CommonUtil.IMAGE_WIDTH, CommonUtil.IMAGE_HEIGHT);
	}

	public int destoryFacerec() {
		return nativeDestoryFacerec(xfacerec);
	}

	public static native long nativeInitFacerec(String datapath, String modelpath, int component, double threshold,
			int facerec);

	public static native int nativeFacerec(long xfacerec, String modelpath, long addr, int width, int height);

	public static native int nativeDestoryFacerec(long xfacerec);

	// face detecion!!!
	private long mNativeObj = 0;

	public XFaceLibrary(String cascadeName, int minFaceSize) {
		mNativeObj = nativeCreateObject(cascadeName, minFaceSize);
	}

	public void start() {
		nativeStart(mNativeObj);
	}

	public void stop() {
		nativeStop(mNativeObj);
	}

	public void setMinFaceSize(int size) {
		nativeSetFaceSize(mNativeObj, size);
	}

	public void detect(Mat imageGray, MatOfRect faces) {
		nativeDetect(mNativeObj, imageGray.getNativeObjAddr(), faces.getNativeObjAddr());
	}

	public void release() {
		nativeDestroyObject(mNativeObj);
		mNativeObj = 0;
	}

	private static native long nativeCreateObject(String cascadeName, int minFaceSize);

	private static native void nativeDestroyObject(long thiz);

	private static native void nativeStart(long thiz);

	private static native void nativeStop(long thiz);

	private static native void nativeSetFaceSize(long thiz, int size);

	private static native void nativeDetect(long thiz, long inputImage, long faces);

}
