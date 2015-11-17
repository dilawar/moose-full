class Data(dict):
    def __init__(self):
        dict.__init__(self)

    def visualizables(self):
        return self.keys()

    def register(self, variable):
        self.setdefault(variable, {})

    def unregister(self, variable):
        return self.pop(variable)

    def assign(self, variable, visualizable, value):
        if variable not in self:
            raise KeyError(
                "Register variable {0} before using!".format(variable))
        else:
            self[variable][visualizable] = value

    def obtain(self, variable, visualizable=None):
        if visualizable is None:
            return self[variable]
        else:
            return self[variable][visualizable]


class SimulationData(Data):
    def __init__(self, simulation_variable):
        Data.__init__(self)
        self._simulation_variable = simulation_variable

    @property
    def simulation_variable(self):
        return self._simulation_variable


class VisualizationData(Data):
    def __init__(self, visualization_variable):
        Data.__init__(self)
        self._visualization_variable = visualization_variable

    @property
    def visualization_variable(self):
        return self._visualization_variable
