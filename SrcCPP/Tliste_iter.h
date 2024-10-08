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

#if !defined _TLISTE_ITER_H
#define _TLISTE_ITER_H

#include "Liste_iter.h"
#include "Tliste.h"

template<class T>
class Tliste_iter : public Liste_iter  
{
public:
  
  inline Tliste_iter();
  inline Tliste_iter(const Tliste<T>&);
  inline Tliste_iter(const Tliste_iter<T>&);
	
  ////////////////////////////
  // modifications
  ////////////////////////////
  
  inline void init();
  // initialise en debut de liste
  inline void annule();
  // met l'iterateur a 0

  inline void operator=(const Tliste_iter<T>& );

  inline void colle(const Tliste<T>&);
  // assigne l'iterateur sur Tliste<T>_1
  // et initialise celui-ci

  /////////////////////////////
  // tests/calculs
  /////////////////////////////

  inline bool operator==(const Tliste_iter<T>&) const;
  inline bool meme_liste(const Tliste_iter<T>&) const;

  inline bool operator==(const T*) const;

  ////////////////////////////
  // utilisation
  ///////////////////////////

  // !!!! un iterateur peut etre a 0 et retourne alors 0

  inline T* operator()();
  inline T* operator()(const Tliste_iter<T>& a);
  // retourne l'objet pointe par l'iterateur
  // fait avancer l'iterateur et s'arrete a la fin
  //                                   ou a Tliste_iter<T>_1
  //                             retourne alors 0

  inline T* operator++();
  inline T* operator++(int);
  // retourne l'objet pointe par l'iterateur
  // fait avancer l'iterateur

  inline T* operator~() const;
  // retourne l'objet pointe par l'iterateur

};

#include "Tliste_iter.inl"

#endif 
