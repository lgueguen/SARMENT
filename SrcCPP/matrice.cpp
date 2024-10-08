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

#include "matrice.h"
#include "sequence.h"
 
#include "mat_iter.h"
#include "partition.h"

#include "lexique.h"
#include "RM_lex.h"
#include "R_lex.h"
#include "posi_motif.h"

#include <ctype.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Cmatrice::Cmatrice(const Fmatrice& M) : Fmatrice(M), _rfin(M._fin)
{
  int i;
  if (M._nvct){
    _nvct=M._nvct;
    _nval=M._nval;
    
    _seq = new vector<double>[_nvct];
    for (i=0;i<_nvct;i++)
      _seq[i].construit_a_vide(_nval);
    
    _rfin=_fin=&_seq[_nvct-1];
    
    _l_desc.recup_prem(M._l_desc,_nval);
    for (i=0;i<_nvct;i++)
      _seq[i].recup_prem(M._seq[i],_nval);
  }
  else
    _seq=_rfin=_fin=0;
}

void Cmatrice::imite(const Fmatrice& M)
{
  if (!(M._nvct && M._nval)){
    nettoie();
    return;
  }
  
  if ((_nval!=M._nval) || (_nvct!=M._nvct))
    nettoie();

  int i,j;
  if (M._nvct){
    _nvct=M._nvct;
    _nval=M._nval;

    _l_desc.recup_prem(M._l_desc,_nval);
    _seq = new vector<double>[_nvct];
    for (i=0;i<_nvct;i++){
      _seq[i].construit_a_vide(_nval);
      for (j=0;j<_nval;j++)
        _seq[i][j]=0;
    }
    _rfin=_fin=&_seq[_nvct-1];
  }
}

void Cmatrice::genere(const int i, const string& s)
{
  nettoie();
  if (i<=0)
    return;

  int l=s.length();
  int k,j=-1;

  //pour eviter les chaines avec doublons 
  while (++j<l)
    if (s.find(s[j],j+1)!=string::npos)
      return;

  _nval=l;
  _nvct=i;
  _seq= new vector<double>[_nvct];
  for (j=0;j<_nvct;j++){
    _seq[j].construit_a_vide(_nval);
    for (k=0;k<_nval;k++)
      _seq[j][k]=0;
  }
  _rfin=_fin=&_seq[_nvct-1];
  
  _l_desc.construit_a_vide(_nval);
  for (j=0;j<_nval;j++)
    _l_desc[j]=s[j];
}

void Cmatrice::genere(const int i, const char* s, int l)
{
  nettoie();
  if (i<=0)
    return;

  //pour eviter les chaines avec doublons 
  int k,j=-1;
  while (++j<l){
    k=j;
    while (++k<l)
      if (s[k]==s[j])
        return;
  }

  _nval=l;
  _nvct=i;
  _seq= new vector<double>[_nvct];
  for (j=0;j<_nvct;j++){
    _seq[j].construit_a_vide(_nval);
    for (k=0;k<_nval;k++)
      _seq[j][k]=0;
  }
  
  _rfin=_fin=&_seq[_nvct-1];
  
  _l_desc.construit_a_vide(_nval);
  for (j=0;j<_nval;j++)
    _l_desc[j]=s[j];
}

void Cmatrice::copie(const Fmatrice& M)
{
  if (!(M._nvct && M._nval)){
    nettoie();
    return;
  }

  if ((_nval!=M._nval) || (_nvct!=M._nvct))
    nettoie();

  int i;
  if (M._nvct){
    _nvct=M._nvct;
    _nval=M._nval;

    _l_desc.recup_prem(M._l_desc,_nval);
    _seq = new vector<double>[_nvct];
    for (i=0;i<_nvct;i++)
      _seq[i].recup_prem(M._seq[i],_nval);

    _rfin=_fin=&_seq[_nvct-1];
  }
}

