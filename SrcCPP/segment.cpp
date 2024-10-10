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

/****************************************************/
/******** CSEGMENT **********************************/
/****************************************************/

#include "segment.h"

#include "posi_motif.h"

#include "sequence.h"
#include "matrice.h"
#include "mat_iter.h"

#include "R_lex.h"
#include "S_lex.h"

#include <stdio.h>
#include <math.h>


/*************************************************/
/************** modifications ********************/
/*************************************************/

bool Csegment::recup_mot(const string& st)
{
  int i=0;
  int l=st.find_first_of(" \t\n");
  
  Cdesc_comp *pm;
  
  _mot.nettoie();

  _mot.aj_fin();
  pm=_mot.ind_fin();
  
  if ((i=pm->construit_de(st,i,0))==-1){
    _mot.nettoie();
    return 0;
  }
  
  while (i<l){
    _mot.aj_fin();
    pm=_mot.ind_fin();
    if ((i=pm->construit_de(st,i,0))==-1){
    _mot.nettoie();
    return 0;
    }
  }
  ajuste();
  return 1;
}


void Csegment::mot_en(Tliste<Cdesc_comp>& mt, int d) const
{
  mt.nettoie();

  Tliste_iter<Cdesc_comp> it1, it2(_dsc_deb);

  int l;
  if ((l=lg_mot())==0)
    return;
  int i=(pos_d()-d)%l;
  if (i>=0)
    for (;i>0;--i)
      it2++;
  else
    for (i+=l;i>0;--i)
      it2++;
  it1=it2;

  Cdesc_comp *c;
  while ((c=it1(it2)))
    mt.aj_fin(*c);
}

////////////////////////////
//  extraction par diff_cnx selon un lexique

// double Csegment::seg_diff_cnx(const Csegment& Se, const Csequence& G, 
// 			      const Clexique& L)
// {
//   vide();
//   if (Se.est_vide())
//     return 0;
//   if (L.nb_desc()==0)
//     return 0;

//   const Cdescripteur *dds, *ds, *fs, *f;
//   dds=G.deb();
//   ds=&G[Se.pos_d()-1];
//   fs=&G[Se.pos_f()-1];
//   f=G.fin();

//   R_lex<Cposi_motif > Lc(L);
//   Tliste_iter<R_Noeud<Cposi_motif> > it_d(Lc._demlex);
//   Tliste_iter<Rmotif_Noeud_val<Cposi_motif> > it_m(Lc._vocces);
//   R_Noeud<Cposi_motif>* pd;
//   Rmotif_Noeud_val<Cposi_motif>* pm;

//   const Cmotif* RPM=0;
//   double max=0;

//   double x;
//   Tliste_iter<Cdesc_comp> cour=Se._dsc_deb;

//   x=(~cour)?(~cour)->multiplie(dds,ds,f):0;

//   int p=Se.pos_d();
  
//   //////////// recherche du sous-segment

//   Lc.init(dds,ds, f);
//   Lc+= -x;

//   while (pm=it_m()){
//     pd=it_d();
//     if (pm->val()<=0){
//       pd->g_pos(p+1);
//       pm->g_val(0);
//     }
//     else 
//       pd->g_pos(p);    
//     if (pm->val()>max){
//       max=pm->val();
//       RPM=pd->pt();
//       g_pos_d(pd->pos());
//       g_pos_f(p);
//     }
//   }

//   while (ds++!=fs){
//     p++;
//     Lc.plus_egale(dds,ds,f);
//     x=(++cour)?(~cour)->multiplie(dds,ds,f):0;
//     Lc+= -x;
//     it_m.init();
//     it_d.init();
//     while (pm=it_m()){
//       pd=it_d();
//       if (pm->val()<=0){
// 	pd->g_pos(p+1);
// 	pm->g_val(0);
//       }
//       if (pm->val()>max){
// 	max=pm->val();
// 	RPM=pd->pt();
// 	g_pos_d(pd->pos());
// 	g_pos_f(p);
//       }
//     }
//   }

//   //// recuperation

