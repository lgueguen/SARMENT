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
R_Noeud_val<R>::R_Noeud_val()
{
}

template<class R>
R_Noeud_val<R>::R_Noeud_val(void*)
{
}

template<class R>
R_Noeud_val<R>::R_Noeud_val(const R_Noeud_val<R>& c):
  R_Noeud<R>(c), _val(c._val)
{
}

template<class R>
R_Noeud_val<R>::R_Noeud_val(const R& r, double v):
  R_Noeud<R>(r), _val(v)
{
}

template<class R>
R_Noeud_val<R>::~R_Noeud_val()
{
}
 
////////////////////////////////
//  modifications
////////////////////////////////

template<class R>
double R_Noeud_val<R>::val() const
{
  return _val;
}

template<class R>
void R_Noeud_val<R>::g_val(double x)
{
  _val=x;
}


template<class R>
void R_Noeud_val<R>::init(const R& r, double v, R_Noeud<R>& demar)
{
  _val=v;
  R::operator=(r);
  dev_su(demar);
}

template<class R>
void R_Noeud_val<R>::ajoute_su(double v, R_Noeud<R>& nr)
{
  _val=v;
  dev_su(nr);
}

template<class R>
void R_Noeud_val<R>::operator=(R_Noeud_val<R>& c)
{
  R_Noeud<R>::operator=(c);
  _val=c._val;
}

template<class R>
void R_Noeud_val<R>::operator+=(double v)
{
  _val+=v;
}


