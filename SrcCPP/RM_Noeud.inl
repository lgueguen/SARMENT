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

#include <math.h>

template<class R>
RM_Noeud<R>::RM_Noeud() : R(), _no(1), _su(0), _pd(0)
{
}

template<class R>
RM_Noeud<R>::RM_Noeud(Cdesc_comp* const pd) : R(), _no(1),
					    _su(0), _pd(pd)
{
}

template<class R>
RM_Noeud<R>::RM_Noeud(const R& r, const Cdesc_comp* const pd)
  : R(r), _pd(pd), _no(1), _su(0)
{
}

template<class R>
RM_Noeud<R>::RM_Noeud(const RM_Noeud<R>& C) : R(C), _no(1),
					   _pd(C._pd), _val(C._val)
{
  _su=C._su;
  if (_su)
    _su->_no++;
}

template<class R>
RM_Noeud<R>::~RM_Noeud()
{
  net_succ();
}


template<class R>
double RM_Noeud<R>::val() const
{
  return _val;
}

template<class R>
void RM_Noeud<R>::g_val(double x)
{
  _val=x;
}

template<class R>
void RM_Noeud<R>::nettoie_sur(int l)
{
  RM_Noeud<R>** tab;
  tab=new RM_Noeud<R>*[l];
  RM_Noeud<R>* ps;
  int i=0;

  ps=_su;
  while ((i<l) && ps && ps->Noeud_normal()){
    tab[i++]=ps;
    ps=ps->_su;
  }

  while (--i>=1){
    if (tab[i]->_no<=2){
      tab[i-1]->_su=tab[i]->_su;
      delete tab[i];
    }
  }
  delete[] tab;
}

template<class R>
void RM_Noeud<R>::net_succ()
{
  if (_su && (--(_su->_no)==1) && 
      _su->Noeud_normal() ){
    delete _su;
    _su=0;
  }
}

/////////////////////////////////////
// initialisations
///////////////////////////////////////

template<class R>
void RM_Noeud<R>::init(const Cdescripteur* const i ,
		       const Cdescripteur* const d,
		       const Cdescripteur* const f,
		       RM_Noeud<R>& r)
{
  if (_pd)
    g_val(_pd->multiplie(i,d,f));
  dev_su(r);
}

template<class R>
void RM_Noeud<R>::init(const mat_iter& m_i,
		       RM_Noeud<R>& r)
{
  if (_pd)
    g_val(_pd->multiplie(m_i));
  dev_su(r);
}

template<class R>
void RM_Noeud<R>::init(double val, RM_Noeud<R>& r)
{
  g_val(val);
  dev_su(r);
}

///////////////////////////////////////
// modifications
///////////////////////////////////////

template<class R>
void RM_Noeud<R>::avance()
{
  R::avance();
}

template<class R>
void RM_Noeud<R>::recule()
{
  R::recule();
}

template<class R>
void RM_Noeud<R>::ajoute_su(const Cdescripteur* const i ,
			   const Cdescripteur* const d,
			   const Cdescripteur* const f)
{
  if (_pd)
    _val+=(_pd->multiplie(i,d,f)+_pd->auto_trans());
}

template<class R>
void RM_Noeud<R>::ajoute_su(const mat_iter& m_i)
{
  if (_pd)
    _val+=(_pd->multiplie(m_i)+_pd->auto_trans());
}


template<class R>
void RM_Noeud<R>::ajoute_su(const Cdescripteur* const i ,
			  const Cdescripteur* const d,
			  const Cdescripteur* const f,
			  const Tliste<RM_Noeud<R>* >& L)
{
  static Tliste_iter<RM_Noeud<R>* > it;
  static RM_Noeud<R> **ppr, *prm;
  static double m,x;

  it.colle(L);
  
  if (!(ppr=it()))
    return;
  prm=*ppr;
  m=prm->val()+_pd->trans_de(prm->_pd->num());

  while ((ppr=it())){
    if ((x=(*ppr)->val()+_pd->trans_de((*ppr)->_pd->num()))>m){
      m=x;
      prm=*ppr;
    }
  }

  if (prm->_pd->num()==_pd->num())
    recup_su_de(*prm);
  else
    dev_su(*prm);
  g_val(_pd->multiplie(i,d,f)+m);
}