bool Cmatrice::recup_mat(const std::string& ns)
{
  int i,j,k;
  int deb;

  if (ns[0]=='\0')
    return 0;

  ifstream flot(ns.c_str());
  if (!flot){
    cerr << "Pb opening " << ns << endl;
    return 0;
  }

  nettoie();

  //////////// récup de la longueur des séquences de param 

  string str;

  do {
    getline(flot,str);
  } while (!flot.eof() && !isdigit(str[0]) && (str[0]!='('));

  if (flot.eof()){
      cerr << "Bad header: ";
      cerr << ns << endl;
      flot.close();
      return 0;
  }
    
  deb=(str[0]=='(')?atoi((str.substr(1)).c_str()):atoi(str.c_str());

  if (deb<=0){
    cerr << "Bad start in file ";
    cerr << ns << endl;
    flot.close();
    return 0;
  }

  i=str.find_first_of(" \t");
  i=str.find_first_not_of(" \t",i);

  if (i!=string::npos){
    k=atoi((str.substr(i)).c_str());
    if ((_nvct=1+k-deb)<=0){
      cerr << "Bad boundaries in file ";
      cerr << ns << endl;
      flot.close();
      return 0;
    }
  }
  else{
    _nvct=deb;
    deb=1;
  }

  ///////////// teste l'état des info du fichier ////////////////
  ///////////////// récuperation des données ////////////////////

  do{
    getline(flot,str);
  } while (!flot.eof() && 
	   (str.find_first_not_of(" \t")==string::npos));

  if (flot.eof()){
    cerr << "Problem in file "<< ns << endl;
    flot.close();
    return 0;
  }

  _nval=0;
  i=0;
  bool a=1;
  while (i<str.length()){
    if (!isspace(str[i++])){
      if (a)
        _nval++;
      a=0;
    }
    else 
      a=1;
  }

  if (_nval<=0){
    cerr << "Characters missing " << endl;
    flot.close();
    return 0;
  }
    
  _l_desc.construit_a_vide(_nval);
  i=0;k=0;
  while (i<str.length()){
    if (!isspace(str[i])){
      if (k==_nval){
	cerr << "Error: too many characters " << endl;
        _nval=0;
	_l_desc.construit_a_vide(0);
	flot.close();
	return 0;
      }
      if (isalpha(str[i]))
        _l_desc[k++]=str[i];
      else if (str[i]=='#'){
        j=atoi((str.substr(i+1)).c_str());
        if ((j<0) || (j>=256)){
          cerr << "Error: out of range character " << j << endl;
          _nval=0;
          _l_desc.construit_a_vide(0);
          flot.close();
          return 0;
        }          
        _l_desc[k++]=(char)j;
        i=str.find_first_of(" \t\n\r",i)-1;
      }
      else {
        cerr << "Problem in characters declaration " << endl;
        _nval=0;
	_l_desc.construit_a_vide(0);
	flot.close();
	return 0;
      }
    }
    i++;    
  }
  
  _seq = new Cmatrice::vector<double>[_nvct];
  for (i=0;i<_nvct;i++)
    _seq[i].construit_a_vide(_nval);

  _rfin=_seq+(_nvct-1);  

  i=deb-1;
  while ((!flot.eof()) && (i>0)){
    getline(flot,str);
    if (str.find_first_not_of(" \t")!=string::npos)
      i--;
  }
   
  if (flot.eof()){
    cerr << "File " << ns << " returns empty tabular." << endl;
    nettoie();
    flot.close();
    return 0;
  }

  i=0;
  while (!flot.eof() && (i<_nvct)){
    getline(flot,str);
    if (str.find_first_not_of(" \t")!=string::npos){
      k=0;
      j=0;
      while (k<_nval){
	_seq[i][k++]=atof((str.substr(j)).c_str());
	j=str.find_first_of(" \t",j);
	if ((
	     (j=str.find_first_not_of(" \t",j))==string::npos)
	    && (k<_nval)){
	  cerr << "Error: line " << i+deb << " is too short ";
	  cerr << "in file " << ns << endl;
	  nettoie();
	  flot.close();
	}
      }
      i++;
    }
  }
  
  if (i<_nvct){
    cout << "Too short Matrice..." << endl;
    _nvct=i;
  }
  _fin=_seq+(_nvct-1);
  
  flot.close();
  return 1;
}



