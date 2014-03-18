package edu.thu.xface.util;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;
import java.util.List;
import java.util.Properties;
import java.util.Set;
import java.util.regex.Pattern;

import android.content.Context;
import android.content.res.AssetManager;
import android.os.Environment;
import android.util.Log;
import edu.thu.xface.R;
import edu.thu.xface.adapter.UserInfo;
import edu.thu.xface.adapter.UserpicInfo;

/**
 * 公共参数工具类
 * 
 * @author hujiawei
 * 
 */
public class CommonUtil {

	private static final String TAG = "CommonUtil";

	public static boolean isAppInit = false;

	public static final String CONFIG_FILENAME = "config.properties";// app config
	public static final String USERS_FILENAME = "users.properties";// user infos
	public static final String FACEDATA_FILENAME = "facedata.txt";// face data file
	public static final String ORLFACEDATA_FILENAME = "orlfaces.txt";// face data file
	public static final String FACERECMODEL_FILENAME = "xfacerecognition.yml";// facerec model file
	public static final String LBPCASCADE_FILENAME = "lbpcascade_frontalface.yml";// lbp cascade face file

	public static final String FACE_ALGORITHM = "facerecognizer";
	public static final String FACE_EIGEN = "eigenface";
	public static final String FACE_FISHER = "fisherface";
	public static final String FACE_LBPH = "lbphface";

	public static final String ORL_FACES = "orl_faces";

	public static String FACEDATA_FILEPATH;
	public static String ORLFACEDATA_FILEPATH;
	public static String FACERECMODEL_FILEPATH;
	public static String LBPCASCADE_FILEPATH;

	public static Properties configProps = new Properties();
	public static Properties userProps = new Properties();

	public static File SDFOLDER;// sd root folder for app
	public static File USERFOLDER;// save user data
	public static File CAMERAFOLDER;// save camera take pics
	public static File DEMOFOLDER;// save demo data --> orl_faces!

	public static int IMAGE_WIDTH = 92;// image size for recognizing
	public static int IMAGE_HEIGHT = 112;

	public static int EIGEN_COMPONENT = 10;
	public static double EIGEN_THRESHOLD = 0.0;

	public static String FACERECOGNIZER = FACE_EIGEN;// default is eigenface

	public static void initApp(Context context) {
		Log.i(TAG, "common util init app enter!");
		// load library and init app
		System.loadLibrary("opencv_java");// libs contain many opencv related libs!
		System.loadLibrary("xface");
		Log.i(TAG, "opencv and xface library load!");
		try {
			// app config properties
			configProps.load(Thread.currentThread().getContextClassLoader().getResourceAsStream(CONFIG_FILENAME));
			if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)) {
				SDFOLDER = new File(Environment.getExternalStorageDirectory().getCanonicalPath()
						+ getConfigValue("sd_folder"));
				if (!SDFOLDER.exists()) {
					SDFOLDER.mkdir();
				}
				USERFOLDER = new File(SDFOLDER.getAbsoluteFile() + getConfigValue("user_folder"));
				if (!USERFOLDER.exists()) {
					USERFOLDER.mkdir();
				}
				CAMERAFOLDER = new File(SDFOLDER.getAbsoluteFile() + getConfigValue("camera_folder"));
				if (!CAMERAFOLDER.exists()) {
					CAMERAFOLDER.mkdir();
				}
				DEMOFOLDER = new File(SDFOLDER.getAbsoluteFile() + getConfigValue("demo_folder"));
				if (!DEMOFOLDER.exists()) {
					DEMOFOLDER.mkdir();//
					// orl_faces! --> copy orl_faces to sdcard
					Log.i(TAG, "copy assets: from orl_faces to demo");
					copyAssets(context, ORL_FACES, DEMOFOLDER.getAbsolutePath() + File.separator);//
					copyAssets(context, "data", SDFOLDER.getAbsolutePath() + File.separator);
				}
				IMAGE_WIDTH = Integer.parseInt(getConfigValue("image_width"));
				IMAGE_HEIGHT = Integer.parseInt(getConfigValue("image_height"));
				EIGEN_COMPONENT = Integer.parseInt(getConfigValue("eigen_component"));
				EIGEN_THRESHOLD = Double.parseDouble(getConfigValue("eigen_threshold"));

				// user properties
				File userFile = new File(SDFOLDER.getAbsoluteFile() + File.separator + USERS_FILENAME);
				if (!userFile.exists()) {
					userFile.createNewFile();
				} else {
					userProps.load(new FileInputStream(userFile));
				}
				FACERECOGNIZER = userProps.getProperty(FACE_ALGORITHM);// load FACE_ALGORITHM
				if (FACERECOGNIZER == null) {
					userProps.setProperty(FACE_ALGORITHM, FACE_EIGEN);
					saveUserProperties(userProps);
					FACERECOGNIZER = FACE_EIGEN;
				}
				File facedataFile = new File(SDFOLDER.getAbsoluteFile() + File.separator + FACEDATA_FILENAME);
				if (!facedataFile.exists()) {
					facedataFile.createNewFile();
				}
				FACEDATA_FILEPATH = facedataFile.getAbsolutePath();// data
				ORLFACEDATA_FILEPATH = SDFOLDER.getAbsolutePath() + File.separator + ORLFACEDATA_FILENAME;// orl face
																											// data txt
				FACERECMODEL_FILEPATH = SDFOLDER.getAbsolutePath() + File.separator + FACERECMODEL_FILENAME;// model
				LBPCASCADE_FILEPATH = SDFOLDER.getAbsolutePath() + File.separator + LBPCASCADE_FILENAME;// cascade

			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		isAppInit = true;
		Log.i(TAG, "common util init app exit!");
	}

