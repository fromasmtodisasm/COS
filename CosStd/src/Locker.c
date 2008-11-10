/*
 o---------------------------------------------------------------------o
 |
 | COS Locker (proxy)
 |
 o---------------------------------------------------------------------o
 |
 | C Object System
 |
 | Copyright (c) 2006+ Laurent Deniau, laurent.deniau@cern.ch
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
 | $Id: Locker.c,v 1.3 2008/11/10 08:00:42 ldeniau Exp $
 |
*/

#include <cos/Object.h>
#include <cos/Locker.h>
#include <cos/gen/object.h>
#include <cos/gen/message.h>

// -----

makclass(Locker,Proxy);

// -----

#if !COS_POSIX    // ----------------------------------------------

defmethod(OBJ, galloc, pmLocker)
  retmethod(_1);
endmethod

defmethod(OBJ, ginitWith, pmLocker, Any)
  retmethod(_2);
endmethod

#else // if COS_POSIX ---------------------------------------------

// ----- ctor/dtor

defmethod(OBJ, ginitWith, Locker, Any)
  next_method(self,self2);
  pthread_mutex_init(&self->lock,0);
endmethod

defmethod(OBJ, gdeinit, Locker)
  pthread_mutex_destroy(&self->lock);
  next_method(self);
endmethod

defmethod(OBJ, gclear, Locker)
  pthread_mutex_unlock(&self->lock);
  retmethod(_1);
endmethod

#undef  SORT
#undef  LOCK
#undef  CHCK
#define SORT(l1,l2) if (l1 >  l2) { struct Locker *tmp=l1; l1=l2, l2=tmp; }
#define LOCK(l1,l2) if (l1 != l2) { lock(l2); }
#define CHCK(l)     if (*(OBJ*)_ret == l->Proxy.obj) { *(OBJ*)_ret = (OBJ)l; }

// ----- rank1-{lock,unlock,chkret}

static inline void
lock(struct Locker *l)
{
  pthread_mutex_lock(&l->lock);
}

static inline void
unlock(struct Locker *l)
{
  pthread_mutex_unlock(&l->lock);
}

static inline void
chkret(struct Locker *l1, SEL sel, void *_ret)
{
  if (COS_GEN_ORET(sel)) CHCK(l1);
}

// ----- rank2-{lock,unlock,chkret}

static inline void
lock2(struct Locker *l1, struct Locker *l2)
{
  SORT(l1,l2);
  lock(l1);
  LOCK(l1,l2);
}

static inline void
unlock2(struct Locker *l1, struct Locker *l2)
{
  unlock(l1);
  if (l1 != l2) unlock(l2);
}

static inline void
chkret2(struct Locker *l1, struct Locker *l2, SEL sel, void *_ret)
{
  if (COS_GEN_ORET(sel)) {
    CHCK(l1) else CHCK(l2);
  }
}

// ----- rank3-{lock,unlock,chkret}

static inline void
lock3(struct Locker *l1, struct Locker *l2, struct Locker *l3)
{
  SORT(l1,l2);
  SORT(l1,l3);
  SORT(l2,l3);
  lock(l1);
  LOCK(l1,l2);
  LOCK(l2,l3);
}

static inline void
unlock3(struct Locker *l1, struct Locker *l2, struct Locker *l3)
{
  unlock(l1);
  if (l1 != l2) unlock(l2);
  if (l1 != l3 && l2 != l3) unlock(l3);
}

static inline void
chkret3(struct Locker *l1, struct Locker *l2, struct Locker *l3,
        SEL sel, void *_ret)
{
  if (COS_GEN_ORET(sel)) {
    CHCK(l1) else CHCK(l2) else CHCK(l3);
  }
}

// ----- rank4-{lock,unlock,chkret}

static inline void
lock4(struct Locker *l1, struct Locker *l2, struct Locker *l3,
      struct Locker *l4)
{ // Batcher's Merge-Exchange N=4 (5 compare, 3 parallel steps)
  SORT(l1,l3); SORT(l2,l4);
  SORT(l1,l2); SORT(l3,l4);
  SORT(l2,l3);
  lock(l1);
  LOCK(l1,l2); LOCK(l3,l4);
  LOCK(l2,l3);
}

