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

#if !defined _FMATRIX_H_
#define _FMATRIX_H_

class Csegment;

#include "descripteur.h"

using std::cerr;
using std::endl;

class Fmatrice 
{
protected:


  ////////////////////////////////////////////////////////
  // la classe Fmatrice::vector

  template<class T>
  class vector
  {
  private:
    T* _tab;
    
    inline vector();
    inline vector(const int);
    inline vector(const vector<T>&);
    inline ~vector();

    inline void nettoie();
    
    ////////////////////////////
    /// donnees

    inline T& operator[](int) const;
    
    /////////////////////////////
    ///  modifications

    // !!!! n'initialise pas a 0!!!
    inline void construit_a_vide(const int);
    
    inline void echange(vector<T>&);
    
    inline void recup_prem(const vector<T>&, const int);
    // construit depuis les au plus i premiers elements de vector
    
    inline void exponentielle(const int);
    inline void logarithme(const int);
    inline void log_normalise(const int);

    ////////////////////////////
    ///    tests

    inline bool est_vide() const;
    
    ///////////////////////////
    /// amities

    friend class Fmatrice;
    friend class Cmatrice;
    friend class mat_iter;
  };


  ////////////////////////////////////////////////////
  ////////////////////////////////////////////////////
  // la classe Fmatrice

  // la Fmatrice est sur (0,_nval-1)x(0,_nvct-1)

  int _nval;  // taille des vecteurs
  int _nvct; // nombre de vecteurs;

  vector<double>* _seq;
  vector<double>* _fin;  //dernier element _seq+_nvct-1
  
  // liste des descripteurs utilises (taille _nval)
  vector<Cdescripteur> _l_desc;

  
  ///////////////////////////
  //  donnees
  
  inline Fmatrice::vector<double>& operator[](const int) const;

  inline Fmatrice::vector<double>* deb() const;
  inline Fmatrice::vector<double>* fin() const;

public:

  Fmatrice(const Fmatrice&);
  Fmatrice(void*) {};
  
  inline Fmatrice();
  inline ~Fmatrice();
  inline void nettoie();

  //  bool recup_rel(const Fmatrice&, const Csegment&);
  void recup_rel(const Fmatrice&, int, int);
  
  void melange();
  
  //echange les colonnes int_1 et int_2

  inline void echange(int, int);
  
  ///////////////////////////
  //  donnees
  
  inline int taille() const;
  // retourne le nombre de lignes (vecteurs) _nvct

  inline int nval() const;
  // retourne le nombre de colonnes (taille des vecteurs)

  inline double operator()(const int, const int) const;
  // retourne la valeur aux coordonnees int_1, int_2
  // et 0 si coordonnees hors limites
  // int_1 entre 0 et _nval-1, int_2 entre 0 et _nvct-1
  
  inline void g_val(const double, const int, const int);
  
  inline int ligne(const Cdescripteur) const;
  inline int ligne(const char) const;
  // retourne la ligne de Cdescripteur_1
  //  et -1 si Cdescripteur_1 n'y est pas

  inline Cdescripteur& dsc(int i) const;
  inline char carac(int) const;

  /////////////////////////////////////
  ////// modif

  // met toutes les valeurs a l'exponentielle
  void exponentielle();
  
  // met toutes les valeurs au logarithme neperien
  void logarithme();
  
  // log-normalise les valeurs par position

  void log_normalise();


  /////////////////////////
  /////// sorties      ////

  ostream& operator<<(ostream&) const;
  void ecrit_evalue() const;

  
  friend class mat_iter;
  friend class Cmatrice;
};



#include "Fmatrice.inl"
#endif
