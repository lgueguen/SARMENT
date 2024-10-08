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

#if !defined _MOTIF_H
#define _MOTIF_H

#include "desc_comp.h"
#include "Tliste.h"

class Cmotif : public Cdesc_comp
{
  Cmotif* _pr;	// abc._pr=cab
  Cmotif* _su;	// abc._su=bca

public:

  inline Cmotif(void*);
  inline Cmotif();
  inline ~Cmotif();
	
  ///////////////////////////
  // modifications

  inline void operator=(const Cdesc_comp&);

  inline void g_su(Cmotif*);
  inline void g_pr(Cmotif*);
	
  const Cmotif& justif_d(int) const;	
  // retourne le motif decale de int_1 vers la droite
  const Cmotif& justif_g(int) const;	
  // retourne le motif decale de int_1 vers la gauche

  /////////////////////////////
  //  donnees

  inline Cmotif* su() const;
  inline Cmotif* pr() const;

  inline void liste_desc(Tliste<Cdescripteur>&) const;
  // remplit la liste avec ses descripteurs

  inline void liste_desc(Tliste<Cdesc_comp>&) const;
  // remplit la liste avec ses desc_comp

  /////////////////////////////
  // Calculs

  inline int lg() const;

  int modulo(const Cmotif&) const;	
  //  calcule le dephasage avec Cmotif_1
  //             bca.modulo(abc)=1
  //  si les deux motifs sont differents
  //       retourne la longueur du plus long
  
  ///////////////////////////
  //  Sorties

  ostream& operator<<(ostream&) const;
  void ecrit_num(ostream&) const;
  
  //////////////////////////
  //   tests

  bool tourne(const Cmotif&) const;	
  // true si rotation l'un de l'autre
	
  inline bool operator==(const Cmotif&) const; 
  // ab == abab  ms ab!=ba
  inline bool operator!=(const Cmotif&) const;

};

#include "motif.inl"

#endif 
