import http.server
import os
import socketserver
import threading
import sys
from datetime import datetime, timedelta

import numpy as np
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QPushButton
from PyQt5.QtWebEngineWidgets import QWebEngineView, QWebEngineSettings
from PyQt5.QtCore import QUrl, QDir
from PyQt5 import uic, Qt
import requests
import ctypes

from matplotlib import pyplot as plt, cm

import initfile

# 设置函数参数和返回类型
initfile.mydll.READ_FILE.restype = None
initfile.mydll.TEST_PRINT.restype = None
initfile.mydll.F1_FUNCTION.argtypes = [ctypes.c_int, ctypes.POINTER(ctypes.c_double)]

initfile.mydll.F3_FUNCTION.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double,
                                       ctypes.c_char_p, ctypes.c_char_p]
initfile.mydll.F3_FUNCTION.restype = ctypes.c_int
initfile.mydll.F4_FUNCTION.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_double, ctypes.POINTER(ctypes.c_int),
                                       ctypes.POINTER(ctypes.c_double)]
initfile.mydll.F4_FUNCTION.restype = None
# 定义函数签名
initfile.mydll.F5_FUNCTION.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_double, ctypes.c_double,
                                       ctypes.c_double, ctypes.c_double,
                                       ctypes.c_double, ctypes.c_double, ctypes.c_double,
                                       ctypes.c_double]
initfile.mydll.F5_FUNCTION.restype = ctypes.c_double

initfile.mydll.F6_FUNCTION.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_double, ctypes.c_double,
                                       ctypes.c_double, ctypes.c_double]
initfile.mydll.F6_FUNCTION.restype = ctypes.c_double
# 调用函数示例
initfile.mydll.F7_FUNCTION.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.POINTER(ctypes.c_int)]
initfile.mydll.F8_FUNCTION.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double,
                                       ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_int,
                                       ctypes.POINTER(ctypes.c_int)]
initfile.mydll.F9_FUNCTION.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_double, ctypes.c_double,
                                       ctypes.c_double,
                                       ctypes.c_double,
                                       ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double,
                                       ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double)]
initfile.mydll.get_roads_by_id.argtypes = [ctypes.c_int, np.ctypeslib.ndpointer(dtype=np.double, shape=(100, 2))]
initfile.mydll.get_roads_by_id.restype = None

array_size = 2000
array = (ctypes.c_int * array_size)()
arr = np.zeros((2000, 4), dtype=np.double)

initfile.mydll.READ_FILE()  # 调用READ_FILE函数


def F5_get_period_data(start, end, time_step, a_l_lon, a_l_lat, a_r_lon, a_r_lat, b_l_lon, b_l_lat, b_r_lon, b_r_lat):
    # Convert start and end to datetime objects
    start_time = datetime.strptime(start, "%Y%m%d%H%M%S")
    end_time = datetime.strptime(end, "%Y%m%d%H%M%S")

    results = []
    last_time = start_time
    current_time = start_time
    current_time += timedelta(seconds=time_step)

    while current_time <= end_time:
        last_time_str = last_time.strftime("%Y%m%d%H%M%S")
        current_time_str = current_time.strftime("%Y%m%d%H%M%S")
        res = initfile.mydll.F5_FUNCTION(last_time_str.encode('utf-8'), current_time_str.encode('utf-8'),
                                         a_l_lon, a_l_lat, a_r_lon, a_r_lat, b_l_lon, b_l_lat, b_r_lon, b_r_lat)
        results.append(res)

        # Increment current_time by time_step
        last_time += timedelta(seconds=time_step)
        current_time += timedelta(seconds=time_step)

    return results


def F6_get_period_data(start, end, time_step, a_l_lon, a_l_lat, a_r_lon, a_r_lat):
    start_time = datetime.strptime(start, "%Y%m%d%H%M%S")
    end_time = datetime.strptime(end, "%Y%m%d%H%M%S")

    results = []
    last_time = start_time
    current_time = start_time
    current_time += timedelta(seconds=time_step)

    while current_time <= end_time:
        last_time_str = last_time.strftime("%Y%m%d%H%M%S")
        current_time_str = current_time.strftime("%Y%m%d%H%M%S")
        res = initfile.mydll.F6_FUNCTION(last_time_str.encode('utf-8'), current_time_str.encode('utf-8'),
                                         a_l_lon, a_l_lat, a_r_lon, a_r_lat)
        results.append(res)

        # Increment current_time by time_step
        last_time += timedelta(seconds=time_step)
        current_time += timedelta(seconds=time_step)

    return results


