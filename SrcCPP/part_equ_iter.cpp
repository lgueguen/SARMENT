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

#include "part_equ_iter.h"

#include "part_equ.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Part_equ_iter::Part_equ_iter(const Part_equ& S) :
  _seq(S._seq), _el_c(new Csegment*[S._lg]), _lg(S._lg),
  _iter(new Tliste_iter<Csegment>[S._lg])
{
  int i;
  for (i=0;i<_lg;i++)
    _iter[i].colle(S._seq[i]._fils);
}

Part_equ_iter::Part_equ_iter() :
  _seq(0), _el_c(0), _lg(0), _iter(0)
{
}

Part_equ_iter::~Part_equ_iter()
{
  delete[] _el_c;
  delete[] _iter;
}

////////////////////////////////////////
//  modification

void Part_equ_iter::colle(const Part_equ& S)
{
  _seq=S._seq;
  _lg=S._lg;
  if (_el_c)
    delete _el_c;
  _el_c=new Csegment*[_lg];
  if (_iter)
    delete _iter;
  _iter=new Tliste_iter<Csegment>[_lg];
  int i;
  for (i=0;i<_lg;i++)
    _iter[i].colle(S._seq[i]._fils);
}

