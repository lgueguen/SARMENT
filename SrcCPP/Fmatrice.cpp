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

#include "Fmatrice.h"
#include "segment.h"
#include "random.h"

#include <stdio.h>
#include <math.h>

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Fmatrice::Fmatrice(const Fmatrice& M) : _nval(M._nval)
{
  _nvct=M.taille();
  _seq=M._seq;
  _fin=M._fin;

  _l_desc.recup_prem(M._l_desc,_nval);
}

//////////////////////////////////////////////////////////////////////
// modifications
//////////////////////////////////////////////////////////////////////

    
void Fmatrice::recup_rel(const Fmatrice& F, int d, int f)
{
  nettoie();

  int Ft=F.taille();
  if (f>Ft)
    f=Ft;
  if ((!(F._seq)) || (d >= f))
    return;

  _nval=F._nval;
  _nvct=f-d;
  _l_desc.recup_prem(F._l_desc,_nval);
  
  _seq=F._seq+d;
  _fin=_seq+f-d-1;
  
}
    
void Fmatrice::melange()
{
  int nb=(int)(_nvct*(log(float(_nvct))+1)/2);

  int i;
  int pos1, pos2;
  
  for (i=0;i<nb;i++){
    pos1=Random1(_nvct);
    pos2=Random1(_nvct);
    _seq[pos1].echange(_seq[pos2]);
  } 
}

void Fmatrice::log_normalise(){
  mat_iter mi(*this);
  
  do 
    mi._p_vec->log_normalise(_nval);    
  while (mi());
  
}

void Fmatrice::exponentielle(){
  mat_iter mi(*this);
  
  do
    mi._p_vec->exponentielle(_nval);
  while (mi());
}

void Fmatrice::logarithme(){
  mat_iter mi(*this);
    
  do
    mi._p_vec->logarithme(_nval);
  while (mi());
}

//////////////////////////////////////////////////////////////////////
// sortie texte
//////////////////////////////////////////////////////////////////////

ostream& Fmatrice::operator<<(ostream& s) const
{
  Fmatrice::vector<double>* p=_seq;
  int j;
  char tmp[30];
  
  if (!_seq || _l_desc.est_vide())
    return s;

  sprintf(tmp,"%d\n",taille());
  s << tmp;
  
  s.setf(iostream::left);
  
  for (j = 0;j<_nval;j++){
    _l_desc[j].operator<<(s);
    s << "\t";
  }
  s << endl;
  
  do {
    for (j = 0;j<_nval;j++){
      sprintf(tmp,"%.12g\t", ((*p)[j]));
      s << tmp;
    }
    s << endl;
  } while (p++!=_fin);

  return s;
}
