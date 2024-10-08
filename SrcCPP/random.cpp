// Copyright 2005 Laurent GUEGUEN

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

#include "random.h"

MTRand mtrand1;

double Rand1()  // dans [0;1[
{
  return mtrand1.randExc();
}

int Random1(int max)	// dans <0;max<
{
  return mtrand1.randInt(max-1);
}

int Random2(int min, int max)  // dans <min;max<
{
  return min + mtrand1.randInt(max - min-1);
}
