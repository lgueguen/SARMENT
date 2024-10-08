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

#include "Tliste_iter.h"
#include <stdio.h>

using namespace std;

////////////////////////
// Op_feuille
////////////////////////

Op_feuille::Op_feuille()  
{
  _op=FEU;
}

Op_feuille::~Op_feuille()
{
}

Op_feuille::Op_feuille(const Cdescripteur& d) :
  Cdescripteur(d), _val(1)
{
  _op=FEU;
}

Op_feuille::Op_feuille(const Op_feuille& o) :
  Cdescripteur(o), _val(o._val)
{
  _op=FEU;
}

Op_feuille::Op_feuille(const Cdescripteur& d, double v) :
  Cdescripteur(d), _val(v)
{
  _op=FEU;
}

Op_feuille::Op_feuille(char d, double v) :
  Cdescripteur(d), _val(v)
{
  _op=FEU;
} 

double Op_feuille::multiplie(const Cdescripteur* const poi ,
			     const Cdescripteur* const pod ,
			     const Cdescripteur* const pof )  const
{
  return (Cdescripteur::operator>=(*pod)
	  ?_val
	  :0);
}

double Op_feuille::multiplie(const mat_iter& md)  const
{
  return ((dsc()=='!')
	  ?_val
	  :_val*md.somme(dsc()));
}

double Op_feuille::horslim() const
{
  return (dsc()=='^' || dsc()=='!')?1:0;
}

int Op_feuille::taille()  const
{
  return 1;
}

bool Op_feuille::est_simple()  const
{
  return 1;
}

bool Op_feuille::possede(const Cdescripteur& d)  const
{
  return Cdescripteur::operator==(d);
}

bool Op_feuille::est_alpha()  const
{
  return Cdescripteur::est_alpha();
}

////////////////////////
// Op_plus
////////////////////////

Op_plus::Op_plus()
{
  _op=PLUS;
  _voo=new Var_Op(lpop);
}

Op_plus::Op_plus(const Op_plus& o) 
{
  _op=PLUS;
  Op_desc* pop;
  _voo=new Var_Op(lpop);

  o._voo->it.init();
  while ((o._voo->ppop=o._voo->it())){
    pop=(*(o._voo->ppop))->copie();
    lpop.aj_fin(pop);
  }
}

Op_plus::~Op_plus()
{
  _voo->it.init();
  while ((_voo->ppop=_voo->it()))
    delete (*(_voo->ppop));
  lpop.nettoie();
  delete _voo;
}

double Op_plus::multiplie(const Cdescripteur* const poi,
			  const Cdescripteur* const pod,
			  const Cdescripteur* const pof )  const
{
  _voo->it.init();
  _voo->val=0;
  while ((_voo->ppop=_voo->it()))
    _voo->val+=(*(_voo->ppop))->multiplie(poi, pod, pof);
  return _voo->val;
}

double Op_plus::multiplie(const mat_iter& md)  const
{
  _voo->it.init();
  _voo->val=0;
  while ((_voo->ppop=_voo->it()))
    _voo->val+=(*(_voo->ppop))->multiplie(md);
  return _voo->val;
}

double Op_plus::horslim() const
{
  _voo->it.init();
  _voo->val=0;
  while ((_voo->ppop=_voo->it()))
    _voo->val+=(*(_voo->ppop))->horslim();
  return _voo->val;
}

int Op_plus::taille()  const
{
  _voo->it.init();
  int a,b=0;
  
  while ((_voo->ppop=_voo->it()))
    if ((a=(*(_voo->ppop))->taille())>b)
      b=a;
  return b;
}

bool Op_plus::est_simple()  const
{
  return (lpop.nb_ind()>1)?0
    :lpop.ind_deb()?(*lpop.ind_deb())->est_simple():1;
}

bool Op_plus::possede(const Cdescripteur& d)  const
{
  _voo->it.init();
  while ((_voo->ppop=_voo->it()))
    if ((*(_voo->ppop))->possede(d))
      return 1;
  return 0;
}
  
