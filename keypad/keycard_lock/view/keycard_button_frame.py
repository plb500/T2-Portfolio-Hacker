from PyQt5.QtCore import pyqtSignal
from PyQt5.QtWidgets import QFrame, QHBoxLayout, QVBoxLayout

from keycard_lock.view.graphics_loader import GraphicsLoader
from keycard_lock.view.keycard_button import KeycardButtons
from keycard_lock.view.keycard_flash_label import KeycardFlashLabel, KeycardFlashType


class KeycardButtonFrame(QFrame):
    digitEntered = pyqtSignal(object)

    HORIZONTAL_BUTTON_MARGIN = 22
    VERTICAL_BUTTON_MARGIN = 6

    BUTTON_PRESS_FLASH_TIME_MS = 150
    BAD_PIN_PRESS_FLASH_TIME_MS = 750

    def __init__(self):
        super().__init__()

        self.buttons = {}
        self._flash_label = None

        self._setup_ui()

    def _setup_ui(self):
        self._flash_label = KeycardButtonFrame._build_flash_label()
        self.setContentsMargins(0, 0, 0, 0)

        button_1 = KeycardButtons.BUTTON_1.create_button()
        button_2 = KeycardButtons.BUTTON_2.create_button()
        button_3 = KeycardButtons.BUTTON_3.create_button()
        button_4 = KeycardButtons.BUTTON_4.create_button()
        button_5 = KeycardButtons.BUTTON_5.create_button()
        button_6 = KeycardButtons.BUTTON_6.create_button()
        button_7 = KeycardButtons.BUTTON_7.create_button()
        button_8 = KeycardButtons.BUTTON_8.create_button()
        button_9 = KeycardButtons.BUTTON_9.create_button()
        button_0 = KeycardButtons.BUTTON_0.create_button()
        button_star = KeycardButtons.BUTTON_STAR.create_button()
        button_hash = KeycardButtons.BUTTON_HASH.create_button()
        button_arrows_h = KeycardButtons.BUTTON_ARROWS_HORIZONTAL.create_button()
        button_arrows_v = KeycardButtons.BUTTON_ARROWS_VERTICAL.create_button()

        button_1.pinKeyPressed.connect(self._on_pin_key_pressed)
        button_2.pinKeyPressed.connect(self._on_pin_key_pressed)
        button_3.pinKeyPressed.connect(self._on_pin_key_pressed)
        button_4.pinKeyPressed.connect(self._on_pin_key_pressed)
        button_5.pinKeyPressed.connect(self._on_pin_key_pressed)
        button_6.pinKeyPressed.connect(self._on_pin_key_pressed)
        button_7.pinKeyPressed.connect(self._on_pin_key_pressed)
        button_8.pinKeyPressed.connect(self._on_pin_key_pressed)
        button_9.pinKeyPressed.connect(self._on_pin_key_pressed)
        button_0.pinKeyPressed.connect(self._on_pin_key_pressed)
        button_star.pinKeyPressed.connect(self._on_pin_key_pressed)
        button_hash.pinKeyPressed.connect(self._on_pin_key_pressed)
        button_arrows_h.pinKeyPressed.connect(self._on_pin_key_pressed)
        button_arrows_v.pinKeyPressed.connect(self._on_pin_key_pressed)

        row_1_layout = KeycardButtonFrame._create_button_row_layout([
            button_1,
            button_2,
            button_3
        ])
        row_2_layout = KeycardButtonFrame._create_button_row_layout([
            button_4,
            button_5,
            button_6
        ])
        row_3_layout = KeycardButtonFrame._create_button_row_layout([
            button_7,
            button_8,
            button_9
        ])
        row_4_layout = KeycardButtonFrame._create_button_row_layout([
            button_star,
            button_0,
            button_hash
        ])
        row_5_layout = KeycardButtonFrame._create_button_row_layout([
            button_arrows_h,
            button_arrows_v,
            self._flash_label,
        ])

        v_layout = QVBoxLayout()

        v_layout.addSpacing(KeycardButtonFrame.VERTICAL_BUTTON_MARGIN)

        v_layout.addLayout(row_1_layout)
        v_layout.addStretch(1)
        v_layout.addLayout(row_2_layout)
        v_layout.addStretch(1)
        v_layout.addLayout(row_3_layout)
        v_layout.addStretch(1)
        v_layout.addLayout(row_4_layout)
        v_layout.addStretch(1)
        v_layout.addLayout(row_5_layout)

        v_layout.addSpacing(KeycardButtonFrame.VERTICAL_BUTTON_MARGIN)

        v_layout.setSpacing(0)
        v_layout.setContentsMargins(0, 0, 0, 0)

        self.setStyleSheet("background-color:black;")

        self.setLayout(v_layout)

    @staticmethod
    def _create_button_row_layout(buttons):
        button_layout = QHBoxLayout()

        button_layout.addSpacing(KeycardButtonFrame.HORIZONTAL_BUTTON_MARGIN)

        for index, button in enumerate(buttons):
            button_layout.addWidget(
                button,
                stretch=0
            )

            if index != len(buttons) - 1:
                button_layout.addStretch(1)

        button_layout.addSpacing(KeycardButtonFrame.HORIZONTAL_BUTTON_MARGIN)

        button_layout.setSpacing(0)
        button_layout.setContentsMargins(0, 0, 0, 0)

        return button_layout

    @staticmethod
    def _build_flash_label():
        normal_pixmap = GraphicsLoader.get_pixmap("button_o_idle.png")
        flash_pixmap_good = GraphicsLoader.get_pixmap("button_o_flash_green.png")
        flash_pixmap_bad = GraphicsLoader.get_pixmap("button_o_flash_red.png")
        return KeycardFlashLabel(
            normal_pixmap=normal_pixmap,
            flash_pixmap_green=flash_pixmap_good,
            flash_pixmap_red=flash_pixmap_bad
        )

    def _on_pin_key_pressed(self, pin_key):
        self._flash_label.trigger_flash(
            flash_type=KeycardFlashType.FLASH_RED,
            flash_time=KeycardButtonFrame.BUTTON_PRESS_FLASH_TIME_MS
        )
        self.digitEntered.emit(pin_key)

    def on_good_pin_entered(self):
        self._flash_label.trigger_flash(
            flash_type=KeycardFlashType.FLASH_GREEN,
            flash_time=KeycardButtonFrame.BAD_PIN_PRESS_FLASH_TIME_MS
        )

    def on_bad_pin_entered(self):
        self._flash_label.trigger_flash(
            flash_type=KeycardFlashType.FLASH_RED,
            flash_time=KeycardButtonFrame.BAD_PIN_PRESS_FLASH_TIME_MS
        )
