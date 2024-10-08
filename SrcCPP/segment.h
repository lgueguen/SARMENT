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

#if !defined _SEGMENT_H
#define _SEGMENT_H

#include "posi_motif.h"
#include "desc_comp.h"
#include "Tliste_iter.h"
#include <string>

#include <iostream>
using namespace std;

class Fsequence;
class Clexique;
class Cmotif;
class Fmatrice;
class Cmatrice;

template<class S> class S_lex;
template<class R> class R_lex;
template<class R> class Rmotif_Noeud_val;

class Csegment
{
private:

  double _val;

  Cposition _pos_d;		// position debut
  Cposition _pos_f;		// position fin

  Tliste<Cdesc_comp> _mot;	// !!!! peut etre vide
  Tliste_iter<Cdesc_comp> _dsc_deb;	
  //                ~_dsc_deb retourne le premier descripteur
  Tliste_iter<Cdesc_comp> _dsc_fin;	
  //                ~_dsc_fin retourne le dernier descripteur 

  ///////////////////////////////
  //  modifications
  //  extraction du segment concerne par le maxi
  //     de difference connexe

  double seg_diff_cnx(const Csegment&, const Fsequence&, 
		      const Clexique&);
  double seg_diff_cnx(const Csegment&, const Fmatrice&, 
		      const Clexique&);

  ///////////////////
  //  evaluation   //

  double eval_cl(const Fsequence&) const;
  double eval_cl(const Fmatrice&) const;

  double inter_apres(const Fsequence&, const Csegment*) const;
  double inter_apres(const Fmatrice&, const Csegment*) const;

  double between(const Fsequence&) const;
  double between(const Fmatrice&) const;

  double diff_cnx(const Fsequence&, const Clexique&) const;
  double diff_cnx(const Fmatrice&, const Clexique&) const;

  //////////////////////
  // recuperation    ///

  // positions (a partir de 0) des debuts de ce segment
  //  et du segment suivant
  void recup_max(const R_lex<Cposition>&, Cposition&, Cposition&);
  
  ostream& convertit(ostream&) const;
  ostream& ecrit_num(ostream&) const;

public:

  inline Csegment();
  inline Csegment(void*);
  inline Csegment(const Csegment&);
  inline Csegment(const int, const int, const double);

  inline ~Csegment();
  inline void nettoie();
  
  ////////////////////////////////
  /// modificateurs       ////////

  inline void ajuste();

  inline void g_pos_d(const Cposition&);
  inline void g_pos_d(int);
  inline void g_pos_f(const Cposition&);
  inline void g_pos_f(int);
  inline void g_val(const double);

  inline void operator=(const Csegment&);

  // recupere un predicteur max entre int_1 et int_2
  //   les int_1 et int_2 sont les positions sur le sujet

  //  void recup_max(const Csujet* const, const Clexique&, int, int);
  
  inline void recup_mot(const Cmotif&);
  bool recup_mot(const string&);

  inline void vide_mot(); // enleve le mot
  inline void vide();     // enleve tout

  void mot_en(Tliste<Cdesc_comp>&, int) const;
  inline void recup_mot(const Tliste_iter<Cdesc_comp>&);

  // modification par ajout d'UN descripteur (2eme argument) au segment
  //     le 1er argument est le debut du Csequence
  //     le 3eme est la fin de la Csequence

  inline double all_f(const Cdescripteur* const,
		      const Cdescripteur* const,
		      const Cdescripteur* const); //retourne le GAIN
  inline double rac_f(const Cdescripteur* const,
		      const Cdescripteur* const,
		      const Cdescripteur* const); //retourne le GAIN
  inline double all_d(const Cdescripteur* const,
		      const Cdescripteur* const,
		      const Cdescripteur* const); //retourne le GAIN
  inline double rac_d(const Cdescripteur* const,
		      const Cdescripteur* const,
		      const Cdescripteur* const); //retourne le GAIN

  // modification par ajout d'un tableau de descripteurs (du
  // 2eme au 3eme argument)
  // le 1er argument est le debut du Csequence
  // le 4eme est la fin du Csequence

  inline double all_f(const Cdescripteur* const,
		      const Cdescripteur* const,
		      const Cdescripteur* const, 
		      const Cdescripteur* const); //retourne le GAIN
  inline double rac_f(const Cdescripteur* const,
		      const Cdescripteur* const,
		      const Cdescripteur* const, 
		      const Cdescripteur* const); //retourne le GAIN
  inline double all_d(const Cdescripteur* const,
		      const Cdescripteur* const,
		      const Cdescripteur* const, 
		      const Cdescripteur* const); //retourne le GAIN
  inline double rac_d(const Cdescripteur* const,
		      const Cdescripteur* const,
		      const Cdescripteur* const, 
		      const Cdescripteur* const); //retourne le GAIN

  ///////////////////////////////
  //      tests              ////

  inline bool a_un_mot() const;
  inline bool est_vide() const;
  inline bool operator==(const Csegment&) const;
  inline bool operator!=(const Csegment&) const;

  bool congru_a(const Csegment&) const;
  bool congru_a(const Tliste<Cdesc_comp>&, int) const;

  inline bool deb_entre(const Cposition&, const Cposition&) const;
  
  //////////////////////////////
  //        evaluations      ///

  inline int taille() const;
  // longueur du segment

  inline int lg_mot() const;
  int modulo(const Csegment&) const; // bca.modulo(abc)=1

  inline int pos_d() const;
  inline int pos_f() const;
  inline double val() const;

  inline Cdesc_comp& operator[](int) const;

    /////////////////////////////////
  //  calculs sur les segments   //

  double calc_val(const Fsequence&) const;
  double calc_val(const Fmatrice&) const;
  // valeur sur arg_1

  //  double calc_val(const Csujet&) const;
  
  double ecarttype(const Fsequence&) const;
  double ecarttype(const Fmatrice&) const;
  // ecarttype sur arg_1


  inline double compact() const;
  // valeur divisee par la taille

  int nbe_mm_pred(const Csegment*) const;
  //compte le nombre d'objets predits identiquement 

  ////////////////////////////////
  //     sortie texte          ///

  ostream& operator<<(ostream&) const;

  //////////////////////////////
  ////  Pour les R_Noeud    ////

  inline bool Noeud_normal() const;

  /////////////////////
  //// amities    /////

  friend class Cposopere;
  friend class Cpartition;
};

#include "segment.inl"

#endif
