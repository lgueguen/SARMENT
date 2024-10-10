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

#include "partition.h"

#include "part_equ_iter.h"
#include "part_equ.h"
#include "segment.h"
#include "lexique.h"
#include "sequence.h"
#include "time.h"

#include "Rmotif_Noeud_val.h"
#include "Tliste_iter.h"
#include "Pt_val.h"
#include "RM_lex.h"

#include <string>
#include <stdio.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Cpartition::Cpartition(const Cpartition& S) : _lg(S._lg) , _val(S._val)
{
  if (_lg){
    int i;
    _seq = new Csegment[_lg];
    for (i=0;i<_lg;i++)
      _seq[i]=S[i];
  }
  else
    _seq=0;
}

Cpartition::Cpartition(const Part_equ_iter& Si) : _lg(Si.lg())
{
  int i;
  _seq = new Csegment[_lg];
  double x=0;

  for (i=0;i<_lg;i++){
    if (Si[i]){
      x+=Si[i]->val();
      _seq[i]=*Si[i];
    }
    else
      _seq[i].vide();
  }
  _val=x;
}

//////////////////////
///  modifications 

void Cpartition::operator=(const Cpartition& S) 
{
  _lg=S._lg;
  _val=S._val;

  if (_lg){
    int i;
    _seq = new Csegment[_lg];
    for (i=0;i<_lg;i++)
      _seq[i]=S[i];
  }
  else
    _seq=0;
}

void Cpartition::viterbi(const Fsequence& G, const Clexique& L, int maxseg)
{
  nettoie();
  if (G.deb()==0)
    return;
  
  RM_lex<Cposition> RL(L);

  const Cdescripteur *i, *d, *f;
  d=i=G.deb();
  f=G.fin();
  
  RL.init(i,d,f);
  RL.avance();

  
  while (d++<f){
    RL.cumule(i,d,f);
    RL.avance();
  }

  Csegment* ps;
  RM_Noeud<Cposition> *pr, *prs;

  pr=RL.Noeud_max();

  int lg=pr->lg();

  
  if (maxseg && (lg>maxseg)){
    cerr << "Too many segments (" << lg << ">" << maxseg << ")" << endl;
    cerr << "I do not handle it" << endl;
    nettoie();
    return;
  }
  
  if ((_lg=lg))
    _seq=new Csegment[_lg];

  _val=pr->val();
  do {
    lg--;
    prs=pr->su();
    ps=&_seq[lg];
    ps->_mot.aj_deb();
    (ps->_mot.ind_deb())->g_num(pr->pd()->num());
    ps->g_pos_f(pr->pos());
    ps->g_pos_d(prs->pos()+1);
    ps->ajuste();
    pr=prs;
  } while ((lg>0) && (prs->Noeud_normal()));
}

void Cpartition::viterbi(const Fmatrice& G,
                         const Clexique& L, int maxseg)
{
  nettoie();
  if (G.taille()==0)
    return;
  
  RM_lex<Cposition> RL(L);

  mat_iter m_i(G);
  
  RL.init(m_i);
  RL.avance();

  
  while (m_i++){
    RL.cumule(m_i);
    RL.avance();
  }

  Csegment* ps;
  RM_Noeud<Cposition> *pr, *prs;

  pr=RL.Noeud_max();

  int lg=pr->lg();

  
  if (maxseg && (lg>maxseg)){
    cerr << "Too many segments (" << lg << ">" << maxseg << ")" << endl;
    cerr << "I do not handle it" << endl;
    nettoie();
    return;
  }
  
  if ((_lg=lg))
    _seq=new Csegment[_lg];

  _val=pr->val();
  do {
    lg--;
    prs=pr->su();
    ps=&_seq[lg];
    ps->_mot.aj_deb();
    (ps->_mot.ind_deb())->g_num(pr->pd()->num());
    ps->g_pos_f(pr->pos());
    ps->g_pos_d(prs->pos()+1);
    ps->ajuste();
    pr=prs;
  } while ((lg>0) && (prs->Noeud_normal()));
}

////////////////////////////////
//  recuperations 

bool Cpartition::recup(const string& ns)
{
  if (ns.empty())
    return 0;

  ifstream flot(ns.c_str());
  if (!flot){
    cout << "pb a l'ouverture de " << ns << endl;
    return 0;
  }
  bool t=recup(flot);

  flot.close();
  return t;
}