bool Op_plus::est_alpha()  const
{
  _voo->it.init();
  while ((_voo->ppop=_voo->it()))
    if (!((*(_voo->ppop))->est_alpha()))
      return 0;
  return 1;
}
  
////////////////////////
// Op_plus_tom
////////////////////////

Op_plus_tom::Op_plus_tom()
{
  _op=PLUS_TOM;
  _voo=new Var_Op(lpop);
}

Op_plus_tom::Op_plus_tom(const Op_plus_tom& o) 
{
  _op=PLUS_TOM;
  Op_desc* pop;
  _voo=new Var_Op(lpop);
  
  o._voo->it.init();
  while ((o._voo->ppop=o._voo->it())){
    pop=(*(o._voo->ppop))->copie();
    lpop.aj_fin(pop);
  }
}

Op_plus_tom::~Op_plus_tom()
{
  _voo->it.init();

  while ((_voo->ppop=_voo->it()))
    delete (*(_voo->ppop));
  lpop.nettoie();
}

double Op_plus_tom::multiplie(const Cdescripteur* const poi,
			      const Cdescripteur* const pod,
			      const Cdescripteur* const pof )  const
{						   
  const Cdescripteur *la=pod;
  _voo->it.init();
  _voo->val=0;

  while ((_voo->ppop=_voo->it())){
    if (la<poi)
      _voo->val+=(*(_voo->ppop))->horslim();
    _voo->val+=(*(_voo->ppop))->multiplie(poi, la--, pof);
  }
  return _voo->val;
}

double Op_plus_tom::multiplie(const mat_iter& md)  const
{
  mat_iter la(md);
  
  _voo->it.init();
  _voo->val=0;
  while ((_voo->ppop=_voo->it())){
    _voo->val+=(*(_voo->ppop))->multiplie(la);
    if (!(la--))
      _voo->val+=(*(_voo->ppop))->horslim();
  }
  return _voo->val;
}
  
double Op_plus_tom::horslim() const
{
  _voo->it.init();
  _voo->val=0;
  
  while ((_voo->ppop=_voo->it()))
      _voo->val+=(*(_voo->ppop))->horslim();
  return _voo->val;  
}


int Op_plus_tom::taille()  const
{
  int i,a,b=0;
  
  i=0;
  _voo->it.init();
  
  while ((_voo->ppop=_voo->it())){
    if ((a=(*(_voo->ppop))->taille()+i)>b)
      b=a;
    i++;
  }
  return b;
}

bool Op_plus_tom::est_simple()  const
{
  return (lpop.nb_ind()>1)?0
    :lpop.ind_deb()?(*lpop.ind_deb())->est_simple():1;
}

bool Op_plus_tom::possede(const Cdescripteur& d)  const
{
  _voo->it.init();

  while ((_voo->ppop=_voo->it()))
    if ((*(_voo->ppop))->possede(d))
      return 1;
  return 0;
}
  
bool Op_plus_tom::est_alpha()  const
{
  _voo->it.init();
  while ((_voo->ppop=_voo->it()))
    if (!((*(_voo->ppop))->est_alpha()))
      return 0;
  return 1;
}
  
////////////////////////
// Op_mult
////////////////////////

Op_mult::Op_mult()
{
  _op=MULT;
  _voo=new Var_Op(lpop);
}

Op_mult::Op_mult(const Op_mult& o) 
{
  _op=MULT;
  Op_desc* pop;

  _voo=new Var_Op(lpop);

  o._voo->it.init();
  while ((o._voo->ppop=o._voo->it())){
    pop=(*(o._voo->ppop))->copie();
    lpop.aj_fin(pop);
  }
}

Op_mult::~Op_mult()
{
  _voo->it.init();
  while ((_voo->ppop=_voo->it()))
    delete (*(_voo->ppop));
  lpop.nettoie();
  delete _voo;
}

