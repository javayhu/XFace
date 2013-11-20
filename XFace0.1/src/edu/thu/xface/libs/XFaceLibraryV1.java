package edu.thu.xface.libs;

public class XFaceLibraryV1 {

	private static long facereclong = 0l;

	public static long initFacerec(String filepath) {
		facereclong = nativeInitFacerec(filepath);
		return facereclong;// when error happens, return will be negative
	}

	public static int facerec(int addr) {
		return nativeFacerec(facereclong, addr);
	}

	public static int destoryFacerec() {
		return nativeDestoryFacerec(facereclong);
	}

	public static native long nativeInitFacerec(String filepath);

	public static native int nativeFacerec(long thiz, int addr);// gray and rgba has different address,use native object
																// addr

	public static native int nativeDestoryFacerec(long thiz);

	public static native void testLong(long l);// when passing long value here, native will be lost!

}
