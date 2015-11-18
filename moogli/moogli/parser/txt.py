import .table

LINE_SEPARATOR_REGEX        = r"[\n\r]*"    # Windows or Unix style newline character
COLUMN_SEPARATOR_REGEX      = r"[\t ]*,?[\t ]*"    # Any number of tabs or spaces followed by
COMMENT_REGEX               = r"^[ \t]*#"   # Any number of space characters followed by a tab

def parse(filename):
    return table.parse( filename
                      , LINE_SEPARATOR_REGEX
                      , COLUMN_SEPARATOR_REGEX
                      , COMMENT_REGEX
                      , lambda line: ( table.to_string(line[0])
                                     , map(table.to_float, line[1:])
                                     )
                      )

def create_morphology(data):
    if len(data[0]) is 7:
        return map( lambda line: ( table.to_string(line[0])
                                 , table.to_string(line[1])
                                 , table.to_string(line[2])
                                 , table.to_float(line[3])
                                 , table.to_float(line[4])
                                 , table.to_float(line[5])
                                 , table.to_float(line[6])
                                 )
                  , data
                  )
    elif len(data[0]) is 6:
        return map( lambda line: ( table.to_string(line[0])
                                 , table.to_string(line[1])
                                 , table.to_float(line[2])
                                 , table.to_float(line[3])
                                 , table.to_float(line[4])
                                 , table.to_float(line[5])
                                 )
                  , data
                  )

def create_simulationdata(data):
    return map( lambda line: ( table.to_string(line[0])
                             , map(table.to_float, line[1:])
                             )
              , data
              )

class Simulation