static inline void
unlock4(struct Locker *l1, struct Locker *l2, struct Locker *l3,
        struct Locker *l4)
{
  unlock(l1);
  if (l1 != l2) unlock(l2);
  if (l1 != l3 && l2 != l3) unlock(l3);
  if (l1 != l4 && l2 != l4 && l3 != l4) unlock(l4);
}

static inline void
chkret4(struct Locker *l1, struct Locker *l2, struct Locker *l3,
        struct Locker *l4, SEL sel, void *_ret)
{
  if (COS_GEN_ORET(sel)) {
    CHCK(l1) else CHCK(l2) else CHCK(l3) else CHCK(l4);
  }
}

// ----- rank5-{lock,unlock,chkret}

static inline void
lock5(struct Locker *l1, struct Locker *l2, struct Locker *l3,
      struct Locker *l4, struct Locker *l5)
{ // Batcher's Merge-Exchange N=5 (9 compare, 5 parallel steps)
  SORT(l1,l5); SORT(l2,l4);
  SORT(l1,l3);
  SORT(l3,l5); SORT(l1,l2);
  SORT(l3,l4); SORT(l2,l5);
  SORT(l2,l3); SORT(l4,l5);
  lock(l1);
  LOCK(l1,l2); LOCK(l3,l4);
  LOCK(l2,l3); LOCK(l4,l5);
}

static inline void
unlock5(struct Locker *l1, struct Locker *l2, struct Locker *l3,
        struct Locker *l4, struct Locker *l5)
{
  unlock(l1);
  if (l1 != l2) unlock(l2);
  if (l1 != l3 && l2 != l3) unlock(l3);
  if (l1 != l4 && l2 != l4 && l3 != l4) unlock(l4);
  if (l1 != l5 && l2 != l5 && l3 != l5 && l4 != l5) unlock(l5);
}
 
static inline void
chkret5(struct Locker *l1, struct Locker *l2, struct Locker *l3,
        struct Locker *l4, struct Locker *l5, SEL sel, void *_ret)
{
  if (COS_GEN_ORET(sel)) {
    CHCK(l1) else CHCK(l2) else CHCK(l3) else CHCK(l4) else CHCK(l5);
  }
}

// ----- error/forward (1+3+7+15+31=57 cases!)

// rank 1 (2^1-1=1 cases)
defmethod(void, gunrecognizedMessage1, Locker)
  EPRT(_1,gclear);
  lock(self1);
  next_method(self1);
  unlock(self1);
  UNPRT(_1);
endmethod

// rank 2 (2^2-1=3 cases)
defmethod(void, gunrecognizedMessage2, Locker, Any)
  EPRT(_1,gclear);
  lock(self1);
  next_method(self1,self2);
  unlock(self1);
  UNPRT(_1);
endmethod

defmethod(void, gunrecognizedMessage2, Any, Locker)
  EPRT(_2,gclear);
  lock(self2);
  next_method(self1,self2);
  unlock(self2);
  UNPRT(_2);
endmethod

defmethod(void, gunrecognizedMessage2, Locker, Locker)
  EPRT(_1,gclear); EPRT(_2,gclear);
  lock2(self1,self2);
  next_method(self1,self2);
  unlock2(self1,self2);
  UNPRT(_1);
endmethod

// rank 3 (2^3-1=7 cases)
defmethod(void, gunrecognizedMessage3, Locker, Any, Any)
  EPRT(_1,gclear);
  lock(self1);
  next_method(self1,self2,self3);
  unlock(self1);
  UNPRT(_1);
endmethod

defmethod(void, gunrecognizedMessage3, Any, Locker, Any)
  EPRT(_2,gclear);
  lock(self2);
  next_method(self1,self2,self3);
  unlock(self2);
  UNPRT(_2);
endmethod

defmethod(void, gunrecognizedMessage3, Any, Any, Locker)
  EPRT(_3,gclear);
  lock(self3);
  next_method(self1,self2,self3);
  unlock(self3);
  UNPRT(_3);
endmethod

