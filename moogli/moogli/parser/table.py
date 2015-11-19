import re

def parse( filename
         , line_separator_regex
         , column_separator_regex
         , comment_regex
         , callback
         ):
    """
    http://stackoverflow.com/a/23720594/2801664
    """
    comment = re.compile(comment_regex)
    column_separator = re.compile(column_separator_regex)
    table_data = filter( None
                       , re.split(line_separator_regex
                                 , open(filename).read()
                                 )
                       )
    table_data = [ line.strip() for line in table_data
                   if not comment.match(line)]
    table_data = map( lambda line : re.split(column_separator, line)
                    , table_data
                    )
    table_data = map(callback, table_data)
    return table_data

def to_string(value):
    return value.lstrip('"').lstrip("'").rstrip('"').rstrip("'")

def to_float(value):
    return float(value.lstrip('"').lstrip("'").rstrip('"').rstrip("'"))
