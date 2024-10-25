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

#include "Fsequence.h"

#include "segment.h"
#include <stdio.h>
#include "random.h"

Fsequence::Fsequence() : _deb(0), _fin(0), _vfin(0)
{
}

Fsequence::Fsequence(void*) : _deb(0), _fin(0), _vfin(0)
{
}

Fsequence::Fsequence(const Fsequence& C)
{
  _deb=C._deb;
  _fin=C._fin;
  _vfin=C._vfin;
}

void Fsequence::nettoie()
{
  _deb=0;
  _fin=0;
  _vfin=0;
}

Fsequence::~Fsequence()
{
  _deb=_fin=_vfin=0;
}

/////////////////////////////////////////////////
///  modifs

void Fsequence::operator=(const Fsequence& G)
{
  nettoie();
  _deb=G._deb;
  _fin=G._fin;
  _vfin=G._vfin;
}

void Fsequence::termine(int i){
  if ((i>=0) && (_deb+i<=_vfin))
    _fin=_deb+i;
}
    
void Fsequence::recup_rel(const Fsequence& F, int d, int f)
{
  nettoie();

  int Ft=F.taille();
  if (f>Ft)
    f=Ft-1;

  if ((!(F._deb)) || (d > f))
    return;

  _deb=F._deb+d;
  _vfin=_fin=_deb+f-d;

}

void Fsequence::melange(int nb)
{
  int l=taille();

  int i;
  int pos1, pos2;
  Cdescripteur d;
  
  for (i=0;i<nb;i++){
    pos1=Random1(l);
    pos2=Random1(l);
    d=_deb[pos1];
    _deb[pos1]=_deb[pos2];
    _deb[pos2]=d;
  } 
}

///////////////////////
//// sorties
///////////////////////

ostream& Fsequence::operator<<(ostream& s) const
{
  if (!_deb)
    return s;

  Cdescripteur* p=_deb-1;
  int i=0;
  char tmp[20];

  snprintf(tmp,20,"%d",taille());
  s << tmp << '\n';

  while (p++!=_fin){
    s << p->dsc();
    if (++i==100){
      s << endl;
      i=0;
    }
    else
      if (i % 10 == 0)
	s << ' ';
  }
  return s;
}


