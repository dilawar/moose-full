import numpy

from moogli.visualization.pipeline.producer import SimulationDataProducer


class TableEventDataProducer(SimulationDataProducer):
    def __init__(self, filename, simulation_variable, downsampler,
                 delimiter, comments="#", skip_header=0, skip_footer=0):
        SimulationDataProducer.__init__(self, simulation_variable, downsampler)
        self._filename = filename
        self._data = numpy.genfromtxt(filename, dtype=numpy.float64,
                                      comments=comments, delimiter=delimiter,
                                      skip_header=skip_header,
                                      skip_footer=skip_footer, names=True)
        self._indices = numpy.zeros((len(self._data.dtype.names),),
                                    dtype=numpy.int)

    def produce(self):
        for i in range(len(self._indices)):
            now = self.clock.now()
            index = self._indices[i]
            visualizable = self._data.dtype.names()
            data = self._data[visualizable]
            column_count = len(data)
            j = index
            while j < column_count and data[j] < now:
                j = j + 1

            if j < column_count and data[j] == now:
                j = j + 1

            values = data[index:j]
            value = self.downsampler(visualizable,
                                     self.clock,
                                     values)
            self.output[visualizable] = value
            self._indices[i] = j

    def rewind(self):
        pass

    def reinit(self):
        pass


class TableContinuousDataProducer(SimulationDataProducer):
    def __init__(self, filename, simulation_variable, downsampler,
                 delimiter, comments="#", skip_header=0, skip_footer=0):
        SimulationDataProducer.__init__(self, simulation_variable, downsampler)
        self._filename = filename
        self._data = numpy.genfromtxt(filename, dtype=numpy.float64,
                                      comments=comments, delimiter=delimiter,
                                      skip_header=skip_header,
                                      skip_footer=skip_footer, names=True)
        self._times = self._data[self._data.dtype.names[0]]
        self._index = 0

    def produce(self):
        j = self._index
        now = self.clock.now()
        column_count = len(self._data)
        while j < column_count and self._times[j] < now:
            j = j + 1
        times = self._times[self._index:j + 1]
        for visualizable in self._data.dtype.names[1:]:
            values = self._data[visualizable][self._index:j + 1]
            value = self.downsampler(visualizable,
                                     self.clock,
                                     times,
                                     values)
            self.output[visualizable] = value

        self._index = j

    def rewind(self):
        pass

    def reinit(self):
        pass
