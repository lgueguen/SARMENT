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

#ifndef ARRET_H
#define ARRET_H

#include "parti_comp.h"

using namespace std;

class Arret 
{
  
protected:
  enum TpAR {INC, NBP};
  TpAR _TpAR;

public:
  inline Arret(TpAR = INC);
  inline Arret(const Arret&);
  virtual inline ~Arret();

  bool init();
  bool arrete(const Part_equ&) const;
  Arret* copie() const;
  
};


class Nbe_parts : public Arret
{
  int _nbp;
public:
  inline Nbe_parts(int = 0);
  inline Nbe_parts(const Nbe_parts&);
  inline ~Nbe_parts();

  bool init();
  inline bool arrete(const Part_equ&) const;
};

#include "Arret.inl"

#endif
