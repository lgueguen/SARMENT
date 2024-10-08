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

Part_equ* Parti_comp::dern_seq() const
{
  return _hist.ind_fin();
}

Part_equ* Parti_comp::prem_seq() const
{
  return _hist.ind_deb();
}

double Parti_comp::val_max() const
{
  return _vmax;
}

int Parti_comp::np_max() const
{
  return _npmax;
}