//   if (max==0){
//     operator=(Se);
//     ajuste();
//     return 0;
//   }
//   else{
//     recup_mot(*RPM);
//     _dsc_deb.colle(_mot);
//     p=(pos_d()-Se.pos_d()) % lg_mot();
//     for (;p>0;p--)
//       _dsc_deb++;
//     ajuste();
//     g_val(calc_val(G));
//     return max;
//   }
// }

// double Csegment::seg_diff_cnx(const Csegment& Se,
// 			      const Cmatrice& M, 
// 			      const Clexique& L)
// {
//   vide();
//   if (Se.est_vide())
//     return 0;
//   if (L.nb_desc()==0)
//     return 0;

//   mat_iter mi(M), mi2(M);
//   mi.init(Se.pos_d()-1);
//   mi2.init(((Se.pos_f()>M.taille())?M.taille():Se.pos_f())-1);

//   R_lex<Cposi_motif > Lc(L);
//   Tliste_iter<R_Noeud<Cposi_motif> > it_d(Lc._demlex);
//   Tliste_iter<Rmotif_Noeud_val<Cposi_motif> > it_m(Lc._vocces);
//   R_Noeud<Cposi_motif>* pd;
//   Rmotif_Noeud_val<Cposi_motif>* pm;

//   const Cmotif* RPM=0;
//   double max=0;

//   double x;
//   Tliste_iter<Cdesc_comp> cour=Se._dsc_deb;

//   x=(~cour)?(~cour)->multiplie(mi):0;

//   int p=Se.pos_d();
  
//   //////////// recherche du sous-segment

//   Lc.init(mi);
//   Lc+= -x;

//   while (pm=it_m()){
//     pd=it_d();
//     if (pm->val()<=0){
//       pd->g_pos(p+1);
//       pm->g_val(0);
//     }
//     else 
//       pd->g_pos(p);    
//     if (pm->val()>max){
//       max=pm->val();
//       RPM=pd->pt();
//       g_pos_d(pd->pos());
//       g_pos_f(p);
//     }
//   }

//   while (mi(mi2)){
//     p++;
//     Lc.plus_egale(mi);
//     x=(++cour)?(~cour)->multiplie(mi):0;
//     Lc+= -x;
//     it_m.init();
//     it_d.init();
//     while (pm=it_m()){
//       pd=it_d();
//       if (pm->val()<=0){
// 	pd->g_pos(p+1);
// 	pm->g_val(0);
//       }
//       if (pm->val()>max){
// 	max=pm->val();
// 	RPM=pd->pt();
// 	g_pos_d(pd->pos());
// 	g_pos_f(p);
//       }
//     }
//   }

//   //// recuperation

//   if (max==0){
//     operator=(Se);
//     ajuste();
//     return 0;
//   }
//   else{
//     recup_mot(*RPM);
//     _dsc_deb.colle(_mot);
//     p=(pos_d()-Se.pos_d()) % lg_mot();
//     for (;p>0;p--)
//       _dsc_deb++;
//     ajuste();
//     g_val(calc_val(M));
//     return max;
//   }
// }

/*************************************************/
/************** comparaisons entre segments     **/
/*************************************************/

int Csegment::modulo(const Csegment& m) const
{
  int l=lg_mot();
  int lm=m.lg_mot();

  if ((l!=0) && (lm == l)){
    int i=-1;
    Tliste_iter<Cdesc_comp> iter1(_dsc_deb);
    Tliste_iter<Cdesc_comp> iter2(m._dsc_deb), iterd(m._dsc_deb);
    Cdesc_comp *pd;
    while (++i<l){
      while ((pd=iter2(iterd)))
	if (*pd!=*(++iter1))
	  break;
      if (!pd)
	return i;
      ++iterd;
      iter2=iterd;
      iter1=_dsc_deb;
    }
    return i;
  }
  return((l>lm)?l:lm);
}

bool Csegment::congru_a(const Csegment& p) const
{
  int l=lg_mot();
  int i=modulo(p);

  return ((l==0)?1:((i>=l)?0:(((pos_d()-p.pos_d()-i) % l)==0)));
}