defmethod(void, gunrecognizedMessage3, Locker, Locker, Any)
  EPRT(_1,gclear); EPRT(_2,gclear);
  lock2(self1,self2);
  next_method(self1,self2,self3);
  unlock2(self1,self2);
  UNPRT(_1);
endmethod

defmethod(void, gunrecognizedMessage3, Locker, Any, Locker)
  EPRT(_1,gclear); EPRT(_3,gclear);
  lock2(self1,self3);
  next_method(self1,self2,self3);
  unlock2(self1,self3);
  UNPRT(_1);
endmethod

defmethod(void, gunrecognizedMessage3, Any, Locker, Locker)
  EPRT(_2,gclear); EPRT(_3,gclear);
  lock2(self2,self3);
  next_method(self1,self2,self3);
  unlock2(self2,self3);
  UNPRT(_2);
endmethod

defmethod(void, gunrecognizedMessage3, Locker, Locker, Locker)
  EPRT(_1,gclear); EPRT(_2,gclear); EPRT(_3,gclear);
  lock3(self1,self2,self3);
  next_method(self1,self2,self3);
  unlock3(self1,self2,self3);
  UNPRT(_1);
endmethod

// rank 4 (2^4-1=15 cases)
defmethod(void, gunrecognizedMessage4, Locker, Any, Any, Any)
  EPRT(_1,gclear);
  lock(self1);
  next_method(self1,self2,self3,self4);
  unlock(self1);
  UNPRT(_1);
endmethod

defmethod(void, gunrecognizedMessage4, Any, Locker, Any, Any)
  EPRT(_2,gclear);
  lock(self2);
  next_method(self1,self2,self3,self4);
  unlock(self2);
  UNPRT(_2);
endmethod

defmethod(void, gunrecognizedMessage4, Any, Any, Locker, Any)
  EPRT(_3,gclear);
  lock(self3);
  next_method(self1,self2,self3,self4);
  unlock(self3);
  UNPRT(_3);
endmethod

defmethod(void, gunrecognizedMessage4, Any, Any, Any, Locker)
  EPRT(_4,gclear);
  lock(self4);
  next_method(self1,self2,self3,self4);
  unlock(self4);
  UNPRT(_4);
endmethod

defmethod(void, gunrecognizedMessage4, Locker, Locker, Any, Any)
  EPRT(_1,gclear); EPRT(_2,gclear);
  lock2(self1,self2);
  forward_message(self1->Proxy.obj,self2->Proxy.obj,_3,_4);
  chkret2(self1,self2,_sel,_ret);
  unlock2(self1,self2);
  UNPRT(_1);
endmethod

defmethod(void, gunrecognizedMessage4, Locker, Any, Locker, Any)
  EPRT(_1,gclear); EPRT(_3,gclear);
  lock2(self1,self3);
  forward_message(self1->Proxy.obj,_2,self3->Proxy.obj,_4);
  chkret2(self1,self3,_sel,_ret);
  unlock2(self1,self3);
  UNPRT(_1);
endmethod

defmethod(void, gunrecognizedMessage4, Locker, Any, Any, Locker)
  EPRT(_1,gclear); EPRT(_4,gclear);
  lock2(self1,self4);
  forward_message(self1->Proxy.obj,_2,_3,self4->Proxy.obj);
  chkret2(self1,self4,_sel,_ret);
  unlock2(self1,self4);
  UNPRT(_1);
endmethod

defmethod(void, gunrecognizedMessage4, Any, Locker, Locker, Any)
  EPRT(_2,gclear); EPRT(_3,gclear);
  lock2(self2,self3);
  forward_message(_1,self2->Proxy.obj,self3->Proxy.obj,_4);
  chkret2(self2,self3,_sel,_ret);
  unlock2(self2,self3);
  UNPRT(_2);
endmethod

defmethod(void, gunrecognizedMessage4, Any, Locker, Any, Locker)
  EPRT(_2,gclear); EPRT(_4,gclear);
  lock2(self2,self4);
  forward_message(_1,self2->Proxy.obj,_3,self4->Proxy.obj);
  chkret2(self2,self4,_sel,_ret);
  unlock2(self2,self4);
  UNPRT(_2);
endmethod