double Op_mult::multiplie(const Cdescripteur* const poi,
			  const Cdescripteur* const pod,
			  const Cdescripteur* const pof )  const
{						   
  _voo->it.init();
  _voo->val=1.0;
  while ((_voo->ppop=_voo->it()))
    _voo->val*=(*(_voo->ppop))->multiplie(poi, pod, pof);
  return _voo->val;
}

double Op_mult::multiplie(const mat_iter& md)  const
{
  _voo->it.init();
  _voo->val=1.0;
  while ((_voo->ppop=_voo->it()))
    _voo->val*=(*(_voo->ppop))->multiplie(md);
  return _voo->val;
}
  
double Op_mult::horslim() const
{
  _voo->it.init();
  _voo->val=0;
  while ((_voo->ppop=_voo->it()))
    _voo->val*=(*(_voo->ppop))->horslim();
  return _voo->val;
}

int Op_mult::taille()  const
{
  _voo->it.init();
  int a,b=0;
  
  while ((_voo->ppop=_voo->it()))
    if ((a=(*(_voo->ppop))->taille())>b)
      b=a;
  return b;
}

bool Op_mult::est_simple()  const
{
  return (lpop.nb_ind()>1)?0
    :lpop.ind_deb()?(*lpop.ind_deb())->est_simple():1;
}

bool Op_mult::possede(const Cdescripteur& d)  const
{
  _voo->it.init();
  while ((_voo->ppop=_voo->it()))
    if ((*(_voo->ppop))->possede(d))
      return 1;
  return 0;
}
  
bool Op_mult::est_alpha()  const
{
  _voo->it.init();
  while ((_voo->ppop=_voo->it()))
    if (!((*(_voo->ppop))->est_alpha()))
      return 0;
  return 1;
}

  
////////////////////////
// Op_mot
////////////////////////

Op_mot::Op_mot() 
{
  _op=MOT;
  _voo=new Var_Op(lpop);
}

Op_mot::Op_mot(const Op_mot& o) 
{
  _op=MOT;
  Op_desc* pop;
  _voo=new Var_Op(lpop);
  
  o._voo->it.init();
  while ((o._voo->ppop=o._voo->it()))
  {
    pop=(*(o._voo->ppop))->copie();
    lpop.aj_fin(pop);
  }
}

Op_mot::~Op_mot()
{
  _voo->it.init();
  while ((_voo->ppop=_voo->it()))
    delete (*(_voo->ppop));
  lpop.nettoie();
  delete _voo;
}

double Op_mot::multiplie(const Cdescripteur* const poi,
			 const Cdescripteur* const pod,
			 const Cdescripteur* const pof )  const
{
  _voo->it.init();
  const Cdescripteur *la=pod;
  
  if (!(_voo->it()))
    return 0;
  while ((_voo->ppop=_voo->it()))
  {
    if (++la>pof){
      if ((*(_voo->ppop))->horslim()==0)
	return 0;
    }
    else
      if ((*(_voo->ppop))->multiplie(poi, la, pof) <= 0)
	return 0;
  }
  return (*lpop.ind_deb())->multiplie(poi,pod,pof);
}

double Op_mot::multiplie(const mat_iter& md)  const
{
  _voo->it.init();
  mat_iter la(md);
  
  if (!(_voo->it()))
    return 0;
  while ((_voo->ppop=_voo->it()))
    if (la++){
      if ((*(_voo->ppop))->multiplie(la) <= 0)
	return 0;
    }
    else
      if ((*(_voo->ppop))->horslim() <=0)
	return 0;
  
  return (*lpop.ind_deb())->multiplie(md);  
}

double Op_mot::horslim() const
{
  _voo->it.init();
  
  if (!(_voo->it()))
    return 0;
  while ((_voo->ppop=_voo->it()))
    if ((*(_voo->ppop))->horslim() <=0)
      return 0;
  
  return (*lpop.ind_deb())->horslim();
}
  
