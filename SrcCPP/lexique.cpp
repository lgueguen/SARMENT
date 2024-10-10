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

#include "lexique.h"

#include "Rmotif_Noeud_val.h"
#include "R_lex.h"
#include "RM_lex.h"

#include "posi_motif.h"
#include "Tliste_iter.h"

#include "sequence.h"
#include "matrice.h"

#include "mat_iter.h"

#include <string>
#include <stdio.h>
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Clexique::Clexique(){
}

Clexique::Clexique(void*){
}

Clexique::Clexique(const Clexique& l)
{
  cerr << "appel inopine de Clexique::Clexique";
  cerr << "(const Clexique& l)" << endl;
  exit(0);
}

Clexique::~Clexique()
{
  nettoie();
}

void Clexique::nettoie()
{
  _vocab.nettoie();
}

///////////////////////////////////////////
//  recuperations


bool Clexique::read_fichrecup(const string& nf, const string& alp)
{
  string lex;
  _vocab.nettoie();
  
  if (!(fich_dans_str(nf, lex)))
    return 0;
  return recup(lex,alp);
}
      
bool Clexique::recup(const string& str, const string& alp, bool n)
{
  
  int i,j;
  Tliste<int> Lnum;
    
  if (!n)
    _vocab.nettoie();
  else
    ls_num(Lnum);
  
  string st;
  
  i=str.find_first_not_of(" \t\n");
  while (i!=string::npos){
    j=str.find_first_of(" \t\n",i);
    st=str.substr(i,j-i);
    cree_motifs(st,alp,Lnum);
//    if (!cree_motifs(st,alp,Lnum)){
//       _vocab.nettoie();
//       return 0;
//     }
    i=str.find_first_not_of(" \t\n",j);
  }
  return 1;
}

bool Clexique::cree_motifs(string& st, const string& alp, Tliste<int>& Lnum)
{
  int i;
  int l=alp.length();
  int et;
  if ((et=st.find('$'))==string::npos){
    if (!cree_motifs(st,Lnum))
      return 0;
  }
  else {
    for (i=0;i<l;i++){
      st[et]=alp[i];
      if (!cree_motifs(st,alp,Lnum))
	return 0;
    }
    st[et]='$';
  }
  return 1;
}

bool Clexique::cree_motifs(const string& st, Tliste<int>& Lnum)
{
  int i=0;
  int l;
  
  if ((l=st.find_first_of(" \t\n"))==string::npos)
    l=st.length();
  
  Cmotif *pm, *pmf, *pmd;

  _vocab.aj_deb();
  pm=_vocab.ind_deb();
  int nbdesc=1;
  
  int max;
  string pref,st2;

  if ((i=st.find(":"))!=string::npos){
    pref=st.substr(0,i);
    i++;
  }
  else
    pref="";

  int k=0;

  Tliste<int> Ld;
  Tliste_iter<int> it(Ld);
  int *pi;

  pmf=pmd=pm;
 
  if (pref!="")
    do {
      Ld.aj_fin(atoi(pref.c_str()));
      k=pref.find(",",k);
      if (k!=-1)
	pref.erase(0,k+1);
    } while (k!=-1);

  it.init();
  if ((pi=it())){
    if (!(Lnum.aj_fin_diff(*pi))){
      cout << "Descriptor number " << *pi << " redefined line\n" << st << endl;
      while (nbdesc--){
        _vocab.tue_deb();
        if (nbdesc)
          Lnum.tue_fin();
      }
      return 1;
    }
    if ((i=pm->construit_de(st,i,*pi))==-1){
      while (nbdesc--){
        _vocab.tue_deb();
        Lnum.tue_fin();
      }
      return 1;
    }

    while (i<l){
      _vocab.aj_deb();
      nbdesc++;
      pm=_vocab.ind_deb();
      if ((i=pm->construit_de(st,i,0))==-1){
        while (nbdesc--){
          _vocab.tue_deb();
          Lnum.tue_fin();
        }
	return 1;
      }
      if ((pi=it())){
	if (!(Lnum.aj_fin_diff(*pi))){
	  cout << "Descriptor number " << *pi << " redefined line\n" << st << endl;
          while (nbdesc--){
            _vocab.tue_deb();
            if (nbdesc)
              Lnum.tue_fin();
          }
	  return 1;
	}
      }
      else {
	cout << "Descriptor missing line " << st << endl;
        while (nbdesc--){
          _vocab.tue_deb();
          Lnum.tue_fin();
        }
	return 1;
      }
      pm->g_num(*pi);
      pmf->g_su(pm);
      pm->g_pr(pmf);
      pmf=pm;
    }
  }
  else {
    max=Lnum.est_vide()?0:*Lnum.max()+1;
    Lnum.aj_fin(max);
    
    if ((i=pm->construit_de(st,0,max))==-1){
      while (nbdesc--){
        Lnum.tue_fin();
        _vocab.tue_deb();
      }
      return 1;
    }
    while (i<l){
      _vocab.aj_deb();
      nbdesc++;
      Lnum.aj_fin(++max);
      pm=_vocab.ind_deb();
      if ((i=pm->construit_de(st,i,0))==-1){
        while (nbdesc--){
          Lnum.tue_fin();
          _vocab.tue_deb();
        }
        return 1;
      }
      pm->g_num(max);
      pmf->g_su(pm);
      pm->g_pr(pmf);
      pmf=pm;
    }
  }
  
  pmf->g_su(pmd);
  pmd->g_pr(pmf);
  
  return 1;
}

