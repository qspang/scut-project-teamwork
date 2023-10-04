#pragma once

#include <QtWidgets/QMainWindow>
//#include "ui_fanal_opencv_pro.h"
#include"ui_MainWindow.h"

class fanal_opencv_pro : public QMainWindow
{
    Q_OBJECT

public:
    fanal_opencv_pro(QWidget *parent = Q_NULLPTR);


   Ui::MainWindow ui;
};
