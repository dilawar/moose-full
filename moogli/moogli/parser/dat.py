"""
dat
===

"""
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
