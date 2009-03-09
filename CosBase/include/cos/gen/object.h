#ifndef COS_GEN_OBJECT_H
#define COS_GEN_OBJECT_H

/*
 o---------------------------------------------------------------------o
 |
 | COS generics for objects
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
 | $Id: object.h,v 1.18 2009/03/09 13:33:49 ldeniau Exp $
 |
*/

#include <cos/Object.h>
#include <cos/Generic.h>

// allocator, deallocator
defgeneric(OBJ , galloc        , _1);
defgeneric(OBJ , gallocWithSize, _1, (size_t)extra);
defgeneric(void, gdealloc      , _1);

// constructors, destructor, clearing
defgeneric(OBJ , ginit         , _1);
defgeneric(OBJ , ginitWith     , _1, _2); // includes copy ctor ginitWith(T,T)
defgeneric(OBJ , ginitWith2    , _1, _2, _3);
defgeneric(OBJ , ginitWith3    , _1, _2, _3, _4);
defgeneric(OBJ , ginitWith4    , _1, _2, _3, _4, _5);
defgeneric(OBJ , ginitWithLoc  , _1, _2, (STR)func, (STR)file, (int)line);
defgeneric(OBJ , ginitWithStr  , _1, (STR)str);
defgeneric(OBJ , gdeinit       , _1);
defgeneric(void, gclear        , _1);

// ownership
defgeneric(OBJ , gretain       , _1);
defgeneric(void, grelease      , _1);
defgeneric(void, gdiscard      , _1);
defgeneric(OBJ , gautoRelease  , _1);
defgeneric(U32 , gretainCount  , _1);

// identity, conversion, coercion
defgeneric(OBJ , gisKindOf     , _1, _2); // returns True or False
defgeneric(OBJ , gisInstanceOf , _1, _2); // returns True or False
defgeneric(OBJ , gclass        , _1);
defgeneric(STR , gclassName    , _1);
defgeneric(OBJ , gsuperClass   , _1);
defgeneric(U32 , ghash         , _1);

// properties, keys
defgeneric(OBJ , (GenAccessor)ggetAt, _1, at);
defgeneric(void, (GenAccessor)gputAt, _1, at, what);

// comparison
defgeneric(OBJ , (GenOperator)gisEqual, _1, _2); // return True or False
defgeneric(OBJ , (GenOperator)gcompare, _1, _2); // return Equal,Lesser,Greater

// contract
defgeneric(void, ginvariant    , _1, (STR)func, (STR)file, (int)line);

// exception
defgeneric(void, gthrow        , _1, (STR)func, (STR)file, (int)line);

// initialization
defgeneric(void, ginitialize   , _1);
defgeneric(void, gdeinitialize , _1);

// ----- inliners -----

// inliner gnewXXX (= galloc() + ginitXXX())
static always_inline OBJ
gnew(OBJ _1) {
  return ginit(galloc(_1)); COS_UNUSED(gnew);
}

static always_inline OBJ
gnewWith(OBJ _1, OBJ _2) {
  return ginitWith(galloc(_1),_2);  COS_UNUSED(gnewWith);
}

static always_inline OBJ
gnewWith2(OBJ _1, OBJ _2, OBJ _3) {
  return ginitWith2(galloc(_1),_2,_3);  COS_UNUSED(gnewWith2);
}

static always_inline OBJ
gnewWith3(OBJ _1, OBJ _2, OBJ _3, OBJ _4) {
  return ginitWith3(galloc(_1),_2,_3,_4);  COS_UNUSED(gnewWith3);
}

static always_inline OBJ
gnewWith4(OBJ _1, OBJ _2, OBJ _3, OBJ _4, OBJ _5) {
  return ginitWith4(galloc(_1),_2,_3,_4,_5);  COS_UNUSED(gnewWith4);
}

static always_inline OBJ
gnewWithLoc(OBJ _1, OBJ _2, STR func, STR file, int line) {
  return ginitWithLoc(galloc(_1),_2,func,file,line);  COS_UNUSED(gnewWithLoc);
}

static always_inline OBJ
gnewWithStr(OBJ _1, STR str) {
  return ginitWithStr(galloc(_1),str);  COS_UNUSED(gnewWithStr);
}

// inliner gclone
static always_inline OBJ
gclone(OBJ _1) {
  return ginitWith(galloc(gclass(_1)),_1);  COS_UNUSED(gclone);
}

#endif // COS_GEN_OBJECT_H