////////////////////////////////////////////////////////////
////// Remplissage par calculs
void Cmatrice::FB(const Fsequence& G, const Clexique& L)
{
  if (G.deb()==0)
    return;
  
  int ta=G.taille();
  int i,k;
  int ndesc;
  
  Tliste<int> ldesc;
  L.ls_num(ldesc);
  
  Tliste_iter<int> iti(ldesc);
  int* pi;
  
  if (!(ndesc=ldesc.taille()))
    return;

  // allocation de la Cmatrice
  
  _nval=ndesc;
  _nvct=ta;

  if (_seq)
    delete[] _seq;
  _seq = new vector<double>[_nvct];
  for (i=0;i<_nvct;i++){
    _seq[i].construit_a_vide(_nval);
    for (k=0;k<_nval;k++)
      _seq[i][k]=0;
  }

  _rfin=_fin=_seq+_nvct-1;
  _l_desc.construit_a_vide(_nval);
  
  i=0;
  iti.init();
  while ((pi=iti()))
    _l_desc[i++]=int(*pi);
  
  // remplissage par FB

  RM_lex<Cposition> RL(L);

  const Cdescripteur *di, *dd, *df;
  di=G.deb();
  dd=df=G.fin();

  ///////////////////////////
  // Remplit le tableau du backward

  int lg=G.taille();
  int l=lg;
  
  Tliste<double>* lval=new Tliste<double>[lg+1];
  Tliste_iter<double> itd1, itd2;
  
  RL.init();
  RL.aj_val(lval[l--]);
  
  do {
    RL.backward(di,dd,df);
    RL.aj_val(lval[l--]);
  } while(--dd>di);

  
  /////////////////////////////
  // Realise le forward

  dd=di;
  l=0;
  
  RL.init(di,dd,df);
  RL.aj_val(lval[l]); 
  double m,x;
  
  while (dd++<df){
    itd1.colle(lval[l]);
    itd2.colle(lval[l+1]);
    m=*(itd1++)+*(itd2++);
    g_val(m,0,l);
    for (i=1;i<_nval;i++){
      g_val(x=*(itd1++)+*(itd2++),i,l);
      if (m<x)
	m=x;
    }
    x=0;
    for (i=0;i<_nval;i++)
      x+=exp(_seq[l][i]-m);
    x=log(x)+m;
    for (i=0;i<_nval;i++)
      _seq[l][i]-=x;
    
    RL.forward(di,dd,df);
    RL.maj_val(lval[++l]);
  }

  itd1.colle(lval[l]);
  itd2.colle(lval[l+1]);
  m=*(itd1++)+*(itd2++);
  g_val(m,0,l);
  for (i=1;i<_nval;i++){
    g_val(x=*(itd1++)+*(itd2++),i,l);
    if (m<x)
      m=x;
  }
  x=0;
  for (i=0;i<_nval;i++)
    x+=exp(_seq[l][i]-m);
  x=log(x)+m;
  for (i=0;i<_nval;i++)
    _seq[l][i]-=x;

  delete[] lval;
}

