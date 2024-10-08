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

#if !defined _GENOME_H
#define _GENOME_H


#include <fstream> 
#include <string>
#include <sstream>

#include "Fsequence.h"


using namespace std;

class Csegment;
class Cpartition;

class Csequence : public Fsequence
{
public:

  void genere(int);
  
  ////////////////////////////////
  // recuperations de fichiers
  ////////////////////////////////

  bool recup_seq(const string&);
  bool recup_fst(const string&);

  // d'une position dans le fichier fasta,
  // recupere la sequence qui suit
  // retourne la position du > suivant
  //             -1 quand eof
  //             -2 quand echec
  int recup_fst(ifstream&, int);
  

  //////////////////////
  // modifs

//   void melange(const Csequence&, const Cpartition* const = 0);
  
  void operator=(const Fsequence&);

  void egale(const Fsequence&);
  
  // Csegment a ses positions relatives à Fsequence
  //  (ie est dans le repere <1,Fsequence.taille()>
  bool recup_rel(const Fsequence&, const Csegment&);

  Csequence(int = 0);
  Csequence(void*);
  Csequence(const Fsequence&);
  ~Csequence();
  void nettoie();
};

#endif 