	// copy assets files to sdcard
	private static void copyAssets(Context context, String assetdir, String sddir) {
		AssetManager assetManager = context.getResources().getAssets();
		String[] files;
		try {
			files = assetManager.list(assetdir);
		} catch (IOException e) {
			e.printStackTrace();
			return;//
		}
		File mWorkingPath = new File(sddir);
		// if this directory does not exists, make one.
		if (!mWorkingPath.exists()) {
			mWorkingPath.mkdirs();
		}
		for (int i = 0; i < files.length; i++) {
			try {
				String fileName = files[i];
				// we make sure file name not contains '.' to be a folder.
				if (!fileName.contains(".")) {
					if (0 == assetdir.length()) {
						copyAssets(context, fileName, sddir + fileName + File.separator);
					} else {
						copyAssets(context, assetdir + File.separator + fileName, sddir + fileName + File.separator);
					}
					continue;
				}
				File outFile = new File(mWorkingPath, fileName);
				if (outFile.exists())
					outFile.delete();
				InputStream in = null;
				if (0 != assetdir.length())
					in = assetManager.open(assetdir + File.separator + fileName);
				else
					in = assetManager.open(fileName);
				OutputStream out = new FileOutputStream(outFile);
				// Transfer bytes from in to out
				byte[] buf = new byte[1024];
				int len;
				while ((len = in.read(buf)) > 0) {
					out.write(buf, 0, len);
				}
				in.close();
				out.close();
			} catch (FileNotFoundException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	// get all users, if flag is true, get his pictures count
	public static ArrayList<UserInfo> getAllUsers(boolean flag) {
		Pattern pattern = Pattern.compile("^[-\\+]?[\\d]*$");// number!
		ArrayList<UserInfo> userInfos = new ArrayList<UserInfo>();
		Set<String> keys = CommonUtil.userProps.stringPropertyNames();
		int count = 0, userid;
		for (Iterator iterator = keys.iterator(); iterator.hasNext();) {
			String key = (String) iterator.next();
			if (pattern.matcher(key).matches()) {// key is number, then it's a name
				userid = Integer.parseInt(key);
				if (flag) {
					count = getPicturesCountByUserid(userid);
				}
				userInfos.add(new UserInfo(count, userid, userProps.getProperty(key)));
			}
		}
		Collections.sort(userInfos, new Comparator<UserInfo>() {// sort users according to his userid
					@Override
					public int compare(UserInfo lhs, UserInfo rhs) {
						if (lhs.getUserid() < rhs.getUserid()) {
							return -1;
						} else {
							return 1;
						}
					}
				});
		return userInfos;
	}

	// according to the userid, try to get user pictures count
	private static int getPicturesCountByUserid(int userid) {
		int count = 0;
		File folder = new File(USERFOLDER.getAbsolutePath() + File.separator + String.valueOf(userid));
		if (folder.isDirectory()) {// must be a directory
			count = folder.listFiles().length;// must be file! attention to hidden files!
		}
		return count;
	}

	// according to the userid, try to get user pictures count
	public static List<UserpicInfo> getAllUserpics(int userid) {
		List<UserpicInfo> userpicInfos = new ArrayList<UserpicInfo>();
		File folder = new File(USERFOLDER.getAbsolutePath() + File.separator + String.valueOf(userid));
		if (folder.isDirectory()) {// must be a directory
			String[] files = folder.list();// filenames!
			for (int i = 0; i < files.length; i++) {
				userpicInfos.add(new UserpicInfo(userid, files[i]));//
			}
		}
		return userpicInfos;
	}

	// user pic full path
	public static String getUserpicFullpath(int userid, String filename) {
		return CommonUtil.USERFOLDER.getAbsolutePath() + File.separator + String.valueOf(userid) + File.separator
				+ filename;
	}

	// save properties !
	public static void saveUserProperties(Properties properties) throws Exception {
		OutputStream os = new FileOutputStream(SDFOLDER.getAbsoluteFile() + File.separator + USERS_FILENAME);
		properties.store(os, "user data updated:" + System.currentTimeMillis());
	}

	// get config value for the specified key
	public static String getConfigValue(String key) {
		return configProps.getProperty(key);
	}

}
