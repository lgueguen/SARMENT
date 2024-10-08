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

#if !defined _LISTE_H
#define _LISTE_H

#include "el_liste.h"

class Liste_iter;

template<class T> 
class Tliste;

class Liste  
{
private:
  el_liste* _fin;

  inline void met_fin(el_liste*);

  inline Liste();
  inline Liste(el_liste*);
  ~Liste() {};

  /////////////////////////////
  // modifications
  /////////////////////////////

  inline void nettoie();

  inline void aj_deb(el_liste*);
  inline void aj_fin(el_liste*);

  inline el_liste* enl_deb();
  inline el_liste* enl_fin();
  inline el_liste* enl_i(int);

  el_liste* enl_iter(Liste_iter&);
  
  inline void echange(Liste&);

  /////////////////////////////
  // donnees
  /////////////////////////////

  inline el_liste* ind_deb() const;
  inline el_liste* ind_i(int) const;
  inline el_liste* ind_fin() const;
  
  //////////////////
  // tests
  //////////////////

  inline bool est_vide() const;

  inline void permute_mm_fin(el_liste*, el_liste*);
  // opere une permutation circulaire dans le sens de la liste
  // de el_liste*_1->_so (!!!)  vers el_liste*_2
  // fin est conservee

  friend class Liste_iter;
  
  template<class T>
  friend class Tliste;
};

template<class T>
struct el_Tliste : public el_liste {
  T _ind;
  el_Tliste(const T& a) : _ind(a) {  }
  el_Tliste() {   }
};

#include "Liste.inl"

#endif 