defmethod(void, gunrecognizedMessage4, Any, Any, Locker, Locker)
  EPRT(_3,gclear); EPRT(_4,gclear);
  lock2(self3,self4);
  forward_message(_1,_2,self3->Proxy.obj,self4->Proxy.obj);
  chkret2(self3,self4,_sel,_ret);
  unlock2(self3,self4);
  UNPRT(_3);
endmethod

defmethod(void, gunrecognizedMessage4, Locker, Locker, Locker, Any)
  EPRT(_1,gclear); EPRT(_2,gclear); EPRT(_3,gclear);
  lock3(self1,self2,self3);
  forward_message(self1->Proxy.obj,self2->Proxy.obj,self3->Proxy.obj,_4);
  chkret3(self1,self2,self3,_sel,_ret);
  unlock3(self1,self2,self3);
  UNPRT(_1);
endmethod

defmethod(void, gunrecognizedMessage4, Locker, Locker, Any, Locker)
  EPRT(_1,gclear); EPRT(_2,gclear); EPRT(_4,gclear);
  lock3(self1,self2,self4);
  forward_message(self1->Proxy.obj,self2->Proxy.obj,_3,self4->Proxy.obj);
  chkret3(self1,self2,self4,_sel,_ret);
  unlock3(self1,self2,self4);
  UNPRT(_1);
endmethod

defmethod(void, gunrecognizedMessage4, Locker, Any, Locker, Locker)
  EPRT(_1,gclear); EPRT(_3,gclear); EPRT(_4,gclear);
  lock3(self1,self3,self4);
  forward_message(self1->Proxy.obj,_2,self3->Proxy.obj,self4->Proxy.obj);
  chkret3(self1,self3,self4,_sel,_ret);
  unlock3(self1,self3,self4);
  UNPRT(_1);
endmethod

defmethod(void, gunrecognizedMessage4, Any, Locker, Locker, Locker)
  EPRT(_2,gclear); EPRT(_3,gclear); EPRT(_4,gclear);
  lock3(self2,self3,self4);
  forward_message(_1,self2->Proxy.obj,self3->Proxy.obj,self4->Proxy.obj);
  chkret3(self2,self3,self4,_sel,_ret);
  unlock3(self2,self3,self4);
  UNPRT(_2);
endmethod

defmethod(void, gunrecognizedMessage4, Locker, Locker, Locker, Locker)
  EPRT(_1,gclear); EPRT(_2,gclear); EPRT(_3,gclear); EPRT(_4,gclear);
  lock4(self1,self2,self3,self4);
  forward_message(self1->Proxy.obj,self2->Proxy.obj,self3->Proxy.obj,self4->Proxy.obj);
  chkret4(self1,self2,self3,self4,_sel,_ret);
  unlock4(self1,self2,self3,self4);
  UNPRT(_1);
endmethod

// rank 5 (2^5-1=31 cases)
defmethod(void, gunrecognizedMessage5, Locker, Any, Any, Any, Any)
  EPRT(_1,gclear);
  lock(self1);
  next_method(self1,self2,self3,self4,self5);
  unlock(self1);
  UNPRT(_1);
endmethod

defmethod(void, gunrecognizedMessage5, Any, Locker, Any, Any, Any)
  EPRT(_2,gclear);
  lock(self2);
  next_method(self1,self2,self3,self4,self5);
  unlock(self2);
  UNPRT(_2);
endmethod

defmethod(void, gunrecognizedMessage5, Any, Any, Locker, Any, Any)
  EPRT(_3,gclear);
  lock(self3);
  next_method(self1,self2,self3,self4,self5);
  unlock(self3);
  UNPRT(_3);
endmethod

defmethod(void, gunrecognizedMessage5, Any, Any, Any, Locker, Any)
  EPRT(_4,gclear);
  lock(self4);
  next_method(self1,self2,self3,self4,self5);
  unlock(self4);
  UNPRT(_4);
endmethod

defmethod(void, gunrecognizedMessage5, Any, Any, Any, Any, Locker)
  EPRT(_5,gclear);
  lock(self5);
  next_method(self1,self2,self3,self4,self5);
  unlock(self5);
  UNPRT(_5);
endmethod

