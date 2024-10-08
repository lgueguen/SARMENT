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

/////////////////////////////////
///    construction/destruction
/////////////////////////////////

Liste_iter::Liste_iter()
{
  _li_c = 0;
  _el_c = 0;
}

Liste_iter::~Liste_iter()
{
}

Liste_iter::Liste_iter(const Liste& l)
{
  _li_c = &l;
  _el_c = _li_c->_fin;
}

Liste_iter::Liste_iter(const Liste_iter& i) :	
  _li_c(i._li_c), _el_c(i._el_c)
{
}

/////////////////////////////////////
// modifications
/////////////////////////////////////

void Liste_iter::colle(const Liste& l)
{
  _li_c = &l;
  _el_c = _li_c->_fin;
}

void Liste_iter::init()
{
  _el_c = _li_c->_fin;
}

void Liste_iter::operator=(const Liste_iter& i)
{
  _li_c = i._li_c;
  _el_c = i._el_c;
}

void Liste_iter::annule() {
 _el_c=0; 
}

/////////////////////////////
// tests/calculs
/////////////////////////////

bool Liste_iter::operator==(const Liste_iter& i) const
{
  return (_li_c == i._li_c) && (_el_c == i._el_c);
}

bool Liste_iter::meme_liste(const Liste_iter& i) const
{
  return (_li_c == i._li_c);
}

/////////////////////////////
// utilisation
//////////////////////////////

el_liste* Liste_iter::operator()() // retourne 0 a la fin
{
  el_liste* ret= _el_c ? (_el_c = _el_c->_so) : 0;
  if (_el_c == _li_c->_fin) _el_c = 0;
  return ret;
}

el_liste* Liste_iter::operator()(const Liste_iter& i) 
{
  el_liste* ret= _el_c ? (_el_c = _el_c->_so) : 0;
  if ((_el_c == i._el_c) || ((i._el_c==0) && (_el_c == _li_c->_fin)))
    _el_c = 0;
  return ret;
}

el_liste* Liste_iter::operator++() 
{
  return _el_c ? (_el_c = _el_c->_so) : 0;
}

el_liste* Liste_iter::operator~() const
{
  return (_el_c)? _el_c->_so : 0;
}

el_liste* Liste_iter::ici() const
{
  return (_el_c);
}




