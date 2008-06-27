/*
 o---------------------------------------------------------------------o
 |
 | COS Any (root class of all classes)
 |
 o---------------------------------------------------------------------o
 |
 | C Any System
 |
 | Copyright (c) 2006+ Laurent Deniau, laurent.deniau@cern.ch
 |
 | For more information, see:
 | http://cern.ch/laurent.deniau/cos.html
 |
 o---------------------------------------------------------------------o
 |
 | This file is part of the C Any System framework.
 |
 | The C Any System is free software; you can redistribute it and/or
 | modify it under the terms of the GNU Lesser General Public License
 | as published by the Free Software Foundation; either version 3 of
 | the License, or (at your option) any later version.
 |
 | The C Any System is distributed in the hope that it will be
 | useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 | of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 |
 | See <http://www.gnu.org/licenses> for more details.
 |
 o---------------------------------------------------------------------o
 |
 | $Id: Any.c,v 1.1 2008/06/27 16:17:16 ldeniau Exp $
 |
*/

#include <cos/Object.h>
#include <cos/gen/object.h>
#include <cos/gen/message.h>

// -----

makclass(Any,_);

// ----- ownership

defmethod(OBJ, gretain, Any)
  useclass(ExBadValue);

  switch(self->rc) {
  case COS_RC_LAST  : THROW(ExBadValue);
  case COS_RC_STATIC: break;
  case COS_RC_AUTO  : retmethod(gclone(_1));
  default           : ++self->rc;
  }
  retmethod(_1);
endmethod

defmethod(OBJ, grelease, Any)
  switch(self->rc) {
  case COS_RC_STATIC: break;
  case COS_RC_AUTO  : gdeinit(_1)          ; retmethod(Nil);
  case COS_RC_UNIT  : gdealloc(gdeinit(_1)); retmethod(Nil);
  default           : --self->rc;
  }
  retmethod(_1);
endmethod

defmethod(U32, gretainCount, Any)
  retmethod(self->rc);
endmethod

// ----- identity, conversion, coercion

defmethod(OBJ, gisKindOf, Any, Class)
  retmethod(self1->id == self2->Behavior.id || cos_any_isKindOf(_1,self2)
            ? True : False);
endmethod

defmethod(OBJ, gclass, Any)
  retmethod( (OBJ)cos_class_get(self->id) );
endmethod

defmethod(STR, gclassName, Any)
  retmethod( cos_class_get(self->id)->name );
endmethod

/*
defmethod(U32, ghash, Any)
  STATIC_ASSERT(sizeof_OBJ_greater_than_sizeof_size_t,
                sizeof(OBJ) <= sizeof(size_t));
  retmethod( ((size_t)_1 >> 2) * 2621124293u + 1 );
endmethod
*/

// ----- understanding

defmethod(OBJ, gunderstandMessage1, Any, (SEL)msg)
  retmethod(cos_method_understand1(msg, self1->id)
            ? True : False);
endmethod

defmethod(OBJ, gunderstandMessage2, Any, Any, (SEL)msg)
  retmethod(cos_method_understand2(msg, self1->id, self2->id)
            ? True : False);
endmethod

defmethod(OBJ, gunderstandMessage3, Any, Any, Any, (SEL)msg)
  retmethod(cos_method_understand3(msg, self1->id, self2->id, self3->id)
            ? True : False);
endmethod

defmethod(OBJ, gunderstandMessage4, Any, Any, Any, Any, (SEL)msg)
 retmethod(cos_method_understand4(msg, self1->id, self2->id, self3->id,
                                       self4->id)
           ? True : False);
endmethod

defmethod(OBJ, gunderstandMessage5, Any, Any, Any, Any, Any, (SEL)msg)
 retmethod(cos_method_understand5(msg, self1->id, self2->id, self3->id,
                                       self4->id, self5->id)
           ? True : False);
endmethod

// ----- unrecognized

useclass(ExBadMessage);

static void
trace(SEL sel, OBJ obj[])
{
  char buf[128];
  cos_trace("<Any> unrecognized message %s",
            cos_method_fullName(sel,obj,buf,sizeof buf));
}

defmethod(void, gunrecognizedMessage1, Any)
  OBJ obj[1]; obj[0]=_1;
  trace(_sel,obj);
  THROW(ExBadMessage);
endmethod

defmethod(void, gunrecognizedMessage2, Any, Any)
  OBJ obj[2]; obj[0]=_1, obj[1]=_2;
  trace(_sel,obj);
  THROW(ExBadMessage);
endmethod

defmethod(void, gunrecognizedMessage3, Any, Any, Any)
  OBJ obj[3]; obj[0]=_1, obj[1]=_2, obj[2]=_3;
  trace(_sel,obj);
  THROW(ExBadMessage);
endmethod

defmethod(void, gunrecognizedMessage4, Any, Any, Any, Any)
  OBJ obj[4]; obj[0]=_1, obj[1]=_2, obj[2]=_3, obj[3]=_4;
  trace(_sel,obj);
  THROW(ExBadMessage);
endmethod

defmethod(void, gunrecognizedMessage5, Any, Any, Any, Any, Any)
  OBJ obj[5]; obj[0]=_1, obj[1]=_2, obj[2]=_3, obj[3]=_4, obj[4]=_5;
  trace(_sel,obj);
  THROW(ExBadMessage);
endmethod