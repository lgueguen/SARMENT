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

#if !defined _R_NOEUD_VAL_H
#define _R_NOEUD_VAL_H

#include "R_Noeud.h"

template<class R>
class R_Noeud_val : 
  public R_Noeud<R>
{
protected:

  double _val;
  
public:

  inline R_Noeud_val();
  inline R_Noeud_val(void*);
  inline R_Noeud_val(const R_Noeud_val<R>& );
  inline R_Noeud_val(const R&, double);
  inline ~R_Noeud_val();


  inline double val() const;
  inline void g_val(double);
  

  ////////////////////////////////
  //  modifications
  ////////////////////////////////

  void inline init(const R&, double, R_Noeud<R>&);
  // initialisation avec la valeur, en R_1, et le suivant
  //  est R_Noeud<R>_1

  void inline ajoute_su(double, R_Noeud<R>&);
  // prend la valeur double_1 et aura R_Noeud<R>_1 comme suivant

  void inline operator=(R_Noeud_val<R>&);

  void inline operator+=(double);
  // augmentent la valeur

};

#include "R_Noeud_val.inl"

#endif 