int Op_mot::taille()  const
{
  _voo->it.init();
  int i,a,b=0;

  i=0;
  while ((_voo->ppop=_voo->it()))
  {
    if ((a=(*(_voo->ppop))->taille()+i)>b)
      b=a;
    i++;
  }
  return b;
}

bool Op_mot::est_simple()  const
{
  return (lpop.nb_ind()>1)?0
    :lpop.ind_deb()?(*lpop.ind_deb())->est_simple():1;
}


bool Op_mot::possede(const Cdescripteur& d)  const
{
  _voo->it.init();
  while ((_voo->ppop=_voo->it()))
    if ((*(_voo->ppop))->possede(d))
      return 1;
  return 0;
}
  
bool Op_mot::est_alpha()  const
{
  _voo->it.init();
  while ((_voo->ppop=_voo->it()))
    if (!((*(_voo->ppop))->est_alpha()))
      return 0;
  return 1;
}
  
////////////////////////
// Op_tom
////////////////////////

Op_tom::Op_tom() 
{
  _op=TOM;
  _voo=new Var_Op(lpop);
}

Op_tom::Op_tom(const Op_tom& o) 
{
  _op=TOM;
  Op_desc* pop;
  _voo=new Var_Op(lpop);

  o._voo->it.init();
  while ((o._voo->ppop=o._voo->it())){
    pop=(*(o._voo->ppop))->copie();
    lpop.aj_fin(pop);
  }
}

Op_tom::~Op_tom()
{
  _voo->it.init();
  while ((_voo->ppop=_voo->it()))
    delete (*(_voo->ppop));
  lpop.nettoie();
  delete _voo;
}

double Op_tom::multiplie(const Cdescripteur* const poi,
			 const Cdescripteur* const pod,
			 const Cdescripteur* const pof )  const
{
  const Cdescripteur *la=pod;
  _voo->it.init();
  
  if (!(_voo->it()))
    return 0;
  while ((_voo->ppop=_voo->it())){
    if (--la<poi){
      if ((*(_voo->ppop))->horslim()<=0)
	return 0;
    }
    else
      if ((*(_voo->ppop))->multiplie(poi, la, pof) <= 0)
	return 0;
  }
  return (*lpop.ind_deb())->multiplie(poi, pod,pof);
}

double Op_tom::multiplie(const mat_iter& md)  const
{
  mat_iter la(md);
  _voo->it.init();
  
  if (!(_voo->it()))
    return 0;
  while ((_voo->ppop=_voo->it()))
    if (la--){
      if ((*(_voo->ppop))->multiplie(la) <= 0)
	return 0;
    }
    else
      if ((*(_voo->ppop))->horslim() <=0)
	return 0;
  
  return (*lpop.ind_deb())->multiplie(md);  
}
  
double Op_tom::horslim() const
{
  _voo->it.init();
  
  if (!(_voo->it()))
    return 0;
  while ((_voo->ppop=_voo->it()))
    if ((*(_voo->ppop))->horslim() <=0)
      return 0;
  
  return (*lpop.ind_deb())->horslim();
}
    
  
int Op_tom::taille()  const
{
  int i,a,b=0;

  i=0;
  _voo->it.init();
  while ((_voo->ppop=_voo->it()))
  {
    if ((a=(*(_voo->ppop))->taille()+i)>b)
      b=a;
    i++;
  }
  return b;
}

bool Op_tom::est_simple()  const
{
  return (lpop.nb_ind()>1)?0
    :lpop.ind_deb()?(*lpop.ind_deb())->est_simple():1;
}

bool Op_tom::possede(const Cdescripteur& d)  const
{
  _voo->it.init();
  while ((_voo->ppop=_voo->it()))
    if ((*(_voo->ppop))->possede(d))
      return 1;
  return 0;
}

bool Op_tom::est_alpha()  const
{
  _voo->it.init();
  while ((_voo->ppop=_voo->it()))
    if (!((*(_voo->ppop))->est_alpha()))
      return 0;
  return 1;
}
  
/////////////////////////////////////
/// Bi_pop
/////////////////////////////////////