void Clexique::init_trans()
{
  Tliste_iter<Cmotif> it(_vocab);
  Tliste<int> Lnum;
  Cmotif* pm;
  
  while ((pm=it()))
    Lnum.aj_fin(pm->num());

  it.init();
  while ((pm=it()))
    pm->cree_prec(Lnum);
}

void Clexique::init_trans_vers(int a)
{  
  Tliste_iter<Cmotif> it(_vocab);
  Tliste<int> Lnum;
  Cmotif* pmb;
  
  Cmotif* pma=desc(a);
  if (pma==0)
    return;
  
  while ((pmb=it()))
    Lnum.aj_fin(pmb->num());

  pma->cree_prec(Lnum);
}

int Clexique::g_trans(int d, int f, double x)
{
  Cmotif* pd;
  
  if ((pd=desc(f))==0)
    return 0;
  else
    if (pd->g_trans_de(d,x)==0)
      return 0;
  return 1;
}

bool Clexique::fich_dans_str(const string& nf, string& lex)
{
  ifstream ar;
  lex="";
  int i;
  string str;

  ar.open(nf.c_str());
  if (!ar){
    cout << "Error opening "<< nf << endl;
    return 0;
  }

  do {
    getline(ar,str);
    i=0;
    while ((i=str.find_first_of(" \t",i+1))!=string::npos){
      if (str[i-1]==' ')
	  str.erase(i--,1);
      else 
	str[i]=' ';
    }

    lex+=' ';
    if ((i=str.find(';'))!=string::npos){
      lex+=str.substr(0,i)+'\0';
      ar.close();
      return 1;
    }
    else 
      lex+=str;
  } while (!ar.eof());

  ar.close();
  return 1;
}


void Clexique::met_au_net()
{
  Tliste_iter<Cmotif> iter(_vocab);
  Cmotif *x, *y;

  Tliste<bool> l_tem;
  Tliste_iter<bool> i_tem(l_tem), i2_tem(l_tem);
	
  bool* p_b; 

  int i,j,l;
  bool tem;
  Clexique lex_bis;
  //Recopie du lexique en enlevant les 
  //	redondances au sein meme des motifs

  while ((x=iter())){
    l=x->lg();
    i=1;
    tem=1;
    while (i<=l/2){
      i++;
      if (*x==*iter())
	{
	  lex_bis.ajoute(x,i-1);
	  for (j=0;j<i-1;j++)
	    l_tem.aj_fin(1);
	  tem=0;
	  break;
	}
    }
    if (tem){
      lex_bis.ajoute(x,l);
      for (j=0;j<l;j++)
	l_tem.aj_fin(1);
    }

    for (j=i;j<l;j++)
      iter();
  }

  //Recopie du lexique bis en enlevant les motifs identiques

  _vocab.nettoie();
  Tliste_iter<Cmotif> iter2(lex_bis._vocab);
  iter.colle(lex_bis._vocab);
  i_tem.colle(l_tem);
  while ((x=iter())){
    p_b=i_tem();
    l=x->lg();
    if (*p_b){
      iter2=iter;
      i2_tem=i_tem;
      tem= 1;
      for (i=1;i<l;i++){
	iter2();
	tem &= *i2_tem();
      }
      if (tem){
	while ((y=iter2())){
	  p_b=i2_tem();
	  if (*p_b){
	    *p_b=(*y!=*x);
	  }
	}
	ajoute(x,l);
      }
    }
    for (i=1;i<l;i++){
      iter();
      i_tem();
    }
  }
}

