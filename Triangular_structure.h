#pragma once
#include"base_pic_process.h"


class MatrixHandler :public Base_pic_process
{
public:
	// 上三角求和
	int step;
	int AccumulateUpperTrangle(const Mat& a, int x = 0);
	

	// 下三角求和
	int AccumulateLowerTrangle(const Mat& a, int x = 0);
	
};

// 图像矩阵
class ImgMatrixHandler : public MatrixHandler {
public:
	// 像素大小
	int normalized_pixel = 500;
	// 门限值，当 色值 >= threads_hold, 当成白色处理
	int threads_hold = 255;
	// 标准化Matrix
	Mat NormalizeMatrix(const cv::Mat& pic);
	
};

// TriangularHandler 类
class TriangularHandler : public ImgMatrixHandler {
public:
	using DataType = int;
	using Edge = tuple<DataType, DataType, DataType, DataType>;
	using Trangle = tuple<DataType, DataType, DataType, DataType, DataType>;

	// 上三角形 的颜色
	int set_up_trangle = 255;
	// 下三角形 的颜色
	int set_lo_trangle = 40;
	// 点和线段的颜色
	int set_edge = 1;

	// 宏，用于 Matlab 和 cpp 下标互换
#define MatLab(x) ((x)-1)
  // 求对数
	int MyLogTwo(int x) { return log2(x); }

	// 照抄老师Matlab代码
	void Encode(const Mat& picture, vector<Edge>& edges,
		vector<Trangle>& trangles, int& num_of_point, int& num_of_edge,
		int& num_of_tran);
	

	// 照抄老师Matlab代码
	void Decode(Mat& res, const vector<Edge>& edges,
		const vector<Trangle>& trangles);
	
};

// Raster 抽象类
class BaseRaster {
	virtual cv::Mat Handle(const cv::Mat& mat, int& cost_time, int& point,
		int& line, int& tran) = 0;
};

// TriangularRaster 类
class TriangularRaster : public TriangularHandler, public BaseRaster {
public:
	// 实现了 BaseRaster 的 Handle 方法
	cv::Mat Handle(const cv::Mat& mat, int& cost_time, int& point, int& line,
		int& tran) override {
		// 1.开始计时
		auto startTime = clock();
		// 2.标准化预处理
		auto vec = NormalizeMatrix(mat);
		vector<Edge> edges;
		vector<Trangle> trans;
		// 3.编码

		Encode(vec, edges, trans, point, line, tran);
		// 4.解码，还用 vec 是因为需要知道像素的多少
		Decode(vec, edges, trans);
		// 5.结束计时
		auto endTime = clock();
		// 6.计算时间（单位：毫秒）
		cost_time = (int)(endTime - startTime) / (CLOCKS_PER_SEC / 1000);
		return vec;
	}
};