time_step = 60 * 30


class Widget(QWidget):
    def __init__(self, parent=None):
        super(Widget, self).__init__(parent)
        uic.loadUi('widget.ui', self)
        self.setFixedSize(1450, 930)
        self.webaddr.setText("                                              数据结构大作业---出租车轨迹分析及前端页面")
        self.m_webView = QWebEngineView(self)
        self.m_webView1 = QWebEngineView(self)
        sUrl = "http://localhost:1666/map.html"
        self.m_webView.load(QUrl(sUrl))
        self.lineEdit_1.setText("116.65640")
        self.lineEdit_5.setText("40.00151")
        self.lineEdit_2.setText("116.93210")
        self.lineEdit_6.setText("39.93210")
        self.lineEdit_3.setText("116.15640")
        self.lineEdit_7.setText("40.83210")
        self.lineEdit_4.setText("117.65540")
        self.lineEdit_8.setText("39.93110")
        self.lineEdit_9.setText("20080203011934")
        self.lineEdit_10.setText("20080203230546")
        self.verticalLayout.addWidget(self.m_webView)
        self.verticalLayout_3.addWidget(self.m_webView1)
        self.btn1.clicked.connect(self.btn1_clicked)
        self.btn2.clicked.connect(self.btn2_clicked)
        self.btn3.clicked.connect(self.btn3_clicked)
        self.btn4.clicked.connect(self.btn4_clicked)
        self.btn5.clicked.connect(self.btn5_clicked)
        self.btn6.clicked.connect(self.btn6_clicked)
        self.btn7.clicked.connect(self.btn7_clicked)
        self.btn8.clicked.connect(self.btn8_clicked)
        self.btn0.clicked.connect(self.btn0_clicked)

    def getdata(self):
        text = self.lineEdit_1.text()
        A_L_LO = float(text)
        text = self.lineEdit_5.text()
        A_L_LA = float(text)
        text = self.lineEdit_2.text()
        A_R_LO = float(text)
        text = self.lineEdit_6.text()
        A_R_LA = float(text)
        text = self.lineEdit_3.text()
        B_L_LO = float(text)
        text = self.lineEdit_7.text()
        B_L_LA = float(text)
        text = self.lineEdit_4.text()
        B_R_LO = float(text)
        text = self.lineEdit_8.text()
        B_R_LA = float(text)
        text = self.lineEdit_9.text()
        TIME_L = text
        text = self.lineEdit_10.text()
        TIME_R = text
        return A_L_LO, A_L_LA, A_R_LO, A_R_LA, B_L_LO, B_L_LA, B_R_LO, B_R_LA, TIME_L, TIME_R

    # F1
    def btn1_clicked(self):
        A_L_LO, A_L_LA, A_R_LO, A_R_LA, B_L_LO, B_L_LA, B_R_LO, B_R_LA, TIME_L, TIME_R = self.getdata()
        print(A_L_LO, A_L_LA, A_R_LO, A_R_LA, B_L_LO, B_L_LA, B_R_LO, B_R_LA, TIME_L, TIME_R)
        with open('t1.html', 'r', encoding='utf-8') as f:
            html_content = f.read()
        with open('f1.html', 'w', encoding='utf-8') as k:
            # 将原有HTML文件中的内容写入新HTML文件中
            k.write(html_content)
            k.write('<script type="text/javascript">\n')
            print("2222222222")
            for i in range(1):
                r = np.zeros((200000, 2), dtype=np.double)
                initfile.mydll.F1_FUNCTION(i, r.ctypes.data_as(ctypes.POINTER(ctypes.c_double)))
                k.write(f'var polyline{i} = new BMapGL.Polyline([\n')
                for j in range(200000):
                    if r[j][0] == 0:
                        break
                    p1, p2 = initfile.wgs2bd(r[j][1], r[j][0])
                    k.write(f'new BMapGL.Point({p1},{p2}),\n')
                k.write('], {strokeColor:"blue", strokeWeight:2, strokeOpacity:0.5});\n ')
            k.write('function add_overlay() {\n')
            for i in range(1):
                k.write(f' map.addOverlay(polyline{i}); \n')
            k.write(' }add_overlay();\n')
            k.write('</script>\n')
            print("写入完成")
            sUrl = 'http://localhost:1666/f1.html'
            self.m_webView.load(QUrl(sUrl))

    # F3
    def btn2_clicked(self):
        A_L_LO, A_L_LA, A_R_LO, A_R_LA, B_L_LO, B_L_LA, B_R_LO, B_R_LA, TIME_L, TIME_R = self.getdata()
        result = initfile.mydll.F3_FUNCTION(A_L_LO, A_L_LA, A_R_LO, A_R_LA, TIME_L.encode('utf-8'),
                                            TIME_R.encode('utf-8'))  # 调用F3_FUNCTION函数
        with open('t1.html', 'r', encoding='utf-8') as f:
            html_content = f.read()
        with open('new.html', 'w', encoding='utf-8') as k:
            # 将原有HTML文件中的内容写入新HTML文件中
            k.write(html_content)
            # 在新HTML文件中添加新的JavaScript代码
            k.write('<script type="text/javascript">\n')
            p1, p2 = initfile.wgs2bd(A_L_LA, A_L_LO)
            p3, p4 = initfile.wgs2bd(A_R_LA, A_R_LO)
            k.write(f'var rectangle = new BMapGL.Polygon([\n')
            k.write(f'new BMapGL.Point({p1},{p2}),\n')
            k.write(f'new BMapGL.Point({p1},{p4}),\n')
            k.write(f'new BMapGL.Point({p3},{p4}),\n')
            k.write(f'new BMapGL.Point({p3},{p2}),\n')
            k.write('], {strokeColor:"blue", strokeWeight:2, strokeOpacity:0.5});\n ')
            k.write(f"var opts = ")
            k.write('{')
            k.write(f' position: new BMapGL.Point({p1}, {p2}),')
            k.write('};\n')
            k.write(f"var label = new BMapGL.Label('{result}个', opts);")
            k.write(f"label.setStyle(")
            k.write('{')
            k.write(
                "color: 'red',borderRadius: '5px', borderColor: '#ccc', padding: '10px',fontSize: '16px',height: '30px',lineHeight: '30px',fontFamily: '微软雅黑'});\n")
            k.write('function add_overlay() {')
            k.write(f'map.addOverlay(rectangle);\n')
            k.write(f'map.addOverlay(label);\n')
            k.write('} add_overlay();\n')
            k.write('</script>\n')
        print("写入完成")
        print(result)
        self.textEdit.setText(str(result))
        sUrl = 'http://localhost:1666/new.html'
        self.m_webView.load(QUrl(sUrl))

    # F4
    def btn3_clicked(self):
        A_L_LO, A_L_LA, A_R_LO, A_R_LA, B_L_LO, B_L_LA, B_R_LO, B_R_LA, TIME_L, TIME_R = self.getdata()
        print(A_L_LO, A_L_LA, A_R_LO, A_R_LA, B_L_LO, B_L_LA, B_R_LO, B_R_LA, TIME_L, TIME_R)
        initfile.mydll.F4_FUNCTION(TIME_L.encode('utf-8'), TIME_R.encode('utf-8'), 20000.0,
                                   array, arr.ctypes.data_as(ctypes.POINTER(ctypes.c_double)))  # 调用F3_FUNCTION函数
        length = 0
        t = f'{arr[0][0]} {arr[0][1]}--{arr[0][2]} {arr[0][3]}: {array[0]} \n'
        for i in range(1, len(array)):
            if array[i] == -1:
                break
            t += f'{arr[i][0]} {arr[i][1]}--{arr[i][2]} {arr[i][3]}: {array[i]} \n'
        self.textEdit_2.setText(str(t))
        with open('t1.html', 'r', encoding='utf-8') as f:
            html_content = f.read()
        with open('new.html', 'w', encoding='utf-8') as k:
            # 将原有HTML文件中的内容写入新HTML文件中
            k.write(html_content)
            # 在新HTML文件中添加新的JavaScript代码
            k.write('<script type="text/javascript">\n')
            for i in range(len(array) - 1):
                if array[i] == -1:
                    break
                k.write(f'var rectangle{i} = new BMapGL.Polygon([\n')
                p1, p2 = initfile.wgs2bd(arr[i][1], arr[i][0])
                p3, p4 = initfile.wgs2bd(arr[i][3], arr[i][2])
                k.write(f'new BMapGL.Point({p1},{p2}),\n')
                k.write(f'new BMapGL.Point({p1},{p4}),\n')
                k.write(f'new BMapGL.Point({p3},{p4}),\n')
                k.write(f'new BMapGL.Point({p3},{p2}),\n')
                k.write('], {strokeColor:"blue", strokeWeight:2, strokeOpacity:0.5});\n ')
            for i in range(len(array) - 1):
                if array[i] == -1:
                    break
                p1, p2 = initfile.wgs2bd(arr[i][1], arr[i][0])
                k.write(f"var opts{i} = ")
                k.write('{')
                k.write(f' position: new BMapGL.Point({p1}, {p2}),')
                k.write('};\n')
                k.write(f"var label{i} = new BMapGL.Label('{array[i]}个', opts{i});")
                k.write(f"label{i}.setStyle(")
                k.write('{')
                k.write(
                    "color: 'blue',borderRadius: '5px', borderColor: '#ccc', padding: '10px',fontSize: '16px',height: '30px',lineHeight: '30px',fontFamily: '微软雅黑'});\n")
            k.write('function add_overlay() {\n')
            for iii in range(len(array) - 1):
                if array[iii] == -1:
                    break
                k.write(f'map.addOverlay(rectangle{iii});\n')
            for iii in range(len(array) - 1):
                if array[iii] == -1:
                    break
                k.write(f'map.addOverlay(label{iii});\n')
            k.write('} \nadd_overlay();\n')
            k.write('</script>\n')
        print("写入完成")

        t1 = list(TIME_L)
        t2 = list(TIME_R)
        L2 = int(t2[7]) - int(t1[7])
        length = 0
        for i in range(len(array) - 1):
            if array[i] == -1:
                break
            length = length + 1
        z1 = np.zeros((100, 7), dtype=np.double)
        t3 = TIME_L
        t2[7] = t1[7]
        t4 = ''.join(t2)
        for i in range(7):

            initfile.mydll.F4_FUNCTION(t3.encode('utf-8'), t4.encode('utf-8'), 20000.0,
                                       array, arr.ctypes.data_as(ctypes.POINTER(ctypes.c_double)))  # 调用F3_FUNCTION函数
            for j in range(100):
                if array[j] == -1:
                    break
                z1[j][i] = array[j]
            t3 = list(t3)
            t4 = list(t4)
            t3[7] = f'{int(int(t3[7]) + 1)}'
            t4[7] = f'{int(int(t4[7]) + 1)}'
            t3 = ''.join(t3)
            t4 = ''.join(t4)
        x = np.repeat([20080202, 20080203, 20080204, 20080205, 20080206, 20080207, 20080208], 100)
        y = np.tile(np.arange(1, 101), 7)
        z1 = z1.flatten()
        fig = plt.figure()
        ax = fig.add_subplot(111, projection='3d')
        ax.bar3d(x, y, np.zeros_like(z1), 1, 1, z1, color=cm.ScalarMappable().to_rgba(x))
        plt.xlabel('time')
        plt.ylabel('brides')
        plt.title('3D histogram')
        plt.savefig('temp_image.png')
        script_dir = os.path.dirname(os.path.abspath(__file__))
        photo_path = os.path.join(script_dir, 'temp_image.png')
        photo_url = QUrl.fromLocalFile(photo_path)
        self.m_webView1.load(photo_url)
        sUrl = 'http://localhost:1666/new.html'
        self.m_webView.load(QUrl(sUrl))

    # F5
    def btn4_clicked(self):
        A_L_LO, A_L_LA, A_R_LO, A_R_LA, B_L_LO, B_L_LA, B_R_LO, B_R_LA, TIME_L, TIME_R = self.getdata()
        time_step = 60 * 30
        res = F5_get_period_data(TIME_L, TIME_R, time_step, A_L_LO, A_L_LA, A_R_LO, A_R_LA, B_L_LO, B_L_LA, B_R_LO,
                                 B_R_LA)
        start_time = initfile.convert_str_to_datatime(TIME_L)
        end_time = initfile.convert_str_to_datatime(TIME_R)
        start_datetime = datetime.strptime(start_time, "%Y-%m-%d %H:%M:%S")
        end_datatime = datetime.strptime(end_time, "%Y-%m-%d %H:%M:%S")
        # time_step = 60*60
        time_axis = [start_datetime + timedelta(seconds=i * time_step) for i in range(len(res))]
        color_map = plt.get_cmap('viridis')
        colors = color_map(np.linspace(0, 1, len(res)))
        # 绘制花哨的柱形图
        fig, ax = plt.subplots(figsize=(12, 6))
        ax.bar(time_axis, res, color=colors, alpha=0.8, edgecolor='red', linewidth=2)
        # 设置坐标轴标签和标题
        ax.set_xlabel('Time')
        ax.set_ylabel('Data')
        ax.set_title('car density - time sequential')
        # 调整x轴刻度标签角度
        plt.xticks(rotation=45)
        plt.savefig('f5.png')
        script_dir = os.path.dirname(os.path.abspath(__file__))
        photo_path = os.path.join(script_dir, 'f5.png')
        photo_url = QUrl.fromLocalFile(photo_path)
        self.m_webView1.load(photo_url)
        print(A_L_LO, A_L_LA, A_R_LO, A_R_LA, B_L_LO, B_L_LA, B_R_LO, B_R_LA, TIME_L, TIME_R)

    # F6
    def btn5_clicked(self):
        A_L_LO, A_L_LA, A_R_LO, A_R_LA, B_L_LO, B_L_LA, B_R_LO, B_R_LA, TIME_L, TIME_R = self.getdata()
        time_step = 60 * 30
        res = F6_get_period_data(TIME_L, TIME_R, time_step, A_L_LO, A_L_LA, A_R_LO, A_R_LA)
        start_time = initfile.convert_str_to_datatime(TIME_L)
        end_time = initfile.convert_str_to_datatime(TIME_R)
        start_datetime = datetime.strptime(start_time, "%Y-%m-%d %H:%M:%S")
        end_datatime = datetime.strptime(end_time, "%Y-%m-%d %H:%M:%S")
        # time_step = 60*60
        time_axis = [start_datetime + timedelta(seconds=i * time_step) for i in range(len(res))]
        color_map = plt.get_cmap('viridis')
        colors = color_map(np.linspace(0, 1, len(res)))
        # 绘制花哨的柱形图
        fig, ax = plt.subplots(figsize=(12, 6))
        ax.bar(time_axis, res, color=colors, alpha=0.8, edgecolor='red', linewidth=2)
        # 设置坐标轴标签和标题
        ax.set_xlabel('Time')
        ax.set_ylabel('Data')
        ax.set_title('car density - time sequential')
        # 调整x轴刻度标签角度
        plt.xticks(rotation=45)
        plt.savefig('f6.png')
        script_dir = os.path.dirname(os.path.abspath(__file__))
        photo_path = os.path.join(script_dir, 'f6.png')
        photo_url = QUrl.fromLocalFile(photo_path)
        self.m_webView1.load(photo_url)
        print(A_L_LO, A_L_LA, A_R_LO, A_R_LA, B_L_LO, B_L_LA, B_R_LO, B_R_LA, TIME_L, TIME_R)

    def btn6_clicked(self):
        A_L_LO, A_L_LA, A_R_LO, A_R_LA, B_L_LO, B_L_LA, B_R_LO, B_R_LA, TIME_L, TIME_R = self.getdata()
        sUrl = 'http://localhost:1666/baidu_map.html'
        self.m_webView.load(QUrl(sUrl))
        print(A_L_LO, A_L_LA, A_R_LO, A_R_LA, B_L_LO, B_L_LA, B_R_LO, B_R_LA, TIME_L, TIME_R)

    def btn7_clicked(self):
        A_L_LO, A_L_LA, A_R_LO, A_R_LA, B_L_LO, B_L_LA, B_R_LO, B_R_LA, TIME_L, TIME_R = self.getdata()
        k = 10
        x = 4000
        res_id = (ctypes.c_int * 72183)()
        initfile.mydll.F7_FUNCTION(k, x, res_id)
        with open('t1.html', 'r', encoding='utf-8') as f:
            html_content = f.read()
        with open('new.html', 'w', encoding='utf-8') as k:
            # 将原有HTML文件中的内容写入新HTML文件中
            k.write(html_content)
            k.write('<script type="text/javascript">\n')
            print("222222222222222222")
            all_roads = initfile.get_all_roads_by_id(res_id)
            i = 1
            print("333333333333")
            for road_index, road_points in all_roads.items():
                k.write(f'var polyline{i} = new BMapGL.Polyline([\n')
                i = i + 1
                for point in road_points:
                    p1, p2 = initfile.wgs2bd(point[1], point[0])
                    k.write(f'new BMapGL.Point({p1},{p2}),\n')
                k.write('], {strokeColor:"red", strokeWeight:10, strokeOpacity:0.5});\n ')
            k.write('function add_overlay() {')
            ii = 1
            for road_index, road_points in all_roads.items():
                k.write(f' map.addOverlay(polyline{ii}); \n')
                ii = ii + 1
            k.write('} add_overlay();\n')
            k.write('</script>')
            print("写入完成")
        sUrl = 'http://localhost:1666/new.html'
        self.m_webView.load(QUrl(sUrl))

    # F8
    def btn8_clicked(self):
        A_L_LO, A_L_LA, A_R_LO, A_R_LA, B_L_LO, B_L_LA, B_R_LO, B_R_LA, TIME_L, TIME_R = self.getdata()
        print(A_L_LO, A_L_LA, A_R_LO, A_R_LA, B_L_LO, B_L_LA, B_R_LO, B_R_LA, TIME_L, TIME_R)
        k = 10
        res_id = (ctypes.c_int * 72183)()  # 创建一个包含2000个整数的数组
        initfile.mydll.F8_FUNCTION(A_L_LO, A_L_LA, A_R_LO, A_R_LA, \
                                   B_L_LO, B_L_LA, B_R_LO, B_R_LA, k, res_id)
        with open('t1.html', 'r', encoding='utf-8') as f:
            html_content = f.read()
        with open('new.html', 'w', encoding='utf-8') as k:
            # 将原有HTML文件中的内容写入新HTML文件中
            k.write(html_content)
            # 在新HTML文件中添加新的JavaScript代码
            k.write('<script type="text/javascript">\n')
            all_roads = initfile.get_all_roads_by_id(res_id)
            i = 1
            for road_index, road_points in all_roads.items():
                k.write(f'var polyline{i} = new BMapGL.Polyline([')
                i = i + 1
                for point in road_points:
                    p1, p2 = initfile.wgs2bd(point[1], point[0])
                    k.write(f'new BMapGL.Point({p1},{p2}),')
                k.write('], {strokeColor:"blue", strokeWeight:2, strokeOpacity:0.5}); ')

            p1, p2 = initfile.wgs2bd(A_L_LA, A_L_LO)
            p3, p4 = initfile.wgs2bd(A_R_LA, A_R_LO)
            k.write(f'var rectangle1 = new BMapGL.Polygon([\n')
            k.write(f'new BMapGL.Point({p1},{p2}),\n')
            k.write(f'new BMapGL.Point({p1},{p4}),\n')
            k.write(f'new BMapGL.Point({p3},{p4}),\n')
            k.write(f'new BMapGL.Point({p3},{p2}),\n')
            k.write('], {strokeColor:"blue", strokeWeight:2, strokeOpacity:0.5});\n ')
            k.write(f"var opts1 = \n")
            k.write('{')
            k.write(f' position: new BMapGL.Point({p1}, {p2}),')
            k.write('};\n')
            k.write(f"var label1 = new BMapGL.Label('A矩形区域', opts1);\n")
            k.write(f"label1.setStyle(")
            k.write('{')
            k.write(
                "color: 'red',borderRadius: '5px', borderColor: '#ccc', padding: '10px',fontSize: '16px',height: '30px',lineHeight: '30px',fontFamily: '微软雅黑'});"
                "\n")
            p1, p2 = initfile.wgs2bd(B_L_LA, B_L_LO)
            p3, p4 = initfile.wgs2bd(B_R_LA, B_R_LO)
            k.write(f'var rectangle2 = new BMapGL.Polygon([\n')
            k.write(f'new BMapGL.Point({p1},{p2}),\n')
            k.write(f'new BMapGL.Point({p1},{p4}),\n')
            k.write(f'new BMapGL.Point({p3},{p4}),\n')
            k.write(f'new BMapGL.Point({p3},{p2}),\n')
            k.write('], {strokeColor:"blue", strokeWeight:2, strokeOpacity:0.5});\n ')
            k.write(f"var opts2 = ")
            k.write('{')
            k.write(f' position: new BMapGL.Point({p1}, {p2}),')
            k.write('};\n')
            k.write(f"var label2 = new BMapGL.Label('B矩形区域', opts2);")
            k.write(f"label2.setStyle(")
            k.write('{')
            k.write(
                "color: 'red',borderRadius: '5px', borderColor: '#ccc', padding: '10px',fontSize: '16px',height: '30px',lineHeight: '30px',fontFamily: '微软雅黑'});"
                "\n")

            k.write('function add_overlay() {')
            ii = 1
            for road_index, road_points in all_roads.items():
                k.write(f' map.addOverlay(polyline{ii}); ')
                ii = ii + 1
            k.write('map.addOverlay(rectangle1);\n'
                    'map.addOverlay(rectangle2);\n'
                    'map.addOverlay(label1);\n'
                    'map.addOverlay(label2);\n')
            k.write('} add_overlay();')

            k.write('</script>')
        print("写入完成")

        sUrl = 'http://localhost:1666/new.html'
        self.m_webView.load(QUrl(sUrl))
        sUrl = 'http://localhost:1666/bj.html'
        self.m_webView1.load(QUrl(sUrl))

    # F9
    def btn0_clicked(self):
        A_L_LO, A_L_LA, A_R_LO, A_R_LA, B_L_LO, B_L_LA, B_R_LO, B_R_LA, TIME_L, TIME_R = self.getdata()
        Time = ctypes.c_double()
        # res = (ctypes.c_double * 10000)()
        res = np.zeros((10000, 2), dtype=np.double)
        initfile.mydll.F9_FUNCTION(TIME_L.encode('utf-8'), TIME_R.encode('utf-8'), A_L_LO, A_L_LA, A_R_LO,
                                   A_R_LA, B_L_LO, B_L_LA, B_R_LO, B_R_LA, ctypes.byref(Time),
                                   res.ctypes.data_as(ctypes.POINTER(ctypes.c_double)))
        print(f"Time: {Time.value}")
        self.textEdit.setText(str(f'F9中最短的通行时间{Time.value}'))
        # res = np.unique(res, axis=0)
        # res = res[1:]  # 删除第一行
        print(res)
        for i in range(10000):
            if res[i][0] == 0.0:
                res = res[0:i]
                break  #
        # 打开原有HTML文件，读取内容
        print("sssssssssssssss")
        with open('t1.html', 'r', encoding='utf-8') as f:
            html_content = f.read()
        shape = res.shape
        # 打开新HTML文件，写入内容
        print("mmmmmmmmmmmmmmmmm")
        with open('new.html', 'w', encoding='utf-8') as k:
            print('vvvvvvvvvvvvvvvv')
            # 将原有HTML文件中的内容写入新HTML文件中
            k.write(html_content)
            # 在新HTML文件中添加新的JavaScript代码
            k.write('<script type="text/javascript">\n')
            p1, p2 = initfile.wgs2bd(res[0][1], res[0][0])
            k.write(f'var marker1 = new BMapGL.Marker(new BMapGL.Point({p1}, {p2}));\n ')
            p1, p2 = initfile.wgs2bd(res[shape[0] - 1][1], res[shape[0] - 1][0])
            k.write(
                f'var marker2 = new BMapGL.Marker(new BMapGL.Point({p1}, {p2}));\n ')
            k.write('var polyline = new BMapGL.Polyline([\n')
            print('eeeeeeeeeeeeeeeeeee')
            for i in range(shape[0]):
                p1, p2 = initfile.wgs2bd(res[i][1], res[i][0])
                k.write(f'new BMapGL.Point({p1},{p2}),\n')
            k.write('], {strokeColor:"blue", strokeWeight:2, strokeOpacity:0.5});\n ')
            p1, p2 = initfile.wgs2bd(A_L_LA, A_L_LO)
            p3, p4 = initfile.wgs2bd(A_R_LA, A_R_LO)
            k.write(f'var rectangle1 = new BMapGL.Polygon([\n')
            k.write(f'new BMapGL.Point({p1},{p2}),\n')
            k.write(f'new BMapGL.Point({p1},{p4}),\n')
            k.write(f'new BMapGL.Point({p3},{p4}),\n')
            k.write(f'new BMapGL.Point({p3},{p2}),\n')
            k.write('], {strokeColor:"blue", strokeWeight:2, strokeOpacity:0.5});\n ')
            k.write(f"var opts1 = \n")
            k.write('{')
            k.write(f' position: new BMapGL.Point({p1}, {p2}),')
            k.write('};\n')
            k.write(f"var label1 = new BMapGL.Label('A矩形区域', opts1);\n")
            k.write(f"label1.setStyle(")
            k.write('{')
            k.write(
                "color: 'red',borderRadius: '5px', borderColor: '#ccc', padding: '10px',fontSize: '16px',height: '30px',lineHeight: '30px',fontFamily: '微软雅黑'});"
                "\n")

            print("333333333333")
            p1, p2 = initfile.wgs2bd(B_L_LA, B_L_LO)
            p3, p4 = initfile.wgs2bd(B_R_LA, B_R_LO)
            k.write(f'var rectangle2 = new BMapGL.Polygon([\n')
            k.write(f'new BMapGL.Point({p1},{p2}),\n')
            k.write(f'new BMapGL.Point({p1},{p4}),\n')
            k.write(f'new BMapGL.Point({p3},{p4}),\n')
            k.write(f'new BMapGL.Point({p3},{p2}),\n')
            k.write('], {strokeColor:"blue", strokeWeight:2, strokeOpacity:0.5});\n ')
            k.write(f"var opts2 = ")
            k.write('{')
            k.write(f' position: new BMapGL.Point({p1}, {p2}),')
            k.write('};\n')
            k.write(f"var label2 = new BMapGL.Label('B矩形区域', opts2);")
            k.write(f"label2.setStyle(")
            k.write('{')
            k.write(
                "color: 'red',borderRadius: '5px', borderColor: '#ccc', padding: '10px',fontSize: '16px',height: '30px',lineHeight: '30px',fontFamily: '微软雅黑'});"
                "\n")

            k.write('function add_overlay()'
                    '    {'
                    'map.addOverlay(marker1);           '
                    'map.addOverlay(marker2);           '
                    'map.addOverlay(polyline); '
                    'map.addOverlay(rectangle1);\n'
                    'map.addOverlay(rectangle2);\n'
                    'map.addOverlay(label1);\n'
                    'map.addOverlay(label2);\n'
                    '    }\n'
                    ' add_overlay();')
            k.write('</script>')
            print("写入完成")

        line_total = 2
        sUrl = 'http://localhost:1666/new.html'
        self.m_webView.load(QUrl(sUrl))


