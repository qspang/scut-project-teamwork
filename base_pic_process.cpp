#include"base_pic_process.h"
//int pixel;


QImage Base_pic_process::QCVMat2QImage(const cv::Mat& mat)
{
	const unsigned char* data = mat.data;

	int width = mat.cols;
	int height = mat.rows;
	int bytesPerLine = static_cast<int>(mat.step);
	switch (mat.type())
	{
		//8 bit , ARGB
	case CV_8UC4:
	{
		QImage image(data, width, height, bytesPerLine, QImage::Format_ARGB32);
		return image;
	}

	case CV_8UC3:
	{
		QImage image(data, width, height, bytesPerLine, QImage::Format_RGB888);
		return image.rgbSwapped();
	}
	case CV_8UC1:
	{
		QImage image(data, width, height, bytesPerLine, QImage::Format_Grayscale8);
		return image;
	}
	default:
	{
		qWarning() << "Unsupported cv::Mat type:" << mat.type() << ", Empty QImage will be returned!";
		return QImage();
	}
	}
}



IplImage* Base_pic_process::QImageToIplImageGray(QImage* qImage)
{
	int width = qImage->width();
	int height = qImage->height();
	int widthStep = 0;

	CvSize Size;
	Size.height = height;
	Size.width = width;

	IplImage* charIplImageBuffer = cvCreateImage(Size, IPL_DEPTH_8U, 1);
	widthStep = charIplImageBuffer->widthStep;
	char* charTemp = (char*)charIplImageBuffer->imageData;

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			int index = y * widthStep + x;
			charTemp[index] = (char)qGray(qImage->pixel(x, y));
		}
	}

	return charIplImageBuffer;
}


Base_pic_process::Matrix Base_pic_process::Mat2Vec(const cv::Mat& pic) {
	Size dsize = Size(pixel, pixel);
	cv::Mat mat;
	cv::resize(pic, mat, dsize, 0, 0, INTER_AREA);
	int m = mat.rows, n = mat.cols;
	Matrix ans(m, vector<DataType>(n, 0));
	for (int i = 0; i < mat.rows; ++i) {
		for (int j = 0; j < mat.cols; ++j) {
			if (mat.at<uchar>(i, j) >= 240) {
				ans[i][j] = 1;
			}
			else {
				ans[i][j] = 0;
			}
		}
	}
	return ans;
}


cv::Mat Base_pic_process::Vec2Mat(const Matrix& array) {
	int m = array.size();
	int n = array[0].size();

	cv::Mat img(m, n, CV_8UC1);
	uchar* ptmp = nullptr;
	for (int i = 0; i < m; ++i) {
		ptmp = img.ptr<uchar>(i);
		for (int j = 0; j < n; ++j) {
			if (array[i][j] == 0) {
				// black
				ptmp[j] = 0;
			}
			else {
				// write
				ptmp[j] = 255;
			}
		}
	}
	return img;
}



