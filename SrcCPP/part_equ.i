// Copyright 2004 Laurent GUEGUEN

// This file is part of Sarment.

// Sarment is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.

// Sarment is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with Sarment; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

%module part_equ

%{
  #include "part_equ.h"
  #include <sstream>
  %}

#ifdef SWIGPYTHON
%typemap(out) char* {
  $result=PyString_FromString($1);
  free($1);
}
#endif

class Part_equ
{
public:
  Part_equ();
  ~Part_equ();

};



