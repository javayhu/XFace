package mface.thu.edu.cn.mface.view;

import android.content.ContentValues;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.ColorMatrix;
import android.graphics.ColorMatrixColorFilter;
import android.graphics.Paint;
import android.hardware.Camera;
import android.hardware.Camera.PictureCallback;
import android.hardware.Camera.Size;
import android.provider.MediaStore;
import android.util.AttributeSet;
import android.util.Log;

import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.JavaCameraView;

import java.io.File;
import java.io.FileOutputStream;
import java.util.List;

/**
 * Camera View
 */
public class MFaceCameraView extends JavaCameraView implements PictureCallback {

    private static final String TAG = MFaceCameraView.class.getSimpleName();

    private String mPictureFile;//used to save picture file

    public MFaceCameraView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public List<String> getEffectList() {
        return mCamera.getParameters().getSupportedColorEffects();
    }

    public boolean isEffectSupported() {
        return (mCamera.getParameters().getColorEffect() != null);
    }

    public String getEffect() {
        return mCamera.getParameters().getColorEffect();
    }

    public void setEffect(String effect) {
        Camera.Parameters params = mCamera.getParameters();
        params.setColorEffect(effect);
        mCamera.setParameters(params);
    }

    public List<Size> getResolutionList() {
        return mCamera.getParameters().getSupportedPreviewSizes();
    }

    public void setResolution(Size resolution) {
        disconnectCamera();
        mMaxHeight = resolution.height;
        mMaxWidth = resolution.width;
        connectCamera(getWidth(), getHeight());
    }

    public Size getResolution() {
        return mCamera.getParameters().getPreviewSize();
    }


    public void setCameraFront() {
        disconnectCamera();
        setCameraIndex(CameraBridgeViewBase.CAMERA_ID_FRONT);
        connectCamera(getWidth(), getHeight());
    }

    public void setCameraBack() {
        disconnectCamera();
        setCameraIndex(CameraBridgeViewBase.CAMERA_ID_BACK);
        connectCamera(getWidth(), getHeight());
    }

    public int numberCameras() {
        return Camera.getNumberOfCameras();
    }

    //////////////////////////  methods below do not use any more  ////////////////////////////////

    //outside method calls this to save a picture
    public void takePicture(final String file) {
        Log.e(TAG, "Taking picture");
        this.mPictureFile = file;
        // Postview and jpeg are sent in the same buffers if the queue is not empty when performing a capture.
        // Clear up buffers to avoid mCamera.takePicture to be stuck because of a memory issue
        mCamera.setPreviewCallback(null);
        // PictureCallback is implemented by the current class
        mCamera.takePicture(null, null, this);
    }

    @Override
    public void onPictureTaken(byte[] data, Camera camera) {//the data here is "JPEG" data
        Log.e(TAG, "Saving a bitmap to file");
        mCamera.startPreview();// The camera preview was automatically stopped. Start it again.
        mCamera.setPreviewCallback(this);
        Bitmap bmpOriginal = BitmapFactory.decodeByteArray(data, 0, data.length);
        Bitmap bmpGrayscale = convertToGrayscale(bmpOriginal);//
        try {
            File file = new File(mPictureFile);
            if (!file.exists()) file.createNewFile();
            FileOutputStream fos = new FileOutputStream(file);
            //is other format ok? ok!!! PNG is much clear than JPEG, but image file is also much larger
            bmpGrayscale.compress(Bitmap.CompressFormat.PNG, 0, fos);
            //fos.write(data);// Write the image in a file (in jpeg format)
            fos.flush();
            fos.close();
        } catch (java.io.IOException e) {
            Log.e(TAG, "Exception in taking picture", e);
        }
//        saveImageToGallery();
    }

    // convert original bitmap to gray scale
    private Bitmap convertToGrayscale(Bitmap bmpOriginal) {
        int height = bmpOriginal.getHeight();
        int width = bmpOriginal.getWidth();
        Log.e(TAG, "width=" + width + " height=" + height);//default size is 640*480 px
        Bitmap bmpGrayscale = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);//
        Canvas c = new Canvas(bmpGrayscale);
        Paint paint = new Paint();
        ColorMatrix cm = new ColorMatrix();
        cm.setSaturation(0);
        ColorMatrixColorFilter f = new ColorMatrixColorFilter(cm);
        paint.setColorFilter(f);
        c.drawBitmap(bmpOriginal, 0, 0, paint);
        return bmpGrayscale;
    }

    // Add the JPEG file to the Android Gallery.
    private void saveImageToGallery() {
        ContentValues image = new ContentValues();
        image.put(MediaStore.Images.Media.TITLE, mPictureFile);
        image.put(MediaStore.Images.Media.DISPLAY_NAME, mPictureFile);
        image.put(MediaStore.Images.Media.DESCRIPTION, "Processed by the MFace App");
        image.put(MediaStore.Images.Media.DATE_TAKEN, System.nanoTime()); //
        image.put(MediaStore.Images.Media.MIME_TYPE, "image/jpeg");//png!!!
        image.put(MediaStore.Images.Media.ORIENTATION, 0);
        image.put(MediaStore.Images.Media.DATA, mPictureFile);
        getContext().getContentResolver().insert(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, image);
    }
}


