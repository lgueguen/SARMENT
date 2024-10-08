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

#if !defined _RM_LEX_H
#define _RM_LEX_H

#include "Tliste_iter.h"

#include "position.h"

#include "RM_Noeud.h"

class Clexique;

template<class R>
class RM_lex  
{
private:

  Tliste<RM_Noeud<R>* > _Avoc;

  // liste temporaire de noeuds << vides >> en temps normal
  Tliste<RM_Noeud<R>* > _Nvoc;
  
  RM_Noeud<R> _deb;
  
public:

  inline RM_lex();
  inline RM_lex(const Clexique&);
  inline ~RM_lex();

  ////////////////////////////////
  // modifications
  ///////////////////////////////

  inline void nettoie();

  // initialise la liste _vocces a partir des elements de _demlex
  //            selon les valeurs de prediction sur le segment
  //            de par_1 vers par_2
  inline void init(const Cdescripteur* const,
		   const Cdescripteur* const, 
		   const Cdescripteur* const);

  inline void init(const mat_iter&);

  // initialise la liste des valeurs a 0 
  inline void init();
		   

  inline void plus_egale(const Cdescripteur* const,
			 const Cdescripteur* const, 
			 const Cdescripteur* const);

  inline void plus_egale(const mat_iter&);

  inline void cumule(const Cdescripteur* const,
		     const Cdescripteur* const, 
		     const Cdescripteur* const);

  inline void cumule(const mat_iter&);

  // cumule _Nvoc sur _Avoc puis echange _Avoc et _Nvoc

  inline void avance();
  // fait avancer les RM_Noeud<R> de _Avoc et _Nvoc

  
  // calcule les valeurs par backward (forward) sur _Nvoc depuis _Avoc
  // puis echange _Avoc et _Nvoc

  //! ne respecte pas les su
  inline void backward(const Cdescripteur* const,
		const Cdescripteur* const,
		const Cdescripteur* const);

  inline void backward(const mat_iter&);

  // celle-ci respecte les su
  inline void forward(const Cdescripteur* const,
                      const Cdescripteur* const,
                      const Cdescripteur* const);

  inline void forward(const mat_iter&);
  
  /////////////////////////
  // donnees
  /////////////////////////

  inline void aj_val(Tliste<double>&) const;
  //ajoute les valeurs a la liste, dans le meme ordre que _Avoc

  inline void maj_val(Tliste<double>&) const;
  //met a jour les valeurs de la liste, dans le meme ordre que _Avoc
  // si la liste est trop longue, boucle sur _Avoc
  

  inline double val_max() const; 
  // valeur maxi de _Avoc
  
  inline RM_Noeud<R>* Noeud_max() const; 
  // noeud max de _Avoc, et le plus a gauche si egalite

  inline RM_Noeud<R>* prem_Noeud() const;
  //premier Noeud de _Avoc
  
  ///////////////
  // amities
  ///////////////

  friend class Clexique;
};

#include "lexique.h"
#include "RM_lex.inl"

#endif 