bool Cpartition::recup(ifstream& flot)
{
  int i,j;
  double x;
  char c;
  string buff;
  Tliste<Csegment> prov;
  Tliste_iter<Csegment> it;
  Csegment* sg;
  int lg;

  while (!flot.eof()){
    flot >> c;
    if (c=='M'){
      flot.unget();
      return 0;
    } 
    if ((c!='<') && (c!='X')){
      while (!flot.eof() && (c!='\n'))
 	flot.get(c);
    } 
    else
      break;
  }
  
  flot.unget();
  while (!flot.eof()){
    flot >> c;
    while ((!flot.eof()) && (c!='<')){
      if (c=='X'){
	prov.aj_fin();
	prov.ind_fin()->vide();
	while (c=='X')
	  flot >> c;
      }
      else
	flot >> c;
      if (c=='>')
	break;
    }
    if (flot.eof())
      return 0;
    prov.aj_fin();
    sg=prov.ind_fin();
    flot >> j;
    if (j<0){
      cout << "mauvais type de fichier" << endl;
      return 0;
    }
    sg->g_pos_d(j+1);
    flot >> c;
    if (c=='-'){
      flot >> j;
      if (j<0){
	cout << "mauvais type de fichier" << endl;
	return 0;
      }
      flot >> c;
    }
    sg->g_pos_f(j+1);
    if (c!='>'){
      cout << "Probleme dans le fichier" << endl;
      return 0;
    }
    flot >> x;
    sg->g_val(x);
    j=0;
    if (flot.peek()==':')
      flot.ignore(1);
    getline(flot,buff,' ');
    if (!(sg->recup_mot(buff))){
      cout << "Probleme pour le predicteur " << buff;
      cout << " en <" << sg->pos_d()-1 << "-" << sg->pos_f()-1;
      cout << ">" << endl;
      nettoie();
      return 0;
    }
    
    flot >> c;
    while (!flot.eof() && (c!='X') && (c!='>'))
      flot>> c;
    if (c=='>')
      break;
    flot>> c;
    flot>> c;
  }
  flot >> x;
  g_val(x);
  if ((lg=prov.nb_ind())==0){
    if (_lg)
      nettoie();
    return 0;
  }

  if (lg!=_lg){
    if (_lg)
      nettoie();
    _lg=lg;
    _seq=new Csegment[_lg];
  }
  it.colle(prov);
  for (i=0;i<lg;i++)
    _seq[i]=*(++it);
  return 1;
}

void Cpartition::recup(const Part_equ_iter& Si) 
{
  int i;
  double x=0;
  if (_lg!=Si.lg()){
    if (_seq)
      nettoie();
    _lg=Si.lg();
    _seq = new Csegment[_lg];
  }

  for (i=0;i<_lg;i++){
    if (Si[i])
      {
	x+=Si[i]->val();
	_seq[i]=*Si[i];
      }
    else
      _seq[i].vide();
  }
  _val=x;
}

void Cpartition::recup(const Part_equ& S_eq)
{
  if (_lg!=S_eq.taille()){
    if (_seq)
      nettoie();
    _lg=S_eq.taille();
    _seq = new Csegment[_lg];
  }
  
  Part_equ_iter iter(S_eq);
  int i;
  iter.init();
  for (i=_lg-1;i>=0;i--)
    iter.g(i);
  recup(iter);
}

// double Cpartition::select_min(const Part_equ& S_eq, 
// 			     Fonction_sg& ptf)
// {
//   if (_lg>0)
//     nettoie();
//   _lg=S_eq.taille();
//   _val=S_eq.val();
//   _seq = new Csegment[_lg];

//   Part_equ_iter i_eq(S_eq);
//   Csegment *s_g, *s_d;
//   double v_ptf, m_ptf, x_ptf;
//   i_eq.init();
//   int i;

//   Tliste<R_Noeud<Pt_val<Csegment> >* >* col;
//   col= new Tliste<R_Noeud<Pt_val<Csegment> >* >[_lg];
//   Tliste_iter<R_Noeud<Pt_val<Csegment> >* > i_col;
//   R_Noeud<Pt_val<Csegment> > *rps, *m_rps, **p_rps;
//   Pt_val<Csegment> pvs;

//   // on parcourt le graphe des segments de droite a gauche
// 				// en propageant les minimaux
//   while (s_d=i_eq(_lg-1)){
//     ptf.ajuste(s_d);
//     v_ptf=ptf.calcul();
//     pvs.g_val(v_ptf);
//     pvs.g_pt(s_d);
//     col[_lg-1].aj_fin(new R_Noeud<Pt_val<Csegment> >(pvs));
//   }

