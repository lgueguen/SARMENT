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

#if !defined _POSOPERE_H
#define _POSOPERE_H

#include <fstream>

#include "segment.h"
#include "Tliste.h"

class Cposition;
class Cposi_motif;
class Part_equ;
class Part_equ_iter;
class mot_ces;

using namespace std;

template<class S> class S_lex;
template<class R> class R_lex;
template<class S> class Smotif_Noeud_val;

class Cposopere{

  Tliste<Csegment> _fils;

  /////////////////////
  // recuperations 

  //         des segments maxima calcules a partir des parametres

  inline void recup_max(const S_lex<Cposition >&, 
			Cposition&, Cposition&);
  inline void recup_max(const Smotif_Noeud_val<Cposi_motif >&, 
			Cposi_motif&, Cposi_motif&);
  inline void recup_max(const R_lex<Cposition>&, 
			Cposition&, Cposition&);
  
  void recup(const mot_ces&);
  //  a partir d'un mot_ces

  //  a partir d'un fichier

  char recup(ifstream&);

  /////////////////////////////////
  /// evaluations

  int nbe_segments() const;
  double var_borne_deb() const;

  int max_f() const;
  int max_d() const;
  int min_f() const;
  int min_d() const;

  //////////////////////////////////
  // sorties

  void dessine_segments(double, double, ofstream&) const;
  // dans le graphe des partitions ex aequo
  //          appelee par Part_equ

  /////////////////////////////
  ///    modifications
  
  inline void ajoute(const Csegment&);
  void aj_vide();
  //  ajoute un segment vide

  void operator=(const Cposopere&);

public:

  Cposopere() {};
  Cposopere(void*) {};
  Cposopere(const Cposopere&);
  inline ~Cposopere();


  ///////////////////////////////
  //  amities

  friend class Part_equ;
  friend class mot_ces;
  friend class Part_equ_iter;

  /////////////////////
  // sorties

  ostream& synt(ostream&) const;
  ostream& operator<<(ostream&) const;	

};

#include "posopere.inl"

#endif
