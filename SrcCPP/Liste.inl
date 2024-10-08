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

/////////////////////////////
// construction/destruction
/////////////////////////////

Liste::Liste() : _fin(0)
{
}

Liste::Liste(el_liste* a)
{
  _fin=_fin->_so=a;
}

/////////////////////////////
// modifications
/////////////////////////////

void Liste::nettoie() {
  _fin=0;
}

void Liste::aj_deb(el_liste* a)
{
  if (_fin)
    a->_so=_fin->_so;
  else
    _fin=a;
  _fin->_so=a;
}

void Liste::aj_fin(el_liste* a)
{
  if (_fin){
    a->_so = _fin->_so;
    _fin = _fin->_so = a;
  }
  else
    _fin = a->_so = a;
}

el_liste* Liste::enl_deb()
{
  if (_fin) {
    el_liste* f=_fin->_so;
    if (f==_fin)
      _fin=0;
    else
      _fin->_so=f->_so;
    return(f);
  }
  return 0;
}

el_liste* Liste::enl_fin()
{
  if (_fin) {
    el_liste* f=_fin;
    if (f ==_fin->_so)
      _fin=0;
    else{
      while (_fin->_so!=f)
	_fin=_fin->_so;
      _fin->_so=f->_so;
    }
    return(f);
  }
  return 0;
}

el_liste* Liste::enl_i(int i)
{
  if (!_fin) return 0;
  el_liste* e=_fin;
  el_liste* a;
  while (--i)
    if ((e=e->_so)==_fin)
      return 0;

  if (e==_fin)
    return enl_deb();
  else{
    if (e->_so==_fin)
      _fin=e;
    a=e->_so;
    e->_so=e->_so->_so;
    return a;
  }
}

void Liste::met_fin(el_liste* t) {
  _fin=t;
}

void Liste::permute_mm_fin(el_liste* const i1,
			   el_liste* const i2)
{
  if (!i1 || !i2 || (i1==i2))
    return;
  if (i2==i1->_so){
    if (_fin==i2)
      _fin=i1;
    return;
  }
  if (_fin==i1->_so)
    _fin=i1;
  el_liste* x;
  x=i1->_so;
  i1->_so=x->_so;
  x->_so=i2->_so;
  i2->_so=x;
}

void Liste::echange(Liste& l) {
  el_liste* x=_fin;
  _fin=l._fin;
  l._fin=x;
}

/////////////////////////////
// donnees
/////////////////////////////

el_liste* Liste::ind_deb() const
{
  return (_fin)?_fin->_so:0; 
}

el_liste* Liste::ind_i(int i) const
{
  if (!_fin) return 0;
  el_liste* e=_fin;
  while (--i)
    if ((e=e->_so)==_fin)
      return 0;
  return e->_so;
}

el_liste* Liste::ind_fin() const
{
  return(_fin);
}

////////////////////////////////
// tests
////////////////////////////////

bool Liste::est_vide() const
{
  return((_fin)?0:1);
}

