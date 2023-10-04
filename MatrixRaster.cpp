#include"base_pic_process.h"
#include"Triangular_structure.h"
#include<map>
//extern int pixel = 150;

double MatrixRaster::BPP(vector<MatrixRaster::ColorNode> &cn, int xn, int yn, vector<char> &Q)
{
	int a1 = 0, a2 = 0, a3 = 0;
	for (int i = 0; i < cn.size(); i++) {
		if (cn[i].g1 != -1 && cn[i].g4 != -1) a1++;
		else if (cn[i].g2 == -1 && cn[i].g3 == -1) a3++;
		else  a2++;
	}
	double BPPValue = (Q.size() + 16 * a1 + 8 * a2 + 4.0*a3) / (xn*yn);
	return BPPValue;
}



void MatrixRaster::EnCode(CvMat *R, int height, int width, vector<char> &Q)
{
	int c = 0;
	int offsetValue = 0;
	int count = 0;

	for (int i = 0; i < height; i++)
	{
		c = 0;
		offsetValue = 0;
		count = 0;
		//Q.push_back(' ');
		for (int j = 0; j < width; j++)
		{
			int value = (R->data.ptr + R->step * i)[j];
			if (0 == value)
			{
				if (j == width - 1)
				{
					Q.push_back('0');
				}
			}
			else
			{
				if (1 == value)
				{
					Q.push_back('1');
					Q.push_back('1');
				}
				else if (2 == value)
				{
					Q.push_back('1');
					Q.push_back('0');
					Q.push_back('1');
				}
				else if (3 == value)
				{
					Q.push_back('1');
					Q.push_back('0');
					Q.push_back('0');
				}

				//位置从1开始 不是从0开始 所以多减去1
				int b = ceil(log((double)(width - c - count)) / log(2.0f));
				if (0 == count)
				{
					offsetValue = j;
				}
				else
				{
					offsetValue = j - c - 1;
				}

				b = (b == 0) ? 1 : b;
				count = 1;
				c = j;

				for (int v = b - 1; v >= 0; v--)
				{
					if ((offsetValue & (1 << v)) == 0)
					{
						Q.push_back('0');
					}
					else
					{
						Q.push_back('1');
					}
				}
			}
		}
	}
}

void MatrixRaster::Decode(CvMat *R, int height, int width, const vector<char> &Q)
{
	int n = 0;
	int count = 0;
	//上个非零元素的位置
	int c = 0;
	int row = 0;
	int num = 0;
	for (int pos = 0; pos < Q.size(); pos++)
	{
		if ('1' == Q[pos])
		{
			pos++;
			if ('1' == Q[pos])
			{
				n = 1;

			}
			else
			{
				pos++;
				if ('1' == Q[pos])
				{
					n = 2;
				}
				else
				{
					n = 3;
				}
			}
		}
		else
		{
			row++;
			count = 0;
			c = 0;
			num = 0;
			continue;

		}

		int b = ceil(log((double)(width - c - count)) / log(2.0f));
		b = b == 0 ? 1 : b;
		count = 1;

		int value = 0;
		for (int i = b - 1; i >= 0; i--)
		{
			pos++;
			if ('1' == Q[pos])
			{
				value += 1 << i;
			}
		}

		if (0 == num)
		{
			c = c + value;
		}
		else
		{
			c = c + value + 1;
		}

		num++;


		(R->data.ptr + row * R->step)[c] = n;

		if (c == (width - 1))
		{
			row++;
			count = 0;
			c = 0;
			num = 0;
		}
	}


}




//void Base_pic_process::horizonal(IplImage* img, double margin, double ratio/*int argc,char **argv*/, int num_judge)
//{
//	//	IplImage *img = NULL;
//
//		//if(argc == 6 && (img = cvLoadImage(argv[1],0)) != 0)
//		//{
//
//		//	cvNamedWindow("原图灰度图像",1);
//		//	cvShowImage("原图灰度图像",img);
//			/*一，分割同类块及编码*/
//	int height = img->height;
//	int width = img->width;
//
//	CvMat *markMatrix = cvCreateMat(height, width, CV_8UC1);
//	cvSetZero(markMatrix);
//	CvMat *R = cvCreateMat(height, width, CV_8UC1);
//	cvSetZero(R);
//
//	int num = 0;
//	vector<ColorNode> colorList;
//	vector<Location> locList;
//	//double margin = //atof(argv[4]);
////	double ratio=//atoi(argv[5]);
//	time_t begin, end;
//	begin = clock();
//	/*分块*/
//	StartNamCut(img, markMatrix, R, colorList, locList, height, width, margin, num, ratio, num_judge);
//
//	/*矩阵编码*/
//	vector<char> Q;
//	EnCode(R, height, width, Q);
//	end = clock();
//
//	
//	CvMat *T = cvCreateMat(height, width, CV_8UC1);
//	cvSetZero(T);
//	begin = clock();
//	Decode(T, height, width, Q);
//
//
//
//	IplImage *newImg = cvCreateImage(cvSize(width, height), 8, 1);
//	RestoreImage(newImg, markMatrix, T, colorList, height, width);
//	end = clock();
//
//
//	
//	double BPPValue = BPP(colorList, width, height, Q);
//	
//	IplImage* sketch;
//	int xr, yr;
//	int sketchXn, sketchYn;
//	if (height >= 256 || width >= 256)
//	{
//		xr = 1, yr = 1, sketchXn = height, sketchYn = width;
//		sketch = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
//	}
//	else
//	{
//		xr = 511 / (height - 1);
//		yr = 511 / (width - 1);
//		sketchXn = 512, sketchYn = 512;
//		sketch = cvCreateImage(cvSize(512, 512), IPL_DEPTH_8U, 1);
//	}
//
//	for (int i = 0; i < sketchYn; i++)
//	{
//		uchar* ptrsketch = (uchar*)(sketch->imageData + i * sketch->widthStep);
//		for (int j = 0; j < sketchXn; j++)
//		{
//			ptrsketch[j] = 255;
//		}
//	}
//	for (int i = 0; i < locList.size(); i++)
//	{
//		if (locList[i].x1 == 0 && locList[i].y1 == 0)
//			cvRectangle(sketch, cvPoint(locList[i].x1, locList[i].y1), cvPoint(locList[i].x2, locList[i].y2), cvScalar(0x00, 0x00, 0x00));
//		else   if (locList[i].x1 == 0 && locList[i].y1 != 0)
//			cvRectangle(sketch, cvPoint(locList[i].x1, locList[i].y1 - 1), cvPoint(locList[i].x2, locList[i].y2), cvScalar(0x00, 0x00, 0x00));
//		else   if (locList[i].x1 != 0 && locList[i].y1 == 0)
//			cvRectangle(sketch, cvPoint(locList[i].x1 - 1, locList[i].y1), cvPoint(locList[i].x2, locList[i].y2), cvScalar(0x00, 0x00, 0x00));
//		else   if (locList[i].x1 != 0 && locList[i].y1 != 0)
//			cvRectangle(sketch, cvPoint(locList[i].x1 - 1, locList[i].y1 - 1), cvPoint(locList[i].x2, locList[i].y2), cvScalar(0x00, 0x00, 0x00));
//	}
//	
//}





double MatrixRaster::PSNR(IplImage *img, IplImage *newImg)
{
	int yn = img->height;
	int xn = img->width;
	double sum = 0.0;
	for (int i = 0; i < yn; i++) {
		uchar* imgptr = (uchar*)img->imageData + i * img->widthStep;
		uchar* newImgptr = (uchar*)newImg->imageData + i * newImg->widthStep;
		for (int j = 0; j < xn; j++) {
			sum += pow((double)(imgptr[j] - newImgptr[j]), 2);
		}
	}
	double temp = 255.0 * 255 * xn*yn / sum;
	double PSNRValue = 10 * log(temp) / log(10.0);
	return PSNRValue;
}





