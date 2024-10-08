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

#if !defined _DESCRIPTEUR_H_
#define _DESCRIPTEUR_H_

#include <iostream>
#include <ctype.h>

using namespace std;

class Cdescripteur  
{
  char _dsc;

public:
  inline Cdescripteur(int);
  Cdescripteur() {};
  Cdescripteur(const Cdescripteur& d) : _dsc(d._dsc) {};
  Cdescripteur(const char& c) : _dsc(c) {};
  ~Cdescripteur() {};
  
  inline void g_dsc(const char);
  inline char dsc() const;

  inline void operator=(const Cdescripteur&);
  inline void operator=(const char);
  
  bool cree_de_char(char);
  // retourne 1 ssi le char est alpha

  ///////////////
  //// tests ////

  inline bool operator==(const Cdescripteur&) const;
  inline bool operator!=(const Cdescripteur&) const;

  //  ? > a

  inline bool operator>(const Cdescripteur&) const;	
  inline bool operator<(const Cdescripteur&) const;
  inline bool operator>=(const Cdescripteur&) const;
  inline bool operator<=(const Cdescripteur&) const;

  inline bool est_alpha() const;
  // retourne 1 si le descripteur est dans l'alphabet (isalpha)


  ////////////////
  //// sortie //// 

  ostream& operator<<(ostream&) const;
};

#include "descripteur.inl"

#endif 
