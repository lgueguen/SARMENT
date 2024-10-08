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

#include "parti_comp.h"

#include "Tliste_iter.h"
#include "lexique.h"
#include "S_lex.h"
#include "R_lex.h"
#include "posi_motif.h"

#include "sequence.h"
#include "matrice.h"
#include "mat_iter.h"

#include "Arret.h"
#include <stdio.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Parti_comp::Parti_comp(int i) : _vmax(0), _npmax(0), _pArr(0)
{
  _pArr=(Arret*) new Nbe_parts(i);
}

Parti_comp::Parti_comp() : _vmax(0), _npmax(0), _pArr(0)
{

}

Parti_comp::Parti_comp(void*) : _vmax(0), _npmax(0)
{
}

Parti_comp::Parti_comp(const Parti_comp& p) :
  _vmax(p._vmax), _npmax(p._npmax), _pArr(0)
{
  cerr << "appel inopine de Parti_comp::Parti_comp(const ";
  cerr << "Parti_comp& p)" << endl;
  exit(0);
}

Parti_comp::~Parti_comp()
{
  nettoie();
}

void Parti_comp::nettoie()
{
  _hist.nettoie();
  if (_pArr)
    delete _pArr;
  _pArr=0;
}

bool Parti_comp::init()
{
  nettoie();
  _pArr=(Arret*) new Nbe_parts();
  _pArr->init();
  return true;
  
}

////////////////////////////////////
// modifications

void Parti_comp::g_arret(const Arret& a)
{
  if (_pArr)
    delete _pArr;
  _pArr=a.copie();
}

bool Parti_comp::recup(const string& ns)
{
  bool tem;
  ifstream flot(ns.c_str());
  if (!flot){
    cout << "pb a l'ouverture de " << ns << endl;
    return 0;
  }
  ifstream flotb(ns.c_str());
  tem=recup(flot, flotb);
  flot.close();
  flotb.close();
  return tem;
}

bool Parti_comp::recup(ifstream& flot, ifstream& flotb)
{
  Part_equ* sq;
  string st;
  bool tem;
  int i,j;
  
  nettoie();
  tem =0;
  
  while (!flot.eof()){
    _hist.aj_fin();
    sq=_hist.ind_fin();

    if (sq->recup(flot, flotb))
      tem=1;
    else{
      _hist.tue_fin();
      getline(flotb,st);
      if (((j=st.find("MAX"))!=string::npos) &&
	  ((i=st.find("--->"))!=string::npos)){
	_npmax=atoi((st.substr(j+4)).c_str());
	_vmax=atof((st.substr(i+4)).c_str());
	tem=1;
      }
      else
	tem=0;
      break;
    }
  }
  return (tem || !(_hist.est_vide()));

}

Part_equ* Parti_comp::extrait_prem() const
{
  return _hist.ind_deb();
}


///////////////////////////////////////
//*** a partir de classes d'equivalence 

time_t Parti_comp::exhaustive(const Parti_comp& P, 
			       const Fsequence& G)
{
  Tliste_iter<Part_equ> iter(P._hist);
  Part_equ *Px;
  int i=0;
  time_t tp,tp2;
  nettoie();

  tp=time(0);
  while ((Px=iter())){
    _hist.aj_fin();
    _hist.ind_fin()->exhaustive(*Px, G);
    if (++i==10){
      cout << '|';
      i=0;
    }
    else
      cout << '.';
    cout.flush();
  }
  cout << endl;
  tp2=time(0);
  tp=(long)difftime(tp2,tp);
  return tp;
}

///////////////////////////////////////////
///   calculs
///////////////////////////////////////////

int Parti_comp::nb_seq() const
{
  int i=0;
  Tliste_iter<Part_equ> iter(_hist);

  while(iter()) i++;

  return i;
}



///////////////////////////////////////
//  lissages
///////////////////////////////////////


///////////////////////////////////////
// lissages simples