void MatrixRaster::RestoreImage(IplImage *img, CvMat *markMatrix, CvMat *R, vector<ColorNode> cn, int height, int width)
{
	int x1(0), x2(0), y1(0), y2(0);

	for (vector<ColorNode>::iterator it = cn.begin(); it != cn.end(); it++)
	{
		int i = 0;
		//先查找下一个未计算的分块 起点坐标
		while (x2 < width)
		{
			if ((markMatrix->data.ptr + markMatrix->step*y2)[x2++] == 1)
				break;


			if (x2 == width)
			{
				x1 = x2 = 0;
				y1 = ++y2;
			}

		}

		x1 = --x2;

		//再查找出这个块的 (x1,y1) (x2,y2)
		if (0 == (*it).kind)
		{
			while (x2 < width)
			{
				//x2要先走一步
				if ((R->data.ptr + R->step*y2)[++x2] != 0 || (markMatrix->data.ptr + markMatrix->step*y2)[x2] == 0)
					break;
			}

			x2--;

			while (y2 < height)
			{
				if ((R->data.ptr + R->step*y2)[x2] == 2 || (markMatrix->data.ptr + markMatrix->step*y2)[x2] == 0)
					break;
				y2++;
			}

		}
		else if (1 == (*it).kind)
		{
			while (x2 < width)
			{
				if ((R->data.ptr + R->step*y2)[++x2] == 2)
					break;
			}
		}
		else if (2 == (*it).kind)
		{
			while (y2 < height)
			{
				if ((R->data.ptr + R->step*y2)[x2] == 2)
					break;
				y2++;
			}
		}

		//cout<<"("<<x1<<","<<y1<<")("<<x2<<","<<y2<<")"<<endl;
		RestoreImageValue(img, *it, x1, y1, x2, y2);

		//Mark

		SetMarkMatrix(markMatrix, x1, y1, x2, y2, 0);


		x1 = x2;
		y2 = y1;

	}

}

void MatrixRaster::RestoreImageValue(IplImage *img, ColorNode node, int x1, int y1, int x2, int y2)
{
	uchar *upPtr = (uchar*)(img->imageData + img->widthStep*y1);
	uchar *downPtr = (uchar*)(img->imageData + img->widthStep*y2);

	if (3 == node.kind)
	{
		upPtr[x1] = node.g1;
	}
	else if (1 == node.kind)
	{
		upPtr[x1] = node.g1;
		upPtr[x2] = node.g2;
		for (int x = x1 + 1; x < x2; x++)
		{
			double i2 = (double)(x - x1) / (double)(x2 - x1);
			upPtr[x] = (int)(node.g1 + (node.g2 - node.g1)*i2);

		}
	}
	else if (2 == node.kind)
	{
		upPtr[x1] = node.g1;
		downPtr[x1] = node.g2;
		for (int y = y1 + 1; y < y2; y++)
		{
			double i1 = (double)(y - y1) / (double)(y2 - y1);
			(img->imageData + img->widthStep*y)[x1] = (int)(node.g1 + (node.g2 - node.g1)*i1);

		}
	}
	else
	{
		for (int x = x1; x <= x2; x++)
		{
			for (int y = y1; y <= y2; y++)
			{
				double i1 = (double)(y - y1) / (double)(y2 - y1);
				double i2 = (double)(x - x1) / (double)(x2 - x1);
				double g5 = node.g1 + (node.g2 - node.g1)*i2;
				double g6 = node.g3 + (node.g4 - node.g3)*i2;
				(img->imageData + img->widthStep*y)[x] = (int)(g5 + (g6 - g5) *i1);

			}
		}

		upPtr[x1] = node.g1;
		upPtr[x2] = node.g2;
		downPtr[x1] = node.g3;
		downPtr[x2] = node.g4;
	}
}


void MatrixRaster::SetMarkMatrix(CvMat *markMatrix, int x1, int y1, int x2, int y2, int value)
{
	for (int y = y1; y <= y2; y++)
	{
		for (int x = x1; x <= x2; x++)
		{
			(markMatrix->data.ptr + markMatrix->step*y)[x] = value;
		}
	}
}


void MatrixRaster::display(fanal_opencv_pro& w, time_t& begin, time_t& end, int& num, IplImage*& img, IplImage*& newImg, double& BPPValue)
{
	w.ui.text_1_show->setText(QString::number(end - begin));
	w.ui.text_2_show->setText(QString::number(num));
	w.ui.text_3_show->setText(QString::number(PSNR(img, newImg)));
	w.ui.text_4_show->setText(QString::number(BPPValue));
	w.ui.text_5_show->setText(QString::number(8.0 / BPPValue));
	num = 0;
}



void MatrixRaster::StartNamCut(const IplImage *img, CvMat *markMatrix, CvMat *R, vector<ColorNode> &ColorList, vector<Location> &LocList, int height, int width, double margin, int &num, double ratio, int num_judge)
{

	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			if ((markMatrix->data.ptr + row * markMatrix->step)[col] == 0)
			{
				num++;
				Location loc = FindSameBlock(img, markMatrix, R, col, row, margin, ratio, num_judge);
				LocList.push_back(loc);
				//Mark
				SetMarkMatrix(markMatrix, loc.x1, loc.y1, loc.x2, loc.y2, 1);
				//R
				if (loc.x1 == loc.x2 && loc.y1 == loc.y2)
				{
					uchar * g = R->data.ptr + R->step*loc.y1;
					g[loc.x1] = 3;
					ColorNode node(3, ((uchar*)img->imageData + img->widthStep*loc.y1)[loc.x1]);
					ColorList.push_back(node);
				}
				else if (loc.x1 == loc.x2)
				{
					for (int j = loc.y1; j <= loc.y2; j++)
					{
						uchar *g = R->data.ptr + R->step*j;
						g[loc.x1] = 0;
					}
					(R->data.ptr + R->step*loc.y1)[loc.x1] = 1;
					(R->data.ptr + R->step*loc.y2)[loc.x1] = 2;
					ColorNode node(2, ((uchar*)img->imageData + img->widthStep*loc.y1)[loc.x1], ((uchar*)img->imageData + img->widthStep*loc.y2)[loc.x1]);
					ColorList.push_back(node);

				}
				else
				{
					for (int i = loc.y1; i <= loc.y2; i++)
					{
						for (int j = loc.x1; j <= loc.x2; j++)
						{
							(R->data.ptr + R->step*i)[j] = 0;
						}
					}

					if (loc.y1 == loc.y2)
					{
						ColorNode node(1, ((uchar*)img->imageData + img->widthStep*loc.y1)[loc.x1], ((uchar*)img->imageData + img->widthStep*loc.y1)[loc.x2]);
						ColorList.push_back(node);
					}
					else
					{
						ColorNode node(0, ((uchar*)img->imageData + img->widthStep*loc.y1)[loc.x1], ((uchar*)img->imageData + img->widthStep*loc.y1)[loc.x2],
							((uchar*)img->imageData + img->widthStep*loc.y2)[loc.x1], ((uchar*)img->imageData + img->widthStep*loc.y2)[loc.x2]);
						ColorList.push_back(node);
					}


					(R->data.ptr + R->step*loc.y1)[loc.x1] = 1;
					(R->data.ptr + R->step*loc.y2)[loc.x2] = 2;
				}
			}
		}
	}
}

bool MatrixRaster::IsSameBlock(const IplImage *img, int x1, int y1, int x2, int y2, int margin)
{
	uchar *gy1 = (uchar*)img->imageData + y1 * img->widthStep;
	uchar *g1 = gy1 + x1;
	uchar *g2 = gy1 + x2;
	uchar *gy2 = (uchar*)img->imageData + y2 * img->widthStep;
	uchar *g3 = gy2 + x1;
	uchar *g4 = gy2 + x2;
	bool returnValue = true;

	if (x1 == x2 && y1 == y2)
	{
		returnValue = true;
	}
	else if (y1 == y2)
	{
		for (int x = x1 + 1; x < x2; x++)
		{
			double i2 = (double)(x - x1) / (double)(x2 - x1);
			double g = *g1 + (*g4 - *g1)*i2;
			uchar *gValue = gy1 + x;
			if (abs(*gValue - g) > margin)
			{
				returnValue = false;
				break;
			}

		}
	}
	else if (x1 == x2)
	{
		for (int y = y1 + 1; y < y2; y++)
		{
			double i1 = (double)(y - y1) / (double)(y2 - y1);
			double g = *g1 + (*g4 - *g1)*i1;
			uchar *gy = (uchar*)img->imageData + y * img->widthStep;
			uchar *gValue = gy + x1;
			if (abs(*gValue - g) > margin)
			{
				returnValue = false;
				break;
			}
		}
	}
	else
	{
		for (int x = x1; x <= x2; x++)
		{
			for (int y = y1; y <= y2; y++)
			{
				double i1 = (double)(y - y1) / (double)(y2 - y1);
				double i2 = (double)(x - x1) / (double)(x2 - x1);
				double g5 = *g1 + (*g2 - *g1)*i2;
				double g6 = *g3 + (*g4 - *g3)*i2;
				double g = g5 + (g6 - g5) *i1;
				uchar *gy = (uchar*)img->imageData + y * img->widthStep;
				uchar *gValue = gy + x;
				if (abs(*gValue - g) > margin)
				{
					returnValue = false;
					break;
				}
			}
		}
	}

	return returnValue;
}


