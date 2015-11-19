from .main import PipelineElement
from .data import VisualizationData


class Mediator(PipelineElement):

    def __init__(self):
        PipelineElement.__init__(self)
        self._consumers = set()
        self.input = None
        self.output = None

    @property
    def input(self):
        return self._input

    @input.setter
    def input(self, value):
        self._input = value

    @property
    def output(self):
        return self._output

    @output.setter
    def output(self, value):
        self._output = value

    def mediate(self):
        raise NotImplementedError("")

    def connect_consumers(self, *consumers):
        self._sink_all(consumers)

    def disconnect_consumers(self, *consumers):
        self._unsink_all(consumers)

    @property
    def consumers(self):
        return self._sinks()

    @consumers.setter
    def consumers(self, values):
        self._unsink_all()
        self._sink_all(values)

    def connect_producers(self, *producers):
        self._source_all(producers)

    def disconnect_producers(self, *producers):
        self._unsource_all(producers)

    @property
    def producers(self):
        return self._sources

    @producers.setter
    def producers(self, values):
        self._unsource_all()
        self._source_all(values)


class SimulationDataMediator(Mediator):
    def __init__(self, visualization_variable,
                 normalizer, mapper, parameter=0.0):
        Mediator.__init__(self)
        self.visualization_variable = visualization_variable
        self.normalizer = normalizer
        self.mapper = mapper
        self.parameter = parameter
        self.output = VisualizationData(visualization_variable)
        self._parameters = {}

    @property
    def visualization_variable(self):
        return self._visualization_variable

    @visualization_variable.setter
    def visualization_variable(self, value):
        self._visualization_variable = value
        # self.output._visualization_variable = value

    @property
    def mapper(self):
        return self._mapper

    @mapper.setter
    def mapper(self, value):
        self._mapper = value

    @property
    def normalizer(self):
        return self._normalizer

    @normalizer.setter
    def normalizer(self, value):
        self._normalizer = value

    @property
    def parameter(self):
        return self._parameter

    @parameter.setter
    def parameter(self, value):
        self._parameter = value

    def mediate(self):
        map(self._mediate_producer, self.producers)

    def _mediate_producer(self, producer):
        data = producer.output
        for (visualizable, value) in data.items():
            old_param = self._parameters.get(visualizable,
                                             self.parameter)
            new_param = self.normalizer(visualizable,
                                        value, old_param)
            self._parameters[visualizable] = new_param
            self.output[visualizable] = self.mapper(visualizable,
                                                    new_param)

    def rewind(self):
        self.output = {}

    def reinit(self):
        self._parameters = {}
        self._output = {}
