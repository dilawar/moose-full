from .main import PipelineElement
from .data import SimulationData


class Producer(PipelineElement):

    def __init__(self):
        PipelineElement.__init__(self)
        self.output = None

    def connect(self, mediator):
        self._sink(mediator)

    def disconnect(self, mediator=None):
        if mediator is None:
            self._unsink_all(self._sinks())
        else:
            self._unsink(mediator)

    @property
    def mediators(self):
        return self._sinks()

    @mediators.setter
    def mediators(self, values):
        self._unsink_all(self._sinks())
        self._sink_all(values)

    @property
    def output(self):
        return self._output

    @output.setter
    def output(self, value):
        self._output = value

    def produce(self):
        raise NotImplementedError("")


class SimulationDataProducer(Producer):

    def __init__(self, simulation_variable, downsampler):
        Producer.__init__(self)
        self.output = SimulationData(simulation_variable)
        self.simulation_variable = simulation_variable
        self.downsampler = downsampler

    @property
    def simulation_variable(self):
        return self._simulation_variable

    @simulation_variable.setter
    def simulation_variable(self, value):
        self._simulation_variable = value
        self.output._simulation_variable = value

    @property
    def downsampler(self):
        return self._downsampler

    @downsampler.setter
    def downsampler(self, value):
        self._downsampler = value

    # def _produce_continuous_non_uniform(self):
    #     """
    #     Produces data from a continuous non uniform dataset.
    #     Since simtime increases by a constant amount each tick,
    #     the value of a visualizable is computed by taking a weighted
    #     average of its values at the timepoints before and after/at
    #     the current simulation time.
    #     Loop Invariant => times[index] >= simtime
    #     """
    #     try:
    #         input = self.input[CONTINUOUS_NON_UNIFORM_SERIES]
    #     except:
    #         return
    #     for variable in input:
    #         visualizables = input[variable]["visualizables"]
    #         index = input[variable]["index"]
    #         times = input[variable]["times"]
    #         try:
    #             while times[index] < self.simtime:
    #                 index = index + 1
    #             difference = times[index] - times[index - 1]
    #             left_weight = (times[index] - self.simtime) / difference
    #             right_weight = (self.simtime - times[index - 1]) / difference
    #             for visualizable in visualizables:
    #                 values = visualizables[visualizable]
    #                 left_value = values[index]
    #                 right_value = values[index + 1]
    #                 value = (
    #                     left_weight * left_value + right_weight * right_value
    #                 )
    #                 self.output.assign(variable, visualizable, value)
    #         except IndexError:
    #             for visualizable in visualizables:
    #                 self.output.assign(variable, visualizable, None)

    # def _produce_event_times(self):
    #     input = self.input[SPIKE_TIMES]
    #     for variable in input:
    #         for visualizable in input[variable]:
    #             try:
    #                 spike_times = input[variable][visualizable]
    #                 spike_time = spike_times[spike_times[0]]
    #                 if abs(spike_time - self.simtime) < moogli.epsilon:
    #                     spike_times[0] += 1
    #                     self.output.assign(variable, visualizable, True)
    #                 else:
    #                     self.output.assign(variable, visualizable, False)
    #             except IndexError:
    #                 self.output.assign(variable, visualizable, None)

    # def _produce_spike_series(self):
    #     input = self.input[SPIKE_SERIES]
    #     for variable in input:
    #         for visualizable in input[variable]:
    #             try:
    #                 value = input[variable][visualizable][self.tick]
    #                 self.output.assign(variable, visualizable, value)
    #             except IndexError:
    #                 self.output.assign(variable, visualizable, None)

    # CONTINUOUS_UNIFORM_SERIES
    # CONTINUOUS_NON_UNIFORM_SERIES
    # EVENT_UNIFORM_SERIES
    # EVENT_NON_UNIFORM_SERIES

    # def _produce_continuous_uniform_series(self):
    #     input = self.

    # def _produce_