defmethod(void, gunrecognizedMessage5, Locker, Locker, Any, Any, Any)
  EPRT(_1,gclear); EPRT(_2,gclear);
  lock2(self1,self2);
  forward_message(self1->Proxy.obj,self2->Proxy.obj,_3,_4,_5);
  chkret2(self1,self2,_sel,_ret);
  unlock2(self1,self2);
  UNPRT(_1);
endmethod

defmethod(void, gunrecognizedMessage5, Locker, Any, Locker, Any, Any)
  EPRT(_1,gclear); EPRT(_3,gclear);
  lock2(self1,self3);
  forward_message(self1->Proxy.obj,_2,self3->Proxy.obj,_4,_5);
  chkret2(self1,self3,_sel,_ret);
  unlock2(self1,self3);
  UNPRT(_1);
endmethod

defmethod(void, gunrecognizedMessage5, Locker, Any, Any, Locker, Any)
  EPRT(_1,gclear); EPRT(_4,gclear);
  lock2(self1,self4);
  forward_message(self1->Proxy.obj,_2,_3,self4->Proxy.obj,_5);
  chkret2(self1,self4,_sel,_ret);
  unlock2(self1,self4);
  UNPRT(_1);
endmethod

defmethod(void, gunrecognizedMessage5, Locker, Any, Any, Any, Locker)
  EPRT(_1,gclear); EPRT(_5,gclear);
  lock2(self1,self5);
  forward_message(self1->Proxy.obj,_2,_3,_4,self5->Proxy.obj);
  chkret2(self1,self5,_sel,_ret);
  unlock2(self1,self5);
  UNPRT(_1);
endmethod

defmethod(void, gunrecognizedMessage5, Any, Locker, Locker, Any, Any)
  EPRT(_2,gclear); EPRT(_3,gclear);
  lock2(self2,self3);
  forward_message(_1,self2->Proxy.obj,self3->Proxy.obj,_4,_5);
  chkret2(self2,self3,_sel,_ret);
  unlock2(self2,self3);
  UNPRT(_2);
endmethod

defmethod(void, gunrecognizedMessage5, Any, Locker, Any, Locker, Any)
  EPRT(_2,gclear); EPRT(_4,gclear);
  lock2(self2,self4);
  forward_message(_1,self2->Proxy.obj,_3,self4->Proxy.obj,_5);
  chkret2(self2,self4,_sel,_ret);
  unlock2(self2,self4);
  UNPRT(_2);
endmethod

defmethod(void, gunrecognizedMessage5, Any, Locker, Any, Any, Locker)
  EPRT(_2,gclear); EPRT(_5,gclear);
  lock2(self2,self5);
  forward_message(_1,self2->Proxy.obj,_3,_4,self5->Proxy.obj);
  chkret2(self2,self5,_sel,_ret);
  unlock2(self2,self5);
  UNPRT(_2);
endmethod

defmethod(void, gunrecognizedMessage5, Any, Any, Locker, Locker, Any)
  EPRT(_3,gclear); EPRT(_4,gclear);
  lock2(self3,self4);
  forward_message(_1,_2,self3->Proxy.obj,self4->Proxy.obj,_5);
  chkret2(self3,self4,_sel,_ret);
  unlock2(self3,self4);
  UNPRT(_3);
endmethod

defmethod(void, gunrecognizedMessage5, Any, Any, Locker, Any, Locker)
  EPRT(_3,gclear); EPRT(_5,gclear);
  lock2(self3,self5);
  forward_message(_1,_2,self3->Proxy.obj,_4,self5->Proxy.obj);
  chkret2(self3,self5,_sel,_ret);
  unlock2(self3,self5);
  UNPRT(_3);
endmethod

defmethod(void, gunrecognizedMessage5, Any, Any, Any, Locker, Locker)
  EPRT(_4,gclear); EPRT(_5,gclear);
  lock2(self4,self5);
  forward_message(_1,_2,_3,self4->Proxy.obj,self5->Proxy.obj);
  chkret2(self4,self5,_sel,_ret);
  unlock2(self4,self5);
  UNPRT(_4);
endmethod

