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

#if !defined _RMOTIF_NOEUD_VAL_H
#define _RMOTIF_NOEUD_VAL_H

#include "motif.h"
#include "R_Noeud_val.h"
#include "mat_iter.h"

template<class R>
class Rmotif_Noeud_val : public R_Noeud_val<R>  
{
  const Cmotif* _pt_mot;
public:

  inline Rmotif_Noeud_val();
  inline Rmotif_Noeud_val(void*);
  inline Rmotif_Noeud_val(const Rmotif_Noeud_val<R>&);


  /////////////////////////
  ////// initialisations
  ////////////////////////

  // !!!! necessitent un R::init() et un R::avance() 
  //                  (font appel aux deux une fois)
  // Le _pt_mot est deja fixe.
  // Aura pour suivant R_Noeud<R>_1.

  //  inline void init(const Cobjet&, R_Noeud<R> &);
  // prend pour valeur la prediction de _pt_mot sur Cobjet_1

  inline void init(const Cdescripteur* const, 
		   const Cdescripteur* const, 
		   const Cdescripteur* const, 
		   R_Noeud<R> &);
  // prend pour valeur la prediction de _pt_mot sur 
  //                          le segment entre par_1 et par_2
  inline void init(const mat_iter&, R_Noeud<R> &);
  // prend pour valeur la prediction de _pt_mot sur 
  //                          la matrice depuis mat_iter_1

  ////////////////////////
  // donnees
  ////////////////////////

  inline const Cmotif* pt();

  //////////////////////////////////
  // modifications
  //////////////////////////////////

  inline void g_pt(Cmotif*);
  inline void operator=(Rmotif_Noeud_val<R> &);


  // !!!!! TOUTES les suivantes necessitent R::avance()
  //              et decalent le _pt_mot d'un cran

  inline void ajoute_su(const Cdescripteur* const, 
                        const Cdescripteur* const,
                        const Cdescripteur* const);
  // prend pour valeur la prediction de _pt_mot sur 
  //                          le segment entre arg_2 et arg_3
  //                      arg_1 est le debut de la sequence
  
  inline void ajoute_su(const mat_iter&);
  // prend pour valeur la prediction de _pt_mot sur 
  //                          le segment depuis mat_iter_1

  inline void egale_su(const Cdescripteur* const, 
                       const Cdescripteur* const,
                       const Cdescripteur* const);
  // prend pour valeur la prediction de _pt_mot sur 
  //                          le segment entre arg_2 et arg_3
  //                      arg_1 est le debut de la sequence
  
  inline void egale_su(const mat_iter&);
  // prend pour valeur la prediction de _pt_mot sur 
  //                          le segment depuis mat_iter_1


  // Les deux suivantes retournent 0 si R_Noeud<R>*_1 est utilisee
  //                               R_Noeud<R>*_1  sinon 
  // On prend le noeud de valeur maxi entre this et R_Noeud_val<R>_1

  //   inline R_Noeud<R>* ajoute_su(const Cobjet&, 
  // 			      R_Noeud_val<R>&, 
  // 			      R_Noeud<R>*);
  // additionne la prediction de _pt_mot sur Cobjet_1

  inline R_Noeud<R>* ajoute_su(const Cdescripteur* const, 
			      const Cdescripteur* const, 
			      const Cdescripteur* const, 
			      R_Noeud_val<R>&, 
			      R_Noeud<R>*);
  // additionne la prediction de _pt_mot sur 
  //                          le segment entre arg_1 et arg_2

  inline R_Noeud<R>* ajoute_su(const mat_iter&,
			      R_Noeud_val<R>&, 
			      R_Noeud<R>*);
  // additionne la prediction de _pt_mot sur 
  //                          le segment a partir de mat_iter_1

};

#include "Rmotif_Noeud_val.inl"

#endif 