bool Csegment::congru_a(const Tliste<Cdesc_comp>& mt, int d) const
{
  if (!a_un_mot())
    return 0;

  Tliste_iter<Cdesc_comp> it1, it2(_dsc_deb), itm(mt);

  int l;
  if ((l=lg_mot())==0)
    return 0;
  int i=(pos_d()-d)%l;
  if (i>=0)
    for (;i>0;--i)
      it2++;
  else
    for (i+=l;i>0;--i)
      it2++;
  it1=it2;

  Cdesc_comp *c, *cm;
  while ((c=it1(it2))){
    if (!(cm=itm()))
      return 0;
    if (*cm!=*c)
      return 0;
  }
  if (itm())
    return 0;
  return 1;
}

/*************************************************/
/************ recuperations **********************/
/*************************************************/

void Csegment::recup_max(const R_lex<Cposition>& Lc, 
			 Cposition& posdeb, Cposition& posfin)
{
  Tliste_iter<Rmotif_Noeud_val<Cposition> > iter(Lc._vocces);
  Rmotif_Noeud_val<Cposition> *x, *cm;
  double m1, m2;

  _pos_d=posdeb.pos()+1;
  _pos_f=posfin;

  if ((x=iter())){
    cm=x;
    m2=x->val();
  }
  while ((x=iter())){
    m1=x->val();
    if (m2<m1){
      cm=x;
      m2=m1;
    }
  }
  g_val(m2);
  recup_mot(cm->pt()->justif_g(pos_f()-pos_d()));
}

// void Csegment::recup_max(const R_lex<Cposi_motif>& Lc)
// {
//   Tliste_iter<Rmotif_Noeud_val<Cposi_motif> > iter(Lc._vocces);
//   Rmotif_Noeud_val<Cposi_motif> *x, *cm;
//   double m1, m2;

//   if (x=iter()){
//     cm=x;
//     m2=x->val();
//   }
//   while(x=iter()){
//     m1=x->val();
//     if (m2<m1){
//       cm=x;
//       m2=m1;
//     }
//   }
//   g_val(m2);
//   recup_mot(cm->pt()->justif_g(pos_f()-pos_d()));
// }

// void Csegment::recup_max(const Csujet* const pS, const Clexique& L, int d, int f)
// {
//   if (!pS)
//     return;
  
//   int l=pS->taille();
//   Cposition i;
  
//   _pos_d=(d<=0?1:(d>=l?l:d+1));
//   _pos_f=(f<=0?1:(f>=l?l:f+1));
//   if (_pos_d>_pos_f){
//     i=_pos_d;
//     _pos_d=_pos_f;
//     _pos_f=i;
//   }

//   R_lex<Cposition > Lc(L);

//   Csequence* G=0;
//   Cmatrice* M=0;

//   const Cobjet *ods, *od, *of, *ofs;
//   const Cdescripteur *dds, *dd, *df, *dfs;
//   mat_iter m_i, m_i2;

//   switch(pS->ctg()){
//   case Csujet::GEN:

//     G=(Csequence*)pS;
//     dd=&(*G)[pos_d()-1];
//     df=&(*G)[pos_f()-1];
//     dds=G->deb();
//     dfs=G->fin();
    
//     Lc.init(dds, dd, dfs);
//     while (dd++<df)
//       Lc.plus_egale(dds, dd, dfs);
//     break;

//   case Csujet::SEQ:
//     S=(Sequence*)pS;
//     od=&(*S)[pos_d()-1];
//     of=&(*S)[pos_f()-1];
//     ods=S->deb();
//     ofs=S->fin();
    
//     Lc.init(ods, od, ofs);
//     while (od++<of)
//       Lc.plus_egale(ods, od, ofs);
//     break;

//   case Csujet::MAT:
//     M=(matrice*)pS;
//     m_i.colle(*M);
//     m_i2.colle(*M);
//     m_i.init(pos_d()-1);
//     m_i2.init(pos_f()-1);

