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

/////////////////////
//  construction/destruction

Cmotif::Cmotif()
{
  _su=this;
  _pr=this;
}

Cmotif::Cmotif(void *)
{
  _su=this;
  _pr=this;
}

Cmotif::~Cmotif()
{
  if (_pr!=this){
    _pr->g_su(_su);
    _su->g_pr(_pr);
  }
}

///////////////////////
// donnees


Cmotif* Cmotif::su() const 
{
  return(_su);
}

Cmotif* Cmotif::pr() const 
{
  return(_pr);
}

void Cmotif::liste_desc(Tliste<Cdescripteur>& li) const
{
  const Cmotif* sd=this;
  
  do {
    sd->Cdesc_comp::liste_desc(li);
  } while ((sd=sd->_su)!=this);
}

void Cmotif::liste_desc(Tliste<Cdesc_comp>& li) const
{
  const Cmotif* sd=this;
  
  do {
    li.aj_fin_diff(*sd);
  } while ((sd=sd->_su)!=this);
}

///////////////////////////
// modifications

void Cmotif::g_su(Cmotif* s){
  _su=s;
}

void Cmotif::g_pr(Cmotif* p){
  _pr=p;
}
	
void Cmotif::operator=(const Cdesc_comp& d)
{
  _su=this;
  _pr=this;
  Cdesc_comp::operator=(d);
}

////////////////////////////
//  tests

bool Cmotif::operator==(const Cmotif& m) const
{
  if (Cdesc_comp::operator!=(m))
    return 0;

  const Cmotif* d=su();
  const Cmotif* dm=m.su();
  while ((d!=this) || (dm!=&m)){
    if (d->Cdesc_comp::operator!=(*dm))
      return 0;
    d=d->su();
    dm=dm->su();
  }
  return 1;
}

bool Cmotif::operator!=(const Cmotif& m) const
{
  return !Cmotif::operator==(m);
}

////////////////////////////
//   evaluation

int Cmotif::lg() const
{
  int i=1;
  const Cmotif* d=this;
  while((d=d->su())!=this)
    i++;
  return i;
}