void Clexique::ajoute(Cmotif* m, int lg)
{ 
  int i;
  Cmotif *dern, *suiv, *ici;
  Cmotif *mi;

  mi=m;
  for (i=0;i<lg-1;i++)
    mi=mi->su();

  _vocab.aj_fin(*mi);

  ici=dern=_vocab.ind_fin();

  for (i=1;i<lg;i++){
    suiv=ici;
    mi=mi->pr();
    _vocab.aj_fin(*mi);
    ici=_vocab.ind_fin();
    suiv->g_pr(ici);
    ici->g_su(suiv);
  }
  ici->g_pr(dern);
  dern->g_su(ici);
}


void Clexique::enleve(int i)
{
  Cmotif* pm=desc(i);
  if (pm==0)
    return;

  pm->pr()->g_su(pm->su());
  pm->su()->g_pr(pm->pr());
  

  _vocab.tue(pm);
  return;
}
      

//////////////////////////////////////////////////////
//  donnees
/////////////////////////////////////////////////////

Cmotif* Clexique::desc(int i) const
{
  Tliste_iter<Cmotif> it(_vocab);
  Cmotif* pm;
  while ((pm=it()))
    if (pm->num()==i)
      return pm;
  return 0;
}

int Clexique::num(const Cmotif* const pm) const
{
  return pm->num();
}

void Clexique::ls_num(Tliste<int>& l) const
{
  Tliste_iter<Cmotif> iter(_vocab);
  Cmotif *x;

  l.nettoie();

  while ((x=iter()))
    l.aj_fin(x->num());
}

void Clexique::liste_desc(Tliste<Cdesc_comp>& li) const{
  Tliste_iter<Cmotif> lit(_vocab);
  Cmotif* pm;
  while ((pm=lit()))
    pm->liste_desc(li);
}

bool Clexique::est_vide() const
{
  return _vocab.est_vide();
}

//////////////////////////////////////////////////////////////////////
// Modifications

void Clexique::ajoute(const Cdesc_comp& D)
{ 
  Cmotif* m;
  _vocab.aj_fin();
  (*m).Cdesc_comp::operator=(D);
  m->g_pr(m);
  m->g_su(m);
}

void Clexique::ajoute(const Cdescripteur& d)
{
  Cmotif* m;
  _vocab.aj_fin();
  m=_vocab.ind_fin();
  (*m).Cdesc_comp::operator=(d);
  m->g_pr(m);
  m->g_su(m);
}

void Clexique::enl_fin()
{ 
  Cmotif *prem;

  if (_vocab.ind_fin()){
    prem=_vocab.ind_fin();
    prem=prem->pr();
    while (_vocab.ind_fin()!=prem)
      _vocab.tue_fin();
    _vocab.tue_fin();
  }
}



/////////////////////////////////////////
/////////////////////////////////////////
//   CALCULS
/////////////////////////////////////////
/////////////////////////////////////////

/////////////////////////////////////////
// VALMAX

double Clexique::val_max(const Cdescripteur* const pd,
			 const Cdescripteur* const dd,
			 const Cdescripteur* const df) const
{
  Tliste_iter<Cmotif> iter(_vocab);
  Cmotif *x;
  double v,vm=0;

  if ((x=iter()))
    vm=x->multiplie(pd, dd, df);
  while ((x=iter())){
    v=x->multiplie(pd, dd, df);
    if (v>vm)
      vm=v;
  }

  return vm;
}