void Parti_comp::li_simp(const Fsequence& G, const Clexique& L, 
			  bool hist)
{
  _hist.nettoie();
  if (!_pArr){
    cerr << "Pb: pas d'arret defini" << endl;
    return;
  }
  int np;
  int lg = G.taille();
  if (lg==0)
    return;
  
  const Cdescripteur *dds, *dd_G, *df_G, *di_G;

  R_Noeud_val<Cposition>* predi;	//les anciennes partitions

  R_lex<Cposition> Lc(L);
  //les partitions conditionnelles
     
  R_Noeud_val<Cposition>* neui;	//les nouvelles partitions

  R_Noeud_val<Cposition>* interi, *dp_interi;

  R_Noeud<Cposition>* npar;

  predi = new R_Noeud_val<Cposition>[lg];
  R_Noeud_val<Cposition>* p_predi, *dp_predi;

  neui = new R_Noeud_val<Cposition>[lg];
  R_Noeud_val<Cposition>* p_neui, *dp_neui;

  dp_neui=&(neui[lg-1]);
  dp_predi=&(predi[lg-1]);

  /**********************************************/
  // partitions en 1 classe

  dds=di_G=dd_G=G.deb();
  df_G=G.fin();
  p_predi=predi;
  p_neui=neui;
     
  cout << "1\r";
  cout.flush();
  Lc.init(dds, dd_G, df_G);
  Lc.Noeud_max(*p_predi);
  _vmax=L.val_max(dds, dd_G, df_G);

  while (dd_G++!=df_G){
    Lc.plus_egale(dds, dd_G, df_G);
    Lc.Noeud_max(*++p_predi);
    _vmax+=L.val_max(dds, dd_G, df_G);
  }
  _npmax=L.nbe_max_part(G);

  _hist.aj_fin();
  _hist.ind_fin()->recupere(G, 1, *dp_predi, Lc);

  // ensuite

  if (!(_pArr->arrete(*(_hist.ind_fin())))){
    if (!hist)
      _hist.tue_fin();

    np=2;
    npar=0;
    while (dp_predi->val()<_vmax){
      
      cout << np << "\r";
      cout.flush();
      dd_G=G.deb();
      p_predi=predi;
      p_neui=neui;
      
      Lc.init(dds, dd_G, df_G);
      
      while (dd_G!=di_G){
	Lc.cumule(dds, ++dd_G, df_G, *p_predi++, npar);
	p_neui++;
      }
      
      *p_neui=*p_predi;
      
      while (dd_G!=df_G){	
	Lc.cumule(dds, ++dd_G, df_G, *p_predi++, npar);
	Lc.Noeud_max(*++p_neui);
      }
      interi=predi;
      predi=neui;
      neui=interi;
      dp_interi=dp_predi;
      dp_predi=dp_neui;
      dp_neui=dp_interi;
      
      np++;
      di_G++;
      
      _hist.aj_fin();
      _hist.ind_fin()->recupere(G, np-1, *dp_predi, Lc);
      if (_pArr->arrete(*(_hist.ind_fin())))
	break;
      if (!hist)
	_hist.tue_fin();
    }
  }

  // nettoyage
  
  delete[] neui;
  delete[] predi;
  Lc.nettoie();
  if (npar){
    delete npar;
    npar=0;
  }
   
  Stock<10000, R_Noeud<Cposition> >::detruit();

  cout << endl;
}

