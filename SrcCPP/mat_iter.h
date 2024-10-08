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

#ifndef _MAT_ITER_H_
#define _MAT_ITER_H_

#include "Fmatrice.h"

class mat_iter
{
private:
  
  const Fmatrice* _p_mat;
  Fmatrice::vector<double>* _p_vec;

public:
  
  inline mat_iter();
  inline mat_iter(const Fmatrice&);
  inline mat_iter(const mat_iter&);
  inline mat_iter(const Fmatrice&, 
		  Fmatrice::vector<double>* const);
  inline mat_iter(const Fmatrice&, int);
  inline ~mat_iter();

  ///////////////////////////////////////
  // donnees

  inline const Fmatrice* mat() const;

  inline double operator()(const int) const;
  // retourne la valeur en ligne int_1
  // et 0 si ligne hors limites
  inline double somme(const Cdescripteur&) const;
  // retourne la somme des valeurs ou Cdescripteur_1 est >=

  inline double log_sum_exp() const;
  // retourne le log de la somme des exp a la position
  
  ///////////////////////////
  //  modifications
  
  inline void init();
  inline void finit();
  inline void init(int i);
  inline void operator=(const mat_iter&);
  inline void colle(const Fmatrice&);

  /////////////////////
  // iteration
  
  // incremente l'iterateur et retourne 0 quand ne peut plus avancer
  inline bool operator()();
  inline bool operator()(const mat_iter&);
  // renvoie 0 si egal a mat_iter_1
  inline bool operator++();
  inline bool operator++(int);
  inline bool operator+=(int);
  //renvoie 0 si est deja a la limite et met a fin si depasse

  // decremente l'iterateur et retourne 0 quand ne peut plus reculer
  inline bool operator--();
  inline bool operator--(int);
  inline bool operator-=(int);
  //renvoie 0 si est deja a la limite et met a deb si depasse

  inline mat_iter operator+(int i) const;

  ////////////////////////////
  //  tests

  inline bool fin() const;
  // retourne 1 ssi l'iterateur est a la fin
  inline bool deb() const;
  // retourne 1 ssi l'iterateur est au debut (fin-1)
  
  inline bool operator==(const mat_iter&) const;
  inline bool operator!=(const mat_iter&) const;

  inline bool operator<=(const mat_iter&) const;
  inline bool operator>=(const mat_iter&) const;
  inline bool operator<(const mat_iter&) const;
  inline bool operator>(const mat_iter&) const;

  friend class Fmatrice;
};

#include "mat_iter.inl"

#endif
