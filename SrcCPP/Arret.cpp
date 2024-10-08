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

#include "Arret.h"

bool Arret::arrete(const Part_equ& sq) const
{
  switch  (_TpAR){
  case INC:
    return 1;
  case NBP:
    return ((Nbe_parts*)this)->arrete(sq);
  default:
    return 0;
  }
}

bool Arret::init()
{
  switch  (_TpAR){
  case INC:
    return 1;
  case NBP:
    return ((Nbe_parts*)this)->init();
  default:
    return 0;
  }
}

Arret* Arret::copie() const
{
  switch (_TpAR){
  case INC:
    return new Arret();
  case NBP:
    return (Arret*)new Nbe_parts(*(Nbe_parts*)this);
  default:
    return 0;
  }
}

/////////////////////////////////////////


bool Nbe_parts::init() 
{
  do{
    cout << "nombre de classes: ";
    cin >> _nbp;
    if (!cin)
      cin.clear();
    cin.ignore(100,'\n');
  }
  while (_nbp<0);
  if (_nbp==0)
    return 0;
  return 1;
}