void Parti_comp::li_simp(const Fmatrice& M,
			  const Clexique& L, 
			  bool hist)
{
  _hist.nettoie();
  if (!_pArr){
    cerr << "Pb: pas d'arret defini" << endl;
    return;
  }
  int np;
  int lg = M.taille();
  if (lg==0)
    return;

  mat_iter m_i(M);
  mat_iter m_ii(M);

  R_Noeud_val<Cposition>* predi;	//les anciennes partitions

  R_lex<Cposition> Lc(L);       //les partitions conditionnelles
	
  R_Noeud_val<Cposition>* neui;	//les nouvelles partitions

  R_Noeud_val<Cposition>* interi, *dp_interi;

  R_Noeud<Cposition>* npar;

  predi = new R_Noeud_val<Cposition>[lg];
  R_Noeud_val<Cposition>* p_predi, *dp_predi;

  neui = new R_Noeud_val<Cposition>[lg];
  R_Noeud_val<Cposition>* p_neui, *dp_neui;

  dp_neui=&(neui[lg-1]);
  dp_predi=&(predi[lg-1]);

  /**********************************************/
  // partitions en 1 classe

  m_i.init();
  p_predi=predi;
  p_neui=neui;
	
  cout << "1\r";
  cout.flush();
  Lc.init(m_i);
  Lc.Noeud_max(*p_predi);
  _vmax=L.val_max(m_i);

  while (m_i++){
    Lc.plus_egale(m_i);
    Lc.Noeud_max(*++p_predi);
    _vmax+=L.val_max(m_i);
  }
  _npmax=L.nbe_max_part(M);

  _hist.aj_fin();
  _hist.ind_fin()->recupere(M, 1, *dp_predi, Lc);

  // ensuite

  if (!(_pArr->arrete(*(_hist.ind_fin())))){
    if (!hist)
      _hist.tue_fin();

    np=2;
    npar=0;
    while (dp_predi->val()<_vmax){
    
      cout << np << "\r";
      cout.flush();

      m_i.init();
      p_predi=predi;
      p_neui=neui;
     
      Lc.init(m_i);
  
      while (m_i(m_ii)){
	Lc.cumule(m_i, *p_predi++, npar);
	p_neui++;
      }
      
      *p_neui=*p_predi;

      while (m_i++){
	Lc.cumule(m_i, *p_predi++, npar);
	Lc.Noeud_max(*++p_neui);
      }
      interi=predi;
      predi=neui;
      neui=interi;
      dp_interi=dp_predi;
      dp_predi=dp_neui;
      dp_neui=dp_interi;
      
      np++;
      m_ii++;

      _hist.aj_fin();
      _hist.ind_fin()->recupere(M, np-1, *dp_predi, Lc);
      if (_pArr->arrete(*(_hist.ind_fin())))
	break;
      if (!hist)
	_hist.tue_fin();
    }
  }

  // nettoyage
  
  delete[] neui;
  delete[] predi;
  Lc.nettoie();
  if (npar){
    delete npar;
    npar=0;
  }
  
  Stock<10000, R_Noeud<Cposition> >::detruit();
  
  cout << endl;
}



///////////////////////////////////////
// lissages compliques


