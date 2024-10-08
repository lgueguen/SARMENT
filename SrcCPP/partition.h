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

#if !defined _PARTITION_H
#define _PARTITION_H

#include <fstream>

#include "segment.h"

class Part_equ;
class Part_equ_iter;
class Cparti_simp;
class Fonction_sg;
class Fsequence;
class Clexique;

using namespace std;

class Cpartition 
{
private:
  Csegment* _seq;	
  int _lg;
  double _val;

  ///////////////////////
  //  calculs

  double compacite() const;

public:

  inline Cpartition();
  inline Cpartition(void*);
  Cpartition(const Cpartition&);
  Cpartition(const Part_equ_iter&);

  inline ~Cpartition();
  inline void nettoie();

  //////////////////////////
  //  modifications  

  void operator=(const Cpartition&);
  inline void g_val(const double);

  void viterbi(const Fsequence&, const Clexique&, int);
  void viterbi(const Fmatrice&, const Clexique&, int);

  /////////////////////////
  ///  donnees

  inline const Csegment& operator[](const int) const;
  inline int taille() const;
  inline int dern_pos() const;
  inline double val() const ;

  ///////////////////////////
  ///   recuperations

  void recup(const Part_equ&);
  void recup(const Part_equ_iter&);
  bool recup(ifstream&);
  bool recup(const string&);

//   double select_min(const Cseq_equ&, Fonction_sg&);
//   double select_max(const Cseq_equ&, Fonction_sg&);

  int nbe_mm_pred(const Cpartition&) const;
	
  ///////////////////////
  //  ordres

  void ordre();

  ///////////////////////
  // sorties

  ostream& operator<<(ostream& ) const;

  /////////////////////
  //  amities
  
  friend class Cpartitionnement;
  friend class Cseq_equ;
};

#include "partition.inl"

#endif

