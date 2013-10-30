package org.opencv.samples.fd;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

import org.opencv.android.Utils;
import org.opencv.core.Core;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.MatOfRect;
import org.opencv.core.Point;
import org.opencv.core.Rect;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.highgui.Highgui;
import org.opencv.highgui.VideoCapture;
import org.opencv.imgproc.Imgproc;
import org.opencv.objdetect.CascadeClassifier;
import org.opencv.objdetect.Objdetect;





import android.content.Context;
import android.graphics.Bitmap;
import android.util.Log;
import android.view.SurfaceHolder;

class FdView extends SampleCvViewBase {
    private static final String   TAG = "Sample::FdView";
    private Mat                   mRgba;
    private Mat                   mGray;
    private Mat 				mZoomCorner;
    private Mat                 mZoomWindow;
    private Mat                 mZoomWindow2;
    private Mat                 mResult;
    private Mat 				teplateR;
    private Mat 				teplateL;
    private File                mCascadeFile;
    private CascadeClassifier   mJavaDetector;
    private CascadeClassifier   mCascadeER;
    private CascadeClassifier   mCascadeEL;
    private DetectionBasedTracker mNativeDetector;
    

    private static final Scalar   FACE_RECT_COLOR = new Scalar(0, 255, 0, 255);
    
    public static final int       JAVA_DETECTOR     = 0;
    public static final int       NATIVE_DETECTOR   = 1;
    
    private static final int TM_SQDIFF 			= 0;
    private static final int TM_SQDIFF_NORMED 	= 1;
    private static final int TM_CCOEFF			= 2;
    private static final int TM_CCOEFF_NORMED 	= 3;
    private static final int TM_CCORR 			= 4;
    private static final int TM_CCORR_NORMED 	= 5;
    
    private int                   mDetectorType     = JAVA_DETECTOR;

    private float                 mRelativeFaceSize = 0;
    private int					  mAbsoluteFaceSize = 0;
    private int 				  learn_frames = 0;
    private double 				  match_value;
    private Rect 			      eyearea = new Rect();
    
    public void setMinFaceSize(float faceSize)
    {
		mRelativeFaceSize = faceSize;
		mAbsoluteFaceSize = 0;
    }
    
    public void setDetectorType(int type)
    {
    	if (mDetectorType != type)
    	{
    		mDetectorType = type;
    		
    		if (type == NATIVE_DETECTOR)
    		{
    			Log.i(TAG, "Detection Based Tracker enabled");
    			mNativeDetector.start();
    		}
    		else
    		{
    			Log.i(TAG, "Cascade detector enabled");
    			mNativeDetector.stop();
    		}
    	}
    }
    
    public void resetLearFramesCount()
    {
    	learn_frames = 0;
    }

