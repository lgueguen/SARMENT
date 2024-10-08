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

template<class R>
R_Noeud<R>::R_Noeud(void*) : _su(0) 
{
}

template<class R>
R_Noeud<R>::R_Noeud() : R(), _no(1), _su(0)
{
}

template<class R>
R_Noeud<R>::R_Noeud(const R& r) : R(r), _no(1), _su(0)
{
}

template<class R>
R_Noeud<R>::R_Noeud(const R_Noeud<R>& C) : R(C), _no(1)
{
  _su=C._su;
  if (_su)
    _su->_no++;
}

template<class R>
R_Noeud<R>::~R_Noeud()
{
  net_succ();
}

template<class R>
void R_Noeud<R>::net_succ()
{
  if (_su && (--(_su->_no)==1) && 
      _su->Noeud_normal() )
    delete _su;
}

//////////////
// new/delete
//////////////

template<class R>
void* R_Noeud<R>::operator new(size_t s)
{
  return Stock<10000, R_Noeud<R> >::aj_elem();
}

template<class R>
void R_Noeud<R>::operator delete(void* ad, size_t s)
{
  Stock<10000, R_Noeud<R> >::enl_elem(ad);
}

/////////////////////
//  donnees
/////////////////////

template<class R>
short R_Noeud<R>::no() const
{
  return _no;
}

template<class R>
R_Noeud<R>* R_Noeud<R>::su() const
{ 
  return _su;
}

template<class R>
int R_Noeud<R>::lg() const
{
  return (_su && _su->Noeud_normal())?1+_su->lg():0;
}

///////////////////
// modifications
///////////////////

template<class R>
void R_Noeud<R>::dev_su(R_Noeud<R>& C)
{
  net_succ();
  _su=&C;
  C._no++;
}

template<class R>
void R_Noeud<R>::recup_su_de(R_Noeud<R>& C)
{
  net_succ();
  _su=C._su;
  if (_su)
    _su->_no++;
}

template<class R>
void R_Noeud<R>::operator=(R_Noeud<R>& C)
{
  R::operator=(C);
  recup_su_de(C);
}


