#ifndef COS_GEN_CONTAINER_H
#define COS_GEN_CONTAINER_H

/*
 o---------------------------------------------------------------------o
 |
 | COS generics for containers
 |
 o---------------------------------------------------------------------o
 |
 | C Object System
 |
 | Copyright (c) 2006+ Laurent Deniau, laurent.deniau*cern.ch
 |
 | For more information, see:
 | http://cern.ch/laurent.deniau/cos.html
 |
 o---------------------------------------------------------------------o
 |
 | This file is part of the C Object System framework.
 |
 | The C Object System is free software; you can redistribute it and/or
 | modify it under the terms of the GNU Lesser General Public License
 | as published by the Free Software Foundation; either version 3 of
 | the License, or (at your option) any later version.
 |
 | The C Object System is distributed in the hope that it will be
 | useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 | of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 |
 | See <http://www.gnu.org/licenses> for more details.
 |
 o---------------------------------------------------------------------o
 |
 | $Id: container.h,v 1.2 2008/11/10 08:00:42 ldeniau Exp $
 |
*/

#ifndef COS_OBJECT_H
#error "COS: missing #include <cos/Object.h>"
#endif 

// generic accessors
defgeneric(OBJ, gput   , to, obj);
defgeneric(OBJ, gget   , from);
defgeneric(OBJ, gdrop  , from);
defgeneric(OBJ, gupdate, to, obj);

// generic accessors with location
defgeneric(OBJ, gputAt   , to, obj, at);
defgeneric(OBJ, ggetAt   , from, at);
defgeneric(OBJ, gdropAt  , from, at);
defgeneric(OBJ, gupdateAt, to, obj, at);

// stack-like accessors
defgeneric(OBJ, gpush, to, obj);     // alias for gput
defgeneric(OBJ, gtop , from);        // alias for gget
defgeneric(OBJ, gpop , from);        // alias for gdrop

// seq-like accessor
defgeneric(OBJ, gappend, to  , obj);
defgeneric(OBJ, ginsert, to  , obj);
defgeneric(OBJ, gremove, from, obj);
defgeneric(OBJ, gconcat, to  , obj);
defgeneric(OBJ, glast  , from);

// raw data (copy) accessors
defgeneric(void, ggetValue  , from, what);
defgeneric(void, ggetValueAt, from, what, at);

#endif // COS_GEN_CONTAINER_H
