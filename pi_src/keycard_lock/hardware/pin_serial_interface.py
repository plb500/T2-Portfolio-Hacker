from PyQt5.QtCore import QObject, QIODevice, pyqtSlot, QMutex, QTimer
from PyQt5.QtSerialPort import QSerialPort


class PINSerialInterface(QObject):
    PIN_SEQUENCE_START = 0x80
    PIN_SEQUENCE_END = 0x81
    RECONNECT_TIME_MS = 500

    def __init__(self, parent, port_name, baudrate):
        super().__init__(parent=parent)

        self._port_name = port_name
        self._baudrate = baudrate
        self._serial_port = None
        self._serial_port_active = False

        self._current_pin = None
        self._current_pin_lock = QMutex()

    def open_port(self):
        self._serial_port = QSerialPort()
        self._serial_port.setPortName(self._port_name)
        self._serial_port.setBaudRate(self._baudrate)
        self._serial_port_active = self._serial_port.open(QIODevice.OpenModeFlag.ReadWrite)
        if self._serial_port_active:
            self._serial_port.readyRead.connect(self.on_ready_read)
            self._serial_port.errorOccurred.connect(self._on_serial_port_error)
            self._transmit_current_pin()
        else:
            # print("Connecting to serial port: {} FAILED".format(self._port_name))
            reconnect_timer = QTimer(parent=self)
            reconnect_timer.setSingleShot(True)
            reconnect_timer.timeout.connect(self._on_reconnect_timeout)
            reconnect_timer.start(PINSerialInterface.RECONNECT_TIME_MS)

    def shutdown(self):
        if self._serial_port_active:
            self._serial_port.close()

    def cache_pin(self, pin):
        self._current_pin_lock.lock()
        self._current_pin = pin
        self._current_pin_lock.unlock()
        self._transmit_current_pin()

    def _transmit_current_pin(self):
        # Check status of serial port
        if not self._serial_port_active:
            self.open_port()
            return

        output_bytes = None

        self._current_pin_lock.lock()
        if self._current_pin is not None:
            output_bytes = PINSerialInterface._get_pin_bytes(self._current_pin)
        self._current_pin_lock.unlock()

        if output_bytes is not None:
            print("Transmitting output bytes: {}".format(output_bytes.hex()))
            self._serial_port.write(output_bytes)

    @pyqtSlot()
    def on_ready_read(self):
        # Check status of serial port
        if not self._serial_port_active:
            self.open_port()
            return

        data = self._serial_port.read(self._serial_port.bytesAvailable())
        if b'\x81' in data:
            self._transmit_current_pin()

    @staticmethod
    def _get_pin_bytes(pin):
        output_bytes = bytearray(PINSerialInterface.PIN_SEQUENCE_START.to_bytes(1, "big"))
        checksum = 0

        # Split pin up into digits. Might not be the best way of doing it, but it's the way I'm doing it for now
        for index, c in enumerate(str(pin).zfill(4)):
            digit = int(c)
            checksum ^= digit
            output_byte = (index << 4) | digit
            output_bytes.append(output_byte)

        output_bytes.append(PINSerialInterface.PIN_SEQUENCE_END | (checksum << 2))

        return output_bytes

    def _on_reconnect_timeout(self):
        # print("Reconnecting to serial port")
        self.open_port()

    def _on_serial_port_error(self, _):
        if self._serial_port is not None:
            self._serial_port.close()
            self._serial_port = None

        self._serial_port_active = False
        self.open_port()
