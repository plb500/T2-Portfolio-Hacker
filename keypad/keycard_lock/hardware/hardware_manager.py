from gpiozero import LED

from keycard_lock.hardware.lock_relay import LockRelay
from keycard_lock.hardware.pin_serial_interface import PINSerialInterface
from time import time


class HardwareManager(object):
    # Hold lock open for 10s
    LOCK_HOLD_OPEN_TIME_MS = 10 * 1000

    LCD_GREETING_MESSAGE = "Cyberdyne Systems Access Control. Please insert access card to proceed."
    LCD_UNLOCKED_MESSAGE = "System unlocked"

    def __init__(
            self,
            serial_port_name,
            serial_port_baud,
            lock_relay_pin,
            red_led_pin,
            green_led_pin,
            pin_factory,
            lcd_factory
    ):
        self._pin_serial_interface = PINSerialInterface(
            parent=None,
            port_name=serial_port_name,
            baudrate=serial_port_baud
        )
        self._lock_relay = LockRelay(
            relay_pin=lock_relay_pin,
            pin_factory=pin_factory
        )
        self._char_lcd_controller = lcd_factory.create_char_lcd()
        self._red_led = LED(
            pin=red_led_pin,
            pin_factory=pin_factory
        )
        self._green_led = LED(
            pin=green_led_pin,
            pin_factory=pin_factory
        )

        self._do_lock()

    def initialize_hardware(self, current_pin):
        self._pin_serial_interface.open_port()
        self._pin_serial_interface.cache_pin(current_pin)

    def on_new_pin(self, new_pin):
        self._pin_serial_interface.cache_pin(pin=new_pin)

    def on_lock(self):
        self._do_lock()

    def on_unlock(self):
        self._do_unlock()

    def show_pin(self, pin=None):
        if pin is None:
            self._char_lcd_controller.put_string(HardwareManager.LCD_GREETING_MESSAGE)
        else:
            pin_string = "PIN: {}".format(str(pin).zfill(4))
            self._char_lcd_controller.put_string(pin_string)

    def show_lcd_string(self, display_string):
        self._char_lcd_controller.put_string(display_string)

    def on_update(self):
        current_time_ms = HardwareManager._get_current_time_ms()

        # Check if we should re-lock
        if (
            self._lock_relay.is_unlocked() and
            (current_time_ms >= (self._last_unlock_time + HardwareManager.LOCK_HOLD_OPEN_TIME_MS))
        ):
            self._do_lock()

        # Update LCD
        self._char_lcd_controller.update(current_time=current_time_ms)

    def _do_lock(self):
        self._red_led.on()
        self._green_led.off()
        self._lock_relay.lock()
        self._char_lcd_controller.put_string(HardwareManager.LCD_GREETING_MESSAGE)
        self._last_unlock_time = 0

    def _do_unlock(self):
        self._last_unlock_time = HardwareManager._get_current_time_ms()

        self._red_led.off()
        self._green_led.on()
        self._lock_relay.unlock()
        self._char_lcd_controller.put_string(HardwareManager.LCD_UNLOCKED_MESSAGE)

    @staticmethod
    def _get_current_time_ms():
        return int(time() * 1000)
