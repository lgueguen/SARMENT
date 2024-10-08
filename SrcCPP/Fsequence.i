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

%module Fsequence

%{
#include "sequence.h"
#include <exception>
  %}

%exception __getitem__
{
  try {
    $action
      }
  catch (std::exception &e) {
    PyErr_SetString(PyExc_IndexError, "Mauvais indice");
    return NULL;
  }
}

#ifdef SWIGPYTHON
%typemap(in) (char* str, int len) {
  $1 = PyString_AsString($input);
  $2 = PyString_Size($input);
}

%typemap(out) char* {
  $result=PyString_FromString($1);
  free($1);
}
#endif

class Fsequence 
{
public:

  Fsequence();
  ~Fsequence();

  //melange par transpositions
  void melange(int);

  void recup_rel(const Fsequence&, int, int);

  void termine(int);

  int vtaille() const;
  
  %extend {

    char __getitem__(int i){
      if ((i<-self->taille()) || (i>=self->taille()))
	throw std::exception();
      return self->operator[](i).dsc();
    }

    void __setitem__(int i, char c){
      self->operator[](i).g_dsc(c);
    }
      
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

    void recup_rel(const Csequence& C, int i, int j){
      self->recup_rel((Fsequence&)C,i,j);
    }
  }  
};