double Clexique::val_max(const mat_iter& m_i) const
{
  Tliste_iter<Cmotif> iter(_vocab);
  Cmotif *x;
  double v,vm=0;

  if ((x=iter()))
    vm=x->multiplie(m_i);
  while ((x=iter())){
    v=x->multiplie(m_i);
    if (v>vm)
      vm=v;
  }

  return vm;
}

double Clexique::val_max(const Fsequence& G, int d, int f) const
{
  if (d>=G.taille())
    return 0;
  if (d<0)
    d=0;
  if (f<=0)
    return 0;
  if (f>G.taille())
    f=G.taille();

  const Cdescripteur *dds, *dd, *df, *dfs;
  double x=0;

  dds=G.deb();
  dd=dds+d;
  df=dds+f;
  dfs=G.fin();
  do {
    x+=val_max(dds,dd, dfs);
  } while (++dd<df);

  return x;
}

double Clexique::val_max(const Fmatrice& M, int d, int f) const
{
  if (d>=M.taille())
    return 0;
  if (d<0)
    d=0;
  if (f<=0)
    return 0;
  if (f>M.taille())
    f=M.taille();

  mat_iter m_i, m_i2;
  double x=0;

  m_i.colle(M);
  m_i2.colle(M);
  m_i.init(d);
  m_i2.init(f-1);

  do {
    x+=val_max(m_i);
  } while (m_i(m_i2));

  return x;
}

//////////////////////////////////////////
//  NBE MAX PART

int Clexique::nbe_max_part(const Fsequence& G) const 
{
  R_lex<Cposition> Rl(*this);
  int nbc=0;
  double m,x;
  
  const Cdescripteur *dd, *da, *df;

  dd=G.deb();
  da=dd;
  df=G.fin();
  Rl.init(dd,da,df);
  m=val_max(dd,da, df);
  while (da++<df) {
    x=val_max(dd,da, df);
    Rl.plus_egale(dd,da,df);
    m+=x;
    if (m!=Rl.val_max()){
      Rl.init(dd,da,df);
      m=x;
      nbc++;
    }
  }
  nbc++;
  return nbc;
}

int Clexique::nbe_max_part(const Fmatrice& M) const
{
  R_lex<Cposition> Rl(*this);
  int nbc=0;
  double m,x;

  mat_iter m_i;

  m_i.colle(M);
  Rl.init(m_i);
  m=val_max(m_i);
  while (m_i++) {
    x=val_max(m_i);
    Rl.plus_egale(m_i);
    m+=x;
    if (m!=Rl.val_max()){
      Rl.init(m_i);
      m=x;
      nbc++;
    }
  }
  nbc++;
  return nbc;
}


/////////////////////////////////////////
// VALMIN

double Clexique::val_min(const Cdescripteur* const pd,
			 const Cdescripteur* const dd,
			 const Cdescripteur* const df) const
{
  Tliste_iter<Cmotif> iter(_vocab);
  Cmotif *x;
  double v,vm=0;

  if ((x=iter()))
    vm=x->multiplie(pd, dd, df);
  while ((x=iter())){
    v=x->multiplie(pd, dd, df);
    if (v<vm)
      vm=v;
  }

  return vm;
}

double Clexique::val_min(const mat_iter& m_i) const
{
  Tliste_iter<Cmotif> iter(_vocab);
  Cmotif *x;
  double v,vm=0;

  if ((x=iter()))
    vm=x->multiplie(m_i);
  while ((x=iter())){
    v=x->multiplie(m_i);
    if (v<vm)
      vm=v;
  }

  return vm;
}

/////////////////////////////////////
/// PREDICTION 

double Clexique::pred(const Fsequence& G, int d, int f) const
{
  if (d>G.taille())
    return 0;
  if (d<=0)
    d=1;
  if ((f<0) || (f>G.taille()))
    f=G.taille();
  if (f<d)
    return 0;
 
  R_lex<Cposi_motif > Lc(*this);
  const Cdescripteur *dds, *dd, *df, *dfs;

  dds=G.deb();
  dd=dds+d-1;
  df=dds+f-1;
  dfs=G.fin();
  Lc.init(dds, dd, dfs);
  while (dd++<df)
    Lc.plus_egale(dds, dd, dfs);

  return Lc.val_max();

}

