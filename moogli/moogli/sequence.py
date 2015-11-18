import operator

class Visualizables(list):
    color = { "min"     : (1.0, 0.0, 0.0, 1.0)
            , "max"     : (0.0, 1.0, 0.0, 1.0)
            }
    vm    = { "min"     : -0.08
            , "max"     : 0.02
            }

    def __init__(self):
        list.__init__(self)

    def normalize(self, value, min_value, max_value):
        return ( (value - min_value)
               / (max_value - min_value)
               )

    def clamp(self, value, min_value, max_value):
        if value < min_value: return min_value
        if value > max_value: return max_value
        return value

    def interpolate(self, value, min_color, max_color):
        return map( operator.add
                  , min_color
                  , [ component * value for component in
                      map( operator.sub, max_color, min_color)
                    ]
                  )

    def set_color(self, vms):
        for (vm, visualizable) in zip(vms, self):
            value = self.clamp( self.normalize( vm
                                      , self.vm["min"]
                                      , self.vm["max"]
                                      )
                      , 0.0
                      , 1.0
                      )
            color = self.interpolate(value, self.color["min"], self.color["max"])
            visualizable.set_color(color, 1)
