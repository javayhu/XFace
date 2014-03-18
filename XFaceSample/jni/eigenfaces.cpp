/*
 * Copyright (c) 2012. Philipp Wagner <bytefish[at]gmx[dot]de>.
 * Released to public domain under terms of the BSD Simplified license.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the organization nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 *   See <http://www.opensource.org/licenses/bsd-license>
 */

#include "helper.hpp"
#include "eigenfaces.hpp"

#include <android/log.h>

#define LOG_TAG "xface native lib --- eigenfaces.cpp "
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))

// Reads a sequence from a FileNode::SEQ with type _Tp into a result vector.
template<typename _Tp>
inline void readFileNodeList(const FileNode& fn, vector<_Tp>& result) {
	if (fn.type() == FileNode::SEQ) {
		for (FileNodeIterator it = fn.begin(); it != fn.end();) {
			_Tp item;
			it >> item;
			result.push_back(item);
		}
	}
}

// Writes the a list of given items to a cv::FileStorage.
template<typename _Tp>
inline void writeFileNodeList(FileStorage& fs, const string& name, const vector<_Tp>& items) {
	// typedefs
	typedef typename vector<_Tp>::const_iterator constVecIterator;
	// write the elements in item to fs
	fs << name << "[";
	for (constVecIterator it = items.begin(); it != items.end(); ++it) {
		fs << *it;
	}
	fs << "]";
}

void Eigenfaces::compute(const vector<Mat>& src, const vector<int>& labels) {
	//[hujiawei]not possible!
//	if (src.size() == 0) {
//		string error_message =
//				format(
//						"Empty training data was given. You'll need more than one sample to learn a model.");
//		CV_Error(CV_StsUnsupportedFormat, error_message);
//	}
	// observations in row
	Mat data = asRowMatrix(src, CV_64FC1);
	//hjw opencv 64 float -> vector<Mat> -> Mat
	// number of samples
	int n = data.rows; //5
	// dimensionality of data
	//    int d = data.cols;
	// assert there are as much samples as labels
	//[hujiawei]
//	if (n != labels.size()) {
//		string error_message =
//				format(
//						"The number of samples (src) must equal the number of labels (labels). len(samples)=%d, len(labels)=%d.",
//						n, labels.size());
//		CV_Error(CV_StsBadArg, error_message);
//	}//[hujiawei]
	// clip number of components to be valid
	if ((_num_components <= 0) || (_num_components > n))
		_num_components = n; //5
	// perform the PCA
	PCA pca(data, Mat(), CV_PCA_DATA_AS_ROW, _num_components);
	// copy the PCA results
	_mean = pca.mean.reshape(1, 1); // store the mean vector (1*10304)
	_eigenvalues = pca.eigenvalues.clone(); // eigenvalues by row (80*1)
	_eigenvectors = transpose(pca.eigenvectors); // eigenvectors by column (10304*80)
	_labels = labels; // store labels for prediction (403/402)
	// save projections
	//int sampleIdx = 0
	int sampleIdx = 400; //use orl faces! but miss them all! [hujiawei] 0-399!
	for (; sampleIdx < data.rows; sampleIdx++) {
		Mat p = project(data.row(sampleIdx).clone()); //1*5
		this->_projections.push_back(p); //_projections: vector<Mat>
	}
	LOGD("projection size is %d", _projections.size());
}

void Eigenfaces::predict(const Mat& src, int &minClass, double &minDist) {
	//[hujiawei]no need to do it. when app first comes to facerec without adding a image will trap in here.
//	if (_projections.empty()) {
//		// throw error if no data (or simply return -1?)
//		string error_message =
//				"This cv::Eigenfaces model is not computed yet. Did you call cv::Eigenfaces::train?";
//		CV_Error(CV_StsError, error_message);
//	} else if (_eigenvectors.rows != src.total()) {
//		// check data alignment just for clearer exception messages
//		string error_message =
//				format(
//						"Wrong input image size. Reason: Training and Test images must be of equal size! Expected an image with %d elements, but got %d.",
//						_eigenvectors.rows, src.total());
//		CV_Error(CV_StsError, error_message);
//	}//[hujiawei]
	if (_projections.empty()) {
		minClass = -1; //-1 means do not know the person! or no image data in database!
	}
	// project into PCA subspace
	Mat q = project(src.reshape(1, 1));
	// find 1-nearest neighbor
	_threshold = DBL_MAX;
	minDist = DBL_MAX;
	minClass = -1;
	int minSampleIdx = -1;
	for (int sampleIdx = 0; sampleIdx < _projections.size(); sampleIdx++) {
		double dist = norm(_projections[sampleIdx], q, NORM_L2);
		LOGD("predict for loop %d, dist=%f", sampleIdx, dist);
		if ((dist < minDist) && (dist < _threshold)) {
			minDist = dist;
			minClass = _labels[sampleIdx];
			minSampleIdx = sampleIdx;
		}
	}
//    cout << "minDist is " << minDist << " minClass is " << minClass << " minSampleIndex is " << minSampleIdx << endl;
	LOGD("minDis=%d, minClass=%d, minSampleIdx=%d", minDist, minClass, minSampleIdx);
}