void Parti_comp::li_comp(const Fsequence& G,  const Clexique& L,  bool hist)
{
  _hist.nettoie();
  if (!_pArr){
    cerr << "Pb: pas d'arret defini" << endl;
    return;
  }
  int np;
  int lg=G.taille();

  const Cdescripteur *dds, *dd, *df, *di;
	
  S_Noeud_val<Cposition>* predi;	//les anciennes partitions

  S_lex<Cposition> Lc(L); //les partitions conditionnelles
	
  S_Noeud_val<Cposition>* neui;	//les nouvelles partitions

  S_Noeud_val<Cposition>* interi, *dp_interi;

  S_Noeud<Cposition>* npar;

  predi = new S_Noeud_val<Cposition>[lg];
  S_Noeud_val<Cposition>* p_predi, *dp_predi;
	
  neui = new S_Noeud_val<Cposition>[lg];
  S_Noeud_val<Cposition>* p_neui, *dp_neui;

  dp_neui=&(neui[lg-1]);
  dp_predi=&(predi[lg-1]);

  /**********************************************/
  // partitions en 1 classe

  dds=di=dd=G.deb();
  df=G.fin();
  p_predi=predi;
  p_neui=neui;
	
  Lc.init(dds, dd, df);
  Lc.Noeud_max(*p_predi);
  _vmax=L.val_max(dds, dd, df);

  cout << "1\r";
  cout.flush();
  while (dd++!=df){
    Lc.plus_egale(dds, dd, df);
    Lc.Noeud_max(*++p_predi);
    _vmax+=L.val_max(dds, dd, df);
  }
  _npmax=L.nbe_max_part(G);

  _hist.aj_fin();
  _hist.ind_fin()->recupere(G, 1, *dp_predi, Lc);

  // ensuite

  if (!(_pArr->arrete(*(_hist.ind_fin())))){
    if (!hist)
      _hist.tue_fin();

    np=2;
    npar=0;
    while (dp_predi->val()<_vmax){

      cout << np << "\r";
      cout.flush();
      dd=G.deb();
      p_predi=predi;
      p_neui=neui;
		
      Lc.init(dds, dd, df);

      while (dd!=di){
	Lc.cumule(dds, ++dd, df, *p_predi++, npar);
	p_neui++;
      }

      *p_neui=*p_predi;

      while (dd!=df){	
	Lc.cumule(dds, ++dd, df, *p_predi++, npar);
	Lc.Noeud_max(*++p_neui);
      }
      interi=predi;
      predi=neui;
      neui=interi;
      dp_interi=dp_predi;
      dp_predi=dp_neui;
      dp_neui=dp_interi;

      np++;
      di++;
      
      _hist.aj_fin();
      _hist.ind_fin()->recupere(G, np-1, *dp_predi, Lc);
      if (_pArr->arrete(*(_hist.ind_fin())))
	break;
      if (!hist)
	_hist.tue_fin();
    }
  }

  delete[] neui;
  delete[] predi;
  Lc.nettoie();
  if (npar){
    delete npar;
    npar=0;
  }

  Stock<10000, S_Noeud<Cposition> >::detruit();

  cout << endl;
}

void Parti_comp::li_comp(const Fmatrice& M,  const Clexique& L,  bool hist)
{
  _hist.nettoie();
  if (!_pArr){
    cerr << "Pb: pas d'arret defini" << endl;
    return;
  }
  int np;
  int lg=M.taille();

  mat_iter mi(M);
  mat_iter mi2(M);
	
  S_Noeud_val<Cposition>* predi;	//les anciennes partitions

  S_lex<Cposition> Lc(L); //les partitions conditionnelles
	
  S_Noeud_val<Cposition>* neui;	//les nouvelles partitions

  S_Noeud_val<Cposition>* interi, *dp_interi;

  S_Noeud<Cposition>* npar;

  predi = new S_Noeud_val<Cposition>[lg];
  S_Noeud_val<Cposition>* p_predi, *dp_predi;
	
  neui = new S_Noeud_val<Cposition>[lg];
  S_Noeud_val<Cposition>* p_neui, *dp_neui;

  dp_neui=&(neui[lg-1]);
  dp_predi=&(predi[lg-1]);

  /**********************************************/
  // partitions en 1 classe

  p_predi=predi;
  p_neui=neui;
	
  cout << "1\r";
  cout.flush();
  Lc.init(mi);
  Lc.Noeud_max(*p_predi);
  _vmax=L.val_max(mi);

  while (mi++){
    Lc.plus_egale(mi);
    Lc.Noeud_max(*++p_predi);
    _vmax+=L.val_max(mi);
  }
  _npmax=L.nbe_max_part(M);

  _hist.aj_fin();
  _hist.ind_fin()->recupere(M, 1, *dp_predi, Lc);

  // ensuite

  if (!(_pArr->arrete(*(_hist.ind_fin())))){
    if (!hist)
      _hist.tue_fin();

    np=2;
    npar=0;
    while (dp_predi->val()<_vmax){

      cout << np << "\r";
      cout.flush();
      mi.init();
      p_predi=predi;
      p_neui=neui;
		
      Lc.init(mi);

      while (mi!=mi2){
	mi++;
	Lc.cumule(mi, *p_predi++, npar);
	p_neui++;
      }

      *p_neui=*p_predi;

      while (mi++){	
	Lc.cumule(mi, *p_predi++, npar);
	Lc.Noeud_max(*++p_neui);
      }
      interi=predi;
      predi=neui;
      neui=interi;
      dp_interi=dp_predi;
      dp_predi=dp_neui;
      dp_neui=dp_interi;

      np++;
      mi2++;

      _hist.aj_fin();
      _hist.ind_fin()->recupere(M, np-1, *dp_predi, Lc);
      if (_pArr->arrete(*(_hist.ind_fin())))
	break;
      if (!hist)
	_hist.tue_fin();
    }
  }

  delete[] neui;
  delete[] predi;
  Lc.nettoie();
  if (npar){
    delete npar;
    npar=0;
  }

  Stock<10000, S_Noeud<Cposition> >::detruit();

  cout << endl;
}

