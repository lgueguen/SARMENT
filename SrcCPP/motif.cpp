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

#include "motif.h"
#include "Tliste_iter.h"

#include <stdio.h>

////////////////////////// 
// modifications

const Cmotif& Cmotif::justif_g(int i) const
{
  int j;
  const Cmotif* m=this;
  int f=i % lg();

  if (f>0)
    for (j=0;j<f;j++)
      m=m->pr();
  else
    for (j=0;j<-f;j++)
      m=m->su();

  return(*m);
}

const Cmotif& Cmotif::justif_d(int i) const
{
  int j;
  const Cmotif* m=this;
  int f=i % lg();

  if (f>0)
    for (j=0;j<f;j++)
      m=m->su();
  else
    for (j=0;j<-f;j++)
      m=m->pr();

  return(*m);
}

/////////////////////////////
// Calculs


int Cmotif::modulo(const Cmotif& m) const
{
  int l=lg();
  int lm=m.lg();

  if (lm==l){
    int i=-1;
    const Cmotif *mdep=&m;
    while (++i<l){
      if (Cmotif::operator==(*mdep))
	return i;
      mdep=mdep->su();
    }
  }
  return (l>lm)?l:lm;
}

bool Cmotif::tourne(const Cmotif& m) const
{
  int l=lg();
	
  if (l==m.lg()){
    int i=-1;
    const Cmotif *mdep=&m;
    while (++i<l){
      if (Cmotif::operator==(*mdep))
	return 1;
      mdep=mdep->su();
    }
  }
  return 0;
}


//////////////////////////////
// sorties

ostream& Cmotif::operator<<(ostream& s) const
{
  const Cmotif* d=su();
  Cdesc_comp::operator<<(s);
    
  while (d!=this){
    d->Cdesc_comp::operator<<(s);
    d=d->su();
  }
  return s;
}

void Cmotif::ecrit_num(ostream& s) const
{
  const Cmotif* d=su();
  char tmp[9];

  sprintf(tmp,"%d",num());
  s << tmp;
    
  while (d!=this){
    s << ',';
    sprintf(tmp,"%d",d->num());
    s << tmp;
    d=d->su();
  }
}