double Clexique::pred(const Fmatrice& M, int d, int f) const
{
  if (d>M.taille())
    return 0;
  if (d<=0)
    d=1;
  if ((f<0) || (f>M.taille()))
    f=M.taille();
  if (f<d)
    return 0;

  R_lex<Cposi_motif > Lc(*this);
  mat_iter m_i, m_i2;

  m_i.colle(M);
  m_i2.colle(M);
  m_i.init(d-1);
  m_i2.init(f-1);

  Lc.init(m_i);
  if (m_i(m_i2))
    do {
      Lc.plus_egale(m_i);
    } while (m_i(m_i2));
  
  return Lc.val_max();
}


/************************************************************/
/*   Log vraisemblance */

Tliste<double> Clexique::log_vraisemblance(const Fsequence& S, int max) const
{
  Tliste<double> ret;
  int lS=S.taille();

  if (max>lS)
    max=lS;
  
  int lL=nb_desc();
    
  double mk;

  int i,j,k;
    
  if (lL<=1){
    mk=(lL==1)?pred(S):0;
    
    for (k=0;k<=max;k++)
      ret.aj_fin(mk);
    return ret;
  }
  
  // matrices des valeurs et tableau des sommes des valeurs

  Cmatrice Tpi, Tm;
  Tpi.predictions(S,*this);

  double *mx=new double[lS];
  
  double *tmp1, *tmp2, *tmpp;
  tmp1=new double[lL];
  tmp2=new double[lL];
  
  double vmx;
  double llL=log(double(lL));
    
  // demarrage
  
  Tm.integre(Tpi);
  mat_iter mi(Tm);
  
  i=0;
  do 
    mx[i++]=mi.log_sum_exp();
  while (mi());
  ret.aj_fin(mx[lS-1]-llL);
  
  cout << "1\r";
  cout.flush();

  
  //ensuite
  
  for (k=2;k<=max;k++){
    cout << k << "\r";
    cout.flush();

    for (j=0;j<lL;j++)
      tmp1[j]=Tm(j,k-2);

    for (i=k-1;i<lS;i++){
      for (j=0;j<lL;j++)
        tmp2[j]=Tm(j,i);

      for (j=0;j<lL;j++){
        vmx=Tm(j,i-1);
        if (vmx<mx[i-1])
          vmx=mx[i-1];
        if (vmx<tmp1[j])
          vmx=tmp1[j];
        mk=(i-k+1)*exp(Tm(j,i-1)-vmx)/i+(k-1)*(exp(mx[i-1]-vmx)-exp(tmp1[j]-vmx))/(i*(lL-1));
        Tm.g_val(Tpi(j,i)+vmx+log(mk),j,i);
      }
      tmpp=tmp1;
      tmp1=tmp2;
      tmp2=tmpp;
    }

    mi.init(k-1);
    i=k-1;
    do 
      mx[i++]=mi.log_sum_exp();
    while(mi());

    ret.aj_fin(mx[lS-1]-llL);
  }

  delete mx;
  delete tmp1;
  delete tmp2;
  return ret;
}