void Parti_comp::li_comp(const Fsequence& G, bool hist)
{  
  _hist.nettoie();
  if (!_pArr){
    cerr << "Pb: pas d'arret defini" << endl;
    return;
  }
  int i,j,t;
  int np;
  int lg = G.taille();
  double eval, max;
  Cposition deb,fin;
  Csegment seg;
  
  Tliste<int> ces;       //cesures ex aequo
  
  S_Noeud_val<Cposition> *predi;	//les anciennes partitions
  S_Noeud_val<Cposition> *neui;	        //les nouvelles partitions

  S_Noeud_val<Cposition> *p_predi, *p_neui;

  S_Noeud_val<Cposition> *interi;

  predi = new S_Noeud_val<Cposition>[lg];

  neui = new S_Noeud_val<Cposition>[lg];

  ///////////// première liste de césures //////////////

  S_Noeud<Cposition> Demar(0);
  p_predi=predi;
  p_neui=neui;
  seg.g_pos_d(1);

  cout << "1\r";
  cout.flush();
  for (j=0;j<lg;j++){
    seg.g_pos_f(j+1);
    eval=seg.calc_val(G);  //// calc val du seg [1,j]

    (p_predi++)->init(j+1, eval,Demar);
    (p_neui++)->init(j+1, 0, Demar);    
  }

  _hist.aj_fin();
  _hist.ind_fin()->recupere(G, 1, predi[lg-1]);

  // ensuite

  if (!(_pArr->arrete(*(_hist.ind_fin())))){
    if (!hist)
      _hist.tue_fin();

    np=2;
    
    while (np<lg){
      cout << np << "\r";
      cout.flush();
      

      for (i=np-1;i<lg;i++){

	seg.g_pos_d(np);
	seg.g_pos_f(i+1);

	max=predi[np-2].val() + seg.calc_val(G);
	ces.aj_fin(np-2);

	for (j=np-1;j<i;j++){

	  seg.g_pos_d(j+2);
	  eval=predi[j].val() + seg.calc_val(G);
	
	  if (max <= eval){
	    if (max==eval)
	      ces.aj_fin(j);
	    else{
	      ces.nettoie();
	      ces.aj_fin(j);
	      max=eval;      
	    }
	  }
	}
	neui[i].modif_dev_su(max, *new S_Noeud<Cposition>(predi[ces.enl_deb()]));
	while ((t=ces.enl_deb()))
	  neui[i].g_su(*new S_Noeud<Cposition>(predi[t]));
      }

      // interversion de la liste de partition ancienne avec la nouvelle

      interi=predi;
      predi=neui;
      neui=interi;
    
      _hist.aj_fin();
      _hist.ind_fin()->recupere(G, np, predi[lg-1]);
      if (_pArr->arrete(*(_hist.ind_fin())))
	break;
      if (!hist)
	_hist.tue_fin();
      np++;
    }
  }

  // nettoyage

  delete[] neui;
  delete[] predi;

  Stock<10000, S_Noeud<Cposition> >::detruit();

  cout << endl;

}

