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

%module Cparti_simp
%include "typemaps.i"

%{
  #include "sequence.h"
  #include "parti_simp.h"
  #include "parti_comp.h"
  #include "Arret.h"
  #include <sstream>
  %}

#ifdef SWIGPYTHON
%typemap(out) char* {
  $result=PyString_FromString($1);
  free($1);
}
#endif

class Cparti_simp
{
public:
  Cparti_simp();
  ~Cparti_simp();

  int np_max() const;
  double val_max() const;
  int nb_seq() const;
  
  %extend{
    void lisse(Csequence& g, Clexique& l, int i){
      Parti_comp p(i);
      p.li_simp((Fsequence&)g,l);
      
      self->recup(p);
    }
    
    void lisse(Fsequence& g, Clexique& l, int i){
      Parti_comp p(i);
      p.li_simp(g,l);
      self->recup(p);
    }

    void lisse(Cmatrice& g, Clexique& l, int i){
      Parti_comp p(i);
      p.li_simp((Fmatrice&)g,l);
      self->recup(p);
    }
    
    void lisse(Fmatrice& g, Clexique& l, int i){
      Parti_comp p(i);
      p.li_simp(g,l);
      self->recup(p);
    }

    char* __getitem__(int i){
      Cpartition* p;
      std::ostringstream out;
      string st;
      if ((p=self->seq(i))){
	p->operator<<(out);
	st=out.str();
      }
      else st="";
      char* tmp=(char*)malloc(sizeof(char)*(st.length()+1));
      strcpy(tmp,st.c_str());
      return tmp;
    }

    char* __str__(){
      std::ostringstream out;
      self->operator<<(out);
      string st=out.str();
      char* tmp=(char*)malloc(sizeof(char)*st.length());
      strcpy(tmp,st.c_str());
      return tmp; 
    }
  }
};


