#pragma once
//#ifndef base
//#define base
#include "highgui.h"
#include "cxcore.h"
#include "vector"
#include "cv.h"
#include <vector>
#include <iostream>
#include <time.h>
#include<QLabel>
#include <QtWidgets/QApplication>
#include <QImage>
#include <QFileDialog>
#include"qspinbox.h"
#include <QDebug>
#include "fanal_opencv_pro.h"
#include"qpixmap.h"
#include<qmainwindow>
using namespace cv;
using namespace std;
#include<opencv2/opencv.hpp>
#define INT_MAX 2147483647
#define MatLab(x) ((x)-1)
//extern int pixel = 150;

class Base_pic_process
{
public:
	using DataType = int;
	using Point = tuple<DataType, DataType>;
	using Edge = tuple<DataType, DataType, DataType, DataType>;
	using Trangle = tuple<DataType, DataType, DataType, DataType, DataType>;
	using Matrix = vector<vector<DataType>>;

	 int pixel = 120;

	//void display(fanal_opencv_pro& w, time_t& begin, time_t& end, int& num, IplImage*& img, IplImage*& newImg, double& BPPValue);
	QImage QCVMat2QImage(const cv::Mat& mat);
	IplImage* QImageToIplImageGray(QImage* qImage);
	Matrix Mat2Vec(const cv::Mat& pic);
	cv::Mat Vec2Mat(const Matrix& array);


};


class IBR :public Base_pic_process
{

public:
	
	struct Location
	{
		int x1;
		int x2;
		int y1;
		int y2;
		Location(int ax, int ay, int bx, int by) :x1(ax), y1(ay), x2(bx), y2(by) {};
		Location() {};
	};

	struct Block
	{
		int start_pos;
		int intervals;
	};

	struct ColorNode
	{
		//0普通矩阵，1宽为1矩阵，2长为1矩阵，3孤立点
		int kind, g1, g2, g3, g4;
		ColorNode(int k, int gg1, int gg2 = -1, int gg3 = -1, int gg4 = -1) :kind(k), g1(gg1), g2(gg2), g3(gg3), g4(gg4) {};
		ColorNode() {};
	};
	map<unsigned int, ColorNode> color_list;
	map<unsigned int, Location> block_list;
	double BPP(map<unsigned int, ColorNode>& color_list, int xn, int yn, vector<char>& Q);
	void StartNamCut(const IplImage* img, CvMat* markMatrix, CvMat* R, map<unsigned int, ColorNode>& color_list, map<unsigned int, Location>& block_list, int height, int width, double margin, int& num);
	Location FindSameBlock(const IplImage* img, CvMat* markMatrix, CvMat* R, int x1, int y1, double margin, double ratio);
	bool IsSameBlock(const IplImage* img, int x1, int y1, int x2, int y2, int margin);
	void SetMarkMatrix(CvMat* markMatrix, int x1, int y1, int x2, int y2, int value);
	void EnCode(CvMat* R, int height, int width, vector<char>& Q);
	void Decode(CvMat* R, int height, int width, const vector<char>& Q);
	void RestoreImage(IplImage* img, CvMat* markMatrix, CvMat* R, map<unsigned int, ColorNode>& color_list, int height, int width);
	void RestoreImageValue(IplImage* img, ColorNode node, int x1, int y1, int x2, int y2);
	double PSNR(IplImage* img, IplImage* newImg);

};


//class TriangularRaster :public Base_pic_process
//{
//
//public:
//
//
//	Matrix tem = vector<vector<DataType>>(pixel, vector<DataType>(pixel, 0));
//
//
//	int Encode(const Matrix& picture, vector<Edge>& edges, vector<Trangle>& trangles,
//		int* num_of_point = nullptr, int* num_of_edge = nullptr, int* num_of_tran = nullptr);
//	int Decode(Matrix& res, const vector<Edge>& edges,
//		const vector<Trangle>& trangles);
//
//	int Accumulate(const Matrix& a);
//	Matrix MatrixMinus(const Matrix& a, const Matrix& b);
//	Matrix SplitVector(const Matrix& a, int x1, int y1, int x2, int y2);
//	int get_upper_tri_ele(Matrix &org, int line_num, int row);
//	int get_lower_tri_ele(Matrix&org, int line_num, int row);
//	cv::Mat Handle(const cv::Mat& mat, int& cost_time, int& point, int& line, int& tran);
//
//
//};