void Parti_comp::li_comp(const Fmatrice& M,
			  bool hist)
{
  _hist.nettoie();
  if (!_pArr){
    cerr << "Pb: pas d'arret defini" << endl;
    return;
  }
  int i,j,t;
  int np;
  int lg = M.taille();
  double eval, max;
  Cposition deb,fin;
  Csegment seg;
  
  Tliste<int> ces;       //cesures ex aequo
  
  S_Noeud_val<Cposition> *predi;	//les anciennes partitions
  S_Noeud_val<Cposition> *neui;	        //les nouvelles partitions

  S_Noeud_val<Cposition> *p_predi, *p_neui;

  S_Noeud_val<Cposition> *interi;

  predi = new S_Noeud_val<Cposition>[lg];

  neui = new S_Noeud_val<Cposition>[lg];

  ///////////// première liste de césures //////////////

  S_Noeud<Cposition> Demar(0);
  p_predi=predi;
  p_neui=neui;
  seg.g_pos_d(1);

  cout << "1\r";
  cout.flush();
  for (j=0;j<lg;j++){
    seg.g_pos_f(j+1);
    eval=seg.calc_val(M);  //// calc val du seg [1,j]

    (p_predi++)->init(j+1, eval,Demar);
    (p_neui++)->init(j+1, 0, Demar);    
  }

  _hist.aj_fin();
  _hist.ind_fin()->recupere(M, 1, predi[lg-1]);
  
  // ensuite

  if (!(_pArr->arrete(*(_hist.ind_fin())))){
    if (!hist)
      _hist.tue_fin();

    np=2;
    while (np<lg){
      cout << np << "\r";
      cout.flush();
	
      for (i=np-1;i<lg;i++){

	seg.g_pos_d(np);
	seg.g_pos_f(i+1);

	max=predi[np-2].val() + seg.calc_val(M);
	ces.aj_fin(np-2);

	for (j=np-1;j<i;j++){

	  seg.g_pos_d(j+2);
	  eval=predi[j].val() + seg.calc_val(M);
	
	  if (max <= eval){
	    if (max==eval)
	      ces.aj_fin(j);
	    else{
	      ces.nettoie();
	      ces.aj_fin(j);
	      max=eval;      
	    }
	  }
	}
	neui[i].modif_dev_su(max, *new S_Noeud<Cposition>(predi[ces.enl_deb()]));
	while ((t=ces.enl_deb()))
	  neui[i].g_su(*new S_Noeud<Cposition>(predi[t]));
      }

      // interversion de la liste de partition ancienne avec la nouvelle

      interi=predi;
      predi=neui;
      neui=interi;
    
      _hist.aj_fin();
      _hist.ind_fin()->recupere(M, np, predi[lg-1]);
      if (_pArr->arrete(*(_hist.ind_fin())))
	break;
      if (!hist)
	_hist.tue_fin();
      np++;
    }
  }

  // nettoyage

  delete[] neui;
  delete[] predi;

  Stock<10000, S_Noeud<Cposition> >::detruit();

  cout << endl;
}


//////////////////////////////////
// lissage avec classes d'equivalence