void MatrixRaster::horizonal(IplImage* img, double margin, double ratio/*int argc,char **argv*/, int num_judge)
{
	//	IplImage *img = NULL;

		//if(argc == 6 && (img = cvLoadImage(argv[1],0)) != 0)
		//{

		//	cvNamedWindow("原图灰度图像",1);
		//	cvShowImage("原图灰度图像",img);
			/*一，分割同类块及编码*/
	int height = img->height;
	int width = img->width;

	CvMat *markMatrix = cvCreateMat(height, width, CV_8UC1);
	cvSetZero(markMatrix);
	CvMat *R = cvCreateMat(height, width, CV_8UC1);
	cvSetZero(R);

	int num = 0;
	vector<ColorNode> colorList;
	vector<Location> locList;
	//double margin = //atof(argv[4]);
//	double ratio=//atoi(argv[5]);
	time_t begin, end;
	begin = clock();
	/*分块*/
	StartNamCut(img, markMatrix, R, colorList, locList, height, width, margin, num, ratio, num_judge);

	/*矩阵编码*/
	vector<char> Q;
	EnCode(R, height, width, Q);
	end = clock();

	//	cout<<"编码压缩花费:"<<end-begin<<"ms"<<endl;

		/*for(int i =0;i<height;i++)
		{
			for(int j=0;j<width;j++)
			{
				cout<<(int)(R->data.ptr+i*R->step)[j]<<" ";
			}
			cout<<endl;
		}


		cout<<"编码共："<<Q.size()<<"位"<<endl;
		for (vector<char>::iterator it = Q.begin();it != Q.end();it++)
		{
			cout<<*it;
		}*/

		/*for (vector<ColorNode>::iterator it = colorList.begin();it != colorList.end();it++)
		{
			cout<<(*it).g1<<" "<<(*it).g2<<" "<<(*it).g3<<" "<<(*it).g4<<endl;
		}*/

		/*二，还原图像矩阵及图像*/
	CvMat *T = cvCreateMat(height, width, CV_8UC1);
	cvSetZero(T);
	begin = clock();
	Decode(T, height, width, Q);


	/*cout<<endl;
	for(int i =0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			cout<<(int)(T->data.ptr+i*T->step)[j]<<" ";
		}
		cout<<endl;
	}*/


	IplImage *newImg = cvCreateImage(cvSize(width, height), 8, 1);
	RestoreImage(newImg, markMatrix, T, colorList, height, width);
	end = clock();


	//	cout<<"还原图像耗时:"<<end-begin<<"ms"<<endl;


	//	cout<<"块数:"<<num<<endl;
	 //   cout<<"PSNR值:"<<PSNR(img,newImg)<<endl;
	double BPPValue = BPP(colorList, width, height, Q);
	//	    cout<<"BPP值:"<<BPPValue<<endl;
	//	    cout<<"CR值:"<<8.0/BPPValue<<endl;
	//
		//	cvNamedWindow("压缩图灰度图像",1);
		//	cvShowImage("压缩图灰度图像",newImg);
		//	cvSaveImage(argv[2],img,0);
		 //   cvSaveImage(argv[3],newImg,0);

			//画分割图
	IplImage* sketch;
	int xr, yr;
	int sketchXn, sketchYn;
	if (height >= 256 || width >= 256)
	{
		xr = 1, yr = 1, sketchXn = height, sketchYn = width;
		sketch = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	}
	else
	{
		xr = 511 / (height - 1);
		yr = 511 / (width - 1);
		sketchXn = 512, sketchYn = 512;
		sketch = cvCreateImage(cvSize(512, 512), IPL_DEPTH_8U, 1);
	}

	for (int i = 0; i < sketchYn; i++)
	{
		uchar* ptrsketch = (uchar*)(sketch->imageData + i * sketch->widthStep);
		for (int j = 0; j < sketchXn; j++)
		{
			ptrsketch[j] = 255;
		}
	}
	for (int i = 0; i < locList.size(); i++)
	{
		if (locList[i].x1 == 0 && locList[i].y1 == 0)
			cvRectangle(sketch, cvPoint(locList[i].x1, locList[i].y1), cvPoint(locList[i].x2, locList[i].y2), cvScalar(0x00, 0x00, 0x00));
		else   if (locList[i].x1 == 0 && locList[i].y1 != 0)
			cvRectangle(sketch, cvPoint(locList[i].x1, locList[i].y1 - 1), cvPoint(locList[i].x2, locList[i].y2), cvScalar(0x00, 0x00, 0x00));
		else   if (locList[i].x1 != 0 && locList[i].y1 == 0)
			cvRectangle(sketch, cvPoint(locList[i].x1 - 1, locList[i].y1), cvPoint(locList[i].x2, locList[i].y2), cvScalar(0x00, 0x00, 0x00));
		else   if (locList[i].x1 != 0 && locList[i].y1 != 0)
			cvRectangle(sketch, cvPoint(locList[i].x1 - 1, locList[i].y1 - 1), cvPoint(locList[i].x2, locList[i].y2), cvScalar(0x00, 0x00, 0x00));
	}
	//cvShowImage("分割示意图",sketch);

	//cvWaitKey();
	//cvDestroyAllWindows();
//	}

//	return 0;
}

 void MatrixRaster::fun(double& margin, double& ratio, QLabel*& pTag1, QLabel*& pTag2, QLabel*& pTag3, fanal_opencv_pro& w, time_t& begin,
time_t& end, int& num, IplImage*& img, IplImage* newImg, double& BPPValue, IplImage*& sketch, int num_judge) {
	 
	 switch (num_judge) {
	 case 8:
	 {
		 IBR ibr;
		 int height = img->height;
		 int width = img->width;

		 CvMat* ibr_mat = cvCreateMat(height, width, CV_8UC1);
		 cvSetZero(ibr_mat);
		 CvMat* R = cvCreateMat(height, width, CV_8UC1);
		 cvSetZero(R);

		 int num = 0;

		 time_t begin, end;
		 begin = clock();
		 /*分块*/
		 ibr.StartNamCut(img, ibr_mat, R, ibr.color_list, ibr.block_list, height, width, margin, num);

		 /*矩阵编码*/
		 vector<char> Q;
		 EnCode(R, height, width, Q);
		 end = clock();


		 CvMat* T = cvCreateMat(height, width, CV_8UC1);
		 cvSetZero(T);
		 begin = clock();
		 Decode(T, height, width, Q);


		 IplImage* newImg = cvCreateImage(cvSize(width, height), 8, 1);
		 ibr.RestoreImage(newImg, ibr_mat, T, ibr.color_list, height, width);
		 end = clock();
		 double BPPValue = ibr.BPP(ibr.color_list, width, height, Q);

		 //画分割图
		 IplImage* sketch;
		 int xr, yr;
		 int sketchXn, sketchYn;
		 if (height >= 256 || width >= 256)
		 {
			 xr = 1, yr = 1, sketchXn = height, sketchYn = width;
			 sketch = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
		 }
		 else
		 {
			 xr = 511 / (height - 1);
			 yr = 511 / (width - 1);
			 sketchXn = 512, sketchYn = 512;
			 sketch = cvCreateImage(cvSize(512, 512), IPL_DEPTH_8U, 1);
		 }

		 for (int i = 0; i < sketchYn; i++)
		 {
			 uchar* ptrsketch = (uchar*)(sketch->imageData + i * sketch->widthStep);
			 for (int j = 0; j < sketchXn; j++)
			 {
				 ptrsketch[j] = 255;
			 }
		 }
		 for (int i = 0; i < ibr.block_list.size(); i++)
		 {
			 if (ibr.block_list[i].x1 == 0 && ibr.block_list[i].y1 == 0)
				 cvRectangle(sketch, cvPoint(ibr.block_list[i].x1, ibr.block_list[i].y1), cvPoint(ibr.block_list[i].x2, ibr.block_list[i].y2), cvScalar(0x00, 0x00, 0x00));
			 else   if (ibr.block_list[i].x1 == 0 && ibr.block_list[i].y1 != 0)
				 cvRectangle(sketch, cvPoint(ibr.block_list[i].x1, ibr.block_list[i].y1 - 1), cvPoint(ibr.block_list[i].x2, ibr.block_list[i].y2), cvScalar(0x00, 0x00, 0x00));
			 else   if (ibr.block_list[i].x1 != 0 && ibr.block_list[i].y1 == 0)
				 cvRectangle(sketch, cvPoint(ibr.block_list[i].x1 - 1, ibr.block_list[i].y1), cvPoint(ibr.block_list[i].x2, ibr.block_list[i].y2), cvScalar(0x00, 0x00, 0x00));
			 else   if (ibr.block_list[i].x1 != 0 && ibr.block_list[i].y1 != 0)
				 cvRectangle(sketch, cvPoint(ibr.block_list[i].x1 - 1, ibr.block_list[i].y1 - 1), cvPoint(ibr.block_list[i].x2, ibr.block_list[i].y2), cvScalar(0x00, 0x00, 0x00));
		 }
		 Mat Img1 = cvarrToMat(img);
		 QImage image1 = QCVMat2QImage(Img1);
		 pTag1->setPixmap(QPixmap::fromImage(image1));
		 //pTag1->setGeometry(100, 470, image1.width(), image1.height());
		 pTag1->setGeometry(930, 500, 240, 240);
		 pTag1->setScaledContents(1);
		 Mat Img2 = cvarrToMat(newImg);
		 QImage image2 = QCVMat2QImage(Img2);
		 pTag2->setPixmap(QPixmap::fromImage(image2));

		 pTag2->setGeometry(640, 500, 240, 240);
		 pTag2->setScaledContents(1);

		 Mat Img3 = cvarrToMat(sketch);
		 QImage image3 = QCVMat2QImage(Img3);
		 pTag3->setPixmap(QPixmap::fromImage(image3));
		 pTag3->setGeometry(190, 390, 360, 360);
		 pTag3->setScaledContents(1);
	 }
	 break;
	 case 4:
	 {

		 Mat pic = cvarrToMat(img);
		 // 保存
		 //cvSaveImage("C:\\Users\\pqs - xsz - xia\\Desktop\\c++_plus\\qt_demo(2)\\qt_demo\\\\SCUT_photo_1.jpg",pic);
		 //imwrite("D:\\qtopencv_photo\\SCUT_photo_1.jpg", pic);
		 int n = 8;
		 int cost_time = 0, point = 0, line = 0, tran = 0;
		 TriangularRaster r;

		 // 设置 像素边长
		 //r.pixel = 100;
		 // run!

		 Mat org = r.Handle(pic, cost_time, point, line, tran);
		 // 打印下
		 /*cout << "cost_time: " << cost_time << " point:" << point << " line:" << line
			 << " tran:" << tran << endl;*/
			 // 保存结果
			 /*namedWindow("aa", WINDOW_AUTOSIZE);
			 imshow("aa", org);*/

		 if (num_judge == 5)
		 {
			 imwrite("C:\\Users\\pqs-xsz-xia\\Desktop\\file\\SCUT_photo_0.jpg", org);
			 return;
		 }

		 QString tem;

		 tem = QString::number(cost_time);
		 w.ui.text_1_show->setText(tem);
		 tem = QString::number(tran);
		 w.ui.text_2_show->setText(tem);
		 //tem = QString::number(0);
		 w.ui.text_3_show->setText(" ");
		 //tem = QString::number(0);
		 w.ui.text_4_show->setText(" ");
		 //tem = QString::number(0);
		 w.ui.text_5_show->setText(" ");


		 Mat Img1 = cvarrToMat(img);
		 QImage image1 = QCVMat2QImage(Img1);
		 pTag1->setPixmap(QPixmap::fromImage(image1));
		 //pTag1->setGeometry(100, 470, image1.width(), image1.height());
		 pTag1->setGeometry(930, 500, 240, 240);
		 pTag1->setScaledContents(1);
		 /*Mat Img2 = cvarrToMat(newImg);
		 QImage image2 = QCVMat2QImage(Img2);*/
		 pTag2->setPixmap(QPixmap::fromImage(image1));

		 pTag2->setGeometry(640, 500, 240, 240);
		 pTag2->setScaledContents(1);

		 //Mat Img3 = cvarrToMat(sketch);
		 QImage image3 = QCVMat2QImage(org);
		 pTag3->setPixmap(QPixmap::fromImage(image3));
		 pTag3->setGeometry(190, 390, 360, 360);
		 pTag3->setScaledContents(1);
	 }
	 break;
	 default:
	 {
		 int height = img->height;
		 int width = img->width;
		 CvMat* markMatrix = cvCreateMat(height, width, CV_8UC1);
		 cvSetZero(markMatrix);
		 CvMat* R = cvCreateMat(height, width, CV_8UC1);
		 cvSetZero(R);
		 vector<ColorNode> colorList;
		 vector<Location> locList;
		 begin = clock();
		 StartNamCut(img, markMatrix, R, colorList, locList, height, width, margin, num, ratio, num_judge);
		 vector<char> Q;
		 EnCode(R, height, width, Q);
		 end = clock();
		 CvMat* T = cvCreateMat(height, width, CV_8UC1);
		 cvSetZero(T);
		 begin = clock();
		 Decode(T, height, width, Q);
		 newImg = cvCreateImage(cvSize(width, height), 8, 1);

		 RestoreImage(newImg, markMatrix, T, colorList, height, width);
		 //cvShowImage("aaaa", newImg);
		 end = clock();
		 BPPValue = BPP(colorList, width, height, Q);

		 int xr, yr;
		 int sketchXn, sketchYn;
		 if (height >= 256 || width >= 256)
		 {
			 xr = 1, yr = 1, sketchXn = height, sketchYn = width;
			 sketch = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
		 }
		 else
		 {
			 xr = 511 / (height - 1);
			 yr = 511 / (width - 1);
			 sketchXn = 512, sketchYn = 512;
			 sketch = cvCreateImage(cvSize(512, 512), IPL_DEPTH_8U, 1);
		 }

		 for (int i = 0; i < sketchYn; i++)
		 {
			 uchar* ptrsketch = (uchar*)(sketch->imageData + i * sketch->widthStep);
			 for (int j = 0; j < sketchXn; j++)
			 {
				 ptrsketch[j] = 255;
			 }
		 }
		 for (int i = 0; i < locList.size(); i++)
		 {
			 if (locList[i].x1 == 0 && locList[i].y1 == 0)
				 cvRectangle(sketch, cvPoint(locList[i].x1, locList[i].y1), cvPoint(locList[i].x2, locList[i].y2), cvScalar(0x00, 0x00, 0x00));
			 else   if (locList[i].x1 == 0 && locList[i].y1 != 0)
				 cvRectangle(sketch, cvPoint(locList[i].x1, locList[i].y1 - 1), cvPoint(locList[i].x2, locList[i].y2), cvScalar(0x00, 0x00, 0x00));
			 else   if (locList[i].x1 != 0 && locList[i].y1 == 0)
				 cvRectangle(sketch, cvPoint(locList[i].x1 - 1, locList[i].y1), cvPoint(locList[i].x2, locList[i].y2), cvScalar(0x00, 0x00, 0x00));
			 else   if (locList[i].x1 != 0 && locList[i].y1 != 0)
				 cvRectangle(sketch, cvPoint(locList[i].x1 - 1, locList[i].y1 - 1), cvPoint(locList[i].x2, locList[i].y2), cvScalar(0x00, 0x00, 0x00));
		 }

		 Mat Img1 = cvarrToMat(img);
		 QImage image1 = QCVMat2QImage(Img1);
		 pTag1->setPixmap(QPixmap::fromImage(image1));
		 //pTag1->setGeometry(100, 470, image1.width(), image1.height());
		 pTag1->setGeometry(930, 500, 240, 240);
		 pTag1->setScaledContents(1);
		 Mat Img2 = cvarrToMat(newImg);
		 QImage image2 = QCVMat2QImage(Img2);
		 pTag2->setPixmap(QPixmap::fromImage(image2));

		 pTag2->setGeometry(640, 500, 240, 240);
		 pTag2->setScaledContents(1);

		 Mat Img3 = cvarrToMat(sketch);
		 QImage image3 = QCVMat2QImage(Img3);
		 pTag3->setPixmap(QPixmap::fromImage(image3));
		 pTag3->setGeometry(190, 390, 360, 360);
		 pTag3->setScaledContents(1);
	 }
	 break;
	 }

}



