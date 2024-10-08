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

#include "posi_motif.h"
#include "position.h"
#include "part_equ.h"
#include "sequence.h"
#include "partition.h"
#include "part_equ_iter.h"

#include "matrice.h"
#include "mat_iter.h"

#include "S_lex.h"
#include "R_lex.h"

#include "Tliste_iter.h"

#include "time.h"
#include <stdio.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Part_equ::Part_equ(const Part_equ & S) : _lg(S._lg), _val(S._val)
{
  if (_lg){
    int i;
    if (!(_seq=new Cposopere[_lg])){
      cerr << "pb allocation" << endl;
      exit(1);	
    }
    for (i=0;i<_lg;i++)
      _seq[i]=S._seq[i];
  }
  else
    _seq=0;
}

Part_equ::~Part_equ()
{
  if (_lg>0)
    delete[] _seq;
}

///////////////////////////////////////
//  modifications

void Part_equ::operator=(const Part_equ & S) 
{
  if (_lg)
    delete[] _seq;

  _lg=S._lg;
  _val=S._val;

  if (_lg){
    int i;
    _seq=new Cposopere[_lg];
    for (i=0;i<_lg;i++)
      _seq[i]=S._seq[i];
  }
  else
    _seq=0;
}

void Part_equ::operator=(const Cpartition& S) 
{
  if (_lg)
    delete[] _seq;

  _lg=S.taille();
  _val=S.val();

  if (_lg){
    int i;
    _seq=new Cposopere[_lg];
    for (i=0;i<_lg;i++)
      _seq[i].ajoute(S[i]);
  }
  else
    _seq=0;
}

