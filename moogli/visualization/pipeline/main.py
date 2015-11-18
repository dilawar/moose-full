from .clock import Clock


class PipelineElement():
    """Some Documentation
    """

    def __init__(self):
        self._sources = set()
        self._sinks = set()
        self.clock  = Clock()

    @property
    def clock(self):
        return self._clock

    @clock.setter
    def clock(self, value):
        self._clock = value

    def _sink(self, element):
        self._sinks.add(element)
        element._sources.add(self)

    def _unsink(self, element):
        self._sinks.remove(element)
        element._sources.remove(self)

    def _sink_all(self, elements):
        map(self._sink, elements)

    def _unsink_all(self, elements):
        map(self._unsink, elements)

    def _sinks(self):
        return tuple(self._sinks)

    def _source(self, element):
        self._sources.add(element)
        element._sinks.add(self)

    def _unsource(self, element):
        self._sources.remove(element)
        element._sinks.remove(self)

    def _source_all(self, elements):
        map(self._source, elements)

    def _unsource_all(self, elements):
        map(self._unsource, elements)

    def _sources(self):
        return tuple(self._sources)

    def rewind(self):
        raise NotImplementedError("")

    def reinit(self):
        raise NotImplementedError("")


class Pipeline():
    def __init__(self, producer, mediator, consumer):
        self.producer = producer
        self.mediator = mediator
        self.consumer = consumer
        producer.connect(mediator)
        mediator.connect_consumers(consumer)