void MatrixRaster::fun_plus(double& margin, double& ratio, QLabel*& pTag1, QLabel*& pTag2, QLabel*& pTag3, fanal_opencv_pro& w, time_t& begin,
	time_t& end, int& num, IplImage*& img, IplImage* newImg, double& BPPValue, IplImage*& sketch, int num_judge,int pixel_test,int step_test) {

	switch (num_judge) {
	case 8:	
	{
		IBR ibr;
		int height = img->height;
		int width = img->width;
		CvMat* ibr_mat = cvCreateMat(height, width, CV_8UC1);
		cvSetZero(ibr_mat);
		CvMat* R = cvCreateMat(height, width, CV_8UC1);
		cvSetZero(R);
		begin = clock();
		/*分块*/
		ibr.StartNamCut(img, ibr_mat, R, ibr.color_list, ibr.block_list, height, width, margin, num);
		/*矩阵编码*/
		vector<char> Q;
		EnCode(R, height, width, Q);
		end = clock();
		CvMat* T = cvCreateMat(height, width, CV_8UC1);
		cvSetZero(T);
		begin = clock();
		Decode(T, height, width, Q);
		newImg = cvCreateImage(cvSize(width, height), 8, 1);
		ibr.RestoreImage(newImg, ibr_mat, T, ibr.color_list, height, width);
		end = clock();
		BPPValue = ibr.BPP(ibr.color_list, width, height, Q);
		//画分割图
		int xr, yr;
		int sketchXn, sketchYn;
		if (height >= 256 || width >= 256)
		{
			xr = 1, yr = 1, sketchXn = height, sketchYn = width;
			sketch = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
		}
		else
		{
			xr = 511 / (height - 1);
			yr = 511 / (width - 1);
			sketchXn = 512, sketchYn = 512;
			sketch = cvCreateImage(cvSize(512, 512), IPL_DEPTH_8U, 1);
		}

		for (int i = 0; i < sketchYn; i++)
		{
			uchar* ptrsketch = (uchar*)(sketch->imageData + i * sketch->widthStep);
			for (int j = 0; j < sketchXn; j++)
			{
				ptrsketch[j] = 255;
			}
		}
		for (int i = 0; i < ibr.block_list.size(); i++)
		{
			if (ibr.block_list[i].x1 == 0 && ibr.block_list[i].y1 == 0)
				cvRectangle(sketch, cvPoint(ibr.block_list[i].x1, ibr.block_list[i].y1), cvPoint(ibr.block_list[i].x2, ibr.block_list[i].y2), cvScalar(0x00, 0x00, 0x00));
			else   if (ibr.block_list[i].x1 == 0 && ibr.block_list[i].y1 != 0)
				cvRectangle(sketch, cvPoint(ibr.block_list[i].x1, ibr.block_list[i].y1 - 1), cvPoint(ibr.block_list[i].x2, ibr.block_list[i].y2), cvScalar(0x00, 0x00, 0x00));
			else   if (ibr.block_list[i].x1 != 0 && ibr.block_list[i].y1 == 0)
				cvRectangle(sketch, cvPoint(ibr.block_list[i].x1 - 1, ibr.block_list[i].y1), cvPoint(ibr.block_list[i].x2, ibr.block_list[i].y2), cvScalar(0x00, 0x00, 0x00));
			else   if (ibr.block_list[i].x1 != 0 && ibr.block_list[i].y1 != 0)
				cvRectangle(sketch, cvPoint(ibr.block_list[i].x1 - 1, ibr.block_list[i].y1 - 1), cvPoint(ibr.block_list[i].x2, ibr.block_list[i].y2), cvScalar(0x00, 0x00, 0x00));
		}
		Mat Img1 = cvarrToMat(img);
		QImage image1 = QCVMat2QImage(Img1);
		pTag1->setPixmap(QPixmap::fromImage(image1));
		//pTag1->setGeometry(100, 470, image1.width(), image1.height());
		pTag1->setGeometry(930, 500, 240, 240);
		pTag1->setScaledContents(1);
		Mat Img2 = cvarrToMat(newImg);
		QImage image2 = QCVMat2QImage(Img2);
		pTag2->setPixmap(QPixmap::fromImage(image2));

		pTag2->setGeometry(640, 500, 240, 240);
		pTag2->setScaledContents(1);

		Mat Img3 = cvarrToMat(sketch);
		QImage image3 = QCVMat2QImage(Img3);
		pTag3->setPixmap(QPixmap::fromImage(image3));
		pTag3->setGeometry(190, 390, 360, 360);
		pTag3->setScaledContents(1);

	}
	break;
	case 4:	
	{

		Mat pic = cvarrToMat(img);
		// 保存
		//cvSaveImage("C:\\Users\\pqs - xsz - xia\\Desktop\\c++_plus\\qt_demo(2)\\qt_demo\\\\SCUT_photo_1.jpg",pic);
		//imwrite("D:\\qtopencv_photo\\SCUT_photo_1.jpg", pic);
		int n = 8;
		int cost_time = 0, point = 0, line = 0, tran = 0;
		TriangularRaster r;
		r.normalized_pixel = pixel_test;
		r.step = step_test;
		// 设置 像素边长
		//r.pixel = 100;
		// run!

		Mat org = r.Handle(pic, cost_time, point, line, tran);
		// 打印下
		/*cout << "cost_time: " << cost_time << " point:" << point << " line:" << line
			<< " tran:" << tran << endl;*/
			// 保存结果
			/*namedWindow("aa", WINDOW_AUTOSIZE);
			imshow("aa", org);*/
		if (num_judge == 5)
		{
			imwrite("C:\\Users\\pqs-xsz-xia\\Desktop\\file\\SCUT_photo_0.jpg", org);
			return;
		}
		/* imwrite("C:\\Users\\pqs-xsz-xia\\Desktop\\file\\SCUT_photo_0.jpg", org);
		 Mat org_test = imread("C:\\Users\\pqs-xsz-xia\\Desktop\\file\\SCUT_photo_0.jpg", 0);
		 */



		 /*for (int i = 0; i < pixel; ++i) {
			  for (int j = 0; j < pixel; ++j) {
				  if (r.tem[i][j] == 0) {
					  r.tem[i][j] = 1;
				  }
				  else {
					  r.tem[i][j] = 0;
				  }
			  }
		  }*/

		QString tem;

		tem = QString::number(cost_time);
		w.ui.text_1_show->setText(tem);
		tem = QString::number(tran);
		w.ui.text_2_show->setText(tem);
		//tem = QString::number(0);
		w.ui.text_3_show->setText(" ");
		//tem = QString::number(0);
		w.ui.text_4_show->setText(" ");
		//tem = QString::number(0);
		w.ui.text_5_show->setText(" ");



		//auto tem_new = r.Vec2Mat(r.tem);
		////tem_new.bitwise_not();
		//namedWindow("tem_new", WINDOW_AUTOSIZE);
		//imshow("tem_new", tem_new);

		Mat Img1 = cvarrToMat(img);
		QImage image1 = QCVMat2QImage(Img1);
		pTag1->setPixmap(QPixmap::fromImage(image1));
		//pTag1->setGeometry(100, 470, image1.width(), image1.height());
		pTag1->setGeometry(930, 500, 240, 240);
		pTag1->setScaledContents(1);
		/*Mat Img2 = cvarrToMat(newImg);
		QImage image2 = QCVMat2QImage(Img2);*/
		pTag2->setPixmap(QPixmap::fromImage(image1));

		pTag2->setGeometry(640, 500, 240, 240);
		pTag2->setScaledContents(1);

		//Mat Img3 = cvarrToMat(sketch);
		QImage image3 = QCVMat2QImage(org);
		pTag3->setPixmap(QPixmap::fromImage(image3));
		pTag3->setGeometry(190, 390, 360, 360);
		pTag3->setScaledContents(1);
	}
	break;
	case 5:
	{

			Mat pic = cvarrToMat(img);
			// 保存
			//cvSaveImage("C:\\Users\\pqs - xsz - xia\\Desktop\\c++_plus\\qt_demo(2)\\qt_demo\\\\SCUT_photo_1.jpg",pic);
			//imwrite("D:\\qtopencv_photo\\SCUT_photo_1.jpg", pic);
			int n = 8;
			int cost_time = 0, point = 0, line = 0, tran = 0;
			TriangularRaster r;
			r.normalized_pixel = pixel_test;
			r.step = step_test;
			// 设置 像素边长
			//r.pixel = 100;
			// run!

			Mat org = r.Handle(pic, cost_time, point, line, tran);
			// 打印下
			/*cout << "cost_time: " << cost_time << " point:" << point << " line:" << line
				<< " tran:" << tran << endl;*/
				// 保存结果
				/*namedWindow("aa", WINDOW_AUTOSIZE);
				imshow("aa", org);*/
			if (num_judge == 5)
			{
				imwrite("C:\\Users\\pqs-xsz-xia\\Desktop\\file\\SCUT_photo_0.jpg", org);
				return;
			}
			
			
		}
	break;
	default:
	{
	int height = img->height;
	int width = img->width;
	CvMat* markMatrix = cvCreateMat(height, width, CV_8UC1);
	cvSetZero(markMatrix);
	CvMat* R = cvCreateMat(height, width, CV_8UC1);
	cvSetZero(R);
	vector<ColorNode> colorList;
	vector<Location> locList;
	begin = clock();
	StartNamCut(img, markMatrix, R, colorList, locList, height, width, margin, num, ratio, num_judge);
	vector<char> Q;
	EnCode(R, height, width, Q);
	end = clock();
	CvMat* T = cvCreateMat(height, width, CV_8UC1);
	cvSetZero(T);
	begin = clock();
	Decode(T, height, width, Q);
	newImg = cvCreateImage(cvSize(width, height), 8, 1);

	RestoreImage(newImg, markMatrix, T, colorList, height, width);
	//cvShowImage("aaaa", newImg);
	end = clock();
	BPPValue = BPP(colorList, width, height, Q);

	int xr, yr;
	int sketchXn, sketchYn;
	if (height >= 256 || width >= 256)
	{
		xr = 1, yr = 1, sketchXn = height, sketchYn = width;
		sketch = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	}
	else
	{
		xr = 511 / (height - 1);
		yr = 511 / (width - 1);
		sketchXn = 512, sketchYn = 512;
		sketch = cvCreateImage(cvSize(512, 512), IPL_DEPTH_8U, 1);
	}

	for (int i = 0; i < sketchYn; i++)
	{
		uchar* ptrsketch = (uchar*)(sketch->imageData + i * sketch->widthStep);
		for (int j = 0; j < sketchXn; j++)
		{
			ptrsketch[j] = 255;
		}
	}
	for (int i = 0; i < locList.size(); i++)
	{
		if (locList[i].x1 == 0 && locList[i].y1 == 0)
			cvRectangle(sketch, cvPoint(locList[i].x1, locList[i].y1), cvPoint(locList[i].x2, locList[i].y2), cvScalar(0x00, 0x00, 0x00));
		else   if (locList[i].x1 == 0 && locList[i].y1 != 0)
			cvRectangle(sketch, cvPoint(locList[i].x1, locList[i].y1 - 1), cvPoint(locList[i].x2, locList[i].y2), cvScalar(0x00, 0x00, 0x00));
		else   if (locList[i].x1 != 0 && locList[i].y1 == 0)
			cvRectangle(sketch, cvPoint(locList[i].x1 - 1, locList[i].y1), cvPoint(locList[i].x2, locList[i].y2), cvScalar(0x00, 0x00, 0x00));
		else   if (locList[i].x1 != 0 && locList[i].y1 != 0)
			cvRectangle(sketch, cvPoint(locList[i].x1 - 1, locList[i].y1 - 1), cvPoint(locList[i].x2, locList[i].y2), cvScalar(0x00, 0x00, 0x00));
	}

	Mat Img1 = cvarrToMat(img);
	QImage image1 = QCVMat2QImage(Img1);
	pTag1->setPixmap(QPixmap::fromImage(image1));
	//pTag1->setGeometry(100, 470, image1.width(), image1.height());
	pTag1->setGeometry(930, 500, 240, 240);
	pTag1->setScaledContents(1);
	Mat Img2 = cvarrToMat(newImg);
	QImage image2 = QCVMat2QImage(Img2);
	pTag2->setPixmap(QPixmap::fromImage(image2));

	pTag2->setGeometry(640, 500, 240, 240);
	pTag2->setScaledContents(1);

	Mat Img3 = cvarrToMat(sketch);
	QImage image3 = QCVMat2QImage(Img3);
	pTag3->setPixmap(QPixmap::fromImage(image3));
	pTag3->setGeometry(190, 390, 360, 360);
	pTag3->setScaledContents(1);
	}
	break;
	}
	

}