defmethod(void, gunrecognizedMessage5, Locker, Locker, Locker, Any, Any)
  EPRT(_1,gclear); EPRT(_2,gclear); EPRT(_3,gclear);
  lock3(self1,self2,self3);
  forward_message(self1->Proxy.obj,self2->Proxy.obj,self3->Proxy.obj,_4,_5);
  chkret3(self1,self2,self3,_sel,_ret);
  unlock3(self1,self2,self3);
  UNPRT(_1);
endmethod

defmethod(void, gunrecognizedMessage5, Locker, Locker, Any, Locker, Any)
  EPRT(_1,gclear); EPRT(_2,gclear); EPRT(_4,gclear);
  lock3(self1,self2,self4);
  forward_message(self1->Proxy.obj,self2->Proxy.obj,_3,self4->Proxy.obj,_5);
  chkret3(self1,self2,self4,_sel,_ret);
  unlock3(self1,self2,self4);
  UNPRT(_1);
endmethod

defmethod(void, gunrecognizedMessage5, Locker, Locker, Any, Any, Locker)
  EPRT(_1,gclear); EPRT(_2,gclear); EPRT(_5,gclear);
  lock3(self1,self2,self5);
  forward_message(self1->Proxy.obj,self2->Proxy.obj,_3,_4,self5->Proxy.obj);
  chkret3(self1,self2,self5,_sel,_ret);
  unlock3(self1,self2,self5);
  UNPRT(_1);
endmethod

defmethod(void, gunrecognizedMessage5, Locker, Any, Locker, Locker, Any)
  EPRT(_1,gclear); EPRT(_3,gclear); EPRT(_4,gclear);
  lock3(self1,self3,self4);
  forward_message(self1->Proxy.obj,_2,self3->Proxy.obj,self4->Proxy.obj,_5);
  chkret3(self1,self3,self4,_sel,_ret);
  unlock3(self1,self3,self4);
  UNPRT(_1);
endmethod

defmethod(void, gunrecognizedMessage5, Locker, Any, Locker, Any, Locker)
  EPRT(_1,gclear); EPRT(_3,gclear); EPRT(_5,gclear);
  lock3(self1,self3,self5);
  forward_message(self1->Proxy.obj,_2,self3->Proxy.obj,_4,self5->Proxy.obj);
  chkret3(self1,self3,self5,_sel,_ret);
  unlock3(self1,self3,self5);
  UNPRT(_1);
endmethod

defmethod(void, gunrecognizedMessage5, Locker, Any, Any, Locker, Locker)
  EPRT(_1,gclear); EPRT(_4,gclear); EPRT(_5,gclear);
  lock3(self1,self4,self5);
  forward_message(self1->Proxy.obj,_2,_3,self4->Proxy.obj,self5->Proxy.obj);
  chkret3(self1,self4,self5,_sel,_ret);
  unlock3(self1,self4,self5);
  UNPRT(_1);
endmethod

defmethod(void, gunrecognizedMessage5, Any, Locker, Locker, Locker, Any)
  EPRT(_2,gclear); EPRT(_3,gclear); EPRT(_4,gclear);
  lock3(self2,self3,self4);
  forward_message(_1,self2->Proxy.obj,self3->Proxy.obj,self4->Proxy.obj,_5);
  chkret3(self2,self3,self4,_sel,_ret);
  unlock3(self2,self3,self4);
  UNPRT(_2);
endmethod

defmethod(void, gunrecognizedMessage5, Any, Locker, Locker, Any, Locker)
  EPRT(_2,gclear); EPRT(_3,gclear); EPRT(_5,gclear);
  lock3(self2,self3,self5);
  forward_message(_1,self2->Proxy.obj,self3->Proxy.obj,_4,self5->Proxy.obj);
  chkret3(self2,self3,self5,_sel,_ret);
  unlock3(self2,self3,self5);
  UNPRT(_2);
endmethod

defmethod(void, gunrecognizedMessage5, Any, Locker, Any, Locker, Locker)
  EPRT(_2,gclear); EPRT(_4,gclear); EPRT(_5,gclear);
  lock3(self2,self4,self5);
  forward_message(_1,self2->Proxy.obj,_3,self4->Proxy.obj,self5->Proxy.obj);
  chkret3(self2,self4,self5,_sel,_ret);
  unlock3(self2,self4,self5);
  UNPRT(_2);
