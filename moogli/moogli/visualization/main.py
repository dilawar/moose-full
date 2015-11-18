from .pipeline import SimulationDataConsumer
from moogli.core import NetworkViewer
from PyQt4.QtGui import QWidget, QGridLayout
from PyQt4.QtCore import QTimer


class NetworkVisualizer(SimulationDataConsumer, NetworkViewer):

    def __init__(self, network):
        SimulationDataConsumer.__init__(self)
        NetworkViewer.__init__(self, network)

    def consume(self):
        for mediator in self.mediators:
            for (visualizable, value) in mediator.output.items():
                self.network.visualizables[visualizable].set_color(value)
                # print self.network.visualizables[visualizable]
        self.update()

    def reinit(self):
        pass

    def rewind(self):
        pass


class NetworkVisualizationEnvironment(QWidget):
    def __init__(self, clock):
        QWidget.__init__(self)
        self.clock = clock
        self.setLayout(QGridLayout())
        self._pipelines = set()
        self._done = False
        self._pause = False

    def start(self):
        self._done = False
        self.prelude(self)
        self.update()
        self.run()

    def pause(self):
        self._pause = True

    def resume(self):
        self._pause = False
        self.run()

    def stop(self):
        self._done = True
        self.postlude(self)

    def run(self):
        self.advance()
        print self.clock.now()
#        self.interlude(self)
#        self.update()
#        if self._done:
#            return
#        if self._pause:
#            return
        QTimer.singleShot(1000, self.run)

    @property
    def clock(self):
        return self._clock

    @clock.setter
    def clock(self, value):
        self._clock = value

    def insert(self, pipeline):
        self._pipelines.add(pipeline)
        pipeline.producer.clock = self.clock
        pipeline.mediator.clock = self.clock
        pipeline.consumer.clock = self.clock

    def remove(self, pipeline):
        self._pipelines.remove(pipeline)

    def pipelines(self):
        return tuple(self._pipelines)

    def advance(self):
        self.clock.advance()
        self._advance_pipeline()

    def _advance_pipeline(self):
        producers = set()
        mediators = set()
        consumers = set()
        for pipeline in self.pipelines():
            producers.add(pipeline.producer)
            mediators.add(pipeline.mediator)
            consumers.add(pipeline.consumer)
        map(lambda producer: producer.produce(), producers)
        map(lambda mediator: mediator.mediate(), mediators)
        map(lambda consumer: consumer.consume(), consumers)

    def reinit(self):
        pass

    def rewind(self):
        pass