MatrixRaster::Location MatrixRaster::FindSameBlock(const IplImage* img, CvMat* markMatrix, CvMat* R, int x1, int y1, double margin, double ratio, int num_judge)
{

	int x2(x1), y2(y1);
	int tempX2, tempY2, tempX, tempY;
	if (num_judge == 3)
	{
		while (x2 < img->width && y2 < img->height && (markMatrix->data.ptr + y1 * markMatrix->step)[x2] == 0 && (markMatrix->data.ptr + y2 * markMatrix->step)[x1] == 0)
		{
			if (!IsSameBlock(img, x1, y1, x2, y2, margin))
				break;

			x2++;
			y2++;

		}

		x2--;
		y2--;

		int endX = x2;

		int maxX = ratio == 0 ? INT_MAX : (y2 - y1 + 1) * ratio;

		while (x2 < img->width && (x2 - x1 < maxX) && (markMatrix->data.ptr + y1 * markMatrix->step)[x2] == 0)
		{
			if (!IsSameBlock(img, x1, y1, x2, y2, margin))
			{
				break;
			}
			x2++;
		}
		x2--;

		tempX = x2;
		tempY = y2;

		x2 = endX;
		int maxY = ratio == 0 ? INT_MAX : (x2 - x1 + 1) * ratio;
		while (y2 < img->height && (y2 - y1 < maxY))
		{
			if (!IsSameBlock(img, x1, y1, x2, y2, margin))
			{
				break;
			}

			y2 = y2 + 1;
		}

		y2--;

		if (/*ratio !=0 &&*/((tempX - x1 + 1) * (tempY - y1 + 1) >= (x2 - x1 + 1) * (y2 - y1 + 1)))
		{
			x2 = tempX;
			y2 = tempY;
		}
	}
	else
	{
		while (x2 < img->width && (markMatrix->data.ptr + y1 * markMatrix->step)[x2] == 0)
		{
			if (!IsSameBlock(img, x1, y1, x2, y2, margin))
			{
				break;
			}
			x2++;
		}
		x2--;

		int maxY = ratio == 0 ? INT_MAX : (x2 - x1 + 1) * ratio;

		while (y2 < img->height && ((y2 - y1) < maxY))
		{
			if (!IsSameBlock(img, x1, y1, x2, y2, margin))
			{
				break;
			}

			y2 = y2 + 1;
		}

		y2--;


		if (ratio != 0)
		{
			if ((x2 - x1 + 1) > (y2 - y1 + 1) * ratio)
			{
				x2 = (y2 - y1 + 1) * ratio + x1 - 1;
			}
		}

		tempX2 = x2;
		tempY2 = y2;

		//列优先遍历
		x2 = x1;
		y2 = y1;

		while (y2 < img->height && (markMatrix->data.ptr + y2 * markMatrix->step)[x1] == 0)
		{
			if (!IsSameBlock(img, x1, y1, x2, y2, margin))
			{
				break;
			}
			y2++;
		}

		y2--;

		int maxX = ratio == 0 ? INT_MAX : (y2 - y1 + 1) * ratio;

		while (x2 < img->width && (markMatrix->data.ptr + y1 * markMatrix->step)[x2] == 0 && ((x2 - x1) < maxX))
		{
			if (!IsSameBlock(img, x1, y1, x2, y2, margin))
			{
				break;
			}
			x2++;
		}
		x2--;

		if (ratio != 0)
		{
			if ((y2 - y1 + 1) > (x2 - x1 + 1) * ratio)
			{
				y2 = (x2 - x1 + 1) * ratio + y1 - 1;
			}
		}
	}

	//行，列 面积最大选择
	if (num_judge == 1)
	{
		if ((tempX2 - x1 + 1) * (tempY2 - y1 + 1) >= (x2 - x1 + 1) * (y2 - y1 + 1))
		{
			x2 = tempX2;
			y2 = tempY2;
		}
	}
	if (num_judge == 2)
	{
		if ((tempX2 - x1 + 1) * (tempY2 - y1 + 1) > (x2 - x1 + 1) * (y2 - y1 + 1))
		{
			x2 = tempX2;
			y2 = tempY2;
		}
	}
	if (num_judge == 6)
	{
		while (x2 < img->width && y2 < img->height && (markMatrix->data.ptr + y1 * markMatrix->step)[x2] == 0 && (markMatrix->data.ptr + y2 * markMatrix->step)[x1] == 0)
		{
			if (!IsSameBlock(img, x1, y1, x2, y2, margin))
				break;

			x2++;
			y2++;

		}

		x2--;
		y2--;


		int maxX = ratio == 0 ? INT_MAX : (y2 - y1 + 1) * ratio;

		while (x2 < img->width && (x2 - x1 < maxX) && (markMatrix->data.ptr + y1 * markMatrix->step)[x2] == 0)
		{
			if (!IsSameBlock(img, x1, y1, x2, y2, margin))
			{
				break;
			}
			x2++;
		}
		x2--;


		Location loc(x1, y1, x2, y2);

		return loc;
	}
	if (num_judge == 7)
	{
		while (x2 < img->width && y2 < img->height && (markMatrix->data.ptr + y1 * markMatrix->step)[x2] == 0 && (markMatrix->data.ptr + y2 * markMatrix->step)[x1] == 0)
		{
			if (!IsSameBlock(img, x1, y1, x2, y2, margin))
				break;

			x2++;
			y2++;

		}

		x2--;
		y2--;


		int maxY = ratio == 0 ? INT_MAX : (x2 - x1 + 1) * ratio;

		while (y2 < img->width && (y2 - y1 < maxY) && (markMatrix->data.ptr + y2 * markMatrix->step)[x1] == 0)
		{
			if (!IsSameBlock(img, x1, y1, x2, y2, margin))
			{
				break;
			}
			y2++;
		}
		y2--;


		Location loc(x1, y1, x2, y2);

		return loc;
	}

	Location loc(x1, y1, x2, y2);

	return loc;
}



