package edu.thu.xface;

import android.app.Activity;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import org.opencv.core.Mat;
import org.opencv.highgui.Highgui;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

import edu.thu.xface.libs.XFaceLibrary;
import edu.thu.xface.util.CommonUtil;

/**
 * test
 */
public class TestActivity extends Activity {

    private static final String TAG = TestActivity.class.getName();

    private TextView textView;
    private Button button;

    private XFaceLibrary xface;

    private long startTime;
    private long endTime;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_test);

        textView = (TextView) findViewById(R.id.tv_test_info);
        button = (Button) findViewById(R.id.btn_test);

        xface = CommonUtil.xFaceLibrary;
    }

    public void btn_test(View view) {
        testUserImages();
    }

    private void testUserImages(){
        new AsyncTask<Void, Void, Void>() {

            @Override
            protected void onPreExecute() {
                long result = xface.xfacerec;
                if (result == 0) {
                    xface.initFacerec();// it will take a lot of time!
                }
                button.setEnabled(false);
                super.onPreExecute();
            }

            @Override
            protected Void doInBackground(Void... params) {
                startTime = System.currentTimeMillis();
                try {
                    BufferedReader reader = new BufferedReader(new FileReader(CommonUtil.SDFOLDER.getAbsoluteFile() + File.separator + "lfw.txt"));
                    BufferedWriter writer = new BufferedWriter(new FileWriter(CommonUtil.SDFOLDER.getAbsoluteFile() + File.separator + "lfw-out.txt"));
                    String line, name, folder = CommonUtil.DEMOFOLDER.getAbsolutePath(), file;
                    int count, k;
                    Scanner scanner;
                    while ((line = reader.readLine()) != null) {
                        k = 0;
                        scanner = new Scanner(line);
                        name = scanner.next();
                        count = scanner.nextInt();
                        for (int i = 0; i < count; i++) {
                            file = folder + File.separator + name + File.separator + name + String.format("%04d", i) + ".jpg";
                            Mat mRgb = Highgui.imread(file);
                            Mat mGray = Highgui.imread(file, Highgui.CV_LOAD_IMAGE_GRAYSCALE);
                            if (xface.facedet(mRgb, mGray) == 1) k++;
                        }
                        Log.e(TAG, "name=" + name + "  count=" + k);
                        writer.append(name + " " + count + " " + k);
                        writer.newLine();
                    }
                    reader.close();
                    writer.flush();
                    writer.close();
                } catch (FileNotFoundException e) {
                    e.printStackTrace();
                } catch (IOException e) {
                    e.printStackTrace();
                }
                endTime = System.currentTimeMillis();
                Log.e(TAG, "time=" + (endTime - startTime) / 1000);
                return null;
            }

            @Override
            protected void onPostExecute(Void aVoid) {
                button.setEnabled(true);
                super.onPostExecute(aVoid);
            }
        }.execute();
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_test, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
