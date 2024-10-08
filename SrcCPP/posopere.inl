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

#include "R_lex.h"
#include "position.h"
#include "S_lex.h"
#include "Smotif_Noeud_val.h"
#include "posi_motif.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Cposopere::~Cposopere()
{
  _fils.nettoie();
}

void Cposopere::ajoute(const Csegment& s)
{
  _fils.aj_fin(s);
}

//////////////////////////////////////////////////////////////////////
// recuperations (private)
//////////////////////////////////////////////////////////////////////

void Cposopere::recup_max(const R_lex<Cposition>& Lc, 
		     Cposition& posdeb, Cposition& posfin)
{
  _fils.aj_fin();
  _fils.ind_fin()->recup_max(Lc,posdeb,posfin);
}

void Cposopere::recup_max(const S_lex<Cposition>& Lc, 
			  Cposition& posdeb, Cposition& posfin)
{
  Tliste_iter<Smotif_Noeud_val<Cposition> > iter(Lc._vocces);
  Smotif_Noeud_val<Cposition> *x;
  int deb=posdeb.pos()+1;
  int fin=posfin.pos();

  double m=Lc.val_max();

  Csegment s(deb, fin, m);

  while ((x=iter()))
    if (m==x->val()){
      s.recup_mot(x->pt()->justif_g(fin-deb));
      _fils.aj_fin(s);
    }
}

void Cposopere::recup_max(const Smotif_Noeud_val<Cposi_motif>& Np, 
			  Cposi_motif& pomdeb, Cposi_motif& pomfin)
{
  _fils.aj_fin();
  Csegment* ps=_fils.ind_fin();
  ps->g_pos_d(pomdeb.pos()+1);
  ps->g_pos_f(pomfin.pos());
  ps->g_val(Np.val());
  ps->recup_mot(*pomdeb.pt());
}