endmethod

defmethod(void, gunrecognizedMessage5, Any, Any, Locker, Locker, Locker)
  EPRT(_3,gclear); EPRT(_4,gclear); EPRT(_5,gclear);
  lock3(self3,self4,self5);
  forward_message(_1,_2,self3->Proxy.obj,self4->Proxy.obj,self5->Proxy.obj);
  chkret3(self3,self4,self5,_sel,_ret);
  unlock3(self3,self4,self5);
  UNPRT(_3);
endmethod

defmethod(void, gunrecognizedMessage5, Locker, Locker, Locker, Locker, Any)
  EPRT(_1,gclear); EPRT(_2,gclear); EPRT(_3,gclear); EPRT(_4,gclear);
  lock4(self1,self2,self3,self4);
  forward_message(self1->Proxy.obj,self2->Proxy.obj,self3->Proxy.obj,self4->Proxy.obj,_5);
  chkret4(self1,self2,self3,self4,_sel,_ret);
  unlock4(self1,self2,self3,self4);
  UNPRT(_1);
endmethod

defmethod(void, gunrecognizedMessage5, Locker, Locker, Locker, Any, Locker)
  EPRT(_1,gclear); EPRT(_2,gclear); EPRT(_3,gclear); EPRT(_5,gclear);
  lock4(self1,self2,self3,self5);
  forward_message(self1->Proxy.obj,self2->Proxy.obj,self3->Proxy.obj,_4,self5->Proxy.obj);
  chkret4(self1,self2,self3,self5,_sel,_ret);
  unlock4(self1,self2,self3,self5);
  UNPRT(_1);
endmethod

defmethod(void, gunrecognizedMessage5, Locker, Locker, Any, Locker, Locker)
  EPRT(_1,gclear); EPRT(_2,gclear); EPRT(_4,gclear); EPRT(_5,gclear);
  lock4(self1,self2,self4,self5);
  forward_message(self1->Proxy.obj,self2->Proxy.obj,_3,self4->Proxy.obj,self5->Proxy.obj);
  chkret4(self1,self2,self4,self5,_sel,_ret);
  unlock4(self1,self2,self4,self5);
  UNPRT(_1);
endmethod

defmethod(void, gunrecognizedMessage5, Locker, Any, Locker, Locker, Locker)
  EPRT(_1,gclear); EPRT(_3,gclear); EPRT(_4,gclear); EPRT(_5,gclear);
  lock4(self1,self3,self4,self5);
  forward_message(self1->Proxy.obj,_2,self3->Proxy.obj,self4->Proxy.obj,self5->Proxy.obj);
  chkret4(self1,self3,self4,self5,_sel,_ret);
  unlock4(self1,self3,self4,self5);
  UNPRT(_1);
endmethod

defmethod(void, gunrecognizedMessage5, Any, Locker, Locker, Locker, Locker)
  EPRT(_2,gclear); EPRT(_3,gclear); EPRT(_4,gclear); EPRT(_5,gclear);
  lock4(self2,self3,self4,self5);
  forward_message(_1,self2->Proxy.obj,self3->Proxy.obj,self4->Proxy.obj,self5->Proxy.obj);
  chkret4(self2,self3,self4,self5,_sel,_ret);
  unlock4(self2,self3,self4,self5);
  UNPRT(_2);
endmethod

defmethod(void, gunrecognizedMessage5, Locker, Locker, Locker, Locker, Locker)
  EPRT(_1,gclear); EPRT(_2,gclear); EPRT(_3,gclear); EPRT(_4,gclear); EPRT(_5,gclear);
  lock5(self1,self2,self3,self4,self5);
  forward_message(self1->Proxy.obj,self2->Proxy.obj,self3->Proxy.obj,self4->Proxy.obj,self5->Proxy.obj);
  chkret5(self1,self2,self3,self4,self5,_sel,_ret);
  unlock5(self1,self2,self3,self4,self5);
  UNPRT(_1);
endmethod

#endif // if COS_POSIX

