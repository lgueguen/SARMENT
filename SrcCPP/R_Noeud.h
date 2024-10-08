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

#if !defined _R_NOEUD_H
#define _R_NOEUD_H

#include "Stock.h"

template<class R>
class R_Noeud  : public R
{
private:
  R_Noeud<R>* _su;
  unsigned int _no;
  
  // pour le nettoyage des successeurs
  void net_succ();

public:

  inline R_Noeud();
  inline R_Noeud(void*);
  inline R_Noeud(const R_Noeud<R>&);
  inline R_Noeud(const R&);
  inline ~R_Noeud();

  //////////////
  // new/delete
  //////////////

  //!!!! gérés par Stock
  inline void* operator new(size_t);
  inline void operator delete(void*, size_t); 

  ///////////////
  // donnees
  ///////////////

  inline short no() const;
  inline R_Noeud<R>* su() const;

  //Nombre de R_Noeud() jusqu'au debut
  inline int lg() const;
  
  ///////////////////
  // modifications
  ///////////////////

  inline void dev_su(R_Noeud<R>&);
  
  //n'aura plus que R_Noeud<R>_1 comme noeud suivant

  inline void recup_su_de(R_Noeud<R>&);	
  //n'aura plus que le suivant de R_Noeud<R>_1 comme suivant

  inline void operator=(R_Noeud<R>&);
};

#include "R_Noeud.inl"

#endif 
