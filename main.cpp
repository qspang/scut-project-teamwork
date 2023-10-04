#include "fanal_opencv_pro.h"
#include <QtWidgets/QApplication>
#include"base_pic_process.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    fanal_opencv_pro w;
	int input_num = 1;
	All_work test(input_num);
	//test.work(w,a);


	test.num_judge = 1;
	IplImage* img = cvLoadImage("C:\\Users\\pqs-xsz-xia\\Desktop\\gray_images\\lenazyp256.bmp", 0);
	int height = img->height;
	int width = img->width;
	CvMat* markMatrix = cvCreateMat(height, width, CV_8UC1);
	cvSetZero(markMatrix);
	CvMat* R = cvCreateMat(height, width, CV_8UC1);
	cvSetZero(R);
	int num = 0;
	vector<MatrixRaster::ColorNode> colorList;
	vector<MatrixRaster::Location> locList;
	double margin = 20;
	double ratio = 0;
	time_t begin, end;
	begin = clock();
	test.StartNamCut(img, markMatrix, R, colorList, locList, height, width, margin, num, ratio, test.num_judge);
	vector<char> Q;
	test.EnCode(R, height, width, Q);
	end = clock();
	CvMat* T = cvCreateMat(height, width, CV_8UC1);
	cvSetZero(T);
	begin = clock();
	test.Decode(T, height, width, Q);
	IplImage* newImg = cvCreateImage(cvSize(width, height), 8, 1);
	test.RestoreImage(newImg, markMatrix, T, colorList, height, width);
	end = clock();
	double BPPValue = test.BPP(colorList, width, height, Q);
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

	/*QApplication a(argc, argv);
	Widgets w;*/
	QLabel* pTag1 = new QLabel(&w);
	Mat Img1 = cvarrToMat(img);
	QImage image1 = test.QCVMat2QImage(Img1);
	pTag1->setPixmap(QPixmap::fromImage(image1));
	pTag1->setScaledContents(1);
	//pTag1->setGeometry(100, 470, image1.width(), image1.height());
	pTag1->setGeometry(930, 500, 240, 240);
	QLabel* pTag2 = new QLabel(&w);
	Mat Img2 = cvarrToMat(newImg);
	QImage image2 = test.QCVMat2QImage(Img2);
	pTag2->setPixmap(QPixmap::fromImage(image2));
	//pTag2->setGeometry(700, 470, image2.width(), image2.height());
	pTag2->setGeometry(640, 500, 240, 240);
	pTag2->setScaledContents(1);
	QLabel* pTag3 = new QLabel(&w);
	Mat Img3 = cvarrToMat(sketch);
	QImage image3 = test.QCVMat2QImage(Img3);
	pTag3->setPixmap(QPixmap::fromImage(image3));
	//pTag3->setGeometry(1300, 470, image3.width(), image3.height());
	pTag3->setGeometry(190, 390, 360, 360);
	pTag3->setScaledContents(1);
	QString tem;

	tem = QString::number(end - begin);
	w.ui.text_1_show->setText(tem);
	tem = QString::number(num);
	w.ui.text_2_show->setText(tem);
	tem = QString::number(test.PSNR(img, newImg));
	w.ui.text_3_show->setText(tem);
	tem = QString::number(BPPValue);
	w.ui.text_4_show->setText(tem);
	tem = QString::number(8.0 / BPPValue);
	w.ui.text_5_show->setText(tem);
	w.ui.spinBox3->setRange(0, 1100);
	w.ui.spinBox3->setValue(256);
	w.ui.horizontalSlider_3->setRange(0, 1100);
	w.ui.horizontalSlider_3->setValue(256);
	w.ui.spinBox1->setValue(0);
	w.ui.spinBox2->setValue(0);
	w.ui.spinBox4->setValue(8);
	w.ui.spinBox4->setRange(0, 50);
	w.ui.horizontalSlider_4->setRange(0, 50);
	w.ui.horizontalSlider_4->setValue(8);
	int pixel_test, step_test;

	QObject::connect(w.ui.vertical, &QPushButton::clicked, [&]() {
		test.num_judge = 1;
		test.fun(margin, ratio, pTag1, pTag2, pTag3, w, begin, end, num, img, newImg, BPPValue, sketch, test.num_judge);
		test.display(w, begin, end, num, img, newImg, BPPValue);

	});

	QObject::connect(w.ui.horizontal, &QPushButton::clicked, [&]() {
		test.num_judge = 2;
		test.fun(margin, ratio, pTag1, pTag2, pTag3, w, begin, end, num, img, newImg, BPPValue, sketch, test.num_judge);
		test.display(w, begin, end, num, img, newImg, BPPValue);

	});
	QObject::connect(w.ui.diagonal, &QPushButton::clicked, [&]() {
		test.num_judge = 3;
		test.fun(margin, ratio, pTag1, pTag2, pTag3, w, begin, end, num, img, newImg, BPPValue, sketch, test.num_judge);
		test.display(w, begin, end, num, img, newImg, BPPValue);

	});
	QObject::connect(w.ui.triangle, &QPushButton::clicked, [&]() {
		test.num_judge = 4;
		pixel_test = w.ui.spinBox3->text().toDouble();
		step_test = w.ui.spinBox4->text().toDouble();
		test.fun_plus(margin, ratio, pTag1, pTag2, pTag3, w, begin, end, num, img, newImg, BPPValue, sketch, test.num_judge, pixel_test,step_test);

	});
	QObject::connect(w.ui.diagonal_a, &QPushButton::clicked, [&]() {
		test.num_judge = 6;
		test.fun(margin, ratio, pTag1, pTag2, pTag3, w, begin, end, num, img, newImg, BPPValue, sketch, test.num_judge);
		test.display(w, begin, end, num, img, newImg, BPPValue);

		});
	QObject::connect(w.ui.diagonal_b, &QPushButton::clicked, [&]() {
		test.num_judge = 7;
		test.fun(margin, ratio, pTag1, pTag2, pTag3, w, begin, end, num, img, newImg, BPPValue, sketch, test.num_judge);
		test.display(w, begin, end, num, img, newImg, BPPValue);

		});
	QObject::connect(w.ui.IBR, &QPushButton::clicked, [&]() {
		test.num_judge = 8;
		test.fun(margin, ratio, pTag1, pTag2, pTag3, w, begin, end, num, img, newImg, BPPValue, sketch, test.num_judge);
		test.display(w, begin, end, num, img, newImg, BPPValue);

		});
	QObject::connect(w.ui.button_finish_enter, &QPushButton::clicked, [&]() {
		margin = w.ui.spinBox1->text().toDouble();
		ratio = w.ui.spinBox2->text().toDouble();
		pixel_test = w.ui.spinBox3->text().toDouble();
		step_test = w.ui.spinBox4->text().toDouble();
		test.fun_plus(margin, ratio, pTag1, pTag2, pTag3, w, begin, end, num, img, newImg, BPPValue, sketch, test.num_judge,pixel_test,step_test);
		if (test.num_judge != 4)
		{
			test.display(w, begin, end, num, img, newImg, BPPValue);
		}

	});
	QObject::connect(w.ui.button_save, &QPushButton::clicked, [&]() {

		if (test.num_judge == 4)
		{
			test.num_judge = 5;
			test.fun_plus(margin, ratio, pTag1, pTag2, pTag3, w, begin, end, num, img, newImg, BPPValue, sketch, test.num_judge,pixel_test,step_test);
			test.num_judge = 4;
		}
		else
		{
			cvSaveImage("C:\\Users\\pqs-xsz-xia\\Desktop\\file\\1.bmp", sketch);
		}
	});
	QIcon icon_2;
	icon_2.addFile("C:\\Users\\pqs-xsz-xia\\Desktop\\gray_images\\saveimage.png");
	w.ui.button_save->setFlat(true);
	w.ui.button_save->setIcon(icon_2);
	w.ui.button_save->setIconSize(QSize(30, 30));


	QPushButton* btn_4 = new QPushButton("", &w);
	btn_4->setStyleSheet("background-color:rgba(0,0,0,0);border-color:rgba(0,0,0,255);color:rgba(0,0,0,0);border-style:none;border-width:21px;border-radius:0px");
	QPixmap btn_4_pixmap;
	btn_4_pixmap = QPixmap("C:\\Users\\pqs-xsz-xia\\Desktop\\gray_images\\SCUT_photo.jpg");
	btn_4_pixmap = btn_4_pixmap.scaled(btn_4->width(), btn_4->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	btn_4->setIconSize(QSize(40, 40));
	btn_4->move(560, 430);
	QIcon icon_1;
	icon_1.addFile("C:\\Users\\pqs-xsz-xia\\Desktop\\gray_images\\read_photo.jpg");
	btn_4->setFlat(true);
	btn_4->setIcon(icon_1);
	QLabel* pTag4 = new QLabel(&w);
	QString OpenFile, OpenFilePath;
	QImage image4;
	QObject::connect(btn_4, &QPushButton::clicked, [&]() {
		OpenFile = QFileDialog::getOpenFileName(btn_4,
			"Please choose an image file",
			"",
			"Image Files(*.jpg *.png *.bmp *.pgm *.pbm);;All(*.*)");
		if (OpenFile != "")
		{
			if (image4.load(OpenFile))
			{
				img = test.QImageToIplImageGray(&image4);
				test.fun(margin, ratio, pTag1, pTag2, pTag3, w, begin, end, num, img, newImg, BPPValue, sketch, test.num_judge);
			}
		}
	});


	QObject::connect(w.ui.spinBox1, SIGNAL(valueChanged(int)),
		w.ui.horizontalSlider, SLOT(setValue(int)));
	QObject::connect(w.ui.horizontalSlider, SIGNAL(valueChanged(int)),
		w.ui.spinBox1, SLOT(setValue(int)));
	QObject::connect(w.ui.spinBox2, SIGNAL(valueChanged(int)),
		w.ui.horizontalSlider_2, SLOT(setValue(int)));
	QObject::connect(w.ui.horizontalSlider_2, SIGNAL(valueChanged(int)),
		w.ui.spinBox2, SLOT(setValue(int)));
	QObject::connect(w.ui.spinBox3, SIGNAL(valueChanged(int)),
		w.ui.horizontalSlider_3, SLOT(setValue(int)));
	QObject::connect(w.ui.horizontalSlider_3, SIGNAL(valueChanged(int)),
		w.ui.spinBox3, SLOT(setValue(int)));
	QObject::connect(w.ui.spinBox4, SIGNAL(valueChanged(int)),
		w.ui.horizontalSlider_4, SLOT(setValue(int)));
	QObject::connect(w.ui.horizontalSlider_4, SIGNAL(valueChanged(int)),
		w.ui.spinBox4, SLOT(setValue(int)));


	//w.resize(1980, 1080);
	w.resize(1230, 800);


    w.show();
    return a.exec();
}

