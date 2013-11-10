package edu.thu.xface.libs;

import org.opencv.core.Mat;

import edu.thu.xface.util.CommonUtil;

/**
 * XFace library
 * 
 * @author hujiawei
 * 
 */
public class XFaceLibrary {

	private static long xfacerec = 0;

	public static long initFacerec() {
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

	public static int facerec(Mat mat) {
		return nativeFacerec(xfacerec, CommonUtil.FACERECMODEL_FILEPATH, mat.getNativeObjAddr(),
				CommonUtil.IMAGE_WIDTH, CommonUtil.IMAGE_HEIGHT);
	}

	public static int destoryFacerec() {
		return nativeDestoryFacerec(xfacerec);
	}

	public static native long nativeInitFacerec(String datapath, String modelpath, int component, double threshold,
			int facerec);

	public static native int nativeFacerec(long xfacerec, String modelpath, long addr, int width, int height);

	public static native int nativeDestoryFacerec(long xfacerec);

}
