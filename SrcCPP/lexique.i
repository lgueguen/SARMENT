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

%module Clexique


%{
#include "lexique.h"
#include <sstream>
#include "sequence.h"
#include "matrice.h"
#include <exception>
#include "desc_comp.h"
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

%typemap(in) (char* str, int len) {
  $1 = PyString_AsString($input);
  $2 = PyString_Size($input);
}
#endif

class Clexique
{
public:

  Clexique();
  ~Clexique();

  int g_trans(int, int, double);
  void init_trans();
  void init_trans_vers(int);
  bool est_vide() const;
  int nb_desc() const;
  void met_au_net();
  void enleve(int);

  double val_max(const Fsequence&, int, int);

  double val_max(const Fmatrice& , int, int);    

  %extend{


    /* Ici les +1 sont dus au fait qu'avant les
       segments commencaient en 1*/

    double pred(const Csequence& G, int d, int f){
      return self->pred(G,d+1,f+1);
    }

    double pred(const Cmatrice& M, int d, int f){
      return self->pred(M,d+1,f+1);
    }

    double pred(const Fsequence& G, int d, int f){
      return self->pred(G,d+1,f+1);
    }

    double pred(const Fmatrice& M, int d, int f){
      return self->pred(M,d+1,f+1);
    }

    double llh(const Fsequence& G, int d, int f){
      return self->llh(G,d+1,f+1);
    }

    double llh(const Csequence& G, int d, int f){
      return self->llh(G,d+1,f+1);
    }

    double val_max(const Csequence& G, int d, int f){
      return self->val_max(G,d,f);
    }
    
    double val_max(const Cmatrice& M, int d, int f){
      return self->val_max(M,d,f);
    }

    void read_fich(char* fic, char* alp=0){
      string st(fic);
      string tt;
      tt = (alp)?alp:"";
      self->read_fichrecup(st,tt);
    }

    void read_str(char* fic, char* alp=0){
      string st(fic);
      string tt;
      tt = (alp)?alp:"";
      self->recup(st,tt,0);
    }


    void aj_str(char* fic){
      string st(fic);
      self->recup(st,"",1);
    }

    char* __str__(){
      std::ostringstream out;
      self->operator<<(out);
      string st=out.str();
      char* tmp=(char*)malloc(sizeof(char)*(st.length()+1));
      strcpy(tmp,st.c_str());
      return tmp; 
    }

    char* ls_num(){
      std::ostringstream out;
      self->ls_num(out);
      string st=out.str();
      char* tmp=(char*)malloc(sizeof(char)*(st.length()+1));
      strcpy(tmp,st.c_str());
      return tmp; 
    }
        
    Cdesc_comp __getitem__(int i){
      Cdesc_comp* pd;
      Cdesc_comp d;
      if ((pd=(Cdesc_comp*)self->desc(i))){
        d=*pd;
        return d;
      }
      else
        throw std::exception();
    }

    void __setitem__(int i, Cdesc_comp& d){
      Cdesc_comp* pd;
      if ((pd=(Cdesc_comp*)self->desc(i))){
        pd->operator=(d);
      }
      else
        throw std::exception();
    }

    ////////////// log_vraisemblance ////////////////
    
    PyObject* log_vraisemblance(const Fsequence& f, int d){
      Tliste<double> t=self->log_vraisemblance(f,d);

      Tliste_iter<double> it(t);
      PyObject* po=PyList_New(0);
      double* pi;
      while ((pi=it()))
        PyList_Append(po, PyFloat_FromDouble(*pi));
      return po;
    }
    
    PyObject* log_vraisemblance(const Csequence& f, int d){
      Tliste<double> t=self->log_vraisemblance(f,d);

      Tliste_iter<double> it(t);
      PyObject* po=PyList_New(0);
      double* pi;
      while ((pi=it()))
        PyList_Append(po, PyFloat_FromDouble(*pi));
      return po;
    }
    
    PyObject* log_vraisemblance(const Fmatrice& f, int d){
      Tliste<double> t=self->log_vraisemblance(f,d);
      Tliste_iter<double> it(t);
      PyObject* po=PyList_New(0);
      double* pi;
      while ((pi=it()))
        PyList_Append(po, PyFloat_FromDouble(*pi));
      return po;
    }

    PyObject* log_vraisemblance(const Cmatrice& f, int d){
      Tliste<double> t=self->log_vraisemblance(f,d);
      Tliste_iter<double> it(t);
      PyObject* po=PyList_New(0);
      double* pi;
      while ((pi=it()))
        PyList_Append(po, PyFloat_FromDouble(*pi));
      return po;
    }

    //////////////// ecrit_evalue //////////////////////
    
    char* ecrit_evalue(const Fsequence& G, int d, int f){ 
      std::ostringstream out;
      self->ecrit_evalue(G,out,d+1,f+1);
      string st=out.str();
      char* tmp=(char*)malloc(sizeof(char)*(st.length()+1));
      strcpy(tmp,st.c_str());
      return tmp; 
    }

    
    char* ecrit_evalue(const Csequence& G, int d, int f){
      std::ostringstream out;
      self->ecrit_evalue((Fsequence&)G,out,d+1,f+1);
      string st=out.str();
      char* tmp=(char*)malloc(sizeof(char)*(st.length()+1));
      strcpy(tmp,st.c_str());
      return tmp; 
    }

    char* ecrit_evalue(const Fmatrice& G, int d, int f){ 
      std::ostringstream out;
      self->ecrit_evalue(G,out,d+1,f+1);
      string st=out.str();
      char* tmp=(char*)malloc(sizeof(char)*(st.length()+1));
      strcpy(tmp,st.c_str());
      return tmp; 
    }

    char* ecrit_evalue(const Cmatrice& G, int d, int f){
      std::ostringstream out;
      self->ecrit_evalue((Fmatrice&)G,out,d+1,f+1);
      string st=out.str();
      char* tmp=(char*)malloc(sizeof(char)*(st.length()+1));
      strcpy(tmp,st.c_str());
      return tmp; 
    }

  }
};
