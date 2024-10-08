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

#if !defined _PARTITIONNEMENT_H
#define _PARTITIONNEMENT_H

#include <fstream>


#include "Tliste.h"
#include "partition.h"
#include "Arret.h"

class Csegment;
class Parti_comp;
class Fonction_sg;

class Cparti_simp 
{
  //  typedef double (Cpartition::*FCS) (const Csujet&) const;

  // valeur maximale (toute la sequence est partagee)
  double _vmax;
  // nombre de classes pour la prédiction maximale
  //  (0 si pas de lexique)
  int _npmax;
  
  Tliste<Cpartition> _hist;


public:

  Cparti_simp();
  Cparti_simp(void*);
  ~Cparti_simp();
  Cparti_simp(const Cparti_simp&);

  void nettoie();
  
  //////////////////////////////////
  //  calculs/donnees

  Cpartition* dern_seq() const;
  Cpartition* prem_seq() const;
	

  //pour int_1 entre 1 et nb_seq()
  Cpartition* seq(int) const;
  
  int nb_seq() const;
  double val_max() const;
  int np_max() const;
  
  //////////////////////////////////////////////
  // recuperations

//   void select_min(const Parti_comp&, Fonction_sg&);
//   void select_max(const Parti_comp&, Fonction_sg&);

  void recup(const Parti_comp&);
  bool recup(ifstream&);
  bool recup(const string&);

  // bool = 1 pour stocker l'histoire	
  // retourne le temps du lissage, 0 si echec
  Cpartition* extrait_prem() const;


  /////////////////////////////////////////////
  ///       sorties 

  ostream& operator<<(ostream&) const;

  ///////////////////////////
  // amities

};

#endif 