void Parti_comp::li_comp_cleq(const Fsequence& G, 
			      const Clexique& L, 
			      bool hist)
{
  _hist.nettoie();
  if (!_pArr){
    cerr << "Pb: pas d'arret defini" << endl;
    return;
  }
  int np;
  int lg=G.taille();

  const Cdescripteur *dds, *dd, *df, *di;
	
  S_Noeud_val<Cposi_motif>* predi;	//les anciennes partitions

  S_lex<Cposi_motif >Lc(L);
  //les partitions conditionnelles
	
  S_Noeud_val<Cposi_motif>* neui;       //les nouvelles partitions

  S_Noeud_val<Cposi_motif>* interi, *dp_interi;

  S_Noeud<Cposi_motif>* npar;

  predi = new S_Noeud_val<Cposi_motif>[lg];
  S_Noeud_val<Cposi_motif>* p_predi, *dp_predi;
	
  neui = new S_Noeud_val<Cposi_motif>[lg];
  S_Noeud_val<Cposi_motif>* p_neui, *dp_neui;

  dp_neui=&(neui[lg-1]);
  dp_predi=&(predi[lg-1]);

  /**********************************************/
  // partitions en 1 classe

  di=dd=dds=G.deb();
  df=G.fin();
  p_predi=predi;
  p_neui=neui;
	
  cout << "1\r";
  cout.flush();
  Lc.init(dds, dd, df);
  Lc.Noeud_max(*p_predi);
  _vmax=L.val_max(dds, dd, df);

  while (dd++!=df){
    Lc.plus_egale(dds, dd, df);
    Lc.Noeud_max(*++p_predi);
    _vmax+=L.val_max(dds, dd, df);
  }

  _hist.aj_fin();
  _hist.ind_fin()->recupere(G, 1, *dp_predi, Lc);

  // ensuite

  if (!(_pArr->arrete(*(_hist.ind_fin())))){
    if (!hist)
      _hist.tue_fin();

    np=2;
    npar=0;
    while (dp_predi->val()<_vmax){

      cout << np << "\r";
      cout.flush();

      dd=G.deb();
      p_predi=predi;
      p_neui=neui;
		
      Lc.init(dds, dd, df);
      while (dd!=di){
	Lc.cumule_cleq(dds, ++dd, df, *p_predi++, npar);
	p_neui++;
      }

      *p_neui=*p_predi;

      while (dd!=df){	
	Lc.cumule_cleq(dds, ++dd, df, *p_predi++, npar);
	Lc.Noeud_max(*++p_neui);
      }

      interi=predi;
      predi=neui;
      neui=interi;
      dp_interi=dp_predi;
      dp_predi=dp_neui;
      dp_neui=dp_interi;

      np++;
      di++;

      _hist.aj_fin();
      _hist.ind_fin()->recupere(G, np-1, *dp_predi, Lc);
      if (_pArr->arrete(*(_hist.ind_fin())))
	break;
      if (!hist)
	_hist.tue_fin();
    }
  }

  delete[] neui;
  delete[] predi;
  Lc.nettoie();
  if (npar){
    delete npar;
    npar=0;
  }

  Stock<10000, S_Noeud<Cposi_motif> >::detruit();

  cout << endl;
}

void Parti_comp::li_comp_cleq(const Fmatrice& M, 
			       const Clexique& L, 
			       bool hist)
{
  _hist.nettoie();
  if (!_pArr){
    cerr << "Pb: pas d'arret defini" << endl;
    return;
  }
  int np;
  int lg=M.taille();

  mat_iter mi(M);
  mat_iter mi2(M);
	
  S_Noeud_val<Cposi_motif>* predi;	//les anciennes partitions

  S_lex<Cposi_motif >Lc(L);
  //les partitions conditionnelles
	
  S_Noeud_val<Cposi_motif>* neui;       //les nouvelles partitions

  S_Noeud_val<Cposi_motif>* interi, *dp_interi;

  S_Noeud<Cposi_motif>* npar;

  predi = new S_Noeud_val<Cposi_motif>[lg];
  S_Noeud_val<Cposi_motif>* p_predi, *dp_predi;
	
  neui = new S_Noeud_val<Cposi_motif>[lg];
  S_Noeud_val<Cposi_motif>* p_neui, *dp_neui;

  dp_neui=&(neui[lg-1]);
  dp_predi=&(predi[lg-1]);

  /**********************************************/
  // partitions en 1 classe

  mi.init();
  mi2.init();
  p_predi=predi;
  p_neui=neui;
	
  cout << "1\r";
  cout.flush();
  Lc.init(mi);
  Lc.Noeud_max(*p_predi);
  _vmax=L.val_max(mi);

  while (mi++){
    Lc.plus_egale(mi);
    Lc.Noeud_max(*++p_predi);
    _vmax+=L.val_max(mi);
  }

  _hist.aj_fin();
  _hist.ind_fin()->recupere(M, 1, *dp_predi, Lc);

  // ensuite

  if (!(_pArr->arrete(*(_hist.ind_fin())))){
    if (!hist)
      _hist.tue_fin();

    np=2;
    npar=0;
    while (dp_predi->val()<_vmax){

      cout << np << "\r";
      cout.flush();
      mi.init();
      p_predi=predi;
      p_neui=neui;
		
      Lc.init(mi);
      while (mi!=mi2){
	mi++;
	Lc.cumule_cleq(mi, *p_predi++, npar);
	p_neui++;
      }

      *p_neui=*p_predi;

      while (mi++){	
	Lc.cumule_cleq(mi, *p_predi++, npar);
	Lc.Noeud_max(*++p_neui);
      }

      interi=predi;
      predi=neui;
      neui=interi;
      dp_interi=dp_predi;
      dp_predi=dp_neui;
      dp_neui=dp_interi;

      np++;
      mi2++;

      _hist.aj_fin();
      _hist.ind_fin()->recupere(M, np-1, *dp_predi, Lc);
      if (_pArr->arrete(*(_hist.ind_fin())))
	break;
      if (!hist)
	_hist.tue_fin();
    }
  }


  delete[] neui;
  delete[] predi;
  Lc.nettoie();
  if (npar){
    delete npar;
    npar=0;
  }

  Stock<10000, S_Noeud<Cposi_motif> >::detruit();

}


