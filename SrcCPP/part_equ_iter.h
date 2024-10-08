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

#if !defined _SEQ_EQU_ITER_H
#define _SEQ_EQU_ITER_H

#include "Tliste_iter.h"
#include "segment.h"
#include "posopere.h"

class Part_equ;

class Part_equ_iter  
{
  Csegment** _el_c;
  Tliste_iter<Csegment>* _iter;
  Cposopere* _seq;
  int _lg;

public:	

  ~Part_equ_iter();
  Part_equ_iter(const Part_equ&);
  Part_equ_iter();

  //////////////////////////////
  //  modifications

  inline void operator=(const Part_equ_iter&);
  void colle(const Part_equ&);

  inline void init();
  //  initialise tous les iterateurs en debut de liste
  inline void init(int);
  //  initialise l'iterateur int_1 en debut de liste


  ///////////////////////////////
  //  donnees

  inline int lg() const;
  // la longueur de la Part_equ associee

  inline Csegment* operator[](int) const;
  // retourne le segment pointe en colonne int_1

  //////////////////////////////
  //  iterations

  inline Csegment* operator()(int i);
  //iteration en colonne i

  inline Csegment* d(int);	
  //iteration sur les segments de la colonne i en considerant
  //              qu'il se situe a droite d'un segment auquel il restera connexe
  //   retourne 0 si a la fin de l'iteration sur i
  inline Csegment* g(int);	
  //idem avec la gauche

};

#include "part_equ_iter.inl"

#endif

