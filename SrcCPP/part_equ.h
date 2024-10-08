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

#if !defined _SEQ_EQU_H
#define _SEQ_EQU_H

#include <fstream>

#include "desc_comp.h"
#include "segment.h"
#include "posopere.h"
#include "Tliste.h"

template<class S> class S_lex;
template<class R> class R_lex;
template<class S> class S_Noeud_val;
template<class R> class R_Noeud_val;

class Cposition;
class Cposi_motif;
class Fsequence;
class Cpartition;
class Part_equ_iter;
class Parti_comp;
class Fmatrice;

class mot_ces{

  class borne{
  private:
    int _bor;
    double _val;

  public:

    inline borne();
    inline borne(void*);
    inline borne(const borne&);
    inline borne(int);
    inline ~borne() {};

    ///////////////////////
    //  modif

    inline void operator=(const borne&);
    inline void g_bor(int);
    inline void g_val(double);

    ////////////////////////
    //  donnees

    inline int bor() const;
    inline double val() const;

    ////////////////////////
    //  tests

    inline bool operator==(const borne&) const;
    inline bool operator!=(const borne&) const;
  };

private:

  Tliste<borne> _deb;
  Tliste<borne> _fin;
  Tliste<Cdesc_comp> _mot;
  Tliste<const Csegment*> _l_pseg;

  ///////////////////////////
  // modif

  inline void recupere(const Csegment&);
  //  initialisation a partir d'un segment

  void croit();
  //  trie les listes de borne par ordre croissant 
  void evalue(const Fsequence&);
  //   evalue les borne

  inline void met_au_point(const Fsequence&);
  //  fait les deux ci-dessus

  // sur les listes de borne
  inline void aj_deb(int);
  inline void aj_fin(int);
  inline void aj_deb_diff(int);
  inline void aj_fin_diff(int);

 
  ////////////////////////////
  //  donnees

  inline Tliste<Cdesc_comp>& mot();

  ////////////////////////////
  //  tests

  inline bool pointe(const Csegment*) const;
  //  retourne true ssi Csegment*_1 est dans _l_pseg

  inline bool operator==(const Tliste<Cdesc_comp>&) const;
  //  retourne true ssi meme mot


  ////////////////////////////
  //  calculs

  inline int min_deb() const;
  inline int max_deb() const;
  inline int min_fin() const;
  inline int max_fin() const;

public:
  inline mot_ces();
  inline mot_ces(int);
  inline ~mot_ces();
  inline mot_ces(const mot_ces&);


  ///////////////////////////
  //  amities

  friend class Part_equ;
  friend class Cposopere;
};



////////////////////////////////////////////////////
///////////////////////////////////////////////////
////////////////////////////////////////////////////



class Part_equ 
{
private:
  
  int _lg;
  double _val;
  Cposopere* _seq;		

  ////////////////////////////
  //  recuperations

  void recupere(const Fsequence&, int, 
		const S_Noeud_val<Cposition>&, 
		S_lex<Cposition >&);
  void recupere(const Fmatrice&, int, 
		const S_Noeud_val<Cposition>&, 
		S_lex<Cposition >&);

  void recupere(const Fsequence&, int, 
		const S_Noeud_val<Cposition>& C);
  void recupere(const Fmatrice&, int, 
		const S_Noeud_val<Cposition>& C);

  void recupere(const Fsequence&, int, 
		const S_Noeud_val<Cposi_motif>&,
		S_lex<Cposi_motif >&);  
  void recupere(const Fmatrice&, int, 
		const S_Noeud_val<Cposi_motif>&,
		S_lex<Cposi_motif >&);
  
  void recupere(const Fsequence&, int,
		const R_Noeud_val<Cposition>&, 
		R_lex<Cposition >&);
  void recupere(const Fmatrice&, int,
		const R_Noeud_val<Cposition>&, 
		R_lex<Cposition >&);

  void recupere(const Fsequence&, 
		int,
		const R_Noeud_val<Cposition>&);
  void recupere(const Fmatrice&, 
		int,
		const R_Noeud_val<Cposition>&);

  bool recup(ifstream&, ifstream&);		  
  //  a partir de flots


  /////////////////////////////
  //   sorties

  void dessine_une_part(double, double, ofstream&) const;
  //   une partition entre double_1 et double_2



public:

  inline Part_equ();
  inline Part_equ(void*);
  Part_equ(const Part_equ&);

  ~Part_equ();


  ////////////////////////////
  //  modifications
 		
  inline void g_val(const double);

  void operator=(const Part_equ&);
  void operator=(const Cpartition&);

  bool exhaustive(const Part_equ&, const Fsequence&);
  // reconstruit l'ensemble des segments a partir
  // de la sequence en classes d'equivalence


  ///////////////////////////
  //  recuperation
  
  bool recup(const string&);
  //  a partir d'un nom de fichier


  ///////////////////////////
  //  donnees
  
  inline int taille() const;
  //  nombre de Cposopere
  inline int dern_pos() const;
  inline double val() const;

  void liste_cesures(Tliste<Cposition>&) const;
  // met dans Tliste<Cposition>_1 la liste des cesures


  ////////////////////////////////
  //  fonctions d'evaluation

  double nbe_segments() const;
  //  le nombre de segments
  double nbe_cesures() const;
  //  le nombre de cesures
  double moy_nbe_segments() const;
  //  le nombre moyen de segments par Cposopere
  double moy_nbe_cesures() const;
  //  le nombre moyen de cesures par Cposopere
  double moy_pred_diff() const;
  //  le nombre moyen de predicteurs differents par position
  double var_bornes() const;
  //  somme des variances sur les bornes des Cposopere

  ///////////////////////////////////
  //  sorties


  ostream& operator<<(ostream&) const;
  ostream& synt(ostream&) const;
  
  ////////////////////////////////
  //  amities

  friend class Parti_comp;
  friend class Part_equ_iter;
};

#include "part_equ.inl"

#endif 