bool Part_equ::exhaustive(const Part_equ& S, const Fsequence& Sp)
{
  g_val(S.val());

  mot_ces::borne b;

  int T=S.taille();
  int i,j;
  int min_g, max_d;
  const Csegment *p_sg, *p_sd;// *p_sd1;
  Csegment sg, sd;
	
  const Cdescripteur *ods, *od, *of, *o;
  ods=Sp.deb();
  of=Sp.fin();
  double dif_dg;
	 
  Tliste<mot_ces>* ces;
  ces=new Tliste<mot_ces>[T];

  Tliste_iter<mot_ces> ig_ces;
  Tliste_iter<mot_ces> id_ces;
  mot_ces *p_mg, *p_md, *p_mi;

  Part_equ_iter it_S(S), it_S2(S);

  // lien entre les mot_ces et les segments

  for (i=0;i<T;i++){
    it_S.init(i);
    while ((p_sd=it_S(i))){
      ces[i].aj_fin();
      p_md=ces[i].ind_fin();
      p_md->recupere(*p_sd);
    }
  }				
	
  // de gauche à droite
	
  it_S.init();
  for (i=0;i<T-1;i++){
    ig_ces.colle(ces[i]);
    id_ces.colle(ces[i+1]);
    it_S.init(i);
    while ((p_sg=it_S(i))){
      ig_ces.init();
      while ((p_mg=ig_ces()))
	if (p_mg->pointe(p_sg))
	  break;
      if (!p_mg){
	cout << "Pb dans Part_equ::exhaustive" << endl;
	for (i=0;i<T;i++)
	  ces[i].nettoie();
	delete[] ces;
	return 0;
      }
      min_g=p_mg->min_deb();
      it_S.init(i+1);
      while ((p_sd=it_S.d(i+1))){
	id_ces.init();
	while ((p_md=id_ces()))
	  if (p_md->pointe(p_sd))
	    break;
	if (!p_md){
	  cout << "Pb dans Part_equ::exhaustive" << endl;
	  for (i=0;i<T;i++)
	    ces[i].nettoie();
	  delete[] ces;
	  return 0;
	}
	sg=*p_sg;
	sd=*p_sd;
	dif_dg=0;
	od=&Sp[min_g-1];
	j=sg.pos_f();
	for (o=&Sp[sg.pos_f()-1];o>od;o--){
	  --j;
	  dif_dg+=sg.rac_f(ods,o,of)+sd.all_d(ods,o,of);
	  if (dif_dg>=0){
	    b.g_bor(j);
	    p_mg->_fin.aj_fin_diff(b);
	    b.g_bor(j+1);
	    p_md->_deb.aj_fin_diff(b);
	  }
	}
      }
    }	
  }

  // de droite à gauche 
	
  for (i=T-1;i>0;i--){
    id_ces.colle(ces[i]);
    ig_ces.colle(ces[i-1]);
    it_S.init(i);
    while ((p_sd=it_S(i))){
      id_ces.init();
      while ((p_md=id_ces()))
	if (p_md->pointe(p_sd))
	  break;
      if (!p_md){
	cout << "Pb dans Part_equ::exhaustive" << endl;
	for (i=0;i<T;i++)
	  ces[i].nettoie();
	delete[] ces;
	return 0;
      }
      max_d=p_md->max_fin();
      it_S.init(i-1);
      while ((p_sg=it_S.g(i-1))){
	ig_ces.init();
	while ((p_mg=ig_ces()))
	  if (p_mg->pointe(p_sg))
	    break;
	if (!p_mg){
	  cout << "Pb dans Part_equ::exhaustive" << endl;
	  for (i=0;i<T;i++)
	    ces[i].nettoie();
	  delete[] ces;
	  return 0;
	}
	sg=*p_sg;
	sd=*p_sd;
	dif_dg=0;
	od=&Sp[max_d-1];
	j=sd.pos_d();
	for (o=&Sp[sd.pos_d()-1];o<od;o++){
	  ++j;
	  dif_dg+=sg.all_f(ods,o,of)+sd.rac_d(ods,o,of);
	  if (dif_dg>=0){
	    b.g_bor(j-1);
	    p_mg->_fin.aj_fin_diff(b);
	    b.g_bor(j);
	    p_md->_deb.aj_fin_diff(b);
	  }
	}
      }
    }
  }
	
  // epurage

  for (i=0;i<T;i++){
    id_ces.colle(ces[i]);
    ig_ces.colle(ces[i]);
    while ((p_md=id_ces())){
      ig_ces=id_ces;
      p_mi=0;
      while ((p_mg=ig_ces())){
	if ((p_mg->_mot==p_md->_mot) &&
	    (p_mg->min_deb() <= p_md->max_fin()) &&
	    (p_md->min_deb() <= p_mg->max_fin()))
	  p_mi=p_mg;
      }
      if (p_mi){
	p_mi->_deb.aj_fin_diff(p_md->_deb);
	p_mi->_fin.aj_fin_diff(p_md->_fin);
	p_md->_l_pseg.nettoie();
      }
    }
  }

  // récupération 

  if (_lg>0)
    delete[] _seq;
  _seq= new Cposopere[T];
  _lg=T;
  for (i=0;i<T;i++){
    id_ces.colle(ces[i]);
    while ((p_md=id_ces())){
      if (!p_md->_l_pseg.est_vide()){
	p_md->met_au_point(Sp);
	_seq[i].recup(*p_md);
	//	p_md->remplit(_seq[i], Sp);
      }
    }
  }
  for (i=0;i<T;i++)
    ces[i].nettoie();
  delete[] ces;
  return 1;
}	

/////////////////////////////////////////////
//  ordres

void Part_equ::liste_cesures(Tliste<Cposition>& L) const
{
  L.nettoie();
  Cposopere* p=_seq;
  Tliste<Csegment>* p_fils;
  Tliste_iter<Csegment> itf;
  Csegment* ps;
  int i;
  p++;

  for (i=1;i<_lg;i++){
    p_fils=&((p++)->_fils);
    itf.colle(*p_fils);
    while ((ps=itf()))
      if (ps->pos_d()!=1)
	L.aj_fin_diff(ps->pos_d());
  }

}


///////////////////////////////////////////////
///       calculs  

double Part_equ::nbe_segments() const
{
  int i,n=0;
  Cposopere* p=_seq;

  for (i=0;i<_lg;i++)
    n+=(p++)->nbe_segments();
  return (double)n;
}

double Part_equ::nbe_cesures() const
{
  Tliste<Cposition> L;
  liste_cesures(L);
  return (double)L.nb_ind();
}

double Part_equ::moy_nbe_segments() const
{
  if (_lg==0)
    return 0;
	
  int i,n=0;
  Cposopere* p=_seq;

  for (i=0;i<_lg;i++)
    n+=(p++)->nbe_segments();
  return (double)n/_lg;
}

double Part_equ::moy_nbe_cesures() const
{
  if (_lg==0)
    return 0;

  Tliste<Cposition> L;
  liste_cesures(L);

  return (double)L.nb_ind()/_lg;
}

