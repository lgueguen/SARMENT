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

%module Cmatrice
%include std_string.i
using std::string;

%{
#include "sequence.h"
#include "matrice.h"
  %}

#ifdef SWIGPYTHON
%typemap(in) (const std::string& st) {
   $1 = PyString_AsString($input);
 }

// %typemap(out) char* {
//   $result=PyString_FromString($1);
//   free($1);
// }
#endif


class Cmatrice 
{
  
public:

  Cmatrice();
  ~Cmatrice();

  char carac(int) const;

  void g_val(const double, const int, const int);

  void FB(const Fsequence&, const Clexique&);
  void FB(const Fmatrice&, const Clexique&);
    
  void forward(const Fsequence&, const Clexique&);
  void forward(const Fmatrice&, const Clexique&);

  void backward(const Fsequence&, const Clexique&);
  void backward(const Fmatrice&, const Clexique&);
  
  void smoothe(const Fsequence&, int , int, char);
  void smoothe(const Fmatrice&, int, int, char);

  void predictions(const Fmatrice&, const Clexique& );
  void predictions(const Fsequence&, const Clexique&);

  void exponentielle();
  
  void logarithme();

  void log_normalise();
  
  void integre(const Fmatrice&);

  void integre(const Fsequence&);

  void derive(const Fmatrice&);
  
  void derive(const Fsequence&);

  %extend {

    void copy(const Fmatrice& F){
      self->copie(F);
    }

    void copy(const Cmatrice& F){
      self->copie(F);
    }
    
    void melange(){
      ((Fmatrice*)self)->melange();
    }
    	
    void FB(const Csequence& F, const Clexique& L){
      self->FB(F,L);
    }

    void FB(const Cmatrice& F, const Clexique& L){
      self->FB(F,L);
    }

    void forward(const Csequence& F, const Clexique& L){
      self->forward(F,L);
    }
    
    void forward(const Cmatrice& F, const Clexique& L){
      self->forward(F,L);
    }
    
    void backward(const Csequence& F, const Clexique& L){
      self->backward(F,L);
    }
  
    void backward(const Cmatrice& F, const Clexique& L){
      self->backward(F,L);
    }

    void smoothe(const Csequence& g, int sz, int st, char c){
      self->smoothe(g,sz,st,c);
    }
    
    void smoothe(const Cmatrice& g, int sz, int st, char c){
      self->smoothe(g,sz,st,c);
    }

    void predictions(const Csequence& g, const Clexique& l){
      self->predictions(g,l);
    }
    
    
    void predictions(const Cmatrice& g, const Clexique& l){
      self->predictions(g,l);
    }
    
    void derive(const Csequence& g){
      self->derive(g);
    }
    void derive(const Cmatrice& g){
      self->derive(g);
    }

    void integre(const Csequence& g){
      self->integre(g);
    }
    void integre(const Cmatrice& g){
      self->integre(g);
    }
    
    void genere(int i, char* str, int len){
      self->genere(i,str,len);
    }

    void read_nf(const std::string& str){
      self->recup_mat(str);
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

    int ndesc(){
      return self->nval();
    }

    double __call__(int i, int j){
      //int_1 est le numero de descripteur et int_2 celui de ligne
      return self->operator()(i,j);
    }

  }  
};

