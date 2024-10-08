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

template<class S>
S_Noeud_val<S>::S_Noeud_val():
  S_Noeud<S>(), _val(0)
{
}

template<class S>
S_Noeud_val<S>::S_Noeud_val(void*) 
{
}

template<class S>
S_Noeud_val<S>::S_Noeud_val(const S_Noeud_val<S>& c):
  S_Noeud<S>(c), _val(c._val)
{
}

template<class S>
S_Noeud_val<S>::~S_Noeud_val()
{
}

////////////////////////////////
//  modifications
////////////////////////////////

template<class S>
double S_Noeud_val<S>::val() const
{
  return _val;
}

template<class S>
void S_Noeud_val<S>::g_val(double x)
{
  _val=x;
}


template<class S>
void S_Noeud_val<S>::init(const S& s, double v, S_Noeud<S>& demar)
{
  S::operator=(s);
  _val=v;
  this->dev_su(demar);
}

template<class S>
void S_Noeud_val<S>::modif_dev_su(double v, S_Noeud<S>& s)
{
  _val=v;
  this->dev_su(s);
}  

template<class S>
void S_Noeud_val<S>::operator=(S_Noeud_val<S>& c)
{
  S_Noeud<S>::operator=(c);
  _val=c._val;
}

template<class S>
void S_Noeud_val<S>::operator+=(S_Noeud_val<S>& c)
{
  S_Noeud<S>::operator+=(c);
}

template<class S>
void S_Noeud_val<S>::operator+=(double f)
{
  _val+=f;
}

