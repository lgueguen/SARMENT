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


%module Cdesc_comp

%{
#include "desc_comp.h"
#include <sstream>
#include "sequence.h"
#include "matrice.h"
#include "mat_iter.h"
  %}

#ifdef SWIGPYTHON
%typemap(out) char* {
  $result=PyString_FromString($1);
  free($1);
}

%typemap(in) (char* str, int len) {
  $1 = PyString_AsString($input);
  $2 = PyString_Size($input);
}
#endif

class Cdesc_comp
{
public:
  Cdesc_comp();
  ~Cdesc_comp();

  %extend{
    
    void equals(const Cdesc_comp& d){
      self->operator=(d);
    }
    
    int read_str(char* st,int num){
      int i;
      string s(st);
      i=self->construit_de(s,0,num);
      return i;
    }

    char* __str__(){
      std::ostringstream out;
      self->operator<<(out);
      string st=out.str();
      char* tmp=(char*)malloc(sizeof(char)*(st.length()+1));
      strcpy(tmp,st.c_str());
      return tmp; 
    }

    double pred(const Csequence& G, int p){
      return self->multiplie(G.deb(),G.deb()+p,G.fin());
    }

    double pred(const Cmatrice& M, int p){
      static mat_iter m;
      m.colle(M);
      m.init(p);
      return self->multiplie(m);
    }

    double pred(const Fsequence& G, int p){
      return self->multiplie(G.deb(),G.deb()+p,G.fin());
    }

    double pred(const Fmatrice& M, int p){
      static mat_iter m;
      m.colle(M);
      m.init(p);
      return self->multiplie(m);
    }
  }
};


