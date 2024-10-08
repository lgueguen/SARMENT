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

#if !defined _S_LEX_H
#define _S_LEX_H

#include "Tliste_iter.h"

#include "position.h"
#include "mat_iter.h"

#include "Smotif_Noeud_val.h"

class Clexique;
class Cseq_equ;

template<class S>
class S_lex  
{
private:
  Tliste<Smotif_Noeud_val<S> > _vocces;
  Tliste<S_Noeud<S> > _demlex;

public:

  inline S_lex();
  inline S_lex(const Clexique&);
  inline ~S_lex();

  ////////////////////////////////
  // modifications
  ///////////////////////////////

  inline void nettoie();

  inline void init(const Cdescripteur* const,
		   const Cdescripteur* const,
		   const Cdescripteur* const);
  // initialise la liste _vocces a partir des elements de _demlex
  //            selon les valeurs de prediction sur le segment
  //            de arg_2 vers arg_3
  inline void init(const mat_iter&);


  inline void plus_egale(const Cdescripteur* const, 
			 const Cdescripteur* const, 
			 const Cdescripteur* const);
  // fait avancer les elements de vocces (cf Smotif_Noeud_val::ajoute_su)
  inline void plus_egale(const mat_iter&);

  ////////
  // Les deux suivantes utilisent (s'il existe) *S_Noeud<S>*&_1 pour 
  //          modifier les elements de vocces.
  // Ceux-ci sont compares a S_Noeud_val<S>_1 (cf Smotif_Noeud_val::ajoute_su).
  // S_Noeud<S>*_1 devient 0 ou un S_Noeud<S>* deja alloue mais non utilise.

  inline void cumule(const Cdescripteur* const, 
		     const Cdescripteur* const,
		     const Cdescripteur* const,
		     S_Noeud_val<S>&, S_Noeud<S>*&);
  inline void cumule(const mat_iter&, 
		     S_Noeud_val<S>&, S_Noeud<S>*&);

  inline void cumule_cleq(const Cdescripteur* const, 
			  const Cdescripteur* const, 
			  const Cdescripteur* const, 
			  S_Noeud_val<S>&, S_Noeud<S>*&);
  inline void cumule_cleq(const mat_iter&,
			  S_Noeud_val<S>&, S_Noeud<S>*&);
  // travaille sur les classes d'equivalence

  /////////////////////////
  // donnees
  /////////////////////////

  inline double val_max() const; 
  // valeur maxi de vocces

  inline void Noeud_max(S_Noeud_val<S>& ) const;
  // met dans S_Noeud_val<S>_1 les SNoeud_val de
  //                          vocces de valeur maxi

  ///////////////
  // amities
  ///////////////

  friend class Csegment;
  friend class Cseq_equ;
  friend class Cposopere;
  friend class Clexique;
  friend class Cseq_prim;
};

#include "S_lex.inl"

#endif 
