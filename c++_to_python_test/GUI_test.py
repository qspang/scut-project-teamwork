import http.server
import os
import socketserver
import threading
import webbrowser

from PyQt5.QtCore import QUrl
from PyQt5.QtWebEngineWidgets import QWebEnginePage, QWebEngineView
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout


class Map(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle('Baidu Map')
        self.setGeometry(100, 100, 800, 600)

        # 创建浏览器控件和页面
        self.browser = QWebEngineView()
        self.page = QWebEnginePage(self.browser)
        self.browser.setPage(self.page)

        # 加载网页
        self.browser.load(QUrl("http://localhost:1666/baidu_map.html"))

        # 将浏览器控件添加到主窗口中
        layout = QVBoxLayout(self)
        layout.addWidget(self.browser)
        self.setLayout(layout)

    def closeEvent(self, event):
        # 关闭窗口时停止HTTP服务器
        global httpd
        httpd.shutdown()
        event.accept()


def run_server():
    # 将html文件复制到本地服务器根目录
    os.chdir('C:/Users/hubia/PycharmProjects/c++_to_python_test')  # 切换工作目录到c++_to_python_test
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
           #response = webbrowser.open_new_tab("http://localhost:1666/baidu_map.html")
            break
        except:
            pass

    # 启动PyQt窗口
    app = QApplication([])
    window = Map()
    window.show()
    app.exec_()
