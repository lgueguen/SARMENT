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

%module Cpartition

%{
  #include "sequence.h"
  #include "partition.h"
  #include <sstream>
  %}

#ifdef SWIGPYTHON
%typemap(out) char* {
  if ($1){
    $result=PyString_FromString($1);
    free($1);
  }
  else
    $result=PyString_FromString("");
}
#endif

class Cpartition
{
public:
  Cpartition();
  ~Cpartition();

  double val() const;

  void viterbi(const Fsequence&, const Clexique&, int);
  
  void viterbi(const Fmatrice&, const Clexique&, int);
  
  %extend{
    
    void viterbi(const Csequence& s, const Clexique& l, int i){
      self->viterbi((Fsequence&)s,l,i);
    }
     
    void viterbi(const Cmatrice& m, const Clexique& l, int i){
      self->viterbi((Fmatrice&)m,l,i);
    }

    char* __str__(){
      std::ostringstream out;
      self->operator<<(out);
      string st=out.str();
      if (st.length()==0)
        return 0;
      char* tmp=(char*)malloc(sizeof(char)*st.length());
      strcpy(tmp,st.c_str());
      return tmp; 
    }
  }
};