void IBR::SetMarkMatrix(CvMat* markMatrix, int x1, int y1, int x2, int y2, int value)
{
	for (int y = y1; y <= y2; y++)
	{
		for (int x = x1; x <= x2; x++)
		{
			(markMatrix->data.ptr + markMatrix->step * y)[x] = value;
		}
	}
}

bool IBR::IsSameBlock(const IplImage* img, int x1, int y1, int x2, int y2, int margin)
{
	uchar* gy1 = (uchar*)img->imageData + y1 * img->widthStep;
	uchar* g1 = gy1 + x1;
	uchar* g2 = gy1 + x2;
	uchar* gy2 = (uchar*)img->imageData + y2 * img->widthStep;
	uchar* g3 = gy2 + x1;
	uchar* g4 = gy2 + x2;
	bool returnValue = true;

	if (x1 == x2 && y1 == y2)
	{
		returnValue = true;
	}
	else if (y1 == y2)
	{
		for (int x = x1 + 1; x < x2; x++)
		{
			double i2 = (double)(x - x1) / (double)(x2 - x1);
			double g = *g1 + (*g4 - *g1) * i2;
			uchar* gValue = gy1 + x;
			if (abs(*gValue - g) > margin)
			{
				returnValue = false;
				break;
			}

		}
	}
	else if (x1 == x2)
	{
		for (int y = y1 + 1; y < y2; y++)
		{
			double i1 = (double)(y - y1) / (double)(y2 - y1);
			double g = *g1 + (*g4 - *g1) * i1;
			uchar* gy = (uchar*)img->imageData + y * img->widthStep;
			uchar* gValue = gy + x1;
			if (abs(*gValue - g) > margin)
			{
				returnValue = false;
				break;
			}
		}
	}
	else
	{
		for (int x = x1; x <= x2; x++)
		{
			for (int y = y1; y <= y2; y++)
			{
				double i1 = (double)(y - y1) / (double)(y2 - y1);
				double i2 = (double)(x - x1) / (double)(x2 - x1);
				double g5 = *g1 + (*g2 - *g1) * i2;
				double g6 = *g3 + (*g4 - *g3) * i2;
				double g = g5 + (g6 - g5) * i1;
				uchar* gy = (uchar*)img->imageData + y * img->widthStep;
				uchar* gValue = gy + x;
				if (abs(*gValue - g) > margin)
				{
					returnValue = false;
					break;
				}
			}
		}
	}

	return returnValue;
}



