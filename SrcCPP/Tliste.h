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

#if !defined _TLISTE_H
#define _TLISTE_H

#include "Liste.h"

template <class T> class Tliste_iter;

template<class T>
struct Tliste : private Liste 
{
private:
  void trie(T**, int, int);

public:

  inline Tliste(const Tliste<T>&);
  inline Tliste();
	
  inline ~Tliste();

  //////////////////////////////////////////////////
  /// modifications
  //////////////////////////////////////////////////

  inline void nettoie();

  inline void aj_deb(const T& a); 
  inline void aj_deb(); 
  // ajoute au debut 

  inline void aj_fin(const T& a); 
  inline void aj_fin(); 
  // ajoute a la fin 

  inline bool aj_deb_diff(const T&); 
  // ajoute au debut si n'est pas deja dans la liste
  // et retourne 1 si l'ajout a ete fait
  inline bool aj_fin_diff(const T&); 
  // ajoute a la fin si n'est pas deja dans la liste
  // et retourne 1 si l'ajout a ete fait
  
  inline int aj_fin_diff(const Tliste<T>&);
  // ajoute a la fin les elements differents
  //  et retourne le nombre d'elements ajoutes

  inline void operator=(const Tliste<T>&);
  
  inline T enl_deb();
  inline T* enl_pt_deb();
  inline void tue_deb();
  inline T enl_fin(); // ! parcourt toute la liste
  inline void tue_fin(); // ! parcourt toute la liste

  inline T* enl_iter(Tliste_iter<T>&);
  // enleve et retourne l'individu a l'iterateur
  // si l'iterateur est en fin de liste, il est passe a 0
  
  inline void tue(T*); // tue l'element de pointeur T*
  
  inline void echange(Tliste<T>&);
  // echange ses donnees avec celles de Tliste<T>_1

  // tri dichotomique dans l'ordre croissant
  // recopie une fois toutes les donnees
  // necessite l'operateur < de T
  inline void trie();

  //element max (0 si vide)
  // necessite l'operateur < de T
  inline T* max();
  //element min (0 si vide)
  // necessite l'operateur < de T
  inline T* min();

  ///////////////////////////////////////////////
  // donnees
  ///////////////////////////////////////////////

  inline T* ind_deb() const;
  inline T* ind_fin() const;
  inline T* ind_i(int) const;
  // retourne le pointeur de l'element int_1
  //          0 si int_1 trop grand

  inline T* possede(const T&) const;
  inline T* possede(T* const) const;
  // retourne le pointeur du PREMIER element egal a T_1
  //          0 si pas de tel element
  //  !!! necessitent l'existence de T::operator==

  inline void permute_mm_fin(const Tliste_iter<T>&, const Tliste_iter<T>&);
  //  opere une permutation circulaire dans le sens de la liste
  //  de Tliste_iter<T>_1 vers Tliste_iter<T>_2
  //  fin est conservee
  //  FAIT AVANCER LE PREMIER ITERATEUR D'UN CRAN (comme ++)
  //  FAIT RECULER LE SECOND ITERATEUR D'UN CRAN

  //////////////////////////////////////////////
  //  tests/calculs
  //////////////////////////////////////////////

  inline bool est_vide() const;
  inline int nb_ind() const;
  inline int taille() const {return nb_ind();}

  //  !!! les suivants necessitent l'existence de T::operator==

  inline bool operator==(const Tliste<T>&) const;
  inline bool mm_el(const Tliste<T>&) const;
  // vrai si ont les memes elements (sans l'ordre)
  inline int nb_diff(const Tliste<T>&) const; 
  // nombre d'elements differents
  inline int nb_meme(const Tliste<T>&) const;
  // nombre d'elements identiques

  friend class Tliste_iter<T>;
};

#include "Tliste.inl"

#endif 
