class CharLCDController(object):
    SCROLL_DELAY_MS = 350
    SCROLL_END_HOLD_MS = 700
    CHAR_WIDTH = 16
    WINDOW_SIZE = 12

    def __init__(self):
        self._padding = int((CharLCDController.CHAR_WIDTH - CharLCDController.WINDOW_SIZE) / 2)
        self._display_string = ""
        self._current_display_home = 0
        self._requires_scroll = False
        self._next_scroll_time = 0

    def put_string(self, output_string):
        self._display_string = output_string
        self._current_display_home = 0
        self._next_scroll_time = 0
        self._requires_scroll = (len(output_string) > CharLCDController.WINDOW_SIZE)
        if not self._requires_scroll:
            self.paint_string_window(output_string)

    def do_paint(self, output_string):
        raise NotImplementedError()

    def paint_string_window(self, output_string):
        display_string = ""

        # Left padding
        for i in range(self._padding):
            display_string += " "

        display_string += output_string

        # Right padding
        right_padding = (CharLCDController.CHAR_WIDTH - (self._padding + len(output_string)))
        for i in range(right_padding):
            display_string += " "

        self.do_paint(display_string)

    def update(self, current_time):
        if self._requires_scroll and (current_time >= self._next_scroll_time):
            # Do scroll
            start_pos = self._current_display_home
            end_pos = start_pos + CharLCDController.WINDOW_SIZE
            if end_pos >= len(self._display_string):
                # Reached the end of the string, reset
                end_pos = len(self._display_string)

            if start_pos == end_pos:
                self._current_display_home = 0
                self._next_scroll_time = (current_time + CharLCDController.SCROLL_END_HOLD_MS)
            else:
                self._current_display_home += 1
                self._next_scroll_time = (current_time + CharLCDController.SCROLL_DELAY_MS)

            output_string = self._display_string[start_pos:end_pos]
            self.paint_string_window(output_string)
