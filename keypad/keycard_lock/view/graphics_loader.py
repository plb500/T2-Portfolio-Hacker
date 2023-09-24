import os

from PyQt5 import QtGui
from PyQt5.QtGui import QPixmap


class GraphicsLoader(object):
    VIEW_DIR = os.path.dirname(os.path.abspath(__file__))
    IMAGE_DATA_PATH = os.path.join(VIEW_DIR, "../assets")

    @staticmethod
    def get_full_path(image_relative_path):
        path = os.path.join(GraphicsLoader.IMAGE_DATA_PATH, image_relative_path)
        return path

    @staticmethod
    def get_pixmap(pixmap_file):
        return QPixmap(GraphicsLoader.get_full_path(pixmap_file))

    @staticmethod
    def get_icon(icon_file):
        return QtGui.QIcon(GraphicsLoader.get_full_path(icon_file))