Bi_pop::Bi_pop() : _pop(0), _poc(0)
{
}

Bi_pop::~Bi_pop()
{
  nettoie();
}

void Bi_pop::nettoie()
{
  if (_pop)
    delete _pop;
  _pop=0;
  if (_poc)
    delete _poc;
  _poc=0;
}

Bi_pop* Bi_pop::copie()
{
  Bi_pop* p=new Bi_pop();
  if (_pop)
    p->_pop= _pop->copie();
  if (_poc)
    p->_poc= _poc->copie();
  return p;
}

////////////////////////
// Op_ou_tom
////////////////////////

Op_ou_tom::Op_ou_tom() 
{
  _op=OU_TOM;
  _voo=new Var_Op_ou(lbi);
}

Op_ou_tom::Op_ou_tom(const Op_ou_tom& o) 
{
  _voo=new Var_Op_ou(lbi);
  
  _op=OU_TOM;
  o._voo->it.init();
  while ((o._voo->pbi=o._voo->it()))
  {
    _voo->pbi=o._voo->pbi->copie();
    lbi.aj_fin(*(_voo->pbi));
  }
}

Op_ou_tom::~Op_ou_tom()
{
  delete _voo;
  lbi.nettoie();
}

double Op_ou_tom::multiplie(const Cdescripteur* const poi,
			    const Cdescripteur* const pod,
			    const Cdescripteur* const pof )  const
{
  return ((_voo->pop=choisit(poi,pod,pof))?_voo->pop->multiplie(poi,pod,pof):0);
}

const Op_desc* const Op_ou_tom::choisit(const Cdescripteur* const poi,
					const Cdescripteur* const pod,
					const Cdescripteur* const pof )  const
{
  _voo->it.init();
  if (pod>poi){
    while ((_voo->pbi=_voo->it()))
      if ((_voo->pbi->_poc->multiplie(poi,pod-1,pof))>0)
	return _voo->pbi->_pop->choisit(poi,pod-1,pof);
  }
  else {
    while ((_voo->pbi=_voo->it()))
      if (_voo->pbi->_poc->horslim()>0)
	return _voo->pbi->_pop->choisit(poi,pod-1,pof);
  }
  return 0;
}

double Op_ou_tom::multiplie(const mat_iter& md)  const
{
  return ((_voo->pop=choisit(md))?_voo->pop->multiplie(md):0);
}

const Op_desc* const Op_ou_tom::choisit(const mat_iter& md) const
{
  mat_iter la(md);

  _voo->it.init();
  if (la--){
    while ((_voo->pbi=_voo->it()))
      if ((_voo->pbi->_poc->multiplie(la))>0)
	return _voo->pbi->_pop->choisit(la);
  }
  else {
    while ((_voo->pbi=_voo->it()))
      if ((_voo->pbi->_poc->horslim())>0)
	return _voo->pbi->_pop->choisit();
  } 
  return 0;
}
  
double Op_ou_tom::horslim() const
{
  _voo->it.init();
  const Op_desc* pod=0;
  
  while ((_voo->pbi=_voo->it()))
    if (_voo->pbi->_poc->horslim()>0){
      pod=_voo->pbi->_pop->choisit();
      break;
    }

  return (pod)?pod->horslim():0;
}

const Op_desc* const Op_ou_tom::choisit() const 
{
  _voo->it.init();
  while ((_voo->pbi=_voo->it()))
    if ((_voo->pbi->_poc->horslim())>0)
      return _voo->pbi->_pop->choisit();
  return 0;
}
  
int Op_ou_tom::taille()  const
{
  _voo->it.init();
  int a,b;

  b=0;
  while ((_voo->pbi=_voo->it())){
    if ((a=_voo->pbi->_poc->taille()+1)>b)
      b=a;
    if ((a=_voo->pbi->_pop->taille())>b)
      b=a;
  }
  return b;
}

bool Op_ou_tom::est_simple()  const
{
  return 0;
}