//   for (i=_lg-2;i>=0;i--){
//     i_col.colle(col[i+1]);
//     while (s_g=i_eq(i)){
//       i_eq.init(i+1);
//       i_col.init();
//       if (!s_g->est_vide()){
// 	s_d=i_eq.d(i+1);
// 	while (p_rps=i_col())
// 	  if ((*p_rps)->pt()==s_d)
// 	    break;
// 	ptf.ajuste(s_g);
// 	x_ptf=ptf.calcul();
// 	m_ptf=x_ptf+((p_rps)?(*p_rps)->val():0);
// 	m_rps=(p_rps)?*p_rps:0;
// 	while (s_d=i_eq.d(i+1)){
// 	  while (p_rps=i_col())
// 	    if ((*p_rps)->pt()==s_d)
// 	      break;
// 	  v_ptf=x_ptf+((p_rps)?(*p_rps)->val():0);
// 	  m_rps=(m_ptf<=v_ptf)?m_rps:(p_rps)?*p_rps:0;
// 	  m_ptf=(m_ptf<=v_ptf)?m_ptf:v_ptf;
// 	}
// 	pvs.g_val(m_ptf);
// 	pvs.g_pt(s_g);
// 	col[i].aj_fin(new R_Noeud<Pt_val<Csegment> >(pvs));
// 	(*col[i].ind_fin())->dev_su(*m_rps);
//       }
//       else{ // normalement seulement quand rien a gauche de s_g
// 	s_d=i_eq(i+1);
// 	while (p_rps=i_col())
// 	  if ((*p_rps)->pt()==s_d)
// 	    break;
// 	m_ptf=(p_rps)?(*p_rps)->val():0;
// 	m_rps=(p_rps)?*p_rps:0;
// 	while (s_d=i_eq(i+1)){
// 	  while (p_rps=i_col())
// 	    if ((*p_rps)->pt()==s_d)
// 	      break;
// 	  v_ptf=(p_rps)?(*p_rps)->val():0;
// 	  m_rps=(m_ptf<=v_ptf)?m_rps:(p_rps)?*p_rps:0;
// 	  m_ptf=(m_ptf<=v_ptf)?m_ptf:v_ptf;
// 	}
// 	col[i].aj_deb(new R_Noeud<Pt_val<Csegment> >(*m_rps));
//       }
//     }
//   }

//   // on recupere le minimum

//   i_col.colle(col[0]);
//   if (p_rps=i_col()){
//     m_ptf=(*p_rps)->val();
//     m_rps=(*p_rps);
//   }
//   while (p_rps=i_col()){
//     m_rps=(m_ptf<=(*p_rps)->val())?m_rps:*p_rps;
//     m_ptf=(m_ptf<=(*p_rps)->val())?m_ptf:(*p_rps)->val();
//   }

//   // et on le colle dans la sequence

//   rps=m_rps;
//   for (i=0;i<_lg;i++){
//     if (rps){
//       _seq[i]=*rps->pt();
//       rps=rps->su();
//     }
//     else
//       _seq[i].vide();
//   }

//   // nettoyage
//   for(i=_lg-1;i>=0;i--){
//     i_col.colle(col[i]);
//     while (p_rps=i_col())
//       if ((*p_rps)->no()==1)
// 	delete (*p_rps);
//     col[i].nettoie();
//   }
//   delete[] col;
//   Stock<10000, R_Noeud<Pt_val<Csegment> > >::detruit();

//   return m_ptf;
// }

// double Cpartition::select_max(const Part_equ& S_eq, 
// 			     Fonction_sg& ptf)
// {
//   if (_lg>0)
//     nettoie();
//   _lg=S_eq.taille();
//   _val=S_eq.val();
//   _seq = new Csegment[_lg];

//   Part_equ_iter i_eq(S_eq);
//   Csegment *s_g, *s_d;
//   double v_ptf, m_ptf, x_ptf;
//   i_eq.init();
//   int i;

//   Tliste<R_Noeud<Pt_val<Csegment> >* >* col;
//   col= new Tliste<R_Noeud<Pt_val<Csegment> >* >[_lg];
//   Tliste_iter<R_Noeud<Pt_val<Csegment> >* > i_col;
//   R_Noeud<Pt_val<Csegment> > *rps, *m_rps, **p_rps;
//   Pt_val<Csegment> pvs;