    public FdView(Context context) {
        super(context);

        try {
            InputStream is = context.getResources().openRawResource(R.raw.lbpcascade_frontalface);
            File cascadeDir = context.getDir("cascade", Context.MODE_PRIVATE);
            mCascadeFile = new File(cascadeDir, "lbpcascade_frontalface.xml");
            FileOutputStream os = new FileOutputStream(mCascadeFile);

            byte[] buffer = new byte[4096];
            int bytesRead;
            while ((bytesRead = is.read(buffer)) != -1) {
                os.write(buffer, 0, bytesRead);
            }
            is.close();
            os.close();
            
            
            // --------------------------------- load left eye classificator -----------------------------------
            InputStream iser = context.getResources().openRawResource(R.raw.haarcascade_lefteye_2splits);
            File cascadeDirER = context.getDir("cascadeER", Context.MODE_PRIVATE);
            File cascadeFileER = new File(cascadeDirER, "haarcascade_eye_right.xml");
            FileOutputStream oser = new FileOutputStream(cascadeFileER);

            byte[] bufferER = new byte[4096];
            int bytesReadER;
            while ((bytesReadER = iser.read(bufferER)) != -1) {
                oser.write(bufferER, 0, bytesReadER);
            }
            iser.close();
            oser.close();
            //----------------------------------------------------------------------------------------------------
            
            
            // --------------------------------- load right eye classificator ------------------------------------
            InputStream isel = context.getResources().openRawResource(R.raw.haarcascade_lefteye_2splits);
            File cascadeDirEL = context.getDir("cascadeEL", Context.MODE_PRIVATE);
            File cascadeFileEL = new File(cascadeDirEL, "haarcascade_eye_left.xml");
            FileOutputStream osel = new FileOutputStream(cascadeFileEL);

            byte[] bufferEL = new byte[4096];
            int bytesReadEL;
            while ((bytesReadEL = isel.read(bufferEL)) != -1) {
                osel.write(bufferEL, 0, bytesReadEL);
            }
            isel.close();
            osel.close();
            
            // ------------------------------------------------------------------------------------------------------
          
            
            mJavaDetector = new CascadeClassifier(mCascadeFile.getAbsolutePath());
            mCascadeER = new CascadeClassifier(cascadeFileER.getAbsolutePath());
            mCascadeEL = new CascadeClassifier(cascadeFileER.getAbsolutePath());
            if (mJavaDetector.empty()|| mCascadeER.empty() || mCascadeEL.empty()) {
                Log.e(TAG, "Failed to load cascade classifier");
                mJavaDetector = null;
                mCascadeER=null;
                mCascadeEL=null;
            } else
                Log.i(TAG, "Loaded cascade classifier from " + mCascadeFile.getAbsolutePath());

            mNativeDetector = new DetectionBasedTracker(mCascadeFile.getAbsolutePath(), 0);
            
            cascadeDir.delete();
            cascadeFileER.delete();
            cascadeDirER.delete();
            cascadeFileEL.delete();
            cascadeDirEL.delete();

        } catch (IOException e) {
            e.printStackTrace();
            Log.e(TAG, "Failed to load cascade. Exception thrown: " + e);
        }
    }

    @Override
	public void surfaceCreated(SurfaceHolder holder) {
        synchronized (this) {
            // initialize Mats before usage
            mGray = new Mat();
            mRgba = new Mat();
        }

        super.surfaceCreated(holder);
	}