bool Op_ou_tom::possede(const Cdescripteur& d)  const
{
  _voo->it.init();

  while ((_voo->pbi=_voo->it()))
  {
    if ((_voo->pbi->_poc->possede(d)) || (_voo->pbi->_pop->possede(d)))
      return 1;
  }
  return 0;
}

bool Op_ou_tom::est_alpha()  const
{
  _voo->it.init();

  while ((_voo->pbi=_voo->it()))
  {
    if ((_voo->pbi->_poc->est_alpha()) || (_voo->pbi->_pop->est_alpha()))
      return 1;
  }
  return 0;
}
  


////////////////////////
// Op_ou
////////////////////////

Op_ou::Op_ou() 
{
  _op=OU;
  _voo=new Var_Op_ou(lbi);
}

Op_ou::Op_ou(const Op_ou& o) 
{
  _voo=new Var_Op_ou(lbi);

  _op=OU;
  o._voo->it.init();
  while ((o._voo->pbi=o._voo->it())){
    _voo->pbi=o._voo->pbi->copie();
    lbi.aj_fin(*(_voo->pbi));
  }
}

Op_ou::~Op_ou()
{
  delete _voo;
  lbi.nettoie();
}

double Op_ou::multiplie(const Cdescripteur* const poi,
                        const Cdescripteur* const pod,
                        const Cdescripteur* const pof )  const
{
  return ((_voo->pop=choisit(poi,pod,pof))?_voo->pop->multiplie(poi,pod,pof):0);
}

const Op_desc* const Op_ou::choisit(const Cdescripteur* const poi,
				    const Cdescripteur* const pod,
				    const Cdescripteur* const pof )  const
{
  _voo->it.init();
  while ((_voo->pbi=_voo->it()))
    if ((_voo->pbi->_poc->multiplie(poi,pod,pof))>0)
      return _voo->pbi->_pop->choisit(poi,pod,pof);
  return 0;
}

double Op_ou::multiplie(const mat_iter& md)  const
{
  return ((_voo->pop=choisit(md))?_voo->pop->multiplie(md):0);
}

const Op_desc* const Op_ou::choisit(const mat_iter& md) const
{
  _voo->it.init();
  while ((_voo->pbi=_voo->it()))
    if ((_voo->pbi->_poc->multiplie(md))>0)
      return _voo->pbi->_pop->choisit(md);
  return 0;
}
  
double Op_ou::horslim() const
{
  _voo->it.init();
  const Op_desc* pod=0;
  
  while ((_voo->pbi=_voo->it()))
    if (_voo->pbi->_poc->horslim()>0){
      pod=_voo->pbi->_pop->choisit();
      break;
    }

  return (pod)?pod->horslim():0;
}

const Op_desc* const Op_ou::choisit() const 
{
  _voo->it.init();
  while ((_voo->pbi=_voo->it()))
    if ((_voo->pbi->_poc->horslim())>0)
      return _voo->pbi->_pop->choisit();
  return 0;
}
  
int Op_ou::taille()  const
{
  _voo->it.init();
  int a,b;

  b=0;
  while ((_voo->pbi=_voo->it())){
    if ((a=_voo->pbi->_poc->taille())>b)
      b=a;
    if ((a=_voo->pbi->_pop->taille())>b)
      b=a;
  }
  return b;
}

bool Op_ou::est_simple()  const
{
  return 0;
}

bool Op_ou::possede(const Cdescripteur& d)  const
{
  _voo->it.init();

  while ((_voo->pbi=_voo->it()))
    if ((_voo->pbi->_poc->possede(d)) || (_voo->pbi->_pop->possede(d)))
      return 1;
  return 0;
}

bool Op_ou::est_alpha()  const
{
  _voo->it.init();

  while ((_voo->pbi=_voo->it())){
    if ((_voo->pbi->_poc->est_alpha()) || (_voo->pbi->_pop->est_alpha()))
      return 1;
  }
  return 0;
}
  
////////////////////////
// Op_ou_mot
////////////////////////