//   // on parcourt le graphe des segments de droite a gauche
// 				// en propageant les maximaux
//   while (s_d=i_eq(_lg-1)){
//     ptf.ajuste(s_d);
//     v_ptf=ptf.calcul();
//     pvs.g_val(v_ptf);
//     pvs.g_pt(s_d);
//     col[_lg-1].aj_fin(new R_Noeud<Pt_val<Csegment> >(pvs));
//   }
//   for (i=_lg-2;i>=0;i--){
//     i_col.colle(col[i+1]);
//     while (s_g=i_eq(i)){
//       i_eq.init(i+1);
//       i_col.init();
//       if (!s_g->est_vide()){
// 	s_d=i_eq.d(i+1);
// 	while (p_rps=i_col())
// 	  if ((*p_rps)->pt()==s_d)
// 	    break;
// 	ptf.ajuste(s_g);
// 	x_ptf=ptf.calcul();
// 	m_ptf=x_ptf+((p_rps)?(*p_rps)->val():0);
// 	m_rps=(p_rps)?*p_rps:0;
// 	while (s_d=i_eq.d(i+1)){
// 	  while (p_rps=i_col())
// 	    if ((*p_rps)->pt()==s_d)
// 	      break;
// 	  v_ptf=x_ptf+((p_rps)?(*p_rps)->val():0);
// 	  m_rps=(m_ptf>=v_ptf)?m_rps:(p_rps)?*p_rps:0;
// 	  m_ptf=(m_ptf>=v_ptf)?m_ptf:v_ptf;
// 	}
// 	pvs.g_val(m_ptf);
// 	pvs.g_pt(s_g);
// 	col[i].aj_fin(new R_Noeud<Pt_val<Csegment> >(pvs));
// 	(*col[i].ind_fin())->dev_su(*m_rps);
//       }
//       else{ // normalement seulement quand rien a gauche de s_g
// 	s_d=i_eq(i+1);
// 	while (p_rps=i_col())
// 	  if ((*p_rps)->pt()==s_d)
// 	    break;
// 	m_ptf=(p_rps)?(*p_rps)->val():0;
// 	m_rps=(p_rps)?*p_rps:0;
// 	while (s_d=i_eq(i+1)){
// 	  while (p_rps=i_col())
// 	    if ((*p_rps)->pt()==s_d)
// 	      break;
// 	  v_ptf=(p_rps)?(*p_rps)->val():0;
// 	  m_rps=(m_ptf>=v_ptf)?m_rps:(p_rps)?*p_rps:0;
// 	  m_ptf=(m_ptf>=v_ptf)?m_ptf:v_ptf;
// 	}
// 	col[i].aj_deb(new R_Noeud<Pt_val<Csegment> >(*m_rps));
//       }
//     }
//   }

//   // on recupere le maximum

//   i_col.colle(col[0]);
//   if (p_rps=i_col()){
//     m_ptf=(*p_rps)->val();
//     m_rps=(*p_rps);
//   }
//   while (p_rps=i_col()){
//     m_rps=(m_ptf>=(*p_rps)->val())?m_rps:*p_rps;
//     m_ptf=(m_ptf>=(*p_rps)->val())?m_ptf:(*p_rps)->val();
//   }

//   // et on le colle dans la sequence

//   rps=m_rps;
//   for (i=0;i<_lg;i++){
//     if (rps){
//       _seq[i]=*rps->pt();
//       rps=rps->su();
//     }
//     else
//       _seq[i].vide();
//   }

//   // nettoyage
//   for(i=_lg-1;i>=0;i--){
//     i_col.colle(col[i]);
//     while (p_rps=i_col())
//       if ((*p_rps)->no()==1)
// 	delete (*p_rps);
//     col[i].nettoie();
//   }
//   delete[] col;
//   Stock<10000, R_Noeud<Pt_val<Csegment> > >::detruit();

//   return m_ptf;
// }
				
///////////////////////////////
// sorties

ostream& Cpartition::operator<<(ostream& s) const
{
  int i;

  if (_lg==0)
    return s;
  
  for (i=0;i<_lg-1;i++){
    _seq[i].operator<<(s);
    s << " XXX ";
  }

  _seq[_lg-1].operator<<(s);
  s << " ---> ";
  char tmp[20];
  snprintf(tmp,20,"%.12g",_val);
  s << tmp << '\0';
  return s;
}