///////////////////////////////////////////////
/////       sorties textes 
///////////////////////////////////////////////
			


ostream& Parti_comp::operator<<(ostream& s) const
{
  Tliste_iter<Part_equ> iter(_hist);
  Part_equ* x;
  char tmp[30];
  
  while ((x=iter()))
    x->synt(s);
  s << "MAX";
  if (_npmax>0){
    sprintf(tmp,"(%d)",_npmax);
    s << tmp;
  }
  sprintf(tmp," ---> %.12g\n", _vmax);
  s << tmp;
  return(s);
}


/////////////////////////////////////////////////////:
////   modifs

void Parti_comp::moments_stables(bool*& ok, int f) const
{
  Tliste_iter<Part_equ> iter(_hist);
  Part_equ* pseq;
  int nb_s=nb_seq();
  if (f<nb_s)
    nb_s=f+1;
  int nb_d=0;
  int i, j, j1;
  double* diff= new double[nb_s];  //diff[i]= n°(i+1) - n°(i);
  double x,y;
  bool tem;

  iter.init();
  x=iter()->val();
  i=0;
  while ((pseq=iter())){
    y=pseq->val();
    diff[++i]=y-x;
    x=y;
  }
  j1=1;
  x=diff[1];
  y=diff[1];
  ok[0]=1;
  nb_d=1;
  for (i=1;i<nb_s;i++)
    ok[i]=0;
  i=2;
  while (i<nb_s){
    if (diff[i]==y)
      ++i;
    else{
      if (diff[i]<y){
	if (y==x)
	  y=diff[i++];
	else{
	  j=j1;
	  while ((diff[j]==x) && (j<=i))
	    ++j;
	  if (!ok[j-1])
	    ++nb_d;
	  ok[j-1]=1;
	  j1=j;
	  x=diff[j];
	  y=diff[i];
	  ++i;
	}
      }
      else{
	if (diff[i]<=x){
	  j=j1;
	  tem=1;
	  while ((j+i-j1<nb_s) && (j<i)){
	    if (diff[j]==diff[j-j1+i])
	      ++j;
	    else{
	      tem=0;
	      j=j1;
	      while ((diff[j]==x) && (j<=i))
		++j;
	      if (!ok[j-1])
		++nb_d;
	      ok[j-1]=1;
	      j1=i;
	      x=diff[i++];
	      y=diff[i++];
	      break;
	    }
	  }
	  if (tem){
	    i+=j-j1;
	    j1=j;
	  }
	}
	else{
	  j1=i;
	  x=diff[i++];
	  y=diff[i++];
	}
      }
    }
  }
  if (ok[nb_s-1]==0){
    ok[nb_s-1]=1;
    nb_d++;
  }
  delete[] diff;

}
