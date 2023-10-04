import http.server
import os
import socketserver
import threading
import sys
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout
from PyQt5.QtWebEngineWidgets import QWebEngineView
from PyQt5.QtCore import QUrl, QDir
from PyQt5 import uic
from PyQt5.QtCore import pyqtSlot

class Widget(QWidget):
    def __init__(self, parent=None):
        super(Widget, self).__init__(parent)
        uic.loadUi('widget.ui', self)
        self.setFixedSize(1200, 800)
        self.webaddr.setText("http://localhost:1666/map.html")
        self.m_webView = QWebEngineView(self)
        sUrl = "http://localhost:1666/map.html"
        self.m_webView.load(QUrl(sUrl))
        self.verticalLayout.addWidget(self.m_webView)

        self.GObtn.clicked.connect(self.on_GObtn_clicked)
        self.MAPbtn.clicked.connect(self.on_MAPbtn_clicked)

        # 等待页面加载完成后，执行addClickEvent函数
        self.m_webView.loadFinished.connect(self.addClickEvent)

    def on_GObtn_clicked(self):
        sUrl = 'http://localhost:1666/t1.html'
        self.m_webView.load(QUrl(sUrl))

    def on_MAPbtn_clicked(self):
        mapHtml = "map.html"

        temDir = QDir("map.html")
        absDir = temDir.absolutePath()
        filePath = absDir

        self.m_webView.page().load(QUrl.fromLocalFile(filePath))
        self.webaddr.setText("地图")

    @pyqtSlot(bool)
    def addClickEvent(self, loadOk):
        if loadOk:
            # 定义JavaScript代码
            js_code = """
                var map = document.getElementById("map");  // 获取地图元素
                map.addEventListener('click', function(e) {
                    var lat = e.latlng.lat;
                    var lng = e.latlng.lng;
                    var xhr = new XMLHttpRequest();
                    xhr.open('POST', 'http://localhost:1666/coordinates', true);  // 发送POST请求到Python应用的/coordinates路由
                    xhr.setRequestHeader('Content-Type', 'application/json');
                    xhr.onreadystatechange = function() {
                        if (xhr.readyState === XMLHttpRequest.DONE && xhr.status === 200) {
                            console.log('Coordinates sent');
                        }
                    };
                    var message = JSON.stringify({
                        'latitude': lat,
                        'longitude': lng
                    });
                    xhr.send(message);
                });
            """

            # 在页面中执行JavaScript代码
            self.m_webView.page().runJavaScript(js_code)


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
    widget.show()
    sys.exit(app.exec_())