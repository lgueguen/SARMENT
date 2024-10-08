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

#if !defined _LEXIQUE_H
#define _LEXIQUE_H

#include <ctype.h>

#include "motif.h"
#include "Tliste.h"

template<class S> class S_lex;
template<class R> class R_lex;
template<class R> class RM_lex;
template<class R> class Rmotif_Noeud_val;
template<class S> class Smotif_Noeud_val;
class Cposition;
class Cposi_motif;
class Fsequence;
class Fmatrice;
class Fsequence;
class Cmatrice;

class mat_iter;

using std::ostream;

class Clexique
{

private:

  Tliste<Cmotif> _vocab;  // le motif abc est stocke c->b->a->

  /////////////////////////////////////////////////
  //  utilises lors de la construction

  //int_3 est le numero du premier desc_comp
  //retourne le numero de celui apres le dernier cree
  bool cree_motifs(string&, const string&, Tliste<int>&);
  bool cree_motifs(const string&, Tliste<int>&);
  
  bool fich_dans_str(const string&, string&);

  void ajoute(Cmotif*, int);	
  //  ajoute les int_1 descripteurs correspondant
  //              aux Cmotif_1 a la fin
	

  ////////////////////////////////
  //  sorties

public:

  Clexique() ;
  Clexique(void*);
  Clexique(const Clexique& l);
  ~Clexique();

  bool read_fichrecup(const string&, const string& = "");
  //  depuis le fichier de nom string_1 ( sur l'alphabet dans string_2)
  //       retourne 1 en cas de succes

  bool recup(const string&, const string& = "", bool = 0);
  //  depuis string_1 ( sur l'alphabet dans string_2)
  //       retourne 1 en cas de succes
  // concatène ssi bool_3, sinon nettoie le précédent

  //////////////////////////////
  //  modifications


  //initialise les transitions entre descripteurs
  void init_trans();
  void init_trans_vers(int);


  // ajoute la transition de int_1 vers int_2
  // retourne 1 si pas de probleme, 0 sinon
  int g_trans(int, int, double);

  //ajoute un desc_comp a la liste
  void ajoute(const Cdesc_comp&);
  void ajoute(const Cdescripteur&);

  // enleve le desc_comp de numero int
  void enleve(int);

  // nettoie le lexique en enlevant les doublons dans les motifs
  // ainsi que les motifs identiques
  void met_au_net();

  void enl_fin(); 
  //enleve tout ce qui concerne le dernier motif
  void nettoie();

  bool est_vide() const;
  
  //////////////////////////////////////
  //  donnees

  // retourne le Cmotif* de numero int
  Cmotif* desc(int) const;

  //retourne le numero du Cmotif*, -1 si mauvais pointeur
  int num(const Cmotif* const) const;

  // remplit la liste avec la liste des numeros des descripteurs
  void ls_num(Tliste<int>&) const;

  // remplit la liste avec la liste des Cdesc_comp
  void liste_desc(Tliste<Cdesc_comp>&) const;
  
  /////////////////////////////
  //  calculs

  /////////////////////////////////////////
  // nombre de classes pour atteindre le max

  int nbe_max_part(const Fsequence&) const;
  int nbe_max_part(const Fmatrice&) const;

  /////////////////////////////
  // val_max 

  double val_max(const Cdescripteur* const, 
		 const Cdescripteur* const, 
		 const Cdescripteur*const ) const;

  double val_max(const mat_iter&) const;
  //  valeur de la prediction maximale a partir
  //                                     de mat_iter_1

  
  double val_max(const Fsequence& , int, int) const;
  //  valeur de la prediction maximale possible sur [int_1;int_2[
  double val_max(const Fmatrice&, int, int) const;
  //  valeur de la prediction maximale possible sur [int_1;int_2[
  
  //////////////////////////////////////////
  // val_min 

  double val_min(const Cdescripteur* const, 
		 const Cdescripteur* const, 
		 const Cdescripteur*const ) const;
  
  double val_min(const mat_iter&) const;
  //  valeur de la prediction minimale a partir de
  //                                         mat_iter_1

  //  double val_min(const Csujet* const) const;
  //  valeur de la prediction minimale sur Csujet_1

  //   double val_min(const Csegment&,
  // 		 const Csujet* const) const;
  //  valeur de la prediction minimale sur Csegment_1
  //                                       dans Csujet_1

  
  ////////////////////////////////////////
  // pred

  double pred(const Fsequence&, int d=0, int f=-1) const;
  double pred(const Fmatrice&, int d=0, int f=-1) const;
  //  valeur de la prediction d'un predicteur optimal 
  //                     sur le segment <int_2--int_3> dans Donnee_1


  // retourne la liste des log-vraisemblances des donnees en fonction
  // des nombres de segments
  Tliste<double> log_vraisemblance(const Fsequence&, int) const;
  Tliste<double> log_vraisemblance(const Fmatrice&, int) const;

  // retourne la liste des log-probabilites a posteriori des donnees
  // en fonction des nombres de segments
  Tliste<double> log_a_posteriori(const Fsequence& , int) const;
  Tliste<double> log_a_posteriori(const Fmatrice& , int) const;

  //retourne la log-vraisemblance maximale
  double llh(const Fsequence&, int d=0, int f=-1) const;
   
  int max_lg_mot() const;
  //  longueur maximale de mot
  int max_lg_motif() const;
  //  longueur maximale de motif
  int nb_desc() const;
  //  nombre de Cmotif

  ////////////////////////////////
  //  sorties

  ostream& operator<<(ostream&) const;

  void ecrit_evalue(const Fsequence&, ostream&, int d=0, int f=-1) const;
  void ecrit_evalue(const Fmatrice&, ostream&, int d=0, int f=-1) const;

  // ecrit la liste des numeros des descripteurs
  void ls_num(ostream&) const;

  ///////////////////////////////////:
  //  amities

  friend class R_lex<Cposition>;
  friend class RM_lex<Cposition>;
  friend class R_lex<Cposi_motif>;
  friend class S_lex<Cposition>;
  friend class S_lex<Cposi_motif>;
};

#endif 
