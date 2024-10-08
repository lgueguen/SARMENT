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

Cposition::Cposition() : _pos(0)
{
}

Cposition::Cposition(void*) : _pos(0)
{
}

Cposition::Cposition(const int p) : _pos(p)
{
}

Cposition::Cposition(const Cposition & p) : _pos(p._pos)
{	
}

void Cposition::g_pos(const int toto)
{
  _pos=toto;
}

int Cposition::pos() const
{
  return(_pos);
}

void Cposition::operator=(const Cposition& x)
{
  _pos=x._pos;
}

bool Cposition::operator==(const Cposition& x) const
{
  return(_pos==x._pos);
}

bool Cposition::operator!=(const Cposition& x) const
{
  return(_pos!=x._pos);
}

bool Cposition::operator<(const Cposition& x) const
{
  return(_pos<x._pos);
}

bool Cposition::operator<=(const Cposition& x) const
{
  return(_pos<=x._pos);
}

bool Cposition::operator>(const Cposition& x) const
{
  return(_pos>x._pos);
}

bool Cposition::operator>=(const Cposition& x) const
{
  return(_pos>=x._pos);
}

bool Cposition::entre(const Cposition& d, const Cposition& f) const
{
  return operator<=(f) && operator>=(d);
}


bool Cposition::Noeud_normal() const
{
  return (_pos!=0);
}

void Cposition::operator++(int) {
  _pos++;
}

void Cposition::operator--(int) {
  _pos--;
}

void Cposition::operator++() {
  ++_pos;
}

void Cposition::operator--() {
  --_pos;
}

void Cposition::operator+=(int i) {
  _pos+=i;
}

void Cposition::operator-=(int i) {
  _pos-=i;
}

