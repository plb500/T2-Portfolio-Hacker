import argparse

from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QApplication

from keycard_lock.controller.pin_controller import PINController
from keycard_lock.hardware.hardware_manager import HardwareManager
from keycard_lock.model.pin_store import PINStore
from keycard_lock.view.keycard_button_frame import KeycardButtonFrame
from keycard_lock.view.keycard_window import KeycardWindow

import sys


def run_main():
    serial_port_baud = 9600
    lock_relay_pin = 23
    green_led_pin = 18
    red_led_pin = 4
    lcd_i2c_address = 0x27

    # Handle command-line arguments
    parser = argparse.ArgumentParser(description="Cyberdyne Access Control")
    parser.add_argument("-m", "--mock-pins", action="store_true", dest="mock_pins")
    parser.add_argument("-l", "--mock-lcd", action="store_true", dest="mock_lcd")
    parser.add_argument("-s", "--serial-port", dest="serial_port")
    parser.add_argument("-v", "--cursor-visible", action="store_true", dest="cursor_visible")

    args = parser.parse_args()

    # Check pin factory
    if args.mock_pins:
        from gpiozero.pins.mock import MockFactory
        pin_factory = MockFactory()
    else:
        from gpiozero.pins.rpigpio import RPiGPIOFactory
        pin_factory = RPiGPIOFactory()

    # Check LCD factory
    if args.mock_lcd:
        from keycard_lock.hardware.lcd.char_lcd_factory import MockLCDFactory
        lcd_factory = MockLCDFactory(address=lcd_i2c_address)
    else:
        from keycard_lock.hardware.lcd.char_lcd_factory import HardwareLCDFactory
        lcd_factory = HardwareLCDFactory(address=lcd_i2c_address)

    # Grab serial port
    serial_port = args.serial_port

    # Create application and components
    app = QApplication(sys.argv)

    # Check hide cursor
    if not args.cursor_visible:
        QApplication.setOverrideCursor(Qt.BlankCursor)

    # Model
    pin_store = PINStore(max_value=9999)

    # Outputs (view and hardware)
    hardware_manager = HardwareManager(
        serial_port_name=serial_port,
        serial_port_baud=serial_port_baud,
        lock_relay_pin=lock_relay_pin,
        red_led_pin=red_led_pin,
        green_led_pin=green_led_pin,
        pin_factory=pin_factory,
        lcd_factory=lcd_factory
    )
    pin_view = KeycardButtonFrame()

    hardware_manager.initialize_hardware(current_pin=pin_store.get_current_pin())

    # Controller
    pin_controller = PINController(
        pin_store=pin_store,
        hardware_manager=hardware_manager
    )

    # Wire up controller to view
    pin_view.digitEntered.connect(pin_controller.on_pin_digit_entered)
    pin_controller.goodPIN.connect(pin_view.on_good_pin_entered)
    pin_controller.badPIN.connect(pin_view.on_bad_pin_entered)

    window = KeycardWindow(keycard_frame=pin_view)
    window.show()

    # Launch app - all lines below this will be executed once UI is shut down
    return app.exec()


if __name__ == "__main__":
    sys.exit(run_main())