void Cmatrice::FB(const Fmatrice& G, const Clexique& L)
{
  if (G.taille()==0)
    return;
  
  int ta=G.taille();
  int i,k;
  int ndesc;
  
  Tliste<int> ldesc;
  L.ls_num(ldesc);
  
  Tliste_iter<int> iti(ldesc);
  int* pi;
  
  if (!(ndesc=ldesc.taille()))
    return;

  // allocation de la Cmatrice
  
  _nval=ndesc;
  _nvct=ta;

  if (_seq)
    delete[] _seq;
  _seq = new vector<double>[_nvct];
  for (i=0;i<_nvct;i++){
    _seq[i].construit_a_vide(_nval);
    for (k=0;k<_nval;k++)
      _seq[i][k]=0;
  }

  _rfin=_fin=_seq+_nvct-1;
  _l_desc.construit_a_vide(_nval);
  
  i=0;
  iti.init();
  while ((pi=iti()))
    _l_desc[i++]=int(*pi);
  
  // remplissage par FB

  RM_lex<Cposition> RL(L);

  mat_iter mi(G);

  ///////////////////////////
  // Remplit le tableau du backward

  int lg=G.taille();
  int l=lg;
  
  Tliste<double>* lval=new Tliste<double>[lg+1];
  Tliste_iter<double> itd1, itd2;
  
  RL.init();
  RL.aj_val(lval[l--]);

  mi.finit();
  
  do {
    RL.backward(mi);
    RL.aj_val(lval[l--]);
    mi--;
  } while(l>0);

  
  /////////////////////////////
  // Realise le forward

  mi.init();
  l=0;
  
  RL.init(mi);
  RL.aj_val(lval[l]); 
  double m,x;
  
  while (mi++){
    itd1.colle(lval[l]);
    itd2.colle(lval[l+1]);
    m=*(itd1++)+*(itd2++);
    g_val(m,0,l);
    for (i=1;i<_nval;i++){
      g_val(x=*(itd1++)+*(itd2++),i,l);
      if (m<x)
	m=x;
    }
    x=0;
    for (i=0;i<_nval;i++)
      x+=exp(_seq[l][i]-m);
    x=log(x)+m;
    for (i=0;i<_nval;i++)
      _seq[l][i]-=x;
    
    RL.forward(mi);
    RL.maj_val(lval[++l]);
  }

  itd1.colle(lval[l]);
  itd2.colle(lval[l+1]);
  m=*(itd1++)+*(itd2++);
  g_val(m,0,l);
  for (i=1;i<_nval;i++){
    g_val(x=*(itd1++)+*(itd2++),i,l);
    if (m<x)
      m=x;
  }
  x=0;
  for (i=0;i<_nval;i++)
    x+=exp(_seq[l][i]-m);
  x=log(x)+m;
  for (i=0;i<_nval;i++)
    _seq[l][i]-=x;

  delete[] lval;
}

void Cmatrice::forward(const Fsequence& G, const Clexique& L)
{
  if (G.deb()==0)
    return;
  
  int ta=G.taille();
  int i,k;
  int ndesc;
  
  Tliste<int> ldesc;
  L.ls_num(ldesc);
  
  Tliste_iter<int> iti(ldesc);
  int* pi;
  
  if (!(ndesc=ldesc.taille()))
    return;

  // allocation de la Cmatrice
  
  _nval=ndesc;
  _nvct=ta;

  if (_seq)
    delete[] _seq;
  _seq = new vector<double>[_nvct];
  for (i=0;i<_nvct;i++){
    _seq[i].construit_a_vide(_nval);
    for (k=0;k<_nval;k++)
      _seq[i][k]=0;
  }

  _rfin=_fin=_seq+_nvct-1;
  _l_desc.construit_a_vide(_nval);
  
  i=0;
  iti.init();
  while ((pi=iti()))
    _l_desc[i++]=int(*pi);
  
  // remplissage par forward

  RM_lex<Cposition> RL(L);

  const Cdescripteur *di, *dd, *df;
  dd=di=G.deb();
  df=G.fin();
  int l=0;
  
  Tliste<double> lval;
  Tliste_iter<double> itd(lval);
  
  RL.init(di,dd,df);
  RL.aj_val(lval);
  itd.init();
  for (i=0;i<_nval;i++)
    g_val(*(itd++),i,0);
  
  while (dd++<df){
    l++;
    RL.forward(di,dd,df);
    RL.maj_val(lval);
    
    itd.init();
    for (i=0;i<_nval;i++){
      g_val(*(itd++),i,l);
    }
  }
}