class MatrixRaster :public Base_pic_process
{
public:

	struct Location
	{
		int x1;
		int x2;
		int y1;
		int y2;
		Location(int ax, int ay, int bx, int by) :x1(ax), y1(ay), x2(bx), y2(by) {};
	};

	struct ColorNode
	{
		//0普通矩阵，1宽为1矩阵，2长为1矩阵，3孤立点
		int kind, g1, g2, g3, g4;
		ColorNode(int k, int gg1, int gg2 = -1, int gg3 = -1, int gg4 = -1) :kind(k), g1(gg1), g2(gg2), g3(gg3), g4(gg4) {};
	};


	IplImage* img = cvLoadImage("C:\\Users\\pqs-xsz-xia\\Desktop\\gray_images\\baboonzyp512.bmp", 0);
	IplImage* newImg = nullptr;

	double margin = 20;
	double ratio = 0;
	double BPPValue = 0;
	IplImage* sketch = nullptr;



	void display(fanal_opencv_pro& w, time_t& begin, time_t& end, int& num, IplImage*& img, IplImage*& newImg, double& BPPValue);


	void fun(double& margin, double& ratio, QLabel*& pTag1, QLabel*& pTag2, QLabel*& pTag3, fanal_opencv_pro& w, time_t& begin,
		time_t& end, int& num, IplImage*& img, IplImage* newImg, double& BPPValue, IplImage*& sketch, int num_judge);

	void fun_plus(double& margin, double& ratio, QLabel*& pTag1, QLabel*& pTag2, QLabel*& pTag3, fanal_opencv_pro& w, time_t& begin,
		time_t& end, int& num, IplImage*& img, IplImage* newImg, double& BPPValue, IplImage*& sketch, int num_judge,int pixel_test,int step_test);

	void StartNamCut(const IplImage *img, CvMat *markMatrix, CvMat *R, vector<ColorNode> &ColorList, vector<Location> &LocList, int height, int width, double margin, int &num, double ratio, int num_judge);
	Location FindSameBlock(const IplImage *img, CvMat *markMatrix, CvMat *R, int x1, int y1, double margin, double ratio, int num_judge);
	bool IsSameBlock(const IplImage *img, int x1, int y1, int x2, int y2, int margin);
	void SetMarkMatrix(CvMat *markMatrix, int x1, int y1, int x2, int y2, int value);
	void EnCode(CvMat *R, int height, int width, vector<char> &Q);
	void Decode(CvMat *R, int height, int width, const vector<char> &Q);
	void RestoreImage(IplImage *img, CvMat *markMatrix, CvMat *R, vector<ColorNode>cn, int height, int width);
	void RestoreImageValue(IplImage *img, ColorNode node, int x1, int y1, int x2, int y2);
	double PSNR(IplImage *img, IplImage *newImg);
	double BPP(vector<ColorNode> &cn, int xn, int yn, vector<char> &Q);

	void horizonal(IplImage* img, double margin, double ratio/*int argc,char **argv*/, int num_judge);



};

class All_work :public MatrixRaster
{
public:
	void work(fanal_opencv_pro&w,QApplication&a);

	int num_judge;
	All_work(int num = 1):num_judge(num){}
};

//#endif // !1

