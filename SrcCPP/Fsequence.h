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

#ifndef _FGENOME_H
#define _FGENOME_H

#include <string>

#include "descripteur.h"
#include "Tliste_iter.h"

//// Csequence sans allocation de memoire 

using namespace std;

class Csegment;

class Fsequence
{
protected:


  ///////////////////////////////////////////////////////
  // attributs
  
  Cdescripteur* _deb;
  Cdescripteur* _fin;
  Cdescripteur* _vfin;
  
public:

  Fsequence();
  Fsequence(void*);
  Fsequence(const Fsequence&);
  ~Fsequence();
  void nettoie();

  ///////////////
  // calculs/donnees
  //////////////

  inline Cdescripteur& operator[](const int) const;
  inline int taille() const;
  inline int vtaille() const;

  inline const Cdescripteur* deb() const;
  inline const Cdescripteur* fin() const;

  
  ///////////////
  /// modifs

  void termine(int);
  void operator=(const Fsequence&);

  // Csegment est en positions relatives à Fsequence
  //  (ie est dans le repere <1,Fsequence.taille()>
  //  bool recup_rel(const Fsequence&, const Csegment&);

  //En relatif, à partir de 0 avec les extrémités incluses
  void recup_rel(const Fsequence&, int, int);

  // pratique i permutations aleatoires
  void melange(int i);

  ////////////////////
  // sorties
  ////////////////////

  ostream& operator<<(ostream&) const;
};

#include "Fsequence.inl"

#endif
