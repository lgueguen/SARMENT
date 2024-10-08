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

Cdescripteur& Fsequence::operator[](const int i) const
{
  return (i>=0)?*(_deb+i):*(_fin+i+1);
}

const Cdescripteur* Fsequence::deb() const
{
  return _deb;
}

const Cdescripteur* Fsequence::fin() const
{
  return _fin;
}

/////////////////////////	
/// calculs 
/////////////////////////

int Fsequence::taille() const
{
  return (_deb)?((int)(_fin-_deb))+1:0;
}

int Fsequence::vtaille() const
{
  return (_deb)?((int)(_vfin-_deb))+1:0;
}