Tliste<double> Clexique::log_vraisemblance(const Fmatrice& S, int max) const
{
  Tliste<double> ret;
  int lS=S.taille();

  if (max>lS)
    max=lS;
  
  int lL=nb_desc();
    
  double mk;

  int i,j,k;
    
  if (lL<=1){
    mk=(lL==1)?pred(S):0;
    
    for (k=0;k<=max;k++)
      ret.aj_fin(mk);
    return ret;
  }
  
  // matrices des valeurs et tableau des sommes des valeurs

  Cmatrice Tpi, Tm;
  Tpi.predictions(S,*this);

  double *mx=new double[lS];
  double *tmp1, *tmp2, *tmpp;
  tmp1=new double[lL];
  tmp2=new double[lL];
  
  double vmx;
  double llL=log(double(lL));
    
  // demarrage

  Tm.integre(Tpi);
  mat_iter mi(Tm);

  i=0;
  do 
    mx[i++]=mi.log_sum_exp();
  while (mi());
  ret.aj_fin(mx[lS-1]-llL);
  
  cout << "1\r";
  cout.flush();

  //ensuite
  
  for (k=2;k<=max;k++){
    cout << k << "\r";
    cout.flush();

    for (j=0;j<lL;j++)
      tmp1[j]=Tm(j,k-2);

    for (i=k-1;i<lS;i++){
      for (j=0;j<lL;j++)
        tmp2[j]=Tm(j,i);

      for (j=0;j<lL;j++){
        vmx=Tm(j,i-1);
        if (vmx<mx[i-1])
          vmx=mx[i-1];
        if (vmx<tmp1[j])
          vmx=tmp1[j];
        mk=(i-k+1)*exp(Tm(j,i-1)-vmx)/i+(k-1)*(exp(mx[i-1]-vmx)-exp(tmp1[j]-vmx))/(i*(lL-1));
        Tm.g_val(Tpi(j,i)+vmx+log(mk),j,i);
      }
      tmpp=tmp1;
      tmp1=tmp2;
      tmp2=tmpp;
    }

    mi.init(k-1);
    i=k-1;
    do 
      mx[i++]=mi.log_sum_exp();
    while(mi());

    ret.aj_fin(mx[lS-1]-llL);
  }

  delete mx;
  delete tmp1;
  delete tmp2;
  return ret;
}


/************************************************************/
/************************************************************/
/* probabilites a posteriori */

Tliste<double> Clexique::log_a_posteriori(const Fsequence& S, int max) const
{
  Tliste<double> ret;
  int lS=S.taille();

  if (max>lS)
    max=lS;
  
  int lL=nb_desc();
    
  double mk;

  int i,j,k;
    
  if (lL<=1){
    mk=(lL==1)?pred(S):0;
    
    for (k=0;k<=max;k++)
      ret.aj_fin(mk);
    return ret;
  }
  
  // matrices des valeurs log_normalisees et tableau des sommes des valeurs

  Cmatrice Tpi, Tm;
  Tpi.predictions(S,*this);
  Tpi.log_normalise();
  
  double *sump=new double[lS];
  
  double *tmp1, *tmp2, *tmpp;
  tmp1=new double[lL];
  tmp2=new double[lL];
  
  double mxp;
    
  // demarrage

  Tm.integre(Tpi);
  mat_iter mi(Tm);

  i=0;
  do 
    sump[i++]=mi.log_sum_exp();
  while (mi());
  ret.aj_fin(sump[lS-1]);
  
  cout << "1\r";
  cout.flush();

  //ensuite
  
  for (k=2;k<=max;k++){
    cout << k << "\r";
    cout.flush();

    for (j=0;j<lL;j++)
      tmp1[j]=Tm(j,k-2);

    for (j=0;j<lL;j++)
      tmp2[j]=Tm(j,k-1);

    for (j=0;j<lL;j++){
      mxp=sump[k-2];
      if (mxp<tmp1[j])
        mxp=tmp1[j];
      mk=exp(sump[k-2]-mxp)-exp(tmp1[j]-mxp);
      Tm.g_val(Tpi(j,k-1)+mxp+log(mk),j,k-1);
    }
    
    tmpp=tmp1;
    tmp1=tmp2;
    tmp2=tmpp;
    
    for (i=k;i<lS;i++){
      for (j=0;j<lL;j++)
        tmp2[j]=Tm(j,i);

      for (j=0;j<lL;j++){
        mxp=Tm(j,i-1);
        if (mxp<sump[i-1])
          mxp=sump[i-1];
        if (mxp<tmp1[j])
          mxp=tmp1[j];
        mk=exp(Tm(j,i-1)-mxp)+exp(sump[i-1]-mxp)-exp(tmp1[j]-mxp);
        Tm.g_val(Tpi(j,i)+mxp+log(mk),j,i);
      }
      tmpp=tmp1;
      tmp1=tmp2;
      tmp2=tmpp;
    }

    mi.init(k-1);
    i=k-1;
    do 
      sump[i++]=mi.log_sum_exp();
    while(mi());

    ret.aj_fin(sump[lS-1]);
    
  }

  delete sump;
  delete tmp1;
  delete tmp2;
  return ret;
}


