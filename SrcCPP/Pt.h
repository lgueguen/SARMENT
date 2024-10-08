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

#if !defined _PT_H
#define _PT_H

template<class P>
class Pt  
{
  const P* _pt;
public:

  inline Pt() {};
  inline Pt(const P&);
  inline Pt(const Pt<P>&);
  inline ~Pt();

  ///////////////////
  // modifications
  ///////////////////

  inline void g_pt(const P*);
  inline void operator=(const Pt<P>&);

  ////////////////////
  //     donnees
  ////////////////////

  inline const P* pt() const;

  ////////////////////
  // tests
  ////////////////////

  // pour les Noeuds
  //!!! necessite P::Noeud_normal
  inline bool Noeud_normal() const;
};

#include "Pt.inl"

#endif 
