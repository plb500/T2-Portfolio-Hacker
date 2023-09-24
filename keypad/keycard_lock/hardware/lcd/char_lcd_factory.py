class CharLCDFactory(object):
    def __init__(self, address):
        self._address = address

    def create_char_lcd(self):
        raise NotImplementedError()


class HardwareLCDFactory(CharLCDFactory):
    def __init__(self, address):
        super().__init__(address=address)

    def create_char_lcd(self):
        from keycard_lock.hardware.lcd.hardware_char_lcd_controller import HardwareCharLCDController
        return HardwareCharLCDController(self._address)


class MockLCDFactory(CharLCDFactory):
    def create_char_lcd(self):
        from keycard_lock.hardware.lcd.mock_char_lcd_controller import MockCharLCDController
        return MockCharLCDController()