Tliste<double> Clexique::log_a_posteriori(const Fmatrice& S, int max) const
{
  Tliste<double> ret;
  int lS=S.taille();

  if (max>lS)
    max=lS;
  
  int lL=nb_desc();
    
  double mk;

  int i,j,k;
    
  if (lL<=1){
    mk=(lL==1)?pred(S):0;
    
    for (k=0;k<=max;k++)
      ret.aj_fin(mk);
    return ret;
  }
  
  // matrices des valeurs log_normalisees et tableau des sommes des valeurs

  Cmatrice Tpi, Tm;
  Tpi.predictions(S,*this);
  Tpi.log_normalise();

  double *sump=new double[lS];
  
  double *tmp1, *tmp2, *tmpp;
  tmp1=new double[lL];
  tmp2=new double[lL];
  
  double mxp;
    
  // demarrage

  Tm.integre(Tpi);
  mat_iter mi(Tm);

  i=0;
  do 
    sump[i++]=mi.log_sum_exp();
  while (mi());

  ret.aj_fin(sump[lS-1]);
  
  cout << "1\r";
  cout.flush();

  //ensuite
  
  for (k=2;k<=max;k++){
    cout << k << "\r";
    cout.flush();

    for (j=0;j<lL;j++)
      tmp1[j]=Tm(j,k-2);

    for (j=0;j<lL;j++)
      tmp2[j]=Tm(j,k-1);

    for (j=0;j<lL;j++){
      mxp=sump[k-2];
      if (mxp<tmp1[j])
        mxp=tmp1[j];
      mk=exp(sump[k-2]-mxp)-exp(tmp1[j]-mxp);
      Tm.g_val(Tpi(j,k-1)+mxp+log(mk),j,k-1);
    }
    
    tmpp=tmp1;
    tmp1=tmp2;
    tmp2=tmpp;

    for (i=k;i<lS;i++){
      for (j=0;j<lL;j++)
        tmp2[j]=Tm(j,i);

      for (j=0;j<lL;j++){
        mxp=Tm(j,i-1);
        if (mxp<sump[i-1])
          mxp=sump[i-1];
        if (mxp<tmp1[j])
          mxp=tmp1[j];
        mk=exp(Tm(j,i-1)-mxp)+exp(sump[i-1]-mxp)-exp(tmp1[j]-mxp);
        Tm.g_val(Tpi(j,i)+mxp+log(mk),j,i);
      }
      tmpp=tmp1;
      tmp1=tmp2;
      tmp2=tmpp;
    }

    mi.init(k-1);
    i=k-1;
    do 
      sump[i++]=mi.log_sum_exp();
    while(mi());

    ret.aj_fin(sump[lS-1]);
    
  }

  delete sump;
  delete tmp1;
  delete tmp2;
  return ret;
}

/************************************************************/
/* log-vraisemblance sur des sites independants */

double Clexique::llh(const Fsequence& G, int d, int f) const
{
  if (d>G.taille())
    return 0;
  if (d<=0)
    d=1;
  if ((f<0) || (f>G.taille()))
    f=G.taille();
  if (f<d)
    return 0;
  
  RM_lex<Cposition> RL(*this);

  const Cdescripteur *dds, *dd, *df, *dfs;

  dds=G.deb();
  dd=dds+d-1;
  df=dds+f-1;
  dfs=G.fin();

  RL.init(dds,dd,dfs);
  RL.avance();

  while (dd++<df){
    RL.cumule(dds,dd,dfs);
    RL.avance();
  }

  return RL.Noeud_max()->val();
}



///////////////////////////////////
//  AUTRES

int Clexique::max_lg_mot() const
{
  Tliste_iter<Cmotif> iter(_vocab);
  Cmotif *x;
  int m,i;
  m=0;

  while ((x=iter())){
    i=x->taille();
    if (i>m)
      m=i;
  }
  return m;
}

int Clexique::max_lg_motif() const
{
  Tliste_iter<Cmotif> iter(_vocab);
  Cmotif *x;
  int m,i;
  m=0;

  while ((x=iter())){
    i=x->lg();
    if (i>m)
      m=i;
  }
  return m;
}