double Part_equ::moy_pred_diff() const
{
  long n=0;
  int l=dern_pos();
  int i;
  Cposopere *j, *j_d, *j_f;
	
  Tliste<Cdesc_comp> alph;
  Cdesc_comp ds;
  Tliste_iter<Csegment> itsg;

  Csegment* sg;
  j_d=_seq;j_f=&(_seq[_lg-1]);

  for (i=1;i<=l;i++){
    j=j_d;
    while (j<=j_f){
      if (i>=j->min_d()){
	if (i<=j->max_f()){
	  itsg.colle(j->_fils);
	  while ((sg=itsg()))
	    if ((i>=sg->pos_d()) && (i<=sg->pos_f())){
	      ds=(*sg)[i];
	      if (ds.est_alpha())
		n+=alph.aj_fin_diff(ds);
	    }
	}
	else
	  j_d++;
      }
      else
	break;
      j++;
    }
    alph.nettoie();
  }

  return (double)n/l;
}

double Part_equ::var_bornes() const
{
  double n=0;
  int i;
  for (i=0;i<_lg;i++)
    n+=_seq[i].var_borne_deb();

  return n;
}
	

////////////////////////////////////////////
//  sorties

ostream& Part_equ::operator<<(ostream& s) const
{
  int i;
  char tmp[30];
  
  for (i=0;i<_lg-1;i++){
    _seq[i].operator<<(s);
    s << " XXX ";
  }

  _seq[_lg-1].operator<<(s);

  sprintf(tmp,"----> %.12g",val());
  s << tmp << endl;
  return s;
}

ostream& Part_equ::synt(ostream& s) const
{
  int i;
  char tmp[30];

  for (i=0;i<_lg-1;i++){
    _seq[i].synt(s);
    s  << " XXX ";
  }

  _seq[_lg-1].synt(s);

  sprintf(tmp,"----> %.12g",val());
  s << tmp << endl;
  return s;
}


/////////////////////////////////////////
/////////////////////////////////////////
//  recuperations
/////////////////////////////////////////

bool Part_equ::recup(const string& ns)
{
  if (ns.empty())
    return 0;

  ifstream flot(ns.c_str());
  if (!flot){
    cout << "pb a l'ouverture de " << ns << endl;
    return 0;
  }
  ifstream flotb(ns.c_str());
  bool t=recup(flot, flotb);

  flot.close();
  flotb.close();
  return t;
}

bool Part_equ::recup(ifstream& flot, ifstream& flotb)
{
  char c;
  Tliste<Cposopere> prov;
  int i,lg=1;
  double x;
  int fin=0;
  int lim=-1;

  flotb >> c;
  flotb.unget();
  if (c=='M'){
    return 0;
  }
  
  while (!flotb.eof() && (fin!=2)){
    flotb.get(c);
    switch(c){
    case '-':
      fin=1;
      break;
    case ' ':
    case '\n':
    case '\t':
      if (lim>=4)
	lg++;
      lim=1;
      break;
    case 'X':
      if (lim>0)
	lim++;
      break;
    case '>':
      fin++;
      break;
    default:
      lim=0;
      fin=0;	
      break;
    }
  }
	
  if ((lim==-1) || (fin!=2)){
    //		cout << "Mauvais type de fichier " << endl;
    return 0;
  }
  if (lg!=_lg){
    if (_lg)
      delete[] _seq;
    _lg=lg;
    _seq=new Cposopere[_lg];
  }
	
  c=' ';
  i=0;
  while (c!='>'){
    c=_seq[i++].recup(flot);
    if ((c!='X') && (c!='>'))
      return 0;
    while (c=='X')
      flot.get(c);
  }
  flotb >> x;
  flot >> x;
  g_val(x);

  return 1;
}

