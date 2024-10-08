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

#if !defined _POSI_MOTIF_H
#define _POSI_MOTIF_H

#include "position.h"
#include "motif.h"

class Cposi_motif : public Cposition  
{
  Cmotif* _pt_mot;  //le Cmotif qui commence a droite 
                    //           de la position
public:
  
  inline Cposi_motif();
  inline Cposi_motif(const Cposi_motif&);
  ~Cposi_motif() {};
  
  //////////////////
  // modifications
  //////////////////

  inline void g_motif(Cmotif*);
  inline void operator=(const Cposi_motif&);
  
  ///////////////////
  // donnees
  ///////////////////

  inline void* motif() const;
  inline Cmotif* pt() const;

  ///////////////////
  // tests 
  ///////////////////

  inline bool operator==(const Cposi_motif&) const;
  inline bool operator!=(const Cposi_motif&) const;
  
  inline void surj_de__sur(const Cposi_motif&, Cmotif*);

  ///////////////////
  //  sortie
  ///////////////////

  inline void ecrit_num(ostream& s) const { _pt_mot->ecrit_num(s);}
};

#include "posi_motif.inl"

#endif 
