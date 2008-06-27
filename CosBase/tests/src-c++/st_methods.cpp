/*
 o---------------------------------------------------------------------o
 |
 | COS speed tests - C++ methods
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
 | $Id: st_methods.cpp,v 1.1 2008/06/27 16:17:25 ldeniau Exp $
 |
*/

#include <cstdlib>
#include <cassert>
#include "utest.h"
#include "tests.hpp"
#include "Counter.hpp"

enum { N = 20000000 };

void
st_methods()
{
  Counter cnt;
  IIncr &inc = cnt;

  STEST( "method (0 argument )", N, N, inc.incr() );
  STEST( "method (1 argument )", N, N, inc.incrBy(1) );
  STEST( "method (2 arguments)", N, N, inc.incrBy(1,1) );
  STEST( "method (3 arguments)", N, N, inc.incrBy(1,1,1) );
  STEST( "method (4 arguments)", N, N, inc.incrBy(1,1,1,1) );
  STEST( "method (5 arguments)", N, N, inc.incrBy(1,1,1,1,1) );
  
  assert( cnt.value() == N+N+2*N+3*N+4*N+5*N );
}

void
st_multimethods()
{
  Counter cnt, cnt1(1);
  IAddTo1 &add1 = cnt, &one1 = cnt1;
  IAddTo2 &add2 = cnt, &one2 = cnt1;
  IAddTo3 &add3 = cnt, &one3 = cnt1;
  IAddTo4 &add4 = cnt, &one4 = cnt1;

  STEST( "multimethod (rank 2)", N, N, add1.addTo(one1) );
  STEST( "multimethod (rank 3)", N, N, add2.addTo(one2,one2) );
  STEST( "multimethod (rank 4)", N, N, add3.addTo(one3,one3,one3) );
  STEST( "multimethod (rank 5)", N, N, add4.addTo(one4,one4,one4,one4) );
  
  assert( cnt.value() == N+2*N+3*N+4*N );
}

void
st_memory(void)
{
  enum { P = N/2 };
  static Counter* arr[P];
  U32 i;
  
  i = 0;
  STEST( "new", P, P, arr[i++] = new Counter );

  i = 0;
  STEST( "delete", P, P, delete arr[i++] );
}