//     Lc.init(m_i);
//     if (m_i(m_i2))
//       do {
// 	Lc.plus_egale(m_i);
//       } while (m_i(m_i2));
//     break;
//   default:
//     break;
//   }

//   _pos_d--;
//   recup_max(Lc,_pos_d,_pos_f);
// }
  
/*************************************************/
/*************** calculs *************************/
/*************************************************/

double Csegment::calc_val(const Fsequence& G) const
{
  int lm=lg_mot();
  if (est_vide())
    return 0;
  if (pos_d()>G.taille())
    return 0;

  if (lm==0)               //si pas de descripteur  
    return eval_cl(G);    //appel d'une fonction d'évaluation
  else{
    const Cdescripteur *dds, *d, *f;
    const Cdescripteur *Gf=G.fin();

    dds=G.deb();
    d=&G[pos_d()-1];
    f=&G[pos_f()-1];
    if (Gf<f)
      f=Gf;

    double b;
    Tliste_iter<Cdesc_comp> cour=_dsc_deb;
    Cdesc_comp* dsc;

    //avant

    b=0;
    while (d<=f){
      dsc=++cour;
      b+=dsc->multiplie(dds,d,Gf);
      d++;
    }
    return b;
  }  
}

double Csegment::calc_val(const Fmatrice& M) const
{
  if (est_vide())
    return 0;
  if (pos_d()>M.taille())
    return 0;

  if (lg_mot()==0)        //si pas de descripteur  
    return eval_cl(M);    //appel d'une fonction d'évaluation
  else{
    mat_iter mi(M), mi2(M);

    mi.init(pos_d()-1);
    mi2.init(((pos_f()>M.taille())?M.taille():pos_f())-1);

    double b;
    Tliste_iter<Cdesc_comp> cour=_dsc_deb;
    Cdesc_comp* dsc;

    //avant

    b=0;

    do {
      dsc=++cour;
      b+=dsc->multiplie(mi);
    } while (mi(mi2));

    return b;
  }  
}

double Csegment::ecarttype(const Fsequence& G) const
{
  if (est_vide() || (lg_mot()==0))
    return 0;
  if (pos_d()>G.taille())
    return 0;

  const Cdescripteur *dds, *d, *f;
  const Cdescripteur *Gf=G.fin();
  int lg=pos_f()-pos_d()+1;
  double x,y;
  x=calc_val(G)/lg;

  dds=G.deb();
  d=&G[pos_d()-1];
  f=&G[pos_f()-1];
  if (Gf<f)
    f=Gf;

  double b;
  Tliste_iter<Cdesc_comp> cour=_dsc_deb;
  Cdesc_comp* dsc;

  b=0;
  while (d<=f){
    dsc=++cour;
    y=dsc->multiplie(dds,d,Gf);
    b+=(y-x)*(y-x);
    d++;
  }
  return sqrt(b/lg);
}  


double Csegment::ecarttype(const Fmatrice& M) const
{
  if (est_vide() || (lg_mot()==0))
    return 0;
  if (pos_d()>M.taille())
    return 0;

  mat_iter mi(M), mi2(M);

  mi.init(pos_d()-1);
  mi2.init(((pos_f()>M.taille())?M.taille():pos_f())-1);
  int lg=pos_f()-pos_d()+1;
  double x,y;
  x=calc_val(M)/lg;

  double b;
  Tliste_iter<Cdesc_comp> cour=_dsc_deb;
  Cdesc_comp* dsc;

  b=0;
  do {
    dsc=++cour;
    y=dsc->multiplie(mi);
    b+=(y-x)*(y-x);
  } while (mi(mi2));

  return sqrt(b/lg);
}  


