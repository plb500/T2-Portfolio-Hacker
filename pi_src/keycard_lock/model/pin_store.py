import random


class PINStore(object):
    def __init__(self, max_value):
        self._max_value = max_value
        self._current_pin = None
        self.generate_new_pin()

    def generate_new_pin(self):
        self._current_pin = random.randint(0, self._max_value)
        print("Generated PIN: {}".format(str(self._current_pin).zfill(self.expected_pin_length())))
        return self._current_pin

    def get_current_pin(self):
        return self._current_pin

    def check_pin(self, pin):
        return pin == self._current_pin

    def expected_pin_length(self):
        return len(str(self._max_value))
