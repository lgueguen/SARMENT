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

///////////////////////////////
//  donnees

int Part_equ_iter::lg() const 
{
  return _lg;
}

Csegment* Part_equ_iter::operator[](int i) const
{
  return _el_c[i];
}


///////////////////////////////
//  iterations

void Part_equ_iter::init()
{
  int i;
  for (i=0;i<_lg;i++){
    _iter[i].init();
    _el_c[i]=0;
  }
}

void Part_equ_iter::init(int i)
{
  _iter[i].init();
  _el_c[i]=0;
}

Csegment* Part_equ_iter::operator()(int i)
{
  return _el_c[i]=_iter[i]();
}

Csegment* Part_equ_iter::d(int i)
{
  if (i==0)
    return _el_c[0]=_iter[0]();
  else{
    if (!_el_c[i-1])
      return _el_c[i]=_iter[i]();
    else
      while ((_el_c[i]=_iter[i]()))
	if (_el_c[i]->pos_d()==(_el_c[i-1]->pos_f()+1))
	  break;
  }
  return _el_c[i];
}

Csegment* Part_equ_iter::g(int i)
{
  if (i==_lg-1)
    return _el_c[_lg-1]=_iter[_lg-1]();
  else{
    if (!_el_c[i+1])
      return _el_c[i]=_iter[i]();
    else
      while ((_el_c[i]=_iter[i]()))
	if (_el_c[i]->pos_f()+1==_el_c[i+1]->pos_d())
	  break;
  }

  return _el_c[i];
}

void Part_equ_iter::operator=(const Part_equ_iter& S)
{
  int i;
  _lg=S._lg;
  _seq=S._seq;
  for (i=0;i<_lg;i++){
    _iter[i]=S._iter[i];
    _el_c[i]=S._el_c[i];
  }
}