void IBR::StartNamCut(const IplImage* img, CvMat* markMatrix, CvMat* R, map<unsigned int, ColorNode>& color_list, map<unsigned int, Location>& block_list, int height, int width, double margin, int& num)
{

	unsigned int* block = new unsigned int[height * width];
	for (int i = 0; i < height * width; i++)
	{
		block[i] = 0;
	}

	for (int y = 0; y < height; y++)
	{
		int x = 0;
		uchar* p_y = (uchar*)(img->imageData + y * img->widthStep);

		while (x < width)
		{
			int start_pos = x;
			while (x < width)
			{
				x++;
				if (!IsSameBlock(img, start_pos, y, x, y, margin))
				{
					break;
				}
			}

			x--;
			if (0 == y)
			{
				Location loc;
				ColorNode color;

				if (start_pos == x)
				{
					(R->data.ptr + R->step * y)[x] = 3;
					block[y * width + x] = ++num;
					loc.x1 = loc.x2 = x;
					loc.y1 = loc.y2 = y;

					color.kind = 3;
					color.g1 = p_y[x];
					color.g2 = color.g3 = color.g4 = -1;
				}
				else
				{
					(R->data.ptr + R->step * y)[start_pos] = 1;
					(R->data.ptr + R->step * y)[x] = 2;
					++num;

					for (int j = start_pos; j <= x; j++)
					{
						block[y * width + j] = num;
					}

					loc.x1 = start_pos;
					loc.x2 = x;
					loc.y1 = loc.y2 = y;

					color.kind = 1;
					color.g1 = p_y[start_pos];
					color.g2 = p_y[x];;
					color.g3 = color.g4 = -1;
				}

				block_list.insert(make_pair(num, loc));
				color_list.insert(make_pair(num, color));
			}
			else
			{
				int key;
				map<unsigned int, Location>::iterator it;
				map<unsigned int, ColorNode>::iterator c_it;
				key = block[(y - 1) * width + start_pos];
				it = block_list.find(key);
				c_it = color_list.find(key);

				if (it->second.x1 == start_pos && it->second.x2 == x && IsSameBlock(img, start_pos, it->second.y1, x, y, margin))
				{
					it->second.x2 = x;
					it->second.y2 = y;


					if (3 == (R->data.ptr + R->step * (y - 1))[x])
					{
						(R->data.ptr + R->step * (y - 1))[x] = 1;
						c_it->second.g3 = ((uchar*)(img->imageData + y * img->widthStep))[start_pos];
						c_it->second.kind = 2;
					}
					else
					{
						(R->data.ptr + R->step * (y - 1))[x] = 0;
						c_it->second.g3 = ((uchar*)(img->imageData + y * img->widthStep))[start_pos];
						c_it->second.g4 = ((uchar*)(img->imageData + y * img->widthStep))[x];
						c_it->second.kind = 0;
					}

					(R->data.ptr + R->step * y)[x] = 2;
				}
				else
				{
					Location loc;
					ColorNode color;

					if (start_pos == x)
					{
						(R->data.ptr + R->step * y)[x] = 3;
						loc.x1 = loc.x2 = x;
						loc.y1 = loc.y2 = y;

						color.kind = 3;
						color.g1 = p_y[x];
						color.g2 = color.g3 = color.g4 = -1;
					}
					else
					{
						(R->data.ptr + R->step * y)[start_pos] = 1;
						(R->data.ptr + R->step * y)[x] = 2;

						loc.x1 = start_pos;
						loc.x2 = x;
						loc.y1 = loc.y2 = y;

						color.kind = 1;
						color.g1 = p_y[start_pos];
						color.g2 = p_y[x];;
						color.g3 = color.g4 = -1;
					}
					key = ++num;
					block_list.insert(make_pair(num, loc));
					color_list.insert(make_pair(num, color));
				}

				for (int j = start_pos; j <= x; j++)
				{
					block[y * width + j] = key;
				}
			}

			x++;
		}

	}

	delete[] block;

}


