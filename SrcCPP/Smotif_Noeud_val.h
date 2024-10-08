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

#if !defined _SMOTIF_NOEUD_VAL_H
#define _SMOTIF_NOEUD_VAL_H

#include "motif.h"
#include "mat_iter.h"
#include "S_Noeud_val.h"

template<class S>
class Smotif_Noeud_val : public S_Noeud_val<S>  
{
  Cmotif* _pt_mot;
public:

  inline Smotif_Noeud_val();
  inline Smotif_Noeud_val(int);
  inline Smotif_Noeud_val(const Smotif_Noeud_val<S>&);

  /////////////////////////
  ////// initialisations
  ////////////////////////

  // !!!! necessitent S::init() , S::avance() 
  //  et S::motif() (qui retourne un Cmotif initial ou 0) 
  //                  (font appel aux deux une fois)
  // Le _pt_mot est soit deja fix\'e
  //               soit determine par S_Noeud<S>_1.motif()
  // Aura pour suivant S_Noeud<S>_1.

  inline void init(const Cdescripteur* const, 
		   const Cdescripteur* const, 
		   const Cdescripteur* const, 
		   S_Noeud<S>&);
  
  // prennent pour valeur la prediction de _pt_mot sur 
  //                      le segment entre arg_2 et arg_3
  //                      arg_1 est le debut de la sequence

  inline void init(const mat_iter&, S_Noeud<S>&);
  // prend la valeur depuis mat_iter

  //////////////////////////
  // donnees
  //////////////////////////

  inline Cmotif* pt();

  //////////////////////////
  // modifications
  //////////////////////////

  inline void g_pt(Cmotif*);
  inline void operator=(Smotif_Noeud_val<S>&);


  // !!!!! TOUTES les suivantes necessitent S::avance()
  //              et decalent le _pt_mot d'un cran

  inline void ajoute_su(const Cdescripteur* const, 
		       const Cdescripteur* const,
		       const Cdescripteur* const);
  // prennent pour valeur la prediction de _pt_mot sur 
  //                      le segment entre arg_1 et arg_2

  inline void ajoute_su(const mat_iter&);
  // prend la valeur depuis mat_iter

  // Les deux suivantes retournent 0 si S_Noeud<S>*_1 est utilise
  //                               S_Noeud<S>*_1  sinon 
  // On cumule les noeuds de valeur maxi entre this et S_Noeud_val<S>_1

  inline S_Noeud<S>* ajoute_su(const Cdescripteur* const, 
			      const Cdescripteur* const, 
			      const Cdescripteur* const, 
			      S_Noeud_val<S>&, S_Noeud<S>*);
  // additionne la prediction de _pt_mot sur 
  //            le segment entre Cobjet*_2 et Cobjet*_3
  //            Cobjet*_1 est le debut de la sequence

  inline S_Noeud<S>* ajoute_su(const mat_iter&,
		       S_Noeud_val<S>&, S_Noeud<S>*);
  // prend la valeur depuis mat_iter


  ////// Pareil pour les suivantes avec les classes d'equivalence

  inline S_Noeud<S>* ajoute_su_cleq(const Cdescripteur* const, 
				   const Cdescripteur* const, 
				   const Cdescripteur* const, 
				   S_Noeud_val<S>&, 
				   S_Noeud<S>*);
  inline S_Noeud<S>* ajoute_su_cleq(const mat_iter&,
				   S_Noeud_val<S>&, 
				   S_Noeud<S>*);

};

#include "Smotif_Noeud_val.inl"

#endif 
