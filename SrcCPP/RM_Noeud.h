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

#if !defined _RM_NOEUD_H
#define _RM_NOEUD_H

#include "desc_comp.h"

template<class R>
class RM_Noeud  : public R
{
private:
  RM_Noeud<R>* _su;
  
  const Cdesc_comp* const _pd;
  unsigned int _no;
  
  // pour le nettoyage des successeurs
  void net_succ();

protected:

  double _val;
  
public:

  inline RM_Noeud();
  inline RM_Noeud(Cdesc_comp* const);
  inline RM_Noeud(const RM_Noeud<R>&);
  inline RM_Noeud(const R&, const Cdesc_comp* const);
  
  inline ~RM_Noeud();

  inline double val() const;
  inline void g_val(double);
  
  // prend en argument la longueur du chemin
  inline void nettoie_sur(int);
  
  inline void init(const Cdescripteur* const,
		   const Cdescripteur* const,
		   const Cdescripteur* const,
		   RM_Noeud<R>&);

  inline void init(const mat_iter&,
		   RM_Noeud<R>&);

  inline void init(double,  RM_Noeud<R>&);

  //ajoute la prediction en position et l'auto-transition
  inline void ajoute_su(const Cdescripteur* const,
                        const Cdescripteur* const,
                        const Cdescripteur* const);

  inline void ajoute_su(const mat_iter&);

  //ajoute la prediction en position
  inline void ajoute_pred(const Cdescripteur* const,
			  const Cdescripteur* const,
			  const Cdescripteur* const);

  inline void ajoute_pred(const mat_iter&);

  //pas de viterbi
  inline void ajoute_su(const Cdescripteur* const,
		       const Cdescripteur* const,
		       const Cdescripteur* const,
		       const Tliste<RM_Noeud<R>* >&);

  inline void ajoute_su(const mat_iter&, 
                        const Tliste<RM_Noeud<R>* >&);

  //pas de backward
  inline void backward(const Tliste<RM_Noeud<R>* >&);

  //pas de forward
  inline void forward(const Tliste<RM_Noeud<R>* >&);

  //necessite un R::avance
  inline void avance();
  //necessite un R::recule
  inline void recule();
  
  ///////////////
  // donnees
   ///////////////

  inline short no() const;
  inline RM_Noeud<R>* su() const;

  inline const Cdesc_comp* pd() const;

  //Nombre de RM_Noeud() jusqu'au debut
  inline int lg() const;
  
  ///////////////////
  // modifications
  ///////////////////

  inline void dev_su(RM_Noeud<R>&);
  //n'aura plus que RM_Noeud<R>_1 comme noeud suivant

  inline void recup_su_de(RM_Noeud<R>&);	
  //n'aura plus que le suivant de RM_Noeud<R>_1 comme suivant

  inline void operator=(RM_Noeud<R>&);
};

#include "RM_Noeud.inl"

#endif 
