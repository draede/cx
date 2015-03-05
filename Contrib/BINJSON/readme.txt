BINJSON1
========

HEADER:

MAGIC    = 8 bytes
ENCODING = 4 bytes (0 for now; 0 = no encoding)

FOOTER:
UINT32   = 4 bytes (xxHash of data - header is not used)

ENTRY:

NULL, BOOL, ENDROOTOBJECT, ENDROOTARRAY, ENDOBJECT, ENDARRAY:
UINT8  = entry type - see Spec::TYPE *

INT, REAL:
UINT32 = name size (for childs of objects)
?      = name (for childs of objects)
?......= data

STRING, BLOB:
UINT32 = name size (for childs of objects)
?      = name (for childs of objects)
UINT32 = data size
?......= data

BEGINROOTOBJECT, BEGINROOTARRAY, BEGINOBJECT, BEGINARRAY:
UINT16 = name size (for childs of objects)
?      = name (for childs of objects)