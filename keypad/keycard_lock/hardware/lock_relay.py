from gpiozero import DigitalOutputDevice


class LockRelay(object):
    def __init__(self, relay_pin, pin_factory):
        self._relay = DigitalOutputDevice(
            pin=relay_pin,
            active_high=True,
            initial_value=False,
            pin_factory=pin_factory
        )

    def lock(self):
        self._relay.off()

    def unlock(self):
        self._relay.on()

    def is_unlocked(self):
        return self._relay.value == 1