void Cmatrice::forward(const Fmatrice& G, const Clexique& L)
{
  if (G.taille()==0)
    return;
  
  int ta=G.taille();
  int i,k;
  int ndesc;
  
  Tliste<int> ldesc;
  L.ls_num(ldesc);
  
  Tliste_iter<int> iti(ldesc);
  int* pi;
  
  if (!(ndesc=ldesc.taille()))
    return;

  // allocation de la Cmatrice
  
  _nval=ndesc;
  _nvct=ta;

  if (_seq)
    delete[] _seq;
  _seq = new vector<double>[_nvct];
  for (i=0;i<_nvct;i++){
    _seq[i].construit_a_vide(_nval);
    for (k=0;k<_nval;k++)
      _seq[i][k]=0;
  }

  _rfin=_fin=_seq+_nvct-1;
  _l_desc.construit_a_vide(_nval);
  
  i=0;
  iti.init();
  while ((pi=iti()))
    _l_desc[i++]=int(*pi);
  
  // remplissage par forward

  RM_lex<Cposition> RL(L);

  mat_iter m_i(G);
  int l=0;
  
  Tliste<double> lval;
  Tliste_iter<double> itd(lval);
  
  RL.init(m_i);
  RL.aj_val(lval);
  itd.init();
  for (i=0;i<_nval;i++)
    g_val(*(itd++),i,0);
  
  while (m_i++){
    l++;
    RL.forward(m_i);
    RL.maj_val(lval);
    
    itd.init();
    for (i=0;i<_nval;i++){
      g_val(*(itd++),i,l);
    }
  }
}

void Cmatrice::backward(const Fsequence& G, const Clexique& L)
{
  if (G.deb()==0)
    return;
  
  int ta=G.taille();
  int i,k;
  int ndesc;
  
  Tliste<int> ldesc;
  L.ls_num(ldesc);
  
  Tliste_iter<int> iti(ldesc);
  int* pi;
  
  if (!(ndesc=ldesc.taille()))
    return;

  // allocation de la Cmatrice
  
  _nval=ndesc;
  _nvct=ta;

  if (_seq)
    delete[] _seq;
  _seq = new vector<double>[_nvct];
  
  for (i=0;i<_nvct;i++){
    _seq[i].construit_a_vide(_nval);
    for (k=0;k<_nval;k++)
      _seq[i][k]=0;
  }

  _rfin=_fin=_seq+_nvct-1;
  _l_desc.construit_a_vide(_nval);
  
  i=0;
  iti.init();
  while ((pi=iti()))
    _l_desc[i++]=int(*pi);
  
  ////////////////////////////////////
  // remplissage par backward

  RM_lex<Cposition> RL(L);

  const Cdescripteur *di, *dd, *df;
  di=G.deb();
  dd=df=G.fin();

  int l=G.taille()-1;
  
  Tliste<double> lval;
  Tliste_iter<double> itd(lval);
  
  RL.init();
  RL.aj_val(lval);
  
  itd.init();
  for (i=0;i<_nval;i++)
    g_val(*(itd++),i,l);
  
  while (dd>di){
    l--;
    RL.backward(di,dd--,df);
    RL.maj_val(lval);

    itd.init();
    for (i=0;i<_nval;i++)
      g_val(*(itd++),i,l);
  }
}

void Cmatrice::backward(const Fmatrice& G, const Clexique& L)
{
  if (G.taille()==0)
    return;
  
  int ta=G.taille();
  int i,k;
  int ndesc;
  
  Tliste<int> ldesc;
  L.ls_num(ldesc);
  
  Tliste_iter<int> iti(ldesc);
  int* pi;
  
  if (!(ndesc=ldesc.taille()))
    return;

  // allocation de la Cmatrice
  
  _nval=ndesc;
  _nvct=ta;

  if (_seq)
    delete[] _seq;
  _seq = new vector<double>[_nvct];
  
  for (i=0;i<_nvct;i++){
    _seq[i].construit_a_vide(_nval);
    for (k=0;k<_nval;k++)
      _seq[i][k]=0;
  }

  _rfin=_fin=_seq+_nvct-1;
  _l_desc.construit_a_vide(_nval);
  
  i=0;
  iti.init();
  while ((pi=iti()))
    _l_desc[i++]=int(*pi);
  
  ////////////////////////////////////
  // remplissage par backward

  RM_lex<Cposition> RL(L);

  mat_iter m_i(G);

  int l=G.taille()-1;
  
  Tliste<double> lval;
  Tliste_iter<double> itd(lval);
  
  RL.init();
  RL.aj_val(lval);
  
  itd.init();
  for (i=0;i<_nval;i++)
    g_val(*(itd++),i,l);

  m_i.finit();
  
  do{
    l--;
    RL.backward(m_i);
    RL.maj_val(lval);

    itd.init();
    for (i=0;i<_nval;i++)
      g_val(*(itd++),i,l);
  } while (m_i--);
}

