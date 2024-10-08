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
Pt<P>::Pt(const P& p) : _pt(&p)
{
}

template<class P>
Pt<P>::Pt(const Pt<P>& p) : _pt(p._pt)
{
}

template<class P>
Pt<P>::~Pt()
{

}

////////////////////
// tests
////////////////////

template<class P>
bool Pt<P>::Noeud_normal() const
{
  return _pt->Noeud_normal();
}

////////////////////
//     donnees
////////////////////

template<class P>
const P* Pt<P>::pt() const
{
  return _pt;
}

///////////////////
// modifications
///////////////////

template<class P>
void Pt<P>::g_pt(const P* p)
{
  _pt=p;
}

template<class P>
void Pt<P>::operator=(const Pt<P>& p)
{
  _pt=p._pt;
}


