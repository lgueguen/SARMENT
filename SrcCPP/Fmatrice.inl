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

#include "math.h"

Fmatrice::Fmatrice() : _fin(0), _seq(0), _l_desc(0), 
  _nval(0), _nvct(0)
{
}

Fmatrice::~Fmatrice()
{
  _seq=_fin=0;
}

void Fmatrice::nettoie()
{
  _seq=_fin=0;
  _l_desc.construit_a_vide(0);
  _nval=0;
  _nvct=0;
}

///////////////////////////////////
//  donnees

Fmatrice::vector<double>& Fmatrice::operator[](const int i) const
{
  return (_seq[i]);
}

Cdescripteur& Fmatrice::dsc(int i) const
{
  return ((i>=0) && (i<nval()))
    ?_l_desc[i]
    : (i<0)?_l_desc[0]:_l_desc[nval()-1];    
}

char Fmatrice::carac(int i) const
{
  return ((i>=0) && (i<nval()))
    ?_l_desc[i].dsc()
    : (i<0)?_l_desc[0].dsc():_l_desc[nval()-1].dsc();    
}

int Fmatrice::ligne(const Cdescripteur d) const
{
  int i=-1;
  while (++i<_nval)
    if (_l_desc[i]==d)
      return i;
  return -1;
}

int Fmatrice::ligne(char c) const
{
  int i=-1;
  while (++i<_nval)
    if (_l_desc[i].dsc()==c)
      return i;
  return -1;
}

double Fmatrice::operator()(const int j, const int i) const
{
  return ((j>=0) && (i>=0) 
	  && (i<_nvct) && (j<_nval))?_seq[i][j]:0;
}

void Fmatrice::g_val(const double x, const int j, const int i)
{
  if ((j>=0) && (i>=0) && (i<_nvct) && (j<_nval))
    _seq[i][j]=x;
}

int Fmatrice::taille() const
{
  return _nvct;
}

int Fmatrice::nval() const
{
  return _nval;
}

Fmatrice::vector<double>* Fmatrice::deb() const 
{
  return _seq;
}

Fmatrice::vector<double>* Fmatrice::fin() const 
{
  return _fin;
}
	
void Fmatrice::echange(int i, int j)
{
  if ((i>=0) && (j>=0) && (i<_nvct) && (j<_nvct))
    _seq[i].echange(_seq[j]);
}


///////////////////////////////////////
///////////////////////////////////////
//        Fmatrice::vector<T>
///////////////////////////////////////
//////////////////////////////////////

template<class T>
Fmatrice::vector<T>::vector() :_tab(0)
{
}

template<class T>
Fmatrice::vector<T>::vector(const int i)
  :  _tab((i>0)?new T[i]:0)
{
}

template<class T>
Fmatrice::vector<T>::vector(const vector<T>& v)
{
  cerr << "mauvais appel de ";
  cerr << "Fmatrice::vector<T>::vector(const vector<T>&)" << endl;
}

template<class T>
Fmatrice::vector<T>::~vector()
{
  nettoie();
}

template<class T>
void Fmatrice::vector<T>::nettoie()
{
  if (_tab)
    delete[] _tab;
  _tab=0;
}

/////////////////////////////
///  modifications

template<class T>
void Fmatrice::vector<T>::construit_a_vide(const int i)
{
  nettoie();
  if (i>0)
    _tab=new T[i];
}

template<class T>
void Fmatrice::vector<T>::recup_prem(const vector<T>& v, const int i)
{
  int j;
  nettoie();
  if (i>0){
    _tab=new T[i];
    for (j=0;j<i;j++)
      _tab[j]=v[j];
  }
}
    
template<class T>
void Fmatrice::vector<T>::echange(vector<T>& v)
{
  T* vi=v._tab;
  v._tab=_tab;
  _tab=vi;
}
    
template<class T>
void Fmatrice::vector<T>::logarithme(const int n)
{
  static unsigned i;
  for (i=0;i<n;i++)
    _tab[i]=log(_tab[i]);
}

template<class T>
void Fmatrice::vector<T>::log_normalise(const int n)
{
  static unsigned i;
  static T M;
  static T x;

  M=_tab[0];
  for (i=1;i<n;i++)
    if (M<_tab[i])
      M=_tab[i];

  x=0.0;
  for (i=0;i<n;i++)
    x+=exp(_tab[i]-M);

  x=M+log(x);
  for (i=0;i<n;i++)
    _tab[i]-=x;
}

template<class T>
void Fmatrice::vector<T>::exponentielle(const int n)
{
  static unsigned i;
  for (i=0;i<n;i++)
    _tab[i]=exp(_tab[i]);
}

////////////////////////////
/// donnees

template<class T>
T& Fmatrice::vector<T>::operator[](int j) const
{
  return _tab[j];
}


////////////////////////////
///    tests

template<class T>
bool Fmatrice::vector<T>::est_vide() const
{
  return (_tab==0);
}

