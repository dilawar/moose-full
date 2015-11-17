from .main import PipelineElement


class Consumer(PipelineElement):
   #  __metaclass__ = ABCMeta

    def __init__(self):
        PipelineElement.__init__(self)
        self.input = None
        self._mediators = set()

    @property
    def input(self):
        return self._input

    @input.setter
    def input(self, value):
        self._input = value

    @property
    def mediators(self):
        return tuple(self._sources)

    def consume(self):
        raise NotImplementedError("Implement this function")


class SimulationDataConsumer(Consumer):
    def __init__(self):
        Consumer.__init__(self)
