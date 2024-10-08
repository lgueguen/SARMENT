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

#include "segment.h"

Part_equ::Part_equ() : _lg(0), _seq(0)
{
}

Part_equ::Part_equ(void*) : _lg(0), _seq(0)
{
}

///////////////////////////////////
//  donnees

int Part_equ::taille() const
{
  return(_lg);
}

int Part_equ::dern_pos() const
{
  return(_seq[_lg-1].max_f());
}

double Part_equ::val() const
{
  return _val;
}

/////////////////////////////////
//  modifications

void Part_equ::g_val(const double x)
{
  _val=x;
}


////////////////////////////////////////////////////////////////////////////
/////////////////////  mot_ces  ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////


mot_ces::mot_ces()
{
}

mot_ces::mot_ces(int)
{
}

mot_ces::mot_ces(const mot_ces& mc)
{
  cerr << "appel inopine de  mot_ces::mot_ces(const mot_ces& mc)" << endl;
}

mot_ces::~mot_ces()
{
  _mot.nettoie();
  _deb.nettoie();
  _fin.nettoie();
}

///////////////////////////////////
//  modif

void mot_ces::recupere(const Csegment& s)
{
  s.mot_en(_mot,0);
  borne b;
  b.g_bor(s.pos_d());
  _deb.aj_fin(b);
  b.g_bor(s.pos_f());
  _fin.aj_fin(b);
  _l_pseg.aj_fin(&s);
}

void mot_ces::met_au_point(const Fsequence& s)
{
  croit();
  evalue(s);
}

//////////////////////////////////
// donnees

Tliste<Cdesc_comp>& mot_ces::mot()
{
  return _mot;
}

/////////////////////////////////
//  tests

bool mot_ces::operator==(const Tliste<Cdesc_comp>& mt) const
{
  return _mot==mt;
}

bool mot_ces::pointe(const Csegment* ps) const
{
  return _l_pseg.possede(ps)?1:0;
}

////////////////////////////////////
//  calculs

int mot_ces::min_deb() const
{
  Tliste_iter<borne> it(_deb);
  int m;
  borne* p;
  if ((p=it()))
    m=p->bor();
  else
    return -1;
  while ((p=it()))
    if (m>(p->bor()))
      m=p->bor();

  return m;
}

int mot_ces::max_deb() const
{
  Tliste_iter<borne> it(_deb);
  int m;
  borne* p;
  if ((p=it()))
    m=p->bor();
  else
    return -1;
  while ((p=it()))
    if (m<(p->bor()))
      m=p->bor();

  return m;
}

int mot_ces::min_fin() const
{
  Tliste_iter<borne> it(_fin);
  int m;
  borne* p;
  if ((p=it()))
    m=p->bor();
  else
    return -1;
  while ((p=it()))
    if (m>(p->bor()))
      m=p->bor();

  return m;
}

int mot_ces::max_fin() const
{
  Tliste_iter<borne> it(_fin);
  int m;
  borne* p;
  if ((p=it()))
    m=p->bor();
  else
    return -1;
  while ((p=it()))
    if (m<(p->bor()))
      m=p->bor();

  return m;
}

////////////////////////////////////////////////////////////////////////////
/////////////////////   borne   ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////


mot_ces::borne::borne(void *) : _bor(0), _val(0)
{
}

mot_ces::borne::borne() : _bor(0), _val(0)
{
}

mot_ces::borne::borne(int i) : _bor(i), _val(0)
{
}

mot_ces::borne::borne(const borne& e) 
  : _bor(e._bor), _val(e._val)
{
}

////////////////////////
//  donneess

int mot_ces::borne::bor() const
{
  return _bor;
}

double mot_ces::borne::val() const
{
  return _val;
}

///////////////////////
//  modif

void mot_ces::borne::g_bor(int d)
{
  _bor=d;
}

void mot_ces::borne::g_val(double v)
{
  _val=v;
}

void mot_ces::borne::operator=(const borne& e)
{
  _bor=e._bor;
  _val=e._val;
}

////////////////////////
//  tests

bool mot_ces::borne::operator==(const borne& e) const
{
  return (_bor==e._bor) & (_val==e._val);
}

bool mot_ces::borne::operator!=(const borne& e) const
{
  return (_bor!=e._bor) || (_val!=e._val);
}



