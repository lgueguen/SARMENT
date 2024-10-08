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

%module Fmatrice

%{
#include "sequence.h"
#include "matrice.h"
  %}

// #ifdef SWIGPYTHON
// %typemap(in) (char* str, int len) {
//   $1 = PyString_AsString($input);
//   $2 = PyString_Size($input);
// }

// %typemap(out) char* {
//   $result=PyString_FromString($1);
//   free($1);
// }
// #endif

class Fmatrice 
{
  
public:

  Fmatrice();
  ~Fmatrice();

  char carac(int) const;

  void g_val(const double, const int, const int);

  void recup_rel(const Fmatrice&, int, int);

  void melange();

  void exponentielle();

  void log_normalise();
  
  %extend {

    char* __str__(){
      std::ostringstream out;
      self->operator<<(out);
      string st=out.str();
      char* tmp=(char*)malloc(sizeof(char)*(st.length()+1));
      strcpy(tmp,st.c_str());
      return tmp; 
    }

    int __len__(){
      return self->taille();
    }

    int ndesc(){
      return self->nval();
    }

    void recup_relC(const Cmatrice& C, int i, int j){
      self->recup_rel(C,i,j);
    }
    
    double __call__(int i, int j){
      //int_1 est le numero de descripteur et int_2 celui de ligne
      return self->operator()(i,j);
    }
  }  
};