	@Override
    protected Bitmap processFrame(VideoCapture capture) {
        capture.retrieve(mRgba, Highgui.CV_CAP_ANDROID_COLOR_FRAME_RGBA);
        capture.retrieve(mGray, Highgui.CV_CAP_ANDROID_GREY_FRAME);

        if (mAbsoluteFaceSize == 0)
        {
        	int height = mGray.rows();
        	if (Math.round(height * mRelativeFaceSize) > 0);
        	{
        		mAbsoluteFaceSize = Math.round(height * mRelativeFaceSize);
        	}
        	mNativeDetector.setMinFaceSize(mAbsoluteFaceSize);
        }
        
        MatOfRect faces = new MatOfRect();
        
        if (mDetectorType == JAVA_DETECTOR)
        {
        	if (mJavaDetector != null)
                mJavaDetector.detectMultiScale(mGray, faces, 1.1, 2, 2 // TODO: objdetect.CV_HAAR_SCALE_IMAGE
                        , new Size(mAbsoluteFaceSize, mAbsoluteFaceSize), new Size());
        	
        	
        	 if (mZoomCorner == null || mZoomWindow == null)
        	        CreateAuxiliaryMats();
        	 
        	 
        	 Rect[] facesArray = faces.toArray();
         	
             for (int i = 0; i < facesArray.length; i++){
             	Rect r = facesArray[i];
                 Core.rectangle(mGray, r.tl(), r.br(), new Scalar(0, 255, 0, 255), 3);
                 Core.rectangle(mRgba, r.tl(), r.br(), new Scalar(0, 255, 0, 255), 3);
                 
                 eyearea = new Rect(r.x +r.width/8,(int)(r.y + (r.height/4.5)),r.width - 2*r.width/8,(int)( r.height/3.0));
                 Core.rectangle(mRgba,eyearea.tl(),eyearea.br() , new Scalar(255,0, 0, 255), 2); 
                 Rect eyearea_right = new Rect(r.x +r.width/16,(int)(r.y + (r.height/4.5)),(r.width - 2*r.width/16)/2,(int)( r.height/3.0));
                 Rect eyearea_left = new Rect(r.x +r.width/16 +(r.width - 2*r.width/16)/2,(int)(r.y + (r.height/4.5)),(r.width - 2*r.width/16)/2,(int)( r.height/3.0));
                 Core.rectangle(mRgba,eyearea_left.tl(),eyearea_left.br() , new Scalar(255,0, 0, 255), 2); 
                 Core.rectangle(mRgba,eyearea_right.tl(),eyearea_right.br() , new Scalar(255, 0, 0, 255), 2);
                 
                 if(learn_frames<5){
                 	teplateR = get_template(mCascadeER,eyearea_right,24);
                 	teplateL = get_template(mCascadeEL,eyearea_left,24);
                 	learn_frames++;
                 }else{
                 	
                	 
                 match_value = match_eye(eyearea_right,teplateR,FdActivity.method); 
                 	
                 match_value = match_eye(eyearea_left,teplateL,FdActivity.method); 
                 	;
                 	}
                    Imgproc.resize(mRgba.submat(eyearea_left), mZoomWindow2, mZoomWindow2.size());
                  	Imgproc.resize(mRgba.submat(eyearea_right), mZoomWindow, mZoomWindow.size());
                 
                 	}
        }
        else if (mDetectorType == NATIVE_DETECTOR)
        {
        	if (mNativeDetector != null)
        		mNativeDetector.detect(mGray, faces);
        }
        else
        {
        	Log.e(TAG, "Detection method is not selected!");
        }
        
        Rect[] facesArray = faces.toArray();
        for (int i = 0; i < facesArray.length; i++)
            Core.rectangle(mRgba, facesArray[i].tl(), facesArray[i].br(), FACE_RECT_COLOR, 3);

        Bitmap bmp = Bitmap.createBitmap(mRgba.cols(), mRgba.rows(), Bitmap.Config.ARGB_8888);

        try {
        	Utils.matToBitmap(mRgba, bmp);
        } catch(Exception e) {
        	Log.e(TAG, "Utils.matToBitmap() throws an exception: " + e.getMessage());
            bmp.recycle();
            bmp = null;
        }
        
        return bmp;
    }
	
