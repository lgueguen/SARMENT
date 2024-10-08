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

Cpartition::Cpartition() : _lg(0), _seq(0), _val(0)
{
}

Cpartition::Cpartition(void *) : _lg(0), _seq(0), _val(0)
{
}

Cpartition::~Cpartition()
{
  nettoie();
  _lg=0;
  _val=0;
}

void Cpartition::nettoie()
{
  int i;
  if (_seq){
    for (i=0;i<_lg;i++)
      _seq[i].nettoie();
    delete[] _seq;
  }
  _seq=0;
  _lg=0;
  _val=0;
}


/////////////////////////
// modifications 

void Cpartition::g_val(const double x) 
{ 
  _val=x;
}

//////////////////////
///    donnees

const Csegment& Cpartition::operator[](const int i) const
{
  return(_seq[i]);
}

int Cpartition::taille() const
{
  return(_lg);
}

int Cpartition::dern_pos() const
{
  return(_seq[_lg-1].pos_f());
}

double Cpartition::val() const 
{
  return _val;
}



