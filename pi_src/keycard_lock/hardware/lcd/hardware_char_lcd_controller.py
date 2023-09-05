from RPLCD.i2c import CharLCD
from keycard_lock.hardware.lcd.char_lcd_controller import CharLCDController


class HardwareCharLCDController(CharLCDController):
    def __init__(self, address):
        super().__init__()
        self._char_lcd = CharLCD(
            i2c_expander="PCF8574",
            cols=8, rows=2,
            address=address,
            port=1,
            auto_linebreaks=True,
            backlight_enabled=True
        )

    def do_paint(self, output_string):
        self._char_lcd.cursor_pos = (0, 0)
        self._char_lcd.write_string(output_string)
