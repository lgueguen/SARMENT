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

Cdescripteur::Cdescripteur(int i) : _dsc((char)i)
{
}

void Cdescripteur::operator=(const Cdescripteur& d)
{
	_dsc=d._dsc;
}

void Cdescripteur::operator=(const char c)
{
  _dsc=c;
}

char Cdescripteur::dsc() const
{
	return(_dsc);
}

void Cdescripteur::g_dsc(const char d) 
{
	_dsc=d;
}

bool Cdescripteur::operator!=(const Cdescripteur& d) const
{
	return(!(operator==(d)));
}

bool Cdescripteur::operator==(const Cdescripteur& d) const
{
	return(_dsc==d._dsc);
}

bool Cdescripteur::operator>(const Cdescripteur& d) const
{
	return ('!'==_dsc);
}

bool Cdescripteur::operator<(const Cdescripteur& d) const
{
	return ('!'==d._dsc);
}

bool Cdescripteur::operator>=(const Cdescripteur& d) const
{
	return((operator>(d)) || (operator==(d)));
}

bool Cdescripteur::operator<=(const Cdescripteur& d) const
{
	return((operator<(d)) || (operator==(d)));
}

bool Cdescripteur::est_alpha() const
{
  return (std::isalpha(_dsc));
}
