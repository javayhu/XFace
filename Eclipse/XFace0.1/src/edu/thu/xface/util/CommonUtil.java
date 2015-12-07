package edu.thu.xface.util;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.OutputStream;
import java.util.Properties;

import android.os.Environment;

/**
 * 公共参数工具类
 * 
 * @author hujiawei
 * 
 */
public class CommonUtil {

	public static final String CONFIG_FILENAME = "config.properties";// app config
	public static final String USERS_FILENAME = "users.properties";// user infos
	public static final String FACEDATA_FILENAME = "facedata.txt";// face data file
	public static final String FACERECMODEL_FILENAME = "facerec.yml";// facerec model file
	
	public static final String FACE_ALGORITHM="facerecognizer";
	public static final String FACE_EIGEN="eigenface";
	public static final String FACE_FISHER="fisherface";
	public static final String FACE_LBPH="lbphface";
	

	public static String FACEDATA_FILEPATH;
	public static String FACERECMODEL_FILEPATH;

	public static Properties props = new Properties();
	public static Properties userProps = new Properties();

	public static File SDFOLDER;// sd root folder for app
	public static File USERFOLDER;// save user data
	public static File CAMERAFOLDER;// save camera take pics
	public static File DEMOFOLDER;// save demo data

	public static int IMAGE_WIDTH = 240;// image size for recognizing
	public static int IMAGE_HEIGHT = 360;
	
	public static int EIGEN_COMPONENT = 10;
	public static double EIGEN_THRESHOLD = 0.0;
	
	public static String FACERECOGNIZER = FACE_EIGEN;//eigenface fisherface lbphface

	// static code block
	static {
		try {
			props.load(Thread.currentThread().getContextClassLoader().getResourceAsStream(CONFIG_FILENAME));
			if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)) {
				SDFOLDER = new File(Environment.getExternalStorageDirectory().getCanonicalPath()
						+ getValue("sd_folder"));
				if (!SDFOLDER.exists()) {
					SDFOLDER.mkdir();
				}
				USERFOLDER = new File(SDFOLDER.getAbsoluteFile() + getValue("user_folder"));
				if (!USERFOLDER.exists()) {
					USERFOLDER.mkdir();
				}
				CAMERAFOLDER = new File(SDFOLDER.getAbsoluteFile() + getValue("camera_folder"));
				if (!CAMERAFOLDER.exists()) {
					CAMERAFOLDER.mkdir();
				}
				DEMOFOLDER = new File(SDFOLDER.getAbsoluteFile() + getValue("demo_folder"));
				if (!DEMOFOLDER.exists()) {
					DEMOFOLDER.mkdir();
				}
				IMAGE_WIDTH = Integer.parseInt(getValue("image_width"));
				IMAGE_HEIGHT = Integer.parseInt(getValue("image_height"));
				EIGEN_COMPONENT = Integer.parseInt(getValue("eigen_component"));
				EIGEN_THRESHOLD = Double.parseDouble(getValue("eigen_threshold"));
				//
				File userFile = new File(SDFOLDER.getAbsoluteFile() + File.separator + USERS_FILENAME);
				if (!userFile.exists()) {
					userFile.createNewFile();
				} else {
					userProps.load(new FileInputStream(userFile));
				}
				FACERECOGNIZER = userProps.getProperty(FACE_ALGORITHM);//load facerec
				if (FACERECOGNIZER == null) {
					userProps.setProperty(FACE_ALGORITHM, FACE_EIGEN);
					saveUserProperties(userProps);
				}
				File facedataFile = new File(SDFOLDER.getAbsoluteFile() + File.separator + FACEDATA_FILENAME);
				if (!facedataFile.exists()) {
					facedataFile.createNewFile();
				}
				FACEDATA_FILEPATH = facedataFile.getAbsolutePath();
				FACERECMODEL_FILEPATH = SDFOLDER.getAbsolutePath() + File.separator + FACERECMODEL_FILENAME;
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	// save properties !
	public static void saveUserProperties(Properties properties) throws Exception {
		OutputStream os = new FileOutputStream(SDFOLDER.getAbsoluteFile() + File.separator + USERS_FILENAME);
		properties.store(os, "config updated:" + System.currentTimeMillis());
	}

	// get value for the specified key
	public static String getValue(String key) {
		return props.getProperty(key);
	}

}
