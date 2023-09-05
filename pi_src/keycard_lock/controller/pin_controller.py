from PyQt5.QtCore import QObject, pyqtSignal, QTimer


class PINController(QObject):
    goodPIN = pyqtSignal()
    badPIN = pyqtSignal()

    HARDWARE_MANAGER_UPDATE_MS = 100

    def __init__(self, pin_store, hardware_manager):
        super().__init__()

        self._pin_store = pin_store
        self._hardware_manager = hardware_manager
        self._entered_digits = []

        self._hardware_update_timer = QTimer()
        self._hardware_update_timer.timeout.connect(self._do_hardware_update)
        self._hardware_update_timer.start(PINController.HARDWARE_MANAGER_UPDATE_MS)

    def on_pin_digit_entered(self, digit):
        # Sanity check argument

        # Must only be a single digit
        if len(digit) != 1:
            return

        # Must be a number
        if not digit.isnumeric():
            return

        self._entered_digits.append(digit)
        if len(self._entered_digits) == self._pin_store.expected_pin_length():
            # We have received enough digits. Check PIN.
            pin = int("".join(self._entered_digits))
            match = self._pin_store.check_pin(pin)

            print("Checking entered PIN: {}, match = ".format(
                str(pin).zfill(self._pin_store.expected_pin_length())),
                match
            )

            if match:
                self._on_good_pin_entered()
            else:
                self._on_bad_pin_entered()

            self._entered_digits = []

    def _on_good_pin_entered(self):
        # Generate a new random PIN and tell the hardware manager
        new_pin = self._pin_store.generate_new_pin()
        self._hardware_manager.on_unlock()
        self._hardware_manager.on_new_pin(new_pin)

        self.goodPIN.emit()

    def _on_bad_pin_entered(self):
        self.badPIN.emit()

    def _do_hardware_update(self):
        self._hardware_manager.on_update()
