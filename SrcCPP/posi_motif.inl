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

Cposi_motif::Cposi_motif() : _pt_mot(0)
{
}

Cposi_motif::Cposi_motif(const Cposi_motif& p) : 
	_pt_mot(p._pt_mot), Cposition(p)
{
}

//////////////////
// modifications
//////////////////

void Cposi_motif::g_motif(Cmotif* x)
{
	_pt_mot=x;
}

void Cposi_motif::operator=(const Cposi_motif& p)
{
	Cposition::operator=(p);
	_pt_mot=p._pt_mot;
}

void Cposi_motif::surj_de__sur(const Cposi_motif& p, Cmotif* q)
{
	Cposition::operator=(p);
	_pt_mot=q;
}

///////////////////
// donnees
///////////////////

Cmotif* Cposi_motif::pt() const
{ 
  return (_pt_mot);
}

void* Cposi_motif::motif() const
{ 
  return (_pt_mot);
}

///////////////////
// tests 
///////////////////

bool Cposi_motif::operator==(const Cposi_motif& p) const
{
	return (Cposition::operator==(p) && (_pt_mot==p._pt_mot));
}

bool Cposi_motif::operator!=(const Cposi_motif& p) const
{
	return (Cposition::operator!=(p) || (_pt_mot!=p._pt_mot));
}