void Cmatrice::predictions(const Fsequence& G, const Clexique& L)
{
  if (G.deb()==0)
    return;
  
  int ta=G.taille();
  int i,j;
  int ndesc;
  
  Tliste<int> ldesc;
  R_lex<Cposition> RL(L);
  Tliste_iter<Rmotif_Noeud_val<Cposition> > itrm(RL._vocces);
  Rmotif_Noeud_val<Cposition>* prm;
  
  if (!(ndesc=L.nb_desc()))
    return;

  // allocation de la Cmatrice
  
  _nval=ndesc;
  _nvct=ta;

  if (_seq)
    delete[] _seq;
  _seq = new vector<double>[_nvct];
  
  for (i=0;i<_nvct;i++){
    _seq[i].construit_a_vide(_nval);
    for (j=0;j<_nval;j++)
      _seq[i][j]=0;
  }

  _rfin=_fin=_seq+_nvct-1;
  _l_desc.construit_a_vide(_nval);
  
  i=0;
  itrm.init();
  while ((prm=itrm()))
    _l_desc[i++]=prm->pt()->num();
  
  ////////////////////////////////////
  // remplissage par predictions

  const Cdescripteur *di, *dd, *df;
  dd=di=G.deb();
  df=G.fin();

  RL.init(di,dd,df);
  j=0;
  
  itrm.init();
  for (i=0;i<_nval;i++)
    g_val((++itrm)->val(),i,0);

  while (++dd<=df){
    j++;
    RL.egale(di,dd,df);

    itrm.init();
    for (i=0;i<_nval;i++)
      g_val((++itrm)->val(),i,j);
  }
}

void Cmatrice::predictions(const Fmatrice& M, const Clexique& L)
{
  int ta;
  if (!(ta=M.taille()))
    return;
  
  int i,j;
  int ndesc;
  
  Tliste<int> ldesc;
  R_lex<Cposition> RL(L);
  Tliste_iter<Rmotif_Noeud_val<Cposition> > itrm(RL._vocces);
  Rmotif_Noeud_val<Cposition>* prm;
  
  if (!(ndesc=L.nb_desc()))
    return;

  // allocation de la Cmatrice
  
  _nval=ndesc;
  _nvct=ta;

  if (_seq)
    delete[] _seq;
  _seq = new vector<double>[_nvct];
  
  for (i=0;i<_nvct;i++){
    _seq[i].construit_a_vide(_nval);
    for (j=0;j<_nval;j++)
      _seq[i][j]=0;
  }

  _rfin=_fin=_seq+_nvct-1;
  _l_desc.construit_a_vide(_nval);
  
  i=0;
  itrm.init();
  while ((prm=itrm()))
    _l_desc[i++]=prm->pt()->num();
  
  ////////////////////////////////////
  // remplissage par predictions

  mat_iter mi(M);
  j=0;
  
  RL.init(mi);

  itrm.init();
  for (i=0;i<_nval;i++)
    g_val((++itrm)->val(),i,0);
  
  while (mi()){
    j++;
    RL.egale(mi);

    itrm.init();
    for (i=0;i<_nval;i++)
      g_val((++itrm)->val(),i,j);
  }
}

void Cmatrice::smoothe(const Fsequence& G, const int sz, const int st,
                      char op)
{
  if ((sz<=0) || (st<=0))
    return;

  int ta=G.taille();
  int i,j;

  Tliste<Cdescripteur> ld;
  Tliste_iter<Cdescripteur> ild(ld);
  Cdescripteur* pd;

  const Cdescripteur *dd, *df, *di;

  // allocation de la Cmatrice et du lexique

  for (i=0;i<ta;i++)
    ld.aj_fin_diff(G[i]);

  nettoie();

  _nval=ld.nb_ind();
  _nvct=(ta - sz + 1)/st;

  if (_nvct<=0)
    return;
  
  _seq = new vector<double>[_nvct];
  for (i=0;i<_nvct;i++){
    _seq[i].construit_a_vide(_nval);
    for (j=0;j<_nval;j++)
      _seq[i][j]=0;
  }
  
  _rfin=_fin=&_seq[_nvct-1];
  _l_desc.construit_a_vide(_nval);

    
  i=0;
  ild.init();
  while ((pd=ild()))
    _l_desc[i++]=*pd;

  dd=G.deb();
  df=dd+sz;
  i=0;
  while (i<_nvct) {
    di=dd;
    do{
      ajoute_en(i,*di);
    } while (++di<df);
    dd+=st;
    df+=st;
    if (op=='m')
      divise_en_par(i,sz);
    i++;
  }
}