void Part_equ::recupere(const Fsequence& G, int lg,
			const R_Noeud_val<Cposition>& C,
			R_lex<Cposition >& Lc)
{
  R_Noeud<Cposition> *cm, *csu, *ci;
  int i;
  Cdescripteur *p_p, *p_ps;
  const Cdescripteur *dds;
  
  dds=G.deb();
  const Cdescripteur* pf=G.fin();
  g_val(C.val());
	
  if (_lg>0)
    delete[] _seq;
  _lg=lg;
  if (!(_seq=new Cposopere[_lg])){
    cerr << "pb allocation" << endl;
    return;	
  }
	
  cm=(R_Noeud<Cposition>*)&C;

  for (i=0;i<_lg;i++){

    if (cm->Noeud_normal()){
      csu=cm->su();
      if (*csu<*cm){
	p_ps=&(G[csu->pos()]);
	p_p=&(G[cm->pos()]);
	Lc.init(dds,p_ps, pf);
	while (++p_ps!=p_p) 
	  Lc.plus_egale(dds,p_ps,pf);
	_seq[_lg-i-1].recup_max(Lc, *csu, *cm);
      }
      else
	_seq[_lg-i-1].aj_vide();
    }
    else{
      csu=cm;
      _seq[_lg-i-1].aj_vide();
    }

    ci=cm;
    cm=csu;
    csu=ci;
  }
}

void Part_equ::recupere(const Fmatrice& M, int lg,
			const R_Noeud_val<Cposition>& C,
			R_lex<Cposition >& Lc)
{
  R_Noeud<Cposition> *cm, *csu, *ci;
  int i;
  mat_iter m_i(M);
  mat_iter m_is(M);

  g_val(C.val());
	
  if (_lg>0)
    delete[] _seq;
  _lg=lg;
  if (!(_seq=new Cposopere[_lg])){
    cerr << "pb allocation" << endl;
    return;	
  }
	
  cm=(R_Noeud<Cposition>*)&C;

  for (i=0;i<_lg;i++){

    if (cm->Noeud_normal()){
      csu=cm->su();
      if (*csu<*cm){
	m_i.init(csu->pos());
	m_is.init(cm->pos()-1);
	Lc.init(m_i);
	while (m_i(m_is))
	  Lc.plus_egale(m_i);
	_seq[_lg-i-1].recup_max(Lc, *csu, *cm);
      }
      else
	_seq[_lg-i-1].aj_vide();
    }
    else{
      csu=cm;
      _seq[_lg-i-1].aj_vide();
    }

    ci=cm;
    cm=csu;
    csu=ci;
  }
}

void Part_equ::recupere(const Fsequence& G, 
			int lg,
			const R_Noeud_val<Cposition>& C)
{
  Csegment s;
  R_Noeud<Cposition> *cm, *csu, *ci;
  int i;

  g_val(C.val());

  if (_lg>0)
    delete[] _seq;
  _lg=lg;
  if (!(_seq=new Cposopere[_lg])){
    cerr << "pb allocation" << endl;
    return;	
  }
	
  cm=(R_Noeud<Cposition>*)&C;

  for (i=0;i<_lg;i++){
    if (cm->Noeud_normal()){
      csu=cm->su();
      if (*csu<*cm){
	s.g_pos_d(csu->pos()+1);
	s.g_pos_f(cm->pos());
	s.g_val(s.calc_val(G));
	_seq[_lg-i-1].ajoute(s);
      }
      else
	_seq[_lg-i-1].aj_vide();
    }
    else{
      csu=cm;
      _seq[_lg-i-1].aj_vide();
    }

    ci=cm;
    cm=csu;
    csu=ci;
  }
}

void Part_equ::recupere(const Fmatrice& M, 
			int lg,
			const R_Noeud_val<Cposition>& C)
{
  Csegment s;
  R_Noeud<Cposition> *cm, *csu, *ci;
  int i;

  g_val(C.val());

  if (_lg>0)
    delete[] _seq;
  _lg=lg;
  if (!(_seq=new Cposopere[_lg])){
    cerr << "pb allocation" << endl;
    return;	
  }
	
  cm=(R_Noeud<Cposition>*)&C;

  for (i=0;i<_lg;i++){
    if (cm->Noeud_normal()){
      csu=cm->su();
      if (*csu<*cm){
	s.g_pos_d(csu->pos()+1);
	s.g_pos_f(cm->pos());
	s.g_val(s.calc_val(M));
	_seq[_lg-i-1].ajoute(s);
      }
      else
	_seq[_lg-i-1].aj_vide();
    }
    else{
      csu=cm;
      _seq[_lg-i-1].aj_vide();
    }

    ci=cm;
    cm=csu;
    csu=ci;
  }
}