int Eigenfaces::predict(const Mat& src) {
	int label;
	double dummy;
	predict(src, label, dummy);
	return label;
}

Mat Eigenfaces::project(const Mat& src) {
	Mat W = _eigenvectors;
	Mat mean = _mean;
	// get number of samples and dimension
	int n = src.rows;
	int d = src.cols;
	// make sure the data has the correct shape
	if (W.rows != d) {//[hujiawei] sometimes when frame mat is null, this error will occur
		string error_message = format(
				"Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).",
				src.rows, src.cols, W.rows, W.cols);
		CV_Error(CV_StsBadArg, error_message);
	}
	// make sure mean is correct if not empty
	if (!mean.empty() && (mean.total() != d)) {
		string error_message = format(
				"Wrong mean shape for the given data matrix. Expected %d, but was %d.", d,
				mean.total());
		CV_Error(CV_StsBadArg, error_message);
	}
	// create temporary matrices
	Mat X, Y;
	// make sure you operate on correct type
	src.convertTo(X, W.type()); //X has same data with src, but diffrent address.
								// safe to do, because of above assertion
								// safe to do, because of above assertion
	if (!mean.empty()) {
		for (int i = 0; i < n; i++) {
			Mat r_i = X.row(i);
			subtract(r_i, mean.reshape(1, 1), r_i); //src1,src2,dest
		}
	}
	// finally calculate projection as Y = (X-mean)*W
	gemm(X, W, 1.0, Mat(), 0.0, Y);
	//InputArray src1, InputArray src2, double alpha,
	//InputArray src3, double gamma, OutputArray dst, int flags=0
	return Y;
}

// basically useless
Mat Eigenfaces::reconstruct(const Mat& src) {
	Mat W = _eigenvectors;
	Mat mean = _mean;
	// get number of samples and dimension
	int n = src.rows;
	int d = src.cols;
	// make sure the data has the correct shape
	if (W.cols != d) {
		string error_message = format(
				"Wrong shapes for given matrices. Was size(src) = (%d,%d), size(W) = (%d,%d).",
				src.rows, src.cols, W.rows, W.cols);
		CV_Error(CV_StsBadArg, error_message);
	}
	// make sure mean is correct if not empty
	if (!mean.empty() && (mean.total() != W.rows)) {
		string error_message = format(
				"Wrong mean shape for the given eigenvector matrix. Expected %d, but was %d.",
				W.cols, mean.total());
		CV_Error(CV_StsBadArg, error_message);
	}
	// initalize temporary matrices
	Mat X, Y;
	// copy data & make sure we are using the correct type
	src.convertTo(Y, W.type());
	// calculate the reconstruction
	gemm(Y, W, 1.0, Mat(), 0.0, X, GEMM_2_T);
	// safe to do because of above assertion
	if (!mean.empty()) {
		for (int i = 0; i < n; i++) {
			Mat r_i = X.row(i);
			add(r_i, mean.reshape(1, 1), r_i);
		}
	}
	return X;
}

void Eigenfaces::load(const string& path) {
	//const FileStorage& fs
	//read matrices
	FileStorage fs(path, FileStorage::READ);
	fs["num_components"] >> _num_components;
	fs["threshold"] >> _threshold; // [hujiawei] // when load, threshold may affect the predict!
	fs["mean"] >> _mean;
	fs["eigenvalues"] >> _eigenvalues;
	fs["eigenvectors"] >> _eigenvectors;
	fs["labels"] >> _labels;
	// read sequences
	readFileNodeList(fs["projections"], _projections);
	fs.release();
}

void Eigenfaces::save(const string& path) const {
	//FileStorage& fs
	// write matrices
	FileStorage fs(path, FileStorage::WRITE);
	fs << "num_components" << _num_components;
	fs << "threshold" << _threshold; // [hujiawei]
	fs << "mean" << _mean;
	fs << "eigenvalues" << _eigenvalues; //actually useless!
	fs << "eigenvectors" << _eigenvectors;
	fs << "labels" << _labels;
	// write sequences
	writeFileNodeList(fs, "projections", _projections);
	fs.release();
}

