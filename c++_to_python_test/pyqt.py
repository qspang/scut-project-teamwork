from PyQt5 import QtWidgets, uic
from PyQt5 import uic

class Widget(QtWidgets.QWidget):
    def __init__(self):
        super().__init__()
        uic.loadUi('widget.ui', self)

if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    widget = Widget()
    widget.show()
    sys.exit(app.exec_())