template<class R>
void RM_Noeud<R>::ajoute_su(const mat_iter& m_i,
                            const Tliste<RM_Noeud<R>* >& L)
{
  static Tliste_iter<RM_Noeud<R>* > it;
  static RM_Noeud<R> **ppr, *prm;
  static double m,x;

  it.colle(L);
  
  if (!(ppr=it()))
    return;
  prm=*ppr;
  m=prm->val()+_pd->trans_de(prm->_pd->num());

  while ((ppr=it())){
    if ((x=(*ppr)->val()+_pd->trans_de((*ppr)->_pd->num()))>m){
      m=x;
      prm=*ppr;
    }
  }

  if (prm->_pd->num()==_pd->num())
    recup_su_de(*prm);
  else
    dev_su(*prm);
  g_val(_pd->multiplie(m_i)+m);
}


template<class R>
void RM_Noeud<R>::ajoute_pred(const Cdescripteur* const i ,
			      const Cdescripteur* const d,
			      const Cdescripteur* const f)
{
  if (_pd)
    _val+=(_pd->multiplie(i,d,f));
}

template<class R>
void RM_Noeud<R>::ajoute_pred(const mat_iter& m_i)
{
  if (_pd)
    _val+=(_pd->multiplie(m_i));
}

template<class R>
void RM_Noeud<R>::backward(const Tliste<RM_Noeud<R>* >& L)
{
  static Tliste<double> ld;
  static Tliste_iter<double> itld(ld);
  
  ld.nettoie();
  static double x,m;
  static double *pd;
  
  static Tliste_iter<RM_Noeud<R>* > it;
  static RM_Noeud<R> **ppr;
  static int pdn;
    
  it.colle(L);
  pdn=_pd->num();
  
  if ((ppr=it())){
    m=(*ppr)->val()+(*ppr)->_pd->trans_de(pdn);
    ld.aj_fin(m);
  }
  while ((ppr=it())){
    x=(*ppr)->val()+(*ppr)->_pd->trans_de(pdn);
    ld.aj_fin(x);
    if (m<x) m=x;
  }

  itld.init();
  x=0;
  while ((pd=itld()))
    x+=exp((*pd)-m);
        
  g_val(m+log(x));
}

template<class R>
void RM_Noeud<R>::forward(const Tliste<RM_Noeud<R>* >& L)
{
  static Tliste<double> ld;
  static Tliste_iter<double> itld(ld);
  
  ld.nettoie();
  
  static double x,m;
  static double *pd;
  
  static Tliste_iter<RM_Noeud<R>* > it;
  static RM_Noeud<R> **ppr;

  it.colle(L);
  
  if ((ppr=it())){
    m=(*ppr)->val()+_pd->trans_de((*ppr)->_pd->num());
    ld.aj_fin(m);
  }
  while ((ppr=it())){
    x=(*ppr)->val()+_pd->trans_de((*ppr)->_pd->num());
    ld.aj_fin(x);
    if (m<x)
      m=x;
  }
  
  itld.init();
  x=0;
  while ((pd=itld()))
    x+=exp((*pd)-m);
        
  g_val(m+log(x));
}

/////////////////////
//  donnees
/////////////////////

template<class R>
short RM_Noeud<R>::no() const
{
  return _no;
}

template<class R>
RM_Noeud<R>* RM_Noeud<R>::su() const
{ 
  return _su;
}

template<class R>
int RM_Noeud<R>::lg() const
{
  RM_Noeud<R>* ps=_su;
  int l=1;

  while (ps && ps->Noeud_normal()){
    ps=ps->_su;
    l++;
  }

  return l;
}

///////////////////
// modifications
///////////////////

template<class R>
void RM_Noeud<R>::dev_su(RM_Noeud<R>& C)
{
  net_succ();
  _su=&C;
  C._no++;
}

template<class R>
void RM_Noeud<R>::recup_su_de(RM_Noeud<R>& C)
{
  net_succ();
  _su=C._su;
  if (_su)
    _su->_no++;
}

template<class R>
void RM_Noeud<R>::operator=(RM_Noeud<R>& C)
{
  R::operator=(C);
  recup_su_de(C);
}

template<class R>
const Cdesc_comp* RM_Noeud<R>::pd() const
{
  return _pd;
}
