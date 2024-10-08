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

#if !defined _POSITION_H
#define _POSITION_H

#include <stdlib.h>

class Cposition  
{
protected:
  int _pos;
	
public:

  inline Cposition();
  inline Cposition(const Cposition&);
  inline Cposition(const int);
  inline Cposition(void*);

  ~Cposition() {};

  inline int pos() const;


  /////////////////////////
  ///// modificateurs /////

  inline void g_pos(const int);
  inline void operator=(const Cposition&);
  inline void operator++(int);
  inline void operator--(int);
  inline void operator++();
  inline void operator--();
  inline void operator+=(int);
  inline void operator-=(int);

  /////////////////////////
  /////   tests       /////
  
  inline bool operator==(const Cposition&) const;
  inline bool operator!=(const Cposition&) const;
  inline bool operator<(const Cposition&) const;
  inline bool operator>(const Cposition&) const;
  inline bool operator<=(const Cposition&) const;
  inline bool operator>=(const Cposition&) const;
  
  inline bool entre(const Cposition&, const Cposition&) const;

  ////////////////////////
  ///  pour les Noeuds ///

  inline void g_motif(void*) {};
  inline void* motif() const {return 0;}

  inline void init() {_pos=0;}
  inline void avance() {_pos++;} 
  inline void recule() {_pos--;} 

  inline bool Noeud_normal() const;

};

#include "position.inl"

#endif 
