class Clock():
    """
    """

    def __init__(self, begin=1.0, end=0.0, interval=1.0):
        self.begin = begin
        self.end = end
        self.interval = interval
        self._tick = -1
        self._now = 1.0

    @property
    def begin(self):
        return self._begin

    @begin.setter
    def begin(self, value):
        self._begin = value

    @property
    def end(self):
        return self._end

    @end.setter
    def end(self, value):
        self._end = value

    @property
    def interval(self):
        return self._interval

    @interval.setter
    def interval(self, value):
        self._interval = value

    def tick(self):
        return self._tick

    def now(self):
        return self._now

    def advance(self):
        self._tick = self._tick + 1
        self._now = self.begin + self.tick() * self.interval
