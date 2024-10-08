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

#include "posopere.h"
#include "part_equ.h"

#include <string>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Cposopere::Cposopere(const Cposopere& p) 
{
  Tliste_iter<Csegment> iter(p._fils);
  Csegment* pl;

  while ((pl=iter()))
    _fils.aj_fin(*pl);
}

//////////////////////////////////////////////////////////////////////
// modifications
//////////////////////////////////////////////////////////////////////

void Cposopere::operator=(const Cposopere& p) 
{
  _fils.nettoie();
  Tliste_iter<Csegment> iter(p._fils);
  Csegment* pl;

  while ((pl=iter()))
    _fils.aj_fin(*pl);
}

void Cposopere::aj_vide()
{
  Tliste_iter<Csegment> iter(_fils);
  Csegment* x;
  Csegment v;
  v.vide();

  while ((x=iter()))
    if (v==*x)
      return;

  _fils.aj_fin(v);
}

//////////////////////////////////////////////////////////////////////
// recuperations
//////////////////////////////////////////////////////////////////////

char Cposopere::recup(ifstream& flot)
{
  int j;
  double x;
  char c='N';
  string buff;
  Csegment* sg;
	
  _fils.nettoie();
  while (!flot.eof()){
    flot >> c;
    while ((!flot.eof()) && (c!='<')){
      if (c=='|'){
	_fils.aj_fin();
	_fils.ind_fin()->vide();
	while (c==' ')
	  flot >> c;
      }
      if ((c=='>') || (c=='X'))
	return c;
      flot >> c;
    }
    if (flot.eof())
      break;
    _fils.aj_fin();
    sg=_fils.ind_fin();
    flot >> j;
    if (j<0){
      cout << "mauvais type de fichier" << endl;
      return 'N';
    }
    sg->g_pos_d(j+1);
    flot >> c;
    switch(c){
    case '>':
      sg->g_pos_f(j+1);
      break;
    case '-':
      flot >> j;
      if (j<0){
	cout << "mauvais type de fichier" << endl;
	return 'N';
      }
      sg->g_pos_f(j+1);
      flot >> c;
      break;
    default:
      cout << "mauvais type de fichier" << endl;
      return 'N';
    }

    flot >> x;
    sg->g_val(x);
    j=0;
    if (flot.peek()==':')
      flot.ignore(1);
//     flot.get(buff[0]);
//     while (!isspace(buff[j]) && (j<1999))
//       flot.get(buff[++j]);
    getline(flot,buff,' ');
    if (!(sg->recup_mot(buff))){
      cout << "Probleme pour le predicteur " << buff;
      cout << " en <" << sg->pos_d()-1 << "-" << sg->pos_f()-1;
      cout << ">" << endl;
      return 'N';
    }
    
    j=1;
    while (j && !flot.eof()){
      flot >> c;
      switch(c){
      case '|':
	j=0;
	break;
      case 'X':
      case '>':
	return c;
      case '-':
	break;
      default:
	cout << "mauvais type de fichier" << endl;
	return 'N';
      }
    }
    if (j)
      break;
  }
  if ((flot.eof()) && !_fils.est_vide()){
    cout << "mauvais type de fichier" << endl;
    return 'N';
  }
  return c;
}

void Cposopere::recup(const mot_ces& mc)
{
  Tliste_iter<mot_ces::borne> i_deb(mc._deb);
  Tliste_iter<mot_ces::borne> i_fin(mc._fin);
  Tliste_iter<Cdesc_comp> i1(mc._mot);
  Csegment sg;
  int i;
  int l=mc._mot.nb_ind();
  if (l<=0){
    aj_vide();
    return;
  }
  mot_ces::borne *pd, *pf;

  while ((pd=i_deb())){
    i=pd->bor()%l;
    i1.init();
    while (--i>=0)
      i1++;
    sg.recup_mot(i1);
    sg.g_pos_d(pd->bor());
    i_fin.init();
    while ((pf=i_fin())){
      if (pf->bor()>pd->bor()){
	sg.g_pos_f(pf->bor());
	sg.g_val(pf->val()-pd->val());
	ajoute(sg);
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////
// evaluations
//////////////////////////////////////////////////////////////////////

int Cposopere::nbe_segments() const
{
  Tliste_iter<Csegment> iter(_fils);
  Csegment* sg;
  int i=0;
  while ((sg=iter()))
    if (!sg->est_vide())
      i++;

  return i;
}

double Cposopere::var_borne_deb() const
{
  if (_fils.est_vide())
    return 0;

  int i,p;
  double n, nc;
  Tliste_iter<Csegment> iter(_fils);
  Csegment* sg;

  i=0;
  n=0;
  nc=0;
  while ((sg=iter())){
    if (!sg->est_vide()){
      i++;
      p=sg->pos_d();
      n+=p;
      nc+=p*p;
    }
  }

  if (i==0)
    return 0;

  return nc/i-n*n/(i*i);
}


int Cposopere::max_f() const
{
  Tliste_iter<Csegment> iter(_fils);
  Csegment* x;
  int m=0;

  while ((x=iter()))
    if (!x->est_vide()){
      m=x->pos_f();
      break;
    }
  while ((x=iter()))
    if (!x->est_vide())
      m=(m>=x->pos_f())?m:x->pos_f();

  return m;
}

int Cposopere::min_f() const
{
  Tliste_iter<Csegment> iter(_fils);
  Csegment* x;
  int m=0;

  while ((x=iter()))
    if (!x->est_vide()){
      m=x->pos_f();
      break;
    }
  while ((x=iter()))
    if (!x->est_vide())
      m=(m<=x->pos_f())?m:x->pos_f();

  return m;
}

int Cposopere::max_d() const
{
  Tliste_iter<Csegment> iter(_fils);
  Csegment* x;
  int m=0;

  while ((x=iter()))
    if (!x->est_vide()){
      m=x->pos_d();
      break;
    }
  while ((x=iter()))
    if (!x->est_vide())
      m=(m>=x->pos_d())?m:x->pos_d();

  return m;
}

int Cposopere::min_d() const
{
  Tliste_iter<Csegment> iter(_fils);
  Csegment* x;
  int m=0;

  while ((x=iter()))
    if (!x->est_vide()){
      m=x->pos_d();
      break;
    }
  while ((x=iter()))
    if (!x->est_vide())
      m=(m<=x->pos_d())?m:x->pos_d();

  return m;
}

//////////////////////////////////////////////////////////////////////
// sorties
//////////////////////////////////////////////////////////////////////

ostream& Cposopere::operator<<(ostream& s) const
{
  Tliste_iter<Csegment> iter(_fils);
  Csegment* seg;

  if ((seg=iter()))
    seg->operator<<(s);
  while ((seg=iter())){
    s  << " | ";
    seg->operator<<(s);
  }
  return s;
}

ostream& Cposopere::synt(ostream& s) const
{
  {
    Tliste_iter<Csegment> iter(_fils);
    Csegment* seg;

    if ((seg=iter()))
      seg->operator<<(s);
    while ((seg=iter())){
      s  << " | ";
      seg->operator<<(s);
    }
  }
  return s;
}


