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

#if !defined _LISTE_ITER_H
#define _LISTE_ITER_H

#include "Liste.h"

template<class T> class Tliste_iter;
template<class T> class Tliste;

class Liste_iter  
{
private:
  el_liste* _el_c;
  const Liste* _li_c;

  inline Liste_iter();
  inline ~Liste_iter();
  inline Liste_iter(const Liste_iter&);
  inline Liste_iter(const Liste&);

  ////////////////////////////
  // modifications
  ////////////////////////////

  inline void init();
  // initialise en debut de liste
  inline void annule();
  // met l'iterateur a 0
  
  inline void operator=(const Liste_iter&);

  inline void colle(const Liste&);
  // assigne l'iterateur sur Liste_1
  // et initialise celui-ci

  /////////////////////////////
  // tests/calculs
  /////////////////////////////

  inline bool operator==(const Liste_iter&) const;
  inline bool meme_liste(const Liste_iter&) const;

  /////////////////////////////
  // utilisation
  //////////////////////////////

  // !!!! un iterateur peut etre a 0 et retourne alors 0

  inline el_liste* operator()();
  inline el_liste* operator()(const Liste_iter&);
  // retourne l'objet pointe par l'iterateur
  // fait avancer l'iterateur et s'arrete a la fin
  //                                   ou a Liste_iter_1
  //                             retourne alors 0

  inline el_liste* operator++();
  // retourne l'objet pointe par l'iterateur
  // fait avancer l'iterateur
  
  inline el_liste* operator~() const;
  // retourne l'objet pointe par l'iterateur

  inline el_liste* ici() const;
  // retourne l'objet AVANT l'iterateur

  template<class T> 
  friend class Tliste_iter;
  template<class T> 
  friend class Tliste;

  friend class Liste;
};

#include "Liste_iter.inl"

#endif 