void Part_equ::recupere(const Fsequence& G, int lg, 
			const S_Noeud_val<Cposition>& C, 
			S_lex<Cposition >& Lc)
{	
  if (_lg>0)
    delete[] _seq;
  _lg=lg;
  if (!(_seq=new Cposopere[_lg])){
    cerr << "pb allocation" << endl;
    return;	
  }

  const Cdescripteur* df=G.fin();
  const Cdescripteur* dds=G.deb();
	
  int i;

  g_val(C.val());

  Tliste<S_Noeud<Cposition>*> *l_noe_p, *l_noe_f, *l_noe_i;

  l_noe_p = new Tliste<S_Noeud<Cposition>*>; //liste des noeuds peres

  l_noe_f = new Tliste<S_Noeud<Cposition>*>; //liste des noeuds fils
  Tliste_iter<S_Noeud<Cposition>*> i_noe_f(*l_noe_f);

  Tliste_iter<S_Noeud<Cposition>*> i_su(C._su);

  l_noe_p->aj_deb((S_Noeud<Cposition>*)&C);

  S_Noeud<Cposition> **pt_cf, **pt_nf;
  S_Noeud<Cposition> *np, *nf;

  Csegment seg;

  Cdescripteur *ds, *d;

  for (i=0;i<_lg;i++){
    while ((np=l_noe_p->enl_deb())){ //parcours des noeuds peres
      if (np->Noeud_normal()){
	i_su.colle(np->_su);
	while ((pt_nf=i_su())){ // parcours des noeuds fils
	  nf=*pt_nf;
	  if (nf->Noeud_normal()){
	    i_noe_f.init();
	    while ((pt_cf=i_noe_f())){	
	      //parcours des noeuds fils stockes
	      if (*nf==**pt_cf)
		break;
	    }
	    if (!pt_cf)
	      l_noe_f->aj_fin(nf);
	  }
	  if (*nf<*np){
	    ds=&(G[nf->pos()]);
	    d=&(G[np->pos()]);
	    Lc.init(dds,ds, df);
	    while (++ds!=d)
	      Lc.plus_egale(dds,ds, df);
	    _seq[_lg-i-1].recup_max(Lc, *nf, *np);
	  }
	  else
	    _seq[_lg-i-1].aj_vide();
	}
      }
      else
	_seq[_lg-i-1].aj_vide();
    }
    l_noe_i=l_noe_f;
    l_noe_f=l_noe_p;
    l_noe_p=l_noe_i;
    i_noe_f.colle(*l_noe_f);
  }

  delete l_noe_p;
  delete l_noe_f;
}

void Part_equ::recupere(const Fmatrice & M, int lg, 
			const S_Noeud_val<Cposition>& C, 
			S_lex<Cposition >& Lc)
{	
  if (_lg>0)
    delete[] _seq;
  _lg=lg;
  if (!(_seq=new Cposopere[_lg])){
    cerr << "pb allocation" << endl;
    return;	
  }

  int i;

  g_val(C.val());

  Tliste<S_Noeud<Cposition>*> *l_noe_p, *l_noe_f, *l_noe_i;

  l_noe_p = new Tliste<S_Noeud<Cposition>*>; //liste des noeuds peres

  l_noe_f = new Tliste<S_Noeud<Cposition>*>; //liste des noeuds fils
  Tliste_iter<S_Noeud<Cposition>*> i_noe_f(*l_noe_f);

  Tliste_iter<S_Noeud<Cposition>*> i_su(C._su);

  l_noe_p->aj_deb((S_Noeud<Cposition>*)&C);

  S_Noeud<Cposition> **pt_cf, **pt_nf;
  S_Noeud<Cposition> *np, *nf;

  Csegment seg;

  mat_iter mi(M);
  mat_iter mi2(M);

  for (i=0;i<_lg;i++){
    while ((np=l_noe_p->enl_deb())){ //parcours des noeuds peres
      if (np->Noeud_normal()){
	i_su.colle(np->_su);
	while ((pt_nf=i_su())){ // parcours des noeuds fils
	  nf=*pt_nf;
	  if (nf->Noeud_normal()){
	    i_noe_f.init();
	    while ((pt_cf=i_noe_f())){	
	      //parcours des noeuds fils stockes
	      if (*nf==**pt_cf)
		break;
	    }
	    if (!pt_cf)
	      l_noe_f->aj_fin(nf);
	  }
	  if (*nf<*np){
	    mi.init(nf->pos());
	    mi2.init(np->pos());
	    Lc.init(mi);
	    mi++;
	    while (mi!=mi2){
	      Lc.plus_egale(mi);
	      mi++;
	    }
	    _seq[_lg-i-1].recup_max(Lc, *nf, *np);
	  }
	  else
	    _seq[_lg-i-1].aj_vide();
	}
      }
      else
	_seq[_lg-i-1].aj_vide();
    }
    l_noe_i=l_noe_f;
    l_noe_f=l_noe_p;
    l_noe_p=l_noe_i;
    i_noe_f.colle(*l_noe_f);
  }

  delete l_noe_p;
  delete l_noe_f;
}