void IBR::RestoreImage(IplImage* img, CvMat* markMatrix, CvMat* R, map<unsigned int, ColorNode>& color_list, int height, int width)
{
	int x1(0), x2(0), y1(0), y2(0);

	for (int i = 1; i <= color_list.size(); i++)
	{
		map<unsigned int, ColorNode>::iterator it = color_list.find(i);
		//先查找下一个未计算的分块 起点坐标
		while (x2 < width)
		{
			if ((markMatrix->data.ptr + markMatrix->step * y2)[x2++] == 0)
				break;


			if (x2 == width)
			{
				x1 = x2 = 0;
				y1 = ++y2;
			}

		}

		x1 = --x2;

		//再查找出这个块的 (x1,y1) (x2,y2)
		if (0 == (it->second).kind)
		{
			while (x2 < width)
			{
				//x2要先走一步
				if ((R->data.ptr + R->step * y2)[++x2] != 0 || (markMatrix->data.ptr + markMatrix->step * y2)[x2] == 1)
					break;
			}

			x2--;

			while (y2 < height)
			{
				if ((R->data.ptr + R->step * y2)[x2] == 2 || (markMatrix->data.ptr + markMatrix->step * y2)[x2] == 1)
					break;
				y2++;
			}

		}
		else if (1 == (it->second).kind)
		{
			while (x2 < width)
			{
				if ((R->data.ptr + R->step * y2)[++x2] == 2)
					break;
			}
		}
		else if (2 == (it->second).kind)
		{
			while (y2 < height)
			{
				if ((R->data.ptr + R->step * y2)[x2] == 2)
					break;
				y2++;
			}
		}

		//cout<<"("<<x1<<","<<y1<<")("<<x2<<","<<y2<<")"<<endl;
		RestoreImageValue(img, it->second, x1, y1, x2, y2);

		//Mark

		SetMarkMatrix(markMatrix, x1, y1, x2, y2, 1);


		x1 = x2;
		y2 = y1;

	}

}

void IBR::RestoreImageValue(IplImage* img, ColorNode node, int x1, int y1, int x2, int y2)
{
	uchar* upPtr = (uchar*)(img->imageData + img->widthStep * y1);
	uchar* downPtr = (uchar*)(img->imageData + img->widthStep * y2);

	if (3 == node.kind)
	{
		upPtr[x1] = node.g1;
	}
	else if (1 == node.kind)
	{
		upPtr[x1] = node.g1;
		upPtr[x2] = node.g2;
		for (int x = x1 + 1; x < x2; x++)
		{
			double i2 = (double)(x - x1) / (double)(x2 - x1);
			upPtr[x] = (int)(node.g1 + (node.g2 - node.g1) * i2);

		}
	}
	else if (2 == node.kind)
	{
		upPtr[x1] = node.g1;
		downPtr[x1] = node.g2;
		for (int y = y1 + 1; y < y2; y++)
		{
			double i1 = (double)(y - y1) / (double)(y2 - y1);
			(img->imageData + img->widthStep * y)[x1] = (int)(node.g1 + (node.g2 - node.g1) * i1);

		}
	}
	else
	{
		for (int x = x1; x <= x2; x++)
		{
			for (int y = y1; y <= y2; y++)
			{
				double i1 = (double)(y - y1) / (double)(y2 - y1);
				double i2 = (double)(x - x1) / (double)(x2 - x1);
				double g5 = node.g1 + (node.g2 - node.g1) * i2;
				double g6 = node.g3 + (node.g4 - node.g3) * i2;
				(img->imageData + img->widthStep * y)[x] = (int)(g5 + (g6 - g5) * i1);

			}
		}

		upPtr[x1] = node.g1;
		upPtr[x2] = node.g2;
		downPtr[x1] = node.g3;
		downPtr[x2] = node.g4;
	}
}

double IBR::PSNR(IplImage* img, IplImage* newImg)
{
	int yn = img->height;
	int xn = img->width;
	double sum = 0.0;
	for (int i = 0; i < yn; i++) {
		uchar* imgptr = (uchar*)img->imageData + i * img->widthStep;
		uchar* newImgptr = (uchar*)newImg->imageData + i * newImg->widthStep;
		for (int j = 0; j < xn; j++) {
			sum += pow((double)(imgptr[j] - newImgptr[j]), 2);
		}
	}
	double temp = 255.0 * 255 * xn * yn / sum;
	double PSNRValue = 10 * log(temp) / log(10.0);
	return PSNRValue;
}


IBR::Location IBR:: FindSameBlock(const IplImage* img, CvMat* markMatrix, CvMat* R, int x1, int y1, double margin, double ratio)
{


	int x2(x1), y2(y1);


	while (x2 < img->width && (markMatrix->data.ptr + y1 * markMatrix->step)[x2] == 0)
	{
		if (!IsSameBlock(img, x1, y1, x2, y2, margin))
		{
			break;
		}
		x2++;
	}
	x2--;

	int maxY = ratio == 0 ? INT_MAX : (x2 - x1 + 1) * ratio;

	while (y2 < img->height && ((y2 - y1) < maxY))
	{
		if (!IsSameBlock(img, x1, y1, x2, y2, margin))
		{
			break;
		}

		y2 = y2 + 1;
	}

	y2--;


	if (ratio != 0)
	{
		if ((x2 - x1 + 1) > (y2 - y1 + 1) * ratio)
		{
			x2 = (y2 - y1 + 1) * ratio + x1 - 1;
		}
	}

	Location loc(x1, y1, x2, y2);

	return loc;
}



void IBR::EnCode(CvMat* R, int height, int width, vector<char>& Q)
{
	int c = 0;
	int offsetValue = 0;
	int count = 0;

	for (int i = 0; i < height; i++)
	{
		c = 0;
		offsetValue = 0;
		count = 0;
		//Q.push_back(' ');
		for (int j = 0; j < width; j++)
		{
			int value = (R->data.ptr + R->step * i)[j];
			if (0 == value)
			{
				if (j == width - 1)
				{
					Q.push_back('0');
				}
			}
			else
			{
				if (1 == value)
				{
					Q.push_back('1');
					Q.push_back('1');
				}
				else if (2 == value)
				{
					Q.push_back('1');
					Q.push_back('0');
					Q.push_back('1');
				}
				else if (3 == value)
				{
					Q.push_back('1');
					Q.push_back('0');
					Q.push_back('0');
				}

				//位置从1开始 不是从0开始 所以多减去1
				int b = ceil(log((double)(width - c - count)) / log(2.0f));
				if (0 == count)
				{
					offsetValue = j;
				}
				else
				{
					offsetValue = j - c - 1;
				}

				b = (b == 0) ? 1 : b;
				count = 1;
				c = j;

				for (int v = b - 1; v >= 0; v--)
				{
					if ((offsetValue & (1 << v)) == 0)
					{
						Q.push_back('0');
					}
					else
					{
						Q.push_back('1');
					}
				}
			}
		}
	}
}

void IBR::Decode(CvMat* R, int height, int width, const vector<char>& Q)
{
	int n = 0;
	int count = 0;
	//上个非零元素的位置
	int c = 0;
	int row = 0;
	int num = 0;
	for (int pos = 0; pos < Q.size(); pos++)
	{
		if ('1' == Q[pos])
		{
			pos++;
			if ('1' == Q[pos])
			{
				n = 1;

			}
			else
			{
				pos++;
				if ('1' == Q[pos])
				{
					n = 2;
				}
				else
				{
					n = 3;
				}
			}
		}
		else
		{
			row++;
			count = 0;
			c = 0;
			num = 0;
			continue;
			//如果是0
			//if(c == 0)
			//{
			//	row++;
			//	continue;
			//}
			//else
			//{
			//	row++;
			//	count = 0;
			//	c = 0;
			//	num = 0;
			//	continue;

			//}
		}

		int b = ceil(log((double)(width - c - count)) / log(2.0f));
		b = b == 0 ? 1 : b;
		count = 1;

		int value = 0;
		for (int i = b - 1; i >= 0; i--)
		{
			pos++;
			if ('1' == Q[pos])
			{
				value += 1 << i;
			}
		}

		if (0 == num)
		{
			c = c + value;
		}
		else
		{
			c = c + value + 1;
		}

		num++;


		(R->data.ptr + row * R->step)[c] = n;

		if (c == (width - 1))
		{
			row++;
			count = 0;
			c = 0;
			num = 0;
		}
	}

}


double IBR::BPP(map<unsigned int, ColorNode>& color_list, int xn, int yn, vector<char>& Q)
{
	int a1 = 0, a2 = 0, a3 = 0;
	for (int i = 0; i < color_list.size(); i++) {
		if (color_list[i].g1 != -1 && color_list[i].g4 != -1) a1++;
		else if (color_list[i].g2 == -1 && color_list[i].g3 == -1) a3++;
		else  a2++;
	}
	double BPPValue = (Q.size() + 16 * a1 + 8 * a2 + 4.0 * a3) / (xn * yn);
	return BPPValue;
}