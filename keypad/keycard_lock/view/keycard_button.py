from enum import Enum

from PyQt5.QtCore import pyqtSignal
from PyQt5.QtGui import QPainter
from PyQt5.QtWidgets import QAbstractButton, QSizePolicy

from keycard_lock.view.graphics_loader import GraphicsLoader


class KeycardButtons(Enum):
    BUTTON_0 = "button_0.png", ["0"]
    BUTTON_1 = "button_1.png", ["1"]
    BUTTON_2 = "button_2.png", ["2"]
    BUTTON_3 = "button_3.png", ["3"]
    BUTTON_4 = "button_4.png", ["4"]
    BUTTON_5 = "button_5.png", ["5"]
    BUTTON_6 = "button_6.png", ["6"]
    BUTTON_7 = "button_7.png", ["7"]
    BUTTON_8 = "button_8.png", ["8"]
    BUTTON_9 = "button_9.png", ["9"]
    BUTTON_STAR = "button_star.png", ["*"]
    BUTTON_HASH = "button_hash.png", ["#"]
    BUTTON_ARROWS_HORIZONTAL = "button_arrows_horiz.png", ["L", "R"]
    BUTTON_ARROWS_VERTICAL = "button_arrows_vert.png", ["U", "D"]

    def __init__(self, image_file, outputs):
        self._image_file = image_file
        self._outputs = outputs

    def create_button(self):
        return KeycardButton(
            pixmap=GraphicsLoader.get_pixmap(self._image_file),
            outputs=self._outputs
        )


class KeycardButton(QAbstractButton):
    pinKeyPressed = pyqtSignal(object)

    def __init__(self, pixmap, outputs, parent=None):
        super().__init__(parent)

        self._pixmap = pixmap
        self._outputs = outputs
        self.setSizePolicy(QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Fixed)
        self._pressed_output = None

    # Overridden from QAbstractButton
    def paintEvent(self, event):
        painter = QPainter(self)
        rect = event.rect()
        painter.drawPixmap(rect.left(), rect.top(), self._pixmap, 0, rect.top(), rect.width(), rect.height())

    # Overridden from QAbstractButton
    def sizeHint(self):
        return self._pixmap.size()

    def mousePressEvent(self, e):
        if self._is_split():
            # Check if we are in the top triangle
            x1 = 0
            y1 = 0
            x2 = self.width()
            y2 = 0
            x3 = 0
            y3 = self.height()
            click_pos = e.localPos()
            x = click_pos.x()
            y = click_pos.y()

            if KeycardButton._is_inside(x1, y1, x2, y2, x3, y3, x, y):
                self._pressed_output = self._outputs[0]
            else:
                self._pressed_output = self._outputs[1]
        else:
            self._pressed_output = self._outputs[0]

    def mouseReleaseEvent(self, e):
        self.pinKeyPressed.emit(self._pressed_output)
        self._pressed_output = None

    def _is_split(self):
        return len(self._outputs) > 1

    @staticmethod
    def _area(x1, y1, x2, y2, x3, y3):
        return abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0)

    @staticmethod
    def _is_inside(x1, y1, x2, y2, x3, y3, x, y):
        # Calculate area of main outer triangle ABC
        triangle_area = KeycardButton._area(x1, y1, x2, y2, x3, y3)

        # Calculate area of triangle PBC
        pbc_area = KeycardButton._area(x, y, x2, y2, x3, y3)

        # Calculate area of triangle PAC
        pac_area = KeycardButton._area(x1, y1, x, y, x3, y3)

        # Calculate area of triangle PAB
        pab_area = KeycardButton._area(x1, y1, x2, y2, x, y)

        # Check if sum of sub-triangles is same as A
        return triangle_area == pbc_area + pac_area + pab_area