void Part_equ::recupere(const Fsequence& G, 
			int lg, 
			const S_Noeud_val<Cposition>& C)
{	
  if (_lg>0)
    delete[] _seq;
  _lg=lg;
  if (!(_seq=new Cposopere[_lg])){
    cerr << "pb allocation" << endl;
    return;
  }

  int i;

  g_val(C.val());

  Tliste<S_Noeud<Cposition>*> *l_noe_p, *l_noe_f, *l_noe_i;

  l_noe_p = new Tliste<S_Noeud<Cposition>*>; //liste des noeuds peres
  l_noe_f = new Tliste<S_Noeud<Cposition>*>; //liste des noeuds fils

  Tliste_iter<S_Noeud<Cposition>*> i_noe_f(*l_noe_f);

  Tliste_iter<S_Noeud<Cposition>*> i_su(C._su);

  l_noe_p->aj_deb((S_Noeud<Cposition>*)&C);

  S_Noeud<Cposition> **pt_cf, **pt_nf;
  S_Noeud<Cposition> *np, *nf;

  Csegment seg;

  for (i=0;i<_lg;i++){
    while ((np=l_noe_p->enl_deb())){ //parcours des noeuds peres
      if (np->Noeud_normal()){
	i_su.colle(np->_su);
	seg.g_pos_f(np->pos());
	while ((pt_nf=i_su())){ // parcours des noeuds fils
	  nf=*pt_nf;
	  if (nf->Noeud_normal()){
	    i_noe_f.init();
	    while ((pt_cf=i_noe_f())){ 
	      //parcours des noeuds fils stockes
	      if (*nf==**pt_cf)
		break;
	    }
	    if (!pt_cf)
	      l_noe_f->aj_fin(nf);
	  }
	  if (*nf<*np){
	    seg.g_pos_d(nf->pos()+1);
	    seg.g_val(seg.calc_val(G));
	    _seq[_lg-i-1].ajoute(seg);
	  }
	  else
	    _seq[_lg-i-1].aj_vide();
	}
      }
      else
	_seq[_lg-i-1].aj_vide();
    }
    l_noe_i=l_noe_f;
    l_noe_f=l_noe_p;
    l_noe_p=l_noe_i;
    i_noe_f.colle(*l_noe_f);
  }

  delete l_noe_p;
  delete l_noe_f;
}

void Part_equ::recupere(const Fmatrice& M, 
			int lg, 
			const S_Noeud_val<Cposition>& C)
{	
  if (_lg>0)
    delete[] _seq;
  _lg=lg;
  if (!(_seq=new Cposopere[_lg])){
    cerr << "pb allocation" << endl;
    return;
  }

  int i;

  g_val(C.val());

  Tliste<S_Noeud<Cposition>*> *l_noe_p, *l_noe_f, *l_noe_i;

  l_noe_p = new Tliste<S_Noeud<Cposition>*>; //liste des noeuds peres
  l_noe_f = new Tliste<S_Noeud<Cposition>*>; //liste des noeuds fils

  Tliste_iter<S_Noeud<Cposition>*> i_noe_f(*l_noe_f);

  Tliste_iter<S_Noeud<Cposition>*> i_su(C._su);

  l_noe_p->aj_deb((S_Noeud<Cposition>*)&C);

  S_Noeud<Cposition> **pt_cf, **pt_nf;
  S_Noeud<Cposition> *np, *nf;

  Csegment seg;

  for (i=0;i<_lg;i++){
    while ((np=l_noe_p->enl_deb())){ //parcours des noeuds peres
      if (np->Noeud_normal()){
	i_su.colle(np->_su);
	seg.g_pos_f(np->pos());
	while ((pt_nf=i_su())){ // parcours des noeuds fils
	  nf=*pt_nf;
	  if (nf->Noeud_normal()){
	    i_noe_f.init();
	    while ((pt_cf=i_noe_f())){ 
	      //parcours des noeuds fils stockes
	      if (*nf==**pt_cf)
		break;
	    }
	    if (!pt_cf)
	      l_noe_f->aj_fin(nf);
	  }
	  if (*nf<*np){
	    seg.g_pos_d(nf->pos()+1);
	    seg.g_val(seg.calc_val(M));
	    _seq[_lg-i-1].ajoute(seg);
	  }
	  else
	    _seq[_lg-i-1].aj_vide();
	}
      }
      else
	_seq[_lg-i-1].aj_vide();
    }
    l_noe_i=l_noe_f;
    l_noe_f=l_noe_p;
    l_noe_p=l_noe_i;
    i_noe_f.colle(*l_noe_f);
  }

  delete l_noe_p;
  delete l_noe_f;
}