int Clexique::nb_desc() const
{
  return _vocab.nb_ind();
}

/////////////////////////////////////////
//   sorties
/////////////////////////////////////////

ostream& Clexique::operator<<(ostream& s) const
{
  Tliste_iter<Cmotif> iter(_vocab);
  Cmotif *x;
  int l;

  while ((x=iter())){
    x->ecrit_num(s);
    s<<":";
    x->operator<<(s);
    l=x->lg();
    while (--l>0)
      iter();
    s << '\t';
  }

  s << endl;
  iter.init();
  if ((x=iter())){
    x->tab(s);
    while ((x=iter()))
      x->tab(s);
    s << endl;
  }
  
  return s;
}

void Clexique::ls_num(ostream& s) const
{
  Tliste_iter<Cmotif> iter(_vocab);
  Cmotif *x;
  char tmp[9];

  s << '[';
  
  if ((x=iter())){
    snprintf(tmp,30,"%d",x->num());
    s << tmp;
  }
  
  while ((x=iter())){
    s << ',';
    snprintf(tmp,30,"%d",x->num());
    s << tmp;
  }

  s << ']';
}

void Clexique::ecrit_evalue(const Fsequence& G, ostream& s, int d, int f) const
{
  if (d>G.taille())
    return;
  if (d<=0)
    d=1;
  if ((f<0) || (f>G.taille()))
    f=G.taille();
  if (f<d)
    return;

  R_lex<Cposi_motif > Lc(*this);

  const Cdescripteur *dds, *dd, *df, *dfs;

  dds=G.deb();
  dd=dds+d-1;
  df=dds+f-1;
  dfs=G.fin();


  Lc.init(dds, dd, dfs);


  while (dd++<df)
    Lc.plus_egale(dds, dd, dfs);


  //////// ecriture

  Rmotif_Noeud_val<Cposi_motif>* pm;
  Tliste_iter<Rmotif_Noeud_val<Cposi_motif> > it_m(Lc._vocces);
  Tliste_iter<R_Noeud<Cposi_motif> > it_d(Lc._demlex);
  R_Noeud<Cposi_motif>* pd;

  s << '{';
  char  c[40];

  it_m.init();
  if ((pm=it_m())){
    pd=it_d();
    s << "(";
    pd->ecrit_num(s);
    s << "):";
    snprintf(c,40,"%.12g",pm->val());
    s << c;
  }
  
  while ((pm=it_m())){
    pd=it_d();
    s << ",(";
    pd->ecrit_num(s);
    s << "):";
    snprintf(c,40,"%.12g",pm->val());
    s << c ;
  }

  s << "}";

}
  
void Clexique::ecrit_evalue(const Fmatrice& G, ostream& s, int d, int f) const
{
  if (d>G.taille())
    return;
  if (d<=0)
    d=1;
  if ((f<0) || (f>G.taille()))
    f=G.taille();
  if (f<d)
    return;

  R_lex<Cposi_motif > Lc(*this);

  mat_iter ms, me;

  ms.colle(G);
  ms.init(d-1);
  me.colle(G);
  me.init(f-1);


  Lc.init(ms);
  if (ms(me))
    do {
      Lc.plus_egale(ms);
    } while(ms(me));
  
  //////// ecriture

  Rmotif_Noeud_val<Cposi_motif>* pm;
  Tliste_iter<Rmotif_Noeud_val<Cposi_motif> > it_m(Lc._vocces);
  Tliste_iter<R_Noeud<Cposi_motif> > it_d(Lc._demlex);
  R_Noeud<Cposi_motif>* pd;

  s << '{';
  char  c[40];

  it_m.init();
  if ((pm=it_m())){
    pd=it_d();
    s << "(";
    pd->ecrit_num(s);
    s << "):";
    snprintf(c,40,"%.12g",pm->val());
    s << c;
  }
  
  while ((pm=it_m())){
    pd=it_d();
    s << ",(";
    pd->ecrit_num(s);
    s << "):";
    snprintf(c,40,"%.12g",pm->val());
    s << c;
  }

  s << "}";

}