double Csegment::between(const Fsequence& S) const
{
  if (est_vide() || !a_un_mot())
    return 0;
  int i, j;
  const Cdescripteur *dds,*d_av, *f_av, *d_ap, *f_ap;

  dds=S.deb();
  d_av=&S[0];
  f_av=&S[pos_d()-2];
  d_ap=&S[pos_f()];
  f_ap=S.fin();

  const Cdescripteur* po=d_av;
  double b;
  int lm=lg_mot();
  Tliste_iter<Cdesc_comp> cour=_dsc_deb;
  Cdesc_comp* dsc;

  //avant

  i=(pos_d()-1)%lm;
  j=0;
  while (j++<lm-i)
    cour++;
  po=d_av;
  po--;
  b=0;
  while (po++<f_av){
    dsc=++cour;
    b+=dsc->multiplie(dds,po,f_ap);
  }

  //apres

  po=d_ap;
  po--;
  cour=_dsc_fin;
  cour++;
  while (po++<f_ap){
    dsc=++cour;
    b+=dsc->multiplie(dds,po,f_ap);
  }

  return b;
}

double Csegment::between(const Fmatrice& M) const
{
  if (est_vide() || !a_un_mot())
    return 0;
  int j;
  mat_iter mi(M);
  mat_iter mif(M);

  mif.init(pos_d()-2);

  double b=0;
  int lm=lg_mot();
  Tliste_iter<Cdesc_comp> cour=_dsc_deb;
  Cdesc_comp* dsc;

  //avant

  if (pos_d()>1){
    lm-=(pos_d()-1)%lm;
    j=0;
    while (j++<lm)
      cour++;
    do {
      dsc=++cour;
      b+=dsc->multiplie(mi);
    } while (mi(mif));
  }

  //apres

  if (pos_f()<M.taille()){
    mi.init(pos_f());
    mif.init(M.taille()-1);
    
    cour=_dsc_fin;
    cour++;
    do {
      dsc=++cour;
      b+=dsc->multiplie(mi);
    } while (mi(mif));
  }
  return b;
}

double Csegment::inter_apres(const Fsequence& S, 
			     const Csegment* v) const
{
  if (!v || est_vide() || v->est_vide() || !a_un_mot() || 
      !v->a_un_mot())
    return 0;
  int i, j;
  const Cdescripteur *dds, *d_av, *f_av, *d_ap, *f_ap;

  dds=S.deb();
  d_av=&S[pos_d()-1];
  f_av=&S[pos_f()-1];
  d_ap=&S[v->pos_d()-1];
  f_ap=&S[v->pos_f()-1];
  const Cdescripteur* Sf=S.fin();

  const Cdescripteur* po=d_av;
  double a, b;
  int lm=v->lg_mot();
  Tliste_iter<Cdesc_comp> cour=v->_dsc_deb;
  Cdesc_comp* dsc;

  //avant

  i=(pos_f()-pos_d()+1)%lm;
  j=0;
  while (j++<lm-i)
    cour++;
  po=d_av;
  po--;
  a=0;
  while (po++<f_av){
    dsc=++cour;
    a+=dsc->multiplie(dds,po,Sf);
  }
	
  //apres

  po=d_ap;
  po--;
  cour=_dsc_fin;
  cour++;
  i=0;
  b=0;
  while (po++<f_ap){
    dsc=++cour;
    b+=dsc->multiplie(dds,po,Sf);
  }

  return a+b;
}

double Csegment::inter_apres(const Fmatrice& M, 
			     const Csegment* v) const
{
  if (!v || est_vide() || v->est_vide() || !a_un_mot() || 
      !v->a_un_mot())
    return 0;
  int i, j;
  mat_iter mi(M);
  mat_iter mif(M);

  mi.init(pos_d()-1);
  mif.init(pos_f()-1);

  double a, b;
  int lm=v->lg_mot();
  Tliste_iter<Cdesc_comp> cour=v->_dsc_deb;
  Cdesc_comp* dsc;

  //avant

  i=(pos_f()-pos_d()+1)%lm;
  j=0;
  while (j++<lm-i)
    cour++;
  a=0;

  do {
    dsc=++cour;
    a+=dsc->multiplie(mi);
  } while (mi(mif));

  //apres

  mi.init(v->pos_d()-1);
  mif.init(v->pos_f()-1);
  cour=_dsc_fin;
  cour++;
  i=0;
  b=0;
  do {
    dsc=++cour;
    b+=dsc->multiplie(mi);
  }  while (mi(mif));

  return a+b;
}