void Part_equ::recupere(const Fsequence& G, int lg, 
			const S_Noeud_val<Cposi_motif>& C, 
			S_lex<Cposi_motif >& Lc)
{	
  int i;
	
  g_val(C.val());
  if (_lg>0)
    delete[] _seq;

  _lg=lg;
  if (!(_seq=new Cposopere[_lg])){
    cerr << "pb allocation" << endl;
    return;	
  }

  Tliste<S_Noeud<Cposi_motif>*> *l_noe_p, *l_noe_f, *l_noe_i;

  l_noe_p = new Tliste<S_Noeud<Cposi_motif>*>;  
  //liste des noeuds peres

  l_noe_f = new Tliste<S_Noeud<Cposi_motif>*>;  //liste des noeuds fils
  Tliste_iter<S_Noeud<Cposi_motif>*> i_noe_f(*l_noe_f);

  Tliste_iter<S_Noeud<Cposi_motif>*> i_su(C._su);

  l_noe_p->aj_deb((S_Noeud<Cposi_motif>*)&C);
	
  S_Noeud<Cposi_motif> **pt_nf, **pt_cf;
  S_Noeud<Cposi_motif> *np, *nf;

  const Cdescripteur* d, *ds;
  const Cdescripteur* df=G.fin();
  const Cdescripteur* dds=G.deb();

  Csegment seg;

  Smotif_Noeud_val<Cposi_motif> calc;

  for (i=0;i<_lg;i++){
    while ((np=l_noe_p->enl_deb())){	//parcours des noeuds peres
      if (np->Noeud_normal()){
	i_su.colle(np->_su);  
	while ((pt_nf=i_su())){	// parcours des noeuds fils
	  nf=*pt_nf;
	  i_noe_f.init();
	  while ((pt_cf=i_noe_f())){	
	    //parcours des noeuds fils stockes
	    if (nf->Cposition::operator==(**pt_cf))
	      break;
	  }
	  if (!pt_cf)
	    l_noe_f->aj_fin(nf);
	  if (*nf<*np){
	    calc.g_pt(nf->pt());
	    ds=&(G[nf->pos()]);
	    d=&(G[np->pos()]);
	    calc.init(dds,ds, df, *nf);
	    while (++ds!=d)
	      calc.ajoute_su(dds,ds, df);
	    _seq[_lg-i-1].recup_max(calc, *nf, *np);
	  }
	  else
	    _seq[_lg-i-1].aj_vide();
	}
      }
      else
	_seq[_lg-i-1].aj_vide();
    }
    l_noe_i=l_noe_f;
    l_noe_f=l_noe_p;
    l_noe_p=l_noe_i;
    i_noe_f.colle(*l_noe_f);
  }

  delete l_noe_p;
  delete l_noe_f;
}

