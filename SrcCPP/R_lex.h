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

#if !defined _R_LEX_H
#define _R_LEX_H

#include "Tliste_iter.h"

#include "position.h"
#include "mat_iter.h"

#include "Rmotif_Noeud_val.h"

class Clexique;
class Part_equ;

template<class R>
class R_lex  
{
private:
  Tliste<Rmotif_Noeud_val<R> > _vocces;
  Tliste<R_Noeud<R> > _demlex;

public:

  inline R_lex();
  inline R_lex(const Clexique&);
  inline ~R_lex();

  ////////////////////////////////
  // modifications
  ///////////////////////////////

  inline void nettoie();

  inline void init(const Cdescripteur* const, 
		   const Cdescripteur* const, 
		   const Cdescripteur* const);
  // initialise la liste _vocces a partir des elements de _demlex
  //            selon les valeurs de prediction sur le segment
  //            de par_2 vers par_3
  inline void init(const mat_iter&);
  // initialise la liste _vocces a partir des elements de _demlex
  //            selon les valeurs de prediction sur la partie de 
  //            matrice qui part de mat_iter_1

  inline void plus_egale(const Cdescripteur* const, 
			 const Cdescripteur* const, 
			 const Cdescripteur* const);
  inline void plus_egale(const mat_iter&);
  // fait avancer les elements de vocces 
  // (cf Rmotif_Noeud_val::ajoute_su)

  inline void egale(const Cdescripteur* const, 
                    const Cdescripteur* const, 
                    const Cdescripteur* const);
  inline void egale(const mat_iter&);
  // fait avancer les elements de vocces 
  // (cf Rmotif_Noeud_val::ajoute_su)

  inline void operator+=(const double);
  // modifie les valeurs des elements de _vocces avec +=double_1

  ////////
  // Les deux suivantes utilisent (s'il existe) *R_Noeud<R>*&_1 pour 
  //          modifier les elements de vocces.
  // Ceux-ci sont compares a R_Noeud_val<R>_1 
  //                            (cf Rmotif_Noeud_val::ajoute_su).
  // R_Noeud<R>*_1 devient 0 ou un R_Noeud<R>* deja alloue 
  //                                               mais non utilise.

  inline void cumule(const Cdescripteur* const, 
		     const Cdescripteur* const,
		     const Cdescripteur* const,
		     R_Noeud_val<R>&, 
		     R_Noeud<R>*&);
  inline void cumule(const mat_iter&,
		     R_Noeud_val<R>&, 
		     R_Noeud<R>*&);

  // travaille sur les classes d'equivalence


  /////////////////////////
  // donnees
  /////////////////////////

  inline double val_max() const; 
  // valeur maxi de vocces

  inline void Noeud_max(R_Noeud_val<R>& ) const;
  // met dans R_Noeud_val<R>_1 le R_Noeud_val de vocces de valeur maxi

  ///////////////
  // amities
  ///////////////

  friend class Csegment;
  friend class Clexique;

  friend class Csequence;
  friend class Cmatrice;
};

#include "lexique.h"
#include "R_lex.inl"

#endif 
