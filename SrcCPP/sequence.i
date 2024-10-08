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

%module Csequence
%include std_string.i
using std::string;

%{
#include "sequence.h"
#include <exception>
#include <sstream>
  %}

%exception __getitem__
{
  try {
    $action
      }
  catch (std::exception &e) {
    PyErr_SetString(PyExc_IndexError, "Bad index");
    return NULL;
  }
}

#ifdef SWIGPYTHON
%typemap(out) char* {
  $result=PyString_FromString($1);
  free($1);
}
#endif

class Csequence 
{
public:
  
  ~Csequence();
  Csequence(int = 0);

  // genere sur la longueur $1 a vide
  void genere(int);
  
  %extend {
    
    int vtaille(){
      return self->vtaille();
    }

    void termine(int i){
      self->termine(i);
    }
    
    //melange par transpositions
    void melange(int i){
      ((Fsequence*)self)->melange(i);
    }

    void copie(const Csequence& G){
      self->egale(G);
    }
	      
    int __len__(){
      return self->taille();
    }

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

    int read_nf(char* s){
      self->nettoie();
      string st(s);
      int l=st.length();
      if (st.rfind(".seq")==l-4){
	if (!self->recup_seq(st))
	  return 0;
      }
      else
	if ((st.rfind(".fst")==l-4)
	    || (st.rfind(".fas")==l-4)
            || (st.rfind(".fasta")==l-6)
            || (st.rfind(".fa")==l-3)){
	  if (!self->recup_fst(st))
	    return 0;
	}
	else{
	  cout << "Bad format" << endl;
	  return 0;
	}
      return self->taille();

    }

    
  }
};

      