void Part_equ::recupere(const Fmatrice& M, int lg, 
			const S_Noeud_val<Cposi_motif>& C, 
			S_lex<Cposi_motif >& Lc)
{	
  int i;
	
  g_val(C.val());
  if (_lg>0)
    delete[] _seq;

  _lg=lg;
  if (!(_seq=new Cposopere[_lg])){
    cerr << "pb allocation" << endl;
    return;	
  }

  Tliste<S_Noeud<Cposi_motif>*> *l_noe_p, *l_noe_f, *l_noe_i;

  l_noe_p = new Tliste<S_Noeud<Cposi_motif>*>;  
  //liste des noeuds peres

  l_noe_f = new Tliste<S_Noeud<Cposi_motif>*>;  //liste des noeuds fils
  Tliste_iter<S_Noeud<Cposi_motif>*> i_noe_f(*l_noe_f);

  Tliste_iter<S_Noeud<Cposi_motif>*> i_su(C._su);

  l_noe_p->aj_deb((S_Noeud<Cposi_motif>*)&C);
	
  S_Noeud<Cposi_motif> **pt_nf, **pt_cf;
  S_Noeud<Cposi_motif> *np, *nf;

  mat_iter mi(M);
  mat_iter mi2(M);

  Csegment seg;

  Smotif_Noeud_val<Cposi_motif> calc;

  for (i=0;i<_lg;i++){
    while ((np=l_noe_p->enl_deb())){	//parcours des noeuds peres
      if (np->Noeud_normal()){
	i_su.colle(np->_su);  
	while ((pt_nf=i_su())){	// parcours des noeuds fils
	  nf=*pt_nf;
	  i_noe_f.init();
	  while ((pt_cf=i_noe_f())){	
	    //parcours des noeuds fils stockes
	    if (nf->Cposition::operator==(**pt_cf))
	      break;
	  }
	  if (!pt_cf)
	    l_noe_f->aj_fin(nf);
	  if (*nf<*np){
	    calc.g_pt(nf->pt());
	    mi.init(nf->pos());
	    mi2.init(np->pos());
	    calc.init(mi, *nf);
	    mi++;
	    while (mi!=mi2){
	      calc.ajoute_su(mi);
	      mi++;
	    }
	    _seq[_lg-i-1].recup_max(calc, *nf, *np);
	  }
	  else
	    _seq[_lg-i-1].aj_vide();
	}
      }
      else
	_seq[_lg-i-1].aj_vide();
    }
    l_noe_i=l_noe_f;
    l_noe_f=l_noe_p;
    l_noe_p=l_noe_i;
    i_noe_f.colle(*l_noe_f);
  }

  delete l_noe_p;
  delete l_noe_f;
}


///////////////////////////////////////////////////////////////////////
////////////////  mot_ces  ////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////


////////////////////////
// modif


void mot_ces::croit()
{
  Tliste_iter<borne> it(_deb), it2(_deb);
  borne* pT1, *pT2;
  int min1;

  ////// _deb

  if (_deb.nb_ind()>=2){
    it();
    while ((pT1=~it)){
      min1=pT1->bor();
      it2.init();
      if (it==it2)
	break;
      while ((pT2=~it2)){
	if (pT2->bor()>min1){
	  _deb.permute_mm_fin(it,it2);
	  break;
	}
	it2(it);
      }
      if (!pT2)
	it();
    }
	
    ///////// _fin

    if (_fin.nb_ind()<2)
      return;

    it.colle(_fin);
    it2.colle(_fin);
    it();
    while ((pT1=~it)){
      min1=pT1->bor();
      it2.init();
      if (it==it2)
	break;
      while ((pT2=~it2)){
	if (pT2->bor()>min1){
	  _fin.permute_mm_fin(it,it2);
	  break;
	}
	it2(it);
      }
      if (!pT2)
	it();
    }
  }
}

void mot_ces::evalue(const Fsequence& Sp)
{
  if (_deb.est_vide())
    return;

  double val;
  int ori, mf, md, min, i;
  borne *pbd, *pbf;
  Tliste_iter<borne> it_d(_deb), it_f(_fin);

  pbd=it_d();
  ori=pbd->bor();
  pbd->g_val(0);
  val=0;
	
  const Cdescripteur *pof, *pod=Sp.deb()+ori-1;
  const Cdescripteur* fin=Sp.fin();
  const Cdescripteur* dds=Sp.deb();

  Tliste_iter<Cdesc_comp> it_m(_mot);

  i=ori%(_mot.nb_ind());
  while (--i>=0)
    it_m++;

  pbf=it_f();
  min=mf=pbf->bor();
  if ((pbd=it_d())){
    if (min>(md=pbd->bor()-1))
      min=md;
  }
  else
    md=ori-1;

  while (pbf){
    pof=Sp.deb()+min-1;
    while (pod<=pof)
      val+=(++it_m)->multiplie(dds,pod++,fin);
    if (md==min){
      pbd->g_val(val);
      if ((pbd=it_d()))
	md=pbd->bor()-1;
      else
	md=ori-1;
    }
    if (mf==min){
      pbf->g_val(val);
      if (pbf=it_f())
	mf=pbf->bor();
      else
	mf=ori-1;
    }
    min=(md==ori-1)?mf:((mf<=md)?mf:md);
  }
}

