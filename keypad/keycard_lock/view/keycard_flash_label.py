from enum import Enum

from PyQt5.QtCore import QTimer, pyqtSlot
from PyQt5.QtGui import QPainter
from PyQt5.QtWidgets import QFrame


class KeycardFlashType(Enum):
    NO_FLASH = 0
    FLASH_GREEN = 1
    FLASH_RED = 2


class KeycardFlashLabel(QFrame):
    FLASH_TIME_MS = 600

    def __init__(self, normal_pixmap, flash_pixmap_green, flash_pixmap_red):
        super().__init__()

        self._normal_pixmap = normal_pixmap
        self._flash_pixmap_green = flash_pixmap_green
        self._flash_pixmap_red = flash_pixmap_red
        self._flash_type = KeycardFlashType.NO_FLASH
        self._flash_timer = None

    def paintEvent(self, event):
        painter = QPainter(self)
        rect = event.rect()

        if self._flash_type == KeycardFlashType.NO_FLASH:
            flash_pixmap = self._normal_pixmap
        elif self._flash_type == KeycardFlashType.FLASH_GREEN:
            flash_pixmap = self._flash_pixmap_green
        elif self._flash_type == KeycardFlashType.FLASH_RED:
            flash_pixmap = self._flash_pixmap_red
        else:
            flash_pixmap = None

        if flash_pixmap is not None:
            painter.drawPixmap(rect.left(), rect.top(), flash_pixmap, 0, rect.top(), rect.width(), rect.height())

    # Overridden from QAbstractButton
    def sizeHint(self):
        return self._normal_pixmap.size()

    def trigger_flash(self, flash_type=KeycardFlashType.NO_FLASH, flash_time=FLASH_TIME_MS):
        if self._flash_timer is not None:
            self._flash_timer.stop()

        self._flash_type = flash_type

        self._flash_timer = QTimer()
        self._flash_timer.timeout.connect(self._flash_timeout)
        self._flash_timer.setSingleShot(True)
        self._flash_timer.start(flash_time)
        self.repaint()

    @pyqtSlot()
    def _flash_timeout(self):
        self._flash_type = KeycardFlashType.NO_FLASH
        self._flash_timer = None
        self.repaint()
