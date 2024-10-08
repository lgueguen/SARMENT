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

#if !defined _PARTI_COMP_H
#define _PARTI_COMP_H

#include "Tliste.h"
#include "part_equ.h"

class Arret;

class Clexique;

class Parti_comp  
{
  typedef double (Part_equ::*FSD) () const;

  // valeur maximale (toute la sequence est partagee)
  double _vmax;
  // nombre de classes pour la prédiction maximale
  //    (0 si pas de lexique)
  int _npmax;

  Arret* _pArr;
  Tliste<Part_equ> _hist;


  ////////////////////////////////////
  // gestion
  
  void ajuste_membres();
  // assigne leurs noms et Ctravail aux elements de l'histoire

  void moments_stables(bool*&, int) const;
  // retourne le tableau de taille int correspondant 
  // aux moments stables
  // met 1 en n-1 si n est un moment stable et 0 sinon


  ///////////////////////////////////
  // lissages

  
  // lissage complique (toutes les partitions maxi)
  void li_comp(const Fsequence&, const Clexique&, bool = 1);
  void li_comp(const Fsequence&, bool =1);

  void li_comp(const Fmatrice&, const Clexique&, bool = 1);
  void li_comp(const Fmatrice&, bool =1);

  // lissage complique sous classes d'equivalence (toutes les
  // partitions maxi)

  void li_comp_cleq(const Fsequence&, const Clexique&, bool = 1);
  void li_comp_cleq(const Fmatrice&, const Clexique&, bool = 1);


  //////////////////////////////////
  // ordres

  void ordre_construire();
  void ordre_courbe() const;


  //////////////////////////////////
  //  sorties

  void ecrit_synt(ofstream* const = 0) const;

public:
  Parti_comp();
  Parti_comp(void*);

  //int_1 est le nombre de classes pour l'arrêt
  Parti_comp(int = 0);
  
  Parti_comp(const Parti_comp&);
  ~Parti_comp();

  void nettoie();
  bool init();
  
  //////////////////////////////
  // modifications

  void g_arret(const Arret&);
  
  bool recup(ifstream&, ifstream&);
  bool recup(const string&);

  time_t exhaustive(const Parti_comp&, const Fsequence&);
   // reconstruit l'ensemble des sequences completes a partir de
   // celles de Parti_comp


  ///////////////////////////////////
  // lissages
  
  // Avec un lexique, partitionnement avec prediction 
  // et sans lexique, algorithme de Fisher avec la fonction
  // Csegment::calc_val(Cseq_prim) 
  //  bool_1 = 1   <=>   stockage du partitionnement

  // lissage simple (une seule partition maxi)

  void li_simp(const Fsequence&, const Clexique&, bool = 1);
  //  void li_simp(PyObject*,  PyObject*, const Fsequence&, bool =1);
  
  void li_simp(const Fmatrice&, const Clexique&, bool = 1);
  //  void li_simp(PyObject*,  PyObject*, const Fmatrice&, bool =1);

  //////////////////////////////////
  //  calculs/donnees

  inline Part_equ* dern_seq() const;
  inline Part_equ* prem_seq() const;

  int nb_seq() const;
  inline double val_max() const;
  inline int np_max() const;

  //////////////////////////////////
  //  ordres

  void ordre();
  time_t ordre_lisser(bool = 1);
  // bool = 1 pour stocker l'histoire 
  //          retourne 1 pour succes

  Part_equ* ordre_extraire() const;
  Part_equ* extrait_prem() const;

  ////////////////////////////////////
  ////// sorties

//   void ecrit_val(const string& = "") const;
//   void ecrit_val(ofstream&) const;

  ostream& operator<<(ostream&) const;
  
  friend class Cparti_simp;
};

#include "parti_comp.inl"

#endif 
