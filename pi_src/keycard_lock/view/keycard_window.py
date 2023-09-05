from PyQt5.QtCore import QSize
from PyQt5.QtWidgets import QMainWindow


class KeycardWindow(QMainWindow):
    WINDOW_TITLE = "Keycard Reader"
    WINDOW_WIDTH = 480
    WINDOW_HEIGHT = 800

    def __init__(self, keycard_frame):
        super().__init__()
        self.setWindowTitle(KeycardWindow.WINDOW_TITLE)

        # Set the layout on the application's window
        self.setGeometry(0, 0, KeycardWindow.WINDOW_WIDTH, KeycardWindow.WINDOW_HEIGHT)
        self.setFixedSize(QSize(KeycardWindow.WINDOW_WIDTH, KeycardWindow.WINDOW_HEIGHT))
        self.setCentralWidget(keycard_frame)
