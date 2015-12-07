package edu.thu.xface;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import edu.thu.xface.util.CommonUtil;

public class DemoActivity extends Activity {

	private TextView tv_demo_result;
	private Button btn_demo_test;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_demo);

		tv_demo_result = (TextView) findViewById(R.id.tv_demo_result);
		btn_demo_test = (Button) findViewById(R.id.btn_signup_gotocamera);
	}

	// FaceRecognizer model =
	// Contrib
	// C++: Ptr_FaceRecognizer createEigenFaceRecognizer(int num_components = 0, double threshold = DBL_MAX)
	// Return type 'Ptr_FaceRecognizer' is not supported, skipping the function
	// do the demo test
	public void btn_demo_test(View view) {

		// int[] rawids = new int[] { R.raw.a, R.raw.g, R.raw.d, R.raw.e, R.raw.h };
		// int count = rawids.length;
		// String[] filepaths = new String[count];
		// String[] labels = new String[] { "0", "3", "1", "2", "3" };
		// for (int i = 0; i < rawids.length; i++) {
		// filepaths[i] = saveImageFile(rawids[i], "s" + i + ".jpg");
		// }
		// StringBuffer buffer = new StringBuffer();
		// // ArrayList<Mat> images = new ArrayList<Mat>();
		// for (int i = 0; i < count; i++) {
		// // Mat image = Highgui.imread(filepaths[i]);
		// // images.add(image);
		// buffer.append(filepaths[i] + ";" + labels[i] + "\n");
		// }
		// String datapath = saveFile(buffer, "facedata.txt");
		//
		// DemoLibrary.demotest(datapath, CommonUtil.DEMOFOLDER.getAbsolutePath(), 3);
		//
		// tv_demo_result.setText("Face recognition OK!");
	}

	// save file
	public String saveFile(StringBuffer buffer, String filename) {
		String path = CommonUtil.DEMOFOLDER.getAbsolutePath() + File.separator + filename;
		File file = new File(path);
		try {
			BufferedWriter writer = new BufferedWriter(new FileWriter(file));
			writer.write(buffer.toString());
			writer.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return path;
	}

	// save image
	public String saveImageFile(int id, String filename) {
		InputStream is = getResources().openRawResource(id);
		String path = CommonUtil.DEMOFOLDER.getAbsolutePath() + File.separator + filename;
		File file = new File(path);
		try {
			FileOutputStream fos = new FileOutputStream(file);
			byte[] bytes = new byte[1024];
			int count = 0;
			while ((count = is.read(bytes)) != -1) {
				fos.write(bytes, 0, count);
			}
			is.close();
			fos.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return path;
	}

}
