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

#if !defined(_STOCK_H)
#define _STOCK_H

#include "Tliste_iter.h"
#include "stdlib.h"


template<int T, class W>
class Paquet
{
  W* _elem;
public:
  Paquet() {
    if (!(_elem=(W*)malloc(sizeof(W)*T))){
      cerr << "pbm d'allocation" << endl;
      exit(1);
    }
  }

  ~Paquet()
  { free(_elem);}

  W* elem() {return _elem;}

};


template<int T, class W>
class Stock  
{
  static Tliste<Paquet<T, W> > _Li;
  static Tliste<void*> _Dispo;
  
public:
  Stock();
  ~Stock();
  static W* aj_elem(const W&);
  static W* aj_elem();
  static void enl_elem(void*);
  static void detruit();
  
};

#include "Stock.inl"
#include "Les_stocks.h"

#endif 