def run_server():
    # 将html文件复制到本地服务器根目录
    os.chdir('C:/Users/pqs-xsz-xia/Desktop/c++_to_python_test')  # 切换工作目录到c++_to_python_test
    # 启动HTTP服务器
    PORT = 1666
    Handler = http.server.SimpleHTTPRequestHandler
    global httpd
    with socketserver.TCPServer(("", PORT), Handler) as httpd:
        print(f"Serving at http://localhost:{PORT}")
        httpd = httpd  # 将httpd赋值给全局变量，便于关闭服务器
        httpd.serve_forever()


if __name__ == '__main__':
    # 在子线程中启动HTTP服务器
    server_thread = threading.Thread(target=run_server)
    server_thread.start()
    # 等待HTTP服务器启动
    while True:
        try:
            # response = webbrowser.open_new_tab("http://localhost:1666/baidu_map.html")
            break
        except:
            pass

    app = QApplication(sys.argv)
    widget = Widget()

    # 获取屏幕的宽度和高度
    screen_rect = QApplication.desktop().screenGeometry()
    screen_center = screen_rect.center()

    # 获取窗口的宽度和高度
    widget_rect = widget.geometry()
    widget_center = widget_rect.center()

    # 计算窗口应该位于屏幕中心的坐标
    move_x = screen_center.x() - widget_center.x()
    move_y = screen_center.y() - widget_center.y()

    # 将窗口移动到屏幕中心
    widget.move(move_x, move_y)

    widget.show()

    sys.exit(app.exec_())