/////////////////////////////////////////////
///////////////////////////////
//fonction d'evaluation ne faisant pas appel a un descripteur

double Csegment::eval_cl(const Fsequence& G) const 
{
  if (pos_d()>G.taille())
    return 0;

  double val[4]={0, 0, 0, 0};
  Cdescripteur* x;
  for (x=&G[pos_d()-1];x<&G[pos_f()];x++){
    switch (x->dsc()){
    case 'a':
    case 'A':
      val[0]++;
      break;
    case 't':
    case 'T':
      val[1]++;
      break;
    case 'g':
    case 'G':
      val[2]++;
      break;
    case 'c':
    case 'C':
      val[3]++;
      break;
    default:
      break;
    }
  }

  if (val[0]<val[1])
    val[0]=val[1];
  if (val[0]<val[2])
    val[0]=val[2];
  if (val[0]<val[3])
    val[0]=val[3];

  return (val[0]/(pos_f()-pos_d()+1));
}

double Csegment::eval_cl(const Fmatrice& M) const //fonction
{                                     //d'evaluation par l'oppose
                                      //de la variance
  int n,i,j,p;
  double res = 0;
  double xs = 0;
  double tmp =0;

  n = pos_f()-pos_d()+1;
  p = M.nval();

  if (p==0 || n==0)
    return 0;

  for (i=0;i<p;i++){
    xs=0;
    tmp=0;
    for (j=(pos_d()-1);j<pos_f();j++)
      xs += M(i,j);
    xs/=n;
    for (j=(pos_d()-1);j<pos_f();j++)
      tmp += (M(i,j) - xs) * (M(i,j) - xs);
    res += tmp;
  }
  return -res/n;
}

int Csegment::nbe_mm_pred(const Csegment* v) const
{
  if (!v || est_vide() || v->est_vide() || !a_un_mot() || 
      !v->a_un_mot())
    return 0;
  int l;
  l=((pos_f()<v->pos_f())?pos_f():v->pos_f())-
    (pos_d()>v->pos_d()?pos_d():v->pos_d());

  if (l<=0) return 0;

  int n;
  Tliste_iter<Cdesc_comp> iter(_mot), iterv(v->_mot);
  int i;
	
  iter=_dsc_deb;
  iterv=v->_dsc_deb;

  i=v->pos_d()-pos_d();
  if (i>0){
    i%=lg_mot();
    for (;i>0;i--)
      iter++;
  }
  else{
    i=-i % v->lg_mot();
    for (;i>0;i--)
      iterv++;
  }

  n=0;
  for (i=0;i<l;i++)
    n+=(*iter++==*iterv++);

  return n;
}
	

/*************************************************/
/************* ecriture ************************/
/*************************************************/

ostream& Csegment::operator<<(ostream& s) const
{
  s << "<";
  char  c[40];
  snprintf(c,40,"%d",_pos_d.pos()-1);
  s << c;
  s << "-";
  snprintf(c,40,"%d",_pos_f.pos()-1);
  s << c;
  s << ">";
  ecrit_num(s);
  s << ":";
  snprintf(c,40,"%.12g",_val);
  s << c;
  s << ":";
  convertit(s);
  return s;
}

ostream& Csegment::convertit(ostream& s) const
{
  if (!est_vide() && a_un_mot()){
    Tliste_iter<Cdesc_comp> iter(_dsc_deb);
    Cdesc_comp *d;

    while ((d=iter(_dsc_deb)))
      d->operator<<(s);
  }
  return s;
}

ostream& Csegment::ecrit_num(ostream& s) const
{
  if (!est_vide() && a_un_mot()){
    Tliste_iter<Cdesc_comp> iter(_dsc_deb);
    Cdesc_comp *d;
    char tmp[9];
    
    if ((d=iter(_dsc_deb))){
      snprintf(tmp,9,"%d",d->num());
      s << tmp;
    }
    
    while ((d=iter(_dsc_deb))){
      s << ',';
      snprintf(tmp,9,"%d",d->num());
      s << tmp;
    }
  }
  return s;
}  