void Cmatrice::smoothe(const Fmatrice& M, const int sz, const int st,
                      char op)
{
  if ((sz<=0) || (st<=0))
    return;

  int ta=M.taille();
  int i,j;

  Tliste<Cdescripteur> ld;
  Tliste_iter<Cdescripteur> ild(ld);
  Cdescripteur* pd;

  mat_iter mid, mii, mif;

  // allocation de la Cmatrice et du lexique

  j=M.nval();
  for (i=0;i<j;i++)
    ld.aj_fin_diff(M.dsc(i));

  nettoie();
  
  _nval=ld.nb_ind();
  _nvct=(ta - sz + 1)/st;
  
  if (_nvct<=0)
    return;
  
  _seq = new vector<double>[_nvct];
  for (i=0;i<_nvct;i++){
    _seq[i].construit_a_vide(_nval);
    for (j=0;j<_nval;j++)
      _seq[i][j]=0;
  }

  _rfin=_fin=&_seq[_nvct-1];
  _l_desc.construit_a_vide(_nval);
  
  i=0;
  ild.init();
  while ((pd=ild()))
    _l_desc[i++]=*pd;

  //  remplissage de la Cmatrice

  mid.colle(M);
  mii.colle(M);
  mif.colle(M);
  mif+=sz-1;
  i=0;
  while (i<_nvct){
    mii=mid;
    ajoute_en(i,mii);
    while (mii!=mif){
      mii++;
      ajoute_en(i,mii);
    }
    if (op=='m')
      divise_en_par(i,sz);
    mid+=st;
    mif+=st;
    i++;
  }
}

void Cmatrice::derive(const Fsequence& G)
{
  int ta=G.taille();
  int i,j;

  Tliste<Cdescripteur> ld;
  Tliste_iter<Cdescripteur> ild(ld);
  Cdescripteur* pd;
  
  const Cdescripteur *dd, *df;

  // allocation de la Cmatrice et du lexique

  for (i=0;i<ta;i++)
    ld.aj_fin_diff(G[i]);
  
  _nval=ld.nb_ind();
  _nvct=ta;

  if (_seq)
    delete[] _seq;
  _seq = new vector<double>[_nvct];
  for (i=0;i<_nvct;i++){
    _seq[i].construit_a_vide(_nval);
    for (j=0;j<_nval;j++)
      _seq[i][j]=0;
  }

  _rfin=_fin=_seq+_nvct-1;
  _l_desc.construit_a_vide(_nval);
  
  i=0;
  ild.init();
  while ((pd=ild()))
    _l_desc[i++]=*pd;

  //  remplissage de la Cmatrice

  dd=G.deb();
  df=G.fin();
  ajoute_en(0,*dd);
  i=1;
  do {
    ote_en(i,*dd);
    dd++;
    ajoute_en(i,*dd);
    i++;
  } while (dd<df);
}

void Cmatrice::derive(const Fmatrice& M)
{
  int ta=M.taille();
  int i,j;

  Tliste<Cdescripteur> ld;
  Tliste_iter<Cdescripteur> ild(ld);
  Cdescripteur* pd;
  
  mat_iter mi;

  // allocation de la Cmatrice et du lexique

  j=M.nval();
  for (i=0;i<j;i++)
    ld.aj_fin_diff(M.dsc(i));

  _nval=ld.nb_ind();
  _nvct=ta;

  if (_seq)
    delete[] _seq;
  _seq = new vector<double>[_nvct];
  for (i=0;i<_nvct;i++){
    _seq[i].construit_a_vide(_nval);
    for (j=0;j<_nval;j++)
      _seq[i][j]=0;
  }

  _rfin=_fin=_seq+_nvct-1;
  _l_desc.construit_a_vide(_nval);
  
  i=0;
  ild.init();
  while ((pd=ild()))
    _l_desc[i++]=*pd;

  //  remplissage de la Cmatrice

  mi.colle(M);
  i=1;
  ajoute_en(0,mi);
  do{
    ote_en(i,mi);
    mi++;
    ajoute_en(i,mi);
    i++;
  } while (i<_nvct);
}