	private void CreateAuxiliaryMats() {
	    if (mGray.empty())
	        return;

	    int rows = mGray.rows();
	    int cols = mGray.cols();
	    
	    if (mZoomWindow == null){
	        mZoomWindow = mRgba.submat(rows / 2 + rows / 10 ,rows , cols / 2 + cols / 10, cols );
	        mZoomWindow2 = mRgba.submat(0, rows / 2 - rows / 10 , cols / 2 + cols / 10, cols );
	    }
	    
	}
	
	
	private double  match_eye(Rect area, Mat mTemplate,int type){
		  Point matchLoc; 
		  Mat mROI = mGray.submat(area);
	      int result_cols =  mGray.cols() - mTemplate.cols() + 1;
		  int result_rows = mGray.rows() - mTemplate.rows() + 1;
		  if(mTemplate.cols()==0 ||mTemplate.rows()==0){
			  return 0.0;
		  }
		  mResult = new Mat(result_cols,result_rows, CvType.CV_32FC1);
		  
		  switch (type){
			  case TM_SQDIFF:
				  Imgproc.matchTemplate(mROI, mTemplate, mResult, Imgproc.TM_SQDIFF) ; 
				  break;
			  case TM_SQDIFF_NORMED:
				  Imgproc.matchTemplate(mROI, mTemplate, mResult, Imgproc.TM_SQDIFF_NORMED) ; 
				  break;
			  case TM_CCOEFF:
				  Imgproc.matchTemplate(mROI, mTemplate, mResult, Imgproc.TM_CCOEFF) ; 
				  break;
			  case TM_CCOEFF_NORMED:
				  Imgproc.matchTemplate(mROI, mTemplate, mResult, Imgproc.TM_CCOEFF_NORMED) ; 
				  break;
			  case TM_CCORR:
				  Imgproc.matchTemplate(mROI, mTemplate, mResult, Imgproc.TM_CCORR) ; 
				  break;
			  case TM_CCORR_NORMED:
				  Imgproc.matchTemplate(mROI, mTemplate, mResult, Imgproc.TM_CCORR_NORMED) ; 
				  break;
		  }
		  
		  Core.MinMaxLocResult mmres =  Core.minMaxLoc(mResult);
		  
		  if(type == TM_SQDIFF || type == TM_SQDIFF_NORMED)
		  	{ matchLoc = mmres.minLoc; }
		  else
		    { matchLoc = mmres.maxLoc; }
		  
		  Point  matchLoc_tx = new Point(matchLoc.x+area.x,matchLoc.y+area.y);
		  Point  matchLoc_ty = new Point(matchLoc.x + mTemplate.cols() + area.x , matchLoc.y + mTemplate.rows()+area.y );
		 
		  Core.rectangle(mRgba, matchLoc_tx,matchLoc_ty, new Scalar(255, 255, 0, 255));
		 
		  if(type == TM_SQDIFF || type == TM_SQDIFF_NORMED)
		  	{ return mmres.maxVal; }
		  else
		    { return mmres.minVal; }

	    }
	
	private Mat  get_template(CascadeClassifier clasificator, Rect area,int size){
		Mat template = new Mat();
	    Mat mROI = mGray.submat(area);
	    MatOfRect eyes = new MatOfRect();
	    Point iris = new Point();
	    Rect eye_template = new Rect();
	    clasificator.detectMultiScale(mROI, eyes, 1.15, 2,Objdetect.CASCADE_FIND_BIGGEST_OBJECT|Objdetect.CASCADE_SCALE_IMAGE, new Size(30,30),new Size());
	    

	    Rect[] eyesArray = eyes.toArray();
	    for (int i = 0; i < eyesArray.length; i++){
	    	Rect e = eyesArray[i];
	    	e.x = area.x + e.x;
	    	e.y = area.y + e.y;
	    	Rect eye_only_rectangle = new Rect((int)e.tl().x,(int)( e.tl().y + e.height*0.4),(int)e.width,(int)(e.height*0.6));
	    	mROI = mGray.submat(eye_only_rectangle);
	    	Mat vyrez = mRgba.submat(eye_only_rectangle);
	    	Core.MinMaxLocResult mmG = Core.minMaxLoc(mROI);
	    	
	    	Core.circle(vyrez, mmG.minLoc,2, new Scalar(255, 255, 255, 255),2);
	    	iris.x = mmG.minLoc.x + eye_only_rectangle.x;
	    	iris.y = mmG.minLoc.y + eye_only_rectangle.y;
	    	eye_template = new Rect((int)iris.x-size/2,(int)iris.y-size/2 ,size,size);
	    	Core.rectangle(mRgba,eye_template.tl(),eye_template.br(),new Scalar(255, 0, 0, 255), 2);
	    	template = (mGray.submat(eye_template)).clone();
	    	return template;
	    }
		 return template;
	  }

    @Override
    public void run() {
        super.run();

        synchronized (this) {
            // Explicitly deallocate Mats
            if (mRgba != null)
                mRgba.release();
            if (mGray != null)
                mGray.release();
            if (mCascadeFile != null)
            	mCascadeFile.delete();
            if (mNativeDetector != null)
            	mNativeDetector.release();

            mRgba = null;
            mGray = null;
            mCascadeFile = null;
        }
    }
}
