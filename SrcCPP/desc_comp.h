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

#if !defined _DESC_COMP_H
#define _DESC_COMP_H

#include <string>
#include <iostream>

#include "descripteur.h"
#include "Tliste_iter.h"

#include "mat_iter.h"

#include "Op_desc.h"

using namespace std;

// doublets numero de desc_comp et taux de transition

class Transit
{
private:
  int _num;
  double _tx;

public:

  Transit();
  Transit(int, double);

  inline int num() const;
  inline double tx() const;

  inline void g_num(int);
  inline void g_tx(double);

  inline void operator=(const Transit&);
};

class Cdesc_comp 
{
private:

  Op_desc *pop;

  // numero identifiant
  int _num;

  // gestion des precedents
  
  int t_prec;
  Transit* _prec;
  
  //  Tliste<int> est la liste des num
  void cree_prec(Tliste<int>&);

  // diffusion du produit par double_1
  inline void diffuse(double);

  // destruction privee
  inline void nettoie();

public:
  
  Cdesc_comp();
  Cdesc_comp(const Cdesc_comp&);
  Cdesc_comp(const Cdescripteur&);
  Cdesc_comp(const Cdescripteur&, double);
  Cdesc_comp(char, double);

  //retourne la position en fin de lecture
  // int_2 est le debut de lecture dans la chaine
  // int_3 est le numero identifiant
  int construit_de(const string&, int, int);

  ~Cdesc_comp();

  inline void operator=(const Cdesc_comp&);
  inline void operator=(const Cdescripteur&);
  inline void g_num(int);

  /////////////////////////////////
  /// donnees                 /////

  inline int taille() const; // taille du mot de base (nbe de et + 1)
  inline int num() const;
  
  inline void liste_desc(Tliste<Cdescripteur>&) const;
  // remplit la liste avec ses descripteurs

  inline bool possede(const Cdescripteur&) const;
  inline bool est_alpha() const;
  
  //////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////
  ////////////////////////////////
  //// tests et calculs      /////

  inline bool operator==(const Cdesc_comp&) const; 
  inline bool operator!=(const Cdesc_comp&) const;
  
  inline bool est_simple() const;
  // retourne 1 ssi n'a rien de plus qu'un descripteur

  //////////////////////////////////////////////////////////////////
  // sur des Cseq_prim ou Csequence

  inline double multiplie(const Cdescripteur* const, 
			  const Cdescripteur* const, 
			  const Cdescripteur* const) const;
  // retourne la prediction en par_2 sur une sequence
  // terminant en par_3 et commencant en par_1

  inline double auto_trans() const;
  inline double trans_de(int) const;
  bool g_trans_de(int, double);

  //////////////////////////////////////////////////////////////////
  // sur des matrices

  inline double multiplie(const mat_iter&) const;
  // retourne la prediction en mat_iter_1


  //////////////////////////////////////////////////////////////////
  ////////////////////////////////
  //// sorties                ////

  ostream& operator<<(ostream&) const;
  ostream& tab(ostream&) const;

  ////////////////////////////
  /// amities            /////

  friend class Clexique;
  friend class Csegment;
};

#include "desc_comp.inl"

#endif 