///////////////////////////////////////////////////////////////////

void Cmatrice::integre(const Fsequence& G)
{
  int ta=G.taille();
  int i,j;

  Tliste<Cdescripteur> ld;
  Tliste_iter<Cdescripteur> ild(ld);
  Cdescripteur* pd;
  
  const Cdescripteur *dd, *df;

  // allocation de la Cmatrice et du lexique

  for (i=0;i<ta;i++)
    ld.aj_fin_diff(G[i]);
  
  _nval=ld.nb_ind();
  _nvct=ta;

  if (_seq)
    delete[] _seq;
  _seq = new vector<double>[_nvct];
  for (i=0;i<_nvct;i++){
    _seq[i].construit_a_vide(_nval);
    for (j=0;j<_nval;j++)
      _seq[i][j]=0;
  }

  _rfin=_fin=_seq+_nvct-1;
  _l_desc.construit_a_vide(_nval);
  
  i=0;
  ild.init();
  while ((pd=ild()))
    _l_desc[i++]=*pd;

  mat_iter mi;

  //  remplissage de la Cmatrice

  dd=G.deb();
  df=G.fin();
  mi.colle(*this);

  copie_en(0,*dd);
  dd++;
  i=1;
  while (dd<=df){
    copie_en(i,mi);
    mi++;
    ajoute_en(i,*dd);
    i++;
    dd++;
  } 
}

void Cmatrice::integre(const Fmatrice& M)
{
  int ta=M.taille();
  int i,j;

  Tliste<Cdescripteur> ld;
  Tliste_iter<Cdescripteur> ild(ld);
  Cdescripteur* pd;
  
  mat_iter mi;

  // allocation de la Cmatrice et du lexique

  j=M.nval();
  for (i=0;i<j;i++)
    ld.aj_fin_diff(M.dsc(i));

  _nval=ld.nb_ind();
  _nvct=ta;

  if (_seq)
    delete[] _seq;
  _seq = new vector<double>[_nvct];
  for (i=0;i<_nvct;i++){
    _seq[i].construit_a_vide(_nval);
    for (j=0;j<_nval;j++)
      _seq[i][j]=0;
  }

  _rfin=_fin=_seq+_nvct-1;
  _l_desc.construit_a_vide(_nval);
  
  i=0;
  ild.init();
  while ((pd=ild()))
    _l_desc[i++]=*pd;

  //  remplissage de la Cmatrice

  mat_iter mi2;
  mi2.colle(*this);

  mi.colle(M);

  copie_en(0,mi);
  i=1;
  while(mi()){
    copie_en(i,mi2);
    mi2++;
    ajoute_en(i,mi);
    i++;
  } 
}

////////////////////////////////
// modifications

void Cmatrice::ajoute_en(int i, const mat_iter& m)
{
  int a,j;
  if ((i>=0) && (i<_nvct))  
    for (j=0;j<m.mat()->nval();j++)
      if ((a=ligne(m.mat()->dsc(j)))>=0)
	_seq[i][a]+=m(j);
}

void Cmatrice::copie_en(int i, const mat_iter& m)
{
  int a,j;
  if ((i>=0) && (i<_nvct))  
    for (j=0;j<m.mat()->nval();j++)
      if ((a=ligne(m.mat()->dsc(j)))>=0)
	_seq[i][a]=m(j);
}

void Cmatrice::ote_en(int i, const mat_iter& m)
{
  int a,j;
  if ((i>=0) && (i<_nvct))  
    for (j=0;j<m.mat()->nval();j++)
      if ((a=ligne(m.mat()->dsc(j)))>=0)
	_seq[i][a]-=m(j);
}

