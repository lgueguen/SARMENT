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

#if !defined _S_NOEUD_VAL_H
#define _S_NOEUD_VAL_H

#include "S_Noeud.h"

template<class S>
class S_Noeud_val : 
  public S_Noeud<S>
{
protected:

  double _val;
  
public:
  inline S_Noeud_val();
  inline S_Noeud_val(void*);
  inline S_Noeud_val(const S_Noeud_val<S>& );
  inline ~S_Noeud_val();
  
  ////////////////////////////////
  //  modifications
  ////////////////////////////////

  inline double val() const;
  inline void g_val(double);
  
  void inline init(const S&, double, S_Noeud<S>&);
  // initialisation avec la valeur, en S_1, et le suivant
  //  est S_Noeud<S>_1

  void inline modif_dev_su(double, S_Noeud<S>&);  
  // prend comme valeur double_1
  //    et comme suivants ceux de S_Noeud<S>_1

  void inline operator=(S_Noeud_val<S>&);

  void inline operator+=(S_Noeud_val<S>&);
  //ajoute sans redondance a ses suivants ceux de S_Noeud_val<S>_1 

  void inline operator+=(double);
  // augmentent la valeur
};

#include "S_Noeud_val.inl"

#endif 