//#include "highgui.h"
//#include "cxcore.h"
//#include "vector"
//#include "cv.h"
//#include <vector>
//#include <iostream>
//#include <time.h>
//#include<QLabel>
//#include <QtWidgets/QApplication>
//#include <QImage>
//#include <QFileDialog>
//#include"qspinbox.h"
//#include <QDebug>
//#include "fanal_opencv_pro.h"
//#include"qpixmap.h"
//#include<qmainwindow>
//using namespace cv;
//using namespace std;
//#include<opencv2/opencv.hpp>
//#define INT_MAX 2147483647
//#define MatLab(x) ((x)-1)
//int pixel = 150;
//
//class Base_pic_process
//{
//public:
//	using DataType = int;
//	using Point = tuple<DataType, DataType>;
//	using Edge = tuple<DataType, DataType, DataType, DataType>;
//	using Trangle = tuple<DataType, DataType, DataType, DataType, DataType>;
//	using Matrix = vector<vector<DataType>>;
//	
//
//
//	//void display(fanal_opencv_pro& w, time_t& begin, time_t& end, int& num, IplImage*& img, IplImage*& newImg, double& BPPValue);
//	QImage QCVMat2QImage(const cv::Mat& mat);
//	IplImage* QImageToIplImageGray(QImage* qImage);
//	Matrix Mat2Vec(const cv::Mat& pic);
//	cv::Mat Vec2Mat(const Matrix& array);
//
//
//};
//
//
//class TriangularRaster :public Base_pic_process
//{
//
//public:
//
//
//	Matrix tem = vector<vector<DataType>>(pixel, vector<DataType>(pixel, 0));
//
//
//	int Encode(const Matrix& picture, vector<Edge>& edges, vector<Trangle>& trangles, 
//		int* num_of_point = nullptr, int* num_of_edge = nullptr, int* num_of_tran = nullptr);
//	int Decode(Matrix& res, const vector<Edge>& edges,
//		const vector<Trangle>& trangles);
//
//	int Accumulate(const Matrix& a);
//	Matrix MatrixMinus(const Matrix& a, const Matrix& b);
//	Matrix SplitVector(const Matrix& a, int x1, int y1, int x2, int y2);
//	int get_upper_tri_ele(Matrix &org, int line_num, int row);
//	int get_lower_tri_ele(Matrix&org, int line_num, int row);
//	cv::Mat Handle(const cv::Mat& mat, int& cost_time, int& point, int& line, int& tran);
//
//
//};
//
//
//class MatrixRaster :public Base_pic_process
//{
//public:
//
//	struct Location
//	{
//		int x1;
//		int x2;
//		int y1;
//		int y2;
//		Location(int ax, int ay, int bx, int by) :x1(ax), y1(ay), x2(bx), y2(by) {};
//	};
//
//	struct ColorNode
//	{
//		//0普通矩阵，1宽为1矩阵，2长为1矩阵，3孤立点
//		int kind, g1, g2, g3, g4;
//		ColorNode(int k, int gg1, int gg2 = -1, int gg3 = -1, int gg4 = -1) :kind(k), g1(gg1), g2(gg2), g3(gg3), g4(gg4) {};
//	};
//
//
//	IplImage* img = cvLoadImage("C:\\Users\\hubia\\Downloads\\images\\baboonzyp512.bmp", 0);
//	IplImage* newImg = nullptr;
//
//	double margin = 20;
//	double ratio = 0;
//	double BPPValue = 0;
//	IplImage* sketch = nullptr;
//
//
//
//	void display(fanal_opencv_pro& w, time_t& begin, time_t& end, int& num, IplImage*& img, IplImage*& newImg, double& BPPValue);
//
//
//	void fun(double& margin, double& ratio, QLabel*& pTag1, QLabel*& pTag2, QLabel*& pTag3, QMainWindow& w, time_t& begin,
//		time_t& end, int& num, IplImage*& img, IplImage*& newImg, double& BPPValue, IplImage*& sketch, int num_judge);
//
//
//	void StartNamCut(const IplImage *img, CvMat *markMatrix, CvMat *R, vector<ColorNode> &ColorList, vector<Location> &LocList, int height, int width, double margin, int &num, double ratio, int num_judge);
//	Location FindSameBlock(const IplImage *img, CvMat *markMatrix, CvMat *R, int x1, int y1, double margin, double ratio, int num_judge);
//	bool IsSameBlock(const IplImage *img, int x1, int y1, int x2, int y2, int margin);
//	void SetMarkMatrix(CvMat *markMatrix, int x1, int y1, int x2, int y2, int value);
//	void EnCode(CvMat *R, int height, int width, vector<char> &Q);
//	void Decode(CvMat *R, int height, int width, const vector<char> &Q);
//	void RestoreImage(IplImage *img, CvMat *markMatrix, CvMat *R, vector<ColorNode>cn, int height, int width);
//	void RestoreImageValue(IplImage *img, ColorNode node, int x1, int y1, int x2, int y2);
//	double PSNR(IplImage *img, IplImage *newImg);
//	double BPP(vector<ColorNode> &cn, int xn, int yn, vector<char> &Q);
//
//	void horizonal(IplImage* img, double margin, double ratio/*int argc,char **argv*/, int num_judge);
//
//
//
//};
//
//class All_work :public MatrixRaster
//{
//public:
//	void work(fanal_opencv_pro&w);
//};