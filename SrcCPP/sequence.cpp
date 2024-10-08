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

#include "sequence.h"
#include "partition.h"
#include "lexique.h"

#include <iostream>
#include <ctype.h>
#include <stdlib.h>

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
////////////////////////////////////////////////////////////////

Csequence::Csequence(int i) : Fsequence()
{
  if (i>0){
    _deb=new Cdescripteur[i];
    _vfin=_fin=&_deb[i-1];
  }    
}

Csequence::Csequence(void*) : Fsequence()
{}

Csequence::Csequence(const Fsequence& g) 
{
  cerr << "Bad call of Csequence::Csequence(const Fsequence& g) ";
  cerr << endl;
  exit(0);
}

void Csequence::genere(int l){
  nettoie();
  if (l>0){
    _deb=new Cdescripteur[l];
    _vfin=_fin=&_deb[l-1];
  }
}
  
void Csequence::nettoie()
{
  if (_deb)
    delete[] _deb;
  _deb=0;
}

Csequence::~Csequence()
{
  nettoie();
}

/////////////////////////////////////////////////////
/// modifs

void Csequence::egale(const Fsequence& G)
{
  this->operator=(G);
}

void Csequence::operator=(const Fsequence& G)
{
  int lG=G.taille();
  if (taille()!=lG){
    if (_deb)
      delete[] _deb;
    if (!lG)
      return;
    _deb=new Cdescripteur[lG];
    _vfin=_fin=_deb+lG-1;
  }
  int i;
  for (i=0;i<lG;i++)
    *(_deb+i)=*(G.deb()+i);
  }

////////////////////////////////////////////
// recuperations de fichiers
////////////////////////////////////////////

bool Csequence::recup_seq(const string& st)
{
  int lg;
  Cdescripteur *p_c;
  char c;
  int deb;
  int i,j;
  char buffer[12];
  
  ifstream flot(st.c_str());
  if (!flot){
    cout << "pb opening " << st << endl;
    return 0;
  }
	
  flot >> c;
  i=0;
  while (isdigit(c) && (i<11)){
    buffer[i++]=c;
    flot.get(c);
  }
  buffer[i]='\0';
  deb=atoi(buffer);

  if (deb<=0){
    cout << "bad start in file ";
    cout << st << endl;
    flot.close();
    return 0;
  }

  flot >> c;
  while (!isgraph(c))
    flot.get(c);
  if (isdigit(c)){
    i=0;
    while (isdigit(c) && (i<11)){
      buffer[i++]=c;
      flot.get(c);
    }
    buffer[i]='\0';
    if ((lg=1+atoi(buffer)-deb)<=0){
      cout << "bad boundaries in file ";
      cout << st << endl;
      flot.close();
      return 0;
    }
    while (!isgraph(c))
      flot.get(c);
  }
  else{
    lg=deb;
    deb=1;
  }

  ////////// recuperation des elements de la sequence

  if (_deb)
    delete[] _deb;
  _deb=new Cdescripteur[lg];
  _vfin=_fin=&(_deb[lg-1]);

  p_c=_deb;

  //  on va jusqu'a deb sans rien faire

  i=1;j=0;
  while (!flot.eof() && (i<deb) && (j<100)){
    if (isalpha(c)){
      i++;
      j=0;
    }
    else 
      j++;
    flot.get(c);
  }
  if (flot.eof()){
    cout << "Bad boundaries in file ";
    cout << st << endl;
    if (_deb)
      delete[] _deb;
    _vfin=_fin=_deb=0;
    flot.close();
    return 0;
  }
  if (j==100){
    cout << "Pb in file ";
    cout << st << endl;
    if (_deb)
      delete[] _deb;
    _vfin=_fin=_deb=0;
    flot.close();
    return 0;
  }

  // puis on remplit la sequence

  while (!flot.eof() && (c!='.') && (p_c<=_fin)){
    if (isalpha(c)){
      *p_c=c;
      p_c++;
    }
    flot.get(c);
  }

  if (p_c<=_fin){
    cout << "Sequence-length pb" << endl;
    _fin=--p_c;
  }
  
  while (!flot.eof() && (c!='.'))
    flot >> c;

  flot.close();
  return 1;
}

bool Csequence::recup_fst(const string& st)
{
  ifstream flot(st.c_str());
  if (!flot){
    cout << "pb opening " << st << endl;
    return 0;
  }
  recup_fst(flot,0);
  flot.close();
  return 1;
}    


int Csequence::recup_fst(ifstream& flot, int deb)
{
  long lg;
  int pos, fin;
  char c;
  Cdescripteur* p_c;
  flot.seekg(0,iostream::end);
  fin=flot.tellg();
  fin--;
  if (deb>=fin)
    return -1;
  flot.seekg(deb,iostream::beg);
  
  flot.ignore(1000,'\n');
  lg=0;
  
  c=flot.peek();
  while ((c!='>') && (fin>flot.tellg())){
    if (isalpha(c) || c=='-')
      lg++;
    flot.seekg(1,iostream::cur);
    c=flot.peek();
  }
  if (isalpha(c) || c=='-')
    lg++;
  
  pos=flot.tellg();
  // on recommence
  
  if (_deb)
    delete[] _deb;
  
  if (lg==0){
    cout << "Empty sequence" << endl;
    return -2;
  }
  if (!(_deb=new Cdescripteur[lg])){
    cout << "Allocation pb in Csequence::recup_fst()." << endl;
    return -2;
  }
  
  _vfin=_fin=&(_deb[lg-1]);
  
  flot.seekg(deb,iostream::beg);
  flot.ignore(1000,'\n');
  
  flot >> c;
  p_c=_deb;
  while ((fin>=flot.tellg()) && (p_c<_fin)){
    if (isalpha(c) || c=='-'){
      *p_c=c;
      p_c++;
    }
    flot >> c;
  }
  if (isalpha(c) || c=='-'){
    *p_c=c;
    p_c++;
  }
  
  if (p_c<=_fin){
    cerr << "Memory waste in sequence" << endl;
    _fin=p_c;
  }

  return (pos==fin)?-1:pos;
}
		
bool Csequence::recup_rel(const Fsequence& F, const Csegment& S)
{
  nettoie();

  int d,f;
  int Ft=F.taille();
  
  d=S.pos_d();
  f=S.pos_f();
  if (f>Ft)
    f=Ft;
  
  if ((!(F.deb())) || (d > f))
    return 0;

  _deb=new Cdescripteur[f-d+1];
  _fin=_deb;
  Cdescripteur *po, *pf;
  po=&F[d-1];
  pf=&F[f-1];

  do{
    *(_fin++)=*(po++);
  } while (po<=pf);
  _vfin=--_fin;
  
  return 1;
}

