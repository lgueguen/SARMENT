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


Cmatrice::Cmatrice() : Fmatrice(), _rfin(0)
{
}

// Cmatrice::~Cmatrice()
// {
//   nettoie();
// }

void Cmatrice::nettoie()
{
  vector<double>* pv=_seq;
  if (_seq){
    for (;pv<=_rfin;pv++)
      pv->nettoie();
    if (_seq<_rfin)
      delete[] _seq;
  }
  _seq=_fin=_rfin=0;
  _l_desc.construit_a_vide(0);
  _nval=0;
  _nvct=0;
}

////////////////////////////////
// modifications

void Cmatrice::ajoute_en(int i, const Cdescripteur& d)
{
  int a;
  if ((i>=0) && (i<_nvct))
    if ((a=ligne(d))>=0)
      _seq[i][a]++;
}

void Cmatrice::copie_en(int i, const Cdescripteur& d)
{
  int a;
  if ((i>=0) && (i<_nvct)){
    for (a=0;a<_nval;i++)
      _seq[i][a]=0;
    if ((a=ligne(d))>=0)
      _seq[i][a]=1;
  }
}

void Cmatrice::ote_en(int i, const Cdescripteur& d)
{
  int a;
  if ((i>=0) && (i<_nvct))
    if ((a=ligne(d))>=0)
      _seq[i][a]--;
}

void Cmatrice::divise_en_par(int i, double d)
{
  if ((d!=0) && (i>=0) && (i<_nvct))
    {
      int j;
      for (j=0;j<_nval;j++)
	_seq[i][j]/=d;
    }
}

//////////////////////////
// sorties

///////////////////////////////////
//  donnees

Fmatrice::vector<double>& Cmatrice::operator[](const int i) const
{
  return (_seq[i]);
}

