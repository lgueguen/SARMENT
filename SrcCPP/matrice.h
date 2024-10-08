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

#if !defined _MATRIX_H_
#define _MATRIX_H_

#include <string>
#include <sstream>

#include "Fmatrice.h"

using namespace std;

class mat_iter;
class Cpartition;
class Clexique;
class Fsequence;

class Cmatrice : public Fmatrice
{
private:
  
  vector<double>* _rfin; //dernier element ALLOUE

public:

  Cmatrice(const Fmatrice&);
  Cmatrice(void*) {};
  
  inline Cmatrice();
  ~Cmatrice() {
    nettoie();
  }
      
  inline void nettoie();

  // genere une matrice de longueur int_1, dont les descripteurs
  // sont les caracteres de str_1
  void genere(const int, const string&);
  void genere(const int, const char*, int);

  // genere une matrice de même longueur et même descripteurs
  // que celle donnée en parametre

  void imite(const Fmatrice&);
  
  //remplie par FB
  void FB(const Fsequence&, const Clexique&);
  void FB(const Fmatrice&, const Clexique&);
  
  //remplie par forward
  void forward(const Fsequence&, const Clexique&);
  void forward(const Fmatrice&, const Clexique&);
  
  //remplie par backward  
  void backward(const Fsequence&, const Clexique&);
  void backward(const Fmatrice&, const Clexique&);

  //remplie par prediction position a position
  // Dans la matrice, le numero de descripteur correspond a celui
  //  en debut de donnee, et chaque ligne tient compte des motifs
  void predictions(const Fsequence&, const Clexique&);
  void predictions(const Fmatrice&, const Clexique&);
  
  //////////////////////////////////
  //  modifications

  
  // depuis un nom de fichier
  bool recup_mat(const std::string&);
  
  ///////
  // par le lissage  de Csujet_1 en paquets
  //   si char_1=='m', fait la moyenne
  //   sinon fait la somme
  // paquets de taille int_1, tous les int_2 pas
  void smoothe(const Fsequence&, const int, const int, char);
  void smoothe(const Fmatrice&, const int, const int, char);

  /// par la difference entre positions successives m(i+1)-m(i)
  void derive(const Fsequence&);
  void derive(const Fmatrice&);

  // par la valeur cumulee le long de chaque ligne
  void integre(const Fsequence&);
  void integre(const Fmatrice&);

  // ajoute la donnee $2 en $1
  inline void ajoute_en(int, const Cdescripteur&);
  void ajoute_en(int, const mat_iter&);

  // copie la donnee $2 en $1
  inline void copie_en(int, const Cdescripteur&);
  void copie_en(int, const mat_iter&);

  // soustrait la donnee $2 en $1
  inline void ote_en(int, const Cdescripteur&);
  void ote_en(int, const mat_iter&);

  // divise en $1 par la valeur $2
  inline void divise_en_par(int, double);

  //copie
  void copie(const Fmatrice&);

  ///////////////////////////////////
  //  donnees

  inline Fmatrice::vector<double>& operator[](const int) const;

  ////////////////////////////
  //  ordres 

  friend class mat_iter;

  /////////////////////////
  /////// sorties      ////

  void ecrit_evalue() const;
};


#include "matrice.inl"
#endif
