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

template<class P>
Pt_val<P>::Pt_val() : _val(0)
{

}

template<class P>
Pt_val<P>::Pt_val(const Pt_val<P>& pv) : Pt<P>(pv), _val(pv._val)
{

}

template<class P>
Pt_val<P>::~Pt_val()
{

}

////////////////////////
// donnees
///////////////////////

template<class P>
double Pt_val<P>::val() const
{
  return _val;
}

////////////////////////
// modifications
////////////////////////

template<class P>
void Pt_val<P>::g_val(double x) 
{
  _val=x;
}

template<class P>
void Pt_val<P>::operator=(const Pt_val<P>& p)
{
  Pt<P>::operator=(p);
  _val=p._val;
}