Op_ou_mot::Op_ou_mot() 
{
  _op=OU_MOT;
  _voo=new Var_Op_ou(lbi);
}

Op_ou_mot::Op_ou_mot(const Op_ou_mot& o) 
{
  _voo=new Var_Op_ou(lbi);

  _op=OU_MOT;
  o._voo->it.init();
  while ((o._voo->pbi=o._voo->it())){
    _voo->pbi=o._voo->pbi->copie();
    lbi.aj_fin(*(_voo->pbi));
  }
}

Op_ou_mot::~Op_ou_mot()
{
  delete _voo;
  lbi.nettoie();
}

double Op_ou_mot::multiplie(const Cdescripteur* const poi,
			    const Cdescripteur* const pod,
			    const Cdescripteur* const pof )  const
{
  return ((_voo->pop=choisit(poi,pod,pof))?_voo->pop->multiplie(poi,pod,pof):0);
}

const Op_desc* const Op_ou_mot::choisit(const Cdescripteur* const poi,
					const Cdescripteur* const pod,
					const Cdescripteur* const pof )  const
{
  _voo->it.init();
  if (pod<pof){
    while ((_voo->pbi=_voo->it()))
      if ((_voo->pbi->_poc->multiplie(poi,pod+1,pof))>0)
	return _voo->pbi->_pop->choisit(poi,pod+1,pof);
  }
  else {
    while ((_voo->pbi=_voo->it()))
      if ((_voo->pbi->_poc->horslim())>0)
	return _voo->pbi->_pop->choisit(poi,pod+1,pof);
  }
    
  return 0;
}

double Op_ou_mot::multiplie(const mat_iter& md)  const
{
  return ((_voo->pop=choisit(md))?_voo->pop->multiplie(md):0);
}

const Op_desc* const Op_ou_mot::choisit(const mat_iter& md) const
{
  mat_iter la(md);
  _voo->it.init();
  
  if (la++){
    while ((_voo->pbi=_voo->it()))
      if ((_voo->pbi->_poc->multiplie(la))>0)
	return _voo->pbi->_pop->choisit(la);
  }
  else {
    while ((_voo->pbi=_voo->it()))
      if ((_voo->pbi->_poc->horslim())>0)
	return _voo->pbi->_pop->choisit(la);
  }
  return 0;
}
  
  
double Op_ou_mot::horslim() const
{
  _voo->it.init();
  const Op_desc* pod=0;
  
  while ((_voo->pbi=_voo->it()))
    if (_voo->pbi->_poc->horslim()>0){
      pod=_voo->pbi->_pop->choisit();
      break;
    }

  return (pod)?pod->horslim():0;
}

const Op_desc* const Op_ou_mot::choisit() const 
{
  _voo->it.init();
  while ((_voo->pbi=_voo->it()))
    if ((_voo->pbi->_poc->horslim())>0)
      return _voo->pbi->_pop->choisit();
  return 0;
}
  
int Op_ou_mot::taille()  const
{
  _voo->it.init();
  int a,b;

  b=0;
  while ((_voo->pbi=_voo->it())){
    if ((a=_voo->pbi->_poc->taille()+1)>b)
      b=a;
    if ((a=_voo->pbi->_pop->taille())>b)
      b=a;
  }
  return b;
}

bool Op_ou_mot::est_simple()  const
{
  return 0;
}

bool Op_ou_mot::possede(const Cdescripteur& d)  const
{
  _voo->it.init();

  while ((_voo->pbi=_voo->it()))
  {
    if ((_voo->pbi->_poc->possede(d)) || (_voo->pbi->_pop->possede(d)))
      return 1;
  }
  return 0;
}

bool Op_ou_mot::est_alpha()  const
{
  _voo->it.init();

  while ((_voo->pbi=_voo->it()))
  {
    if ((_voo->pbi->_poc->est_alpha()) || (_voo->pbi->_pop->est_alpha()))
      return 1;
  }
  return 0;
}
  






