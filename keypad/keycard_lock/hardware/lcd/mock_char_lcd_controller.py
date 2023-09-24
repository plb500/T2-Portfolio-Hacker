from keycard_lock.hardware.lcd.char_lcd_controller import CharLCDController


class MockCharLCDController(CharLCDController):
    def __init__(self):
        super().__init__()
        self._last_painted_string = ""

    def do_paint(self, output_string):
        if output_string != self._last_painted_string:
            output_len = len(output_string)
            print("+{}+".format("-" * output_len))
            print("|{}|".format(output_string))
            print("+{}+".format("-" * output_len))
            self._last_painted_string = output_string
