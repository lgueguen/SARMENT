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

#ifndef _OPERATEURS_H
#define _OPERATEURS_H

#include <string>
#include <stdlib.h>
#include <ctype.h>

#include "Tliste_iter.h"
#include "mat_iter.h"

using namespace std;

class Op_desc
{
public:
  enum OP {FEU, PLUS, PLUS_TOM, MULT, MOT, TOM, OU_TOM, INC, OU, OU_MOT};

private:

  // Cdescripteur_1 est le debut de la sequence
  // Cdescripteur_2 est la position courante
  // Cdescripteur_3 est la fin de la sequence
  virtual double multiplie(const Cdescripteur* const,
			   const Cdescripteur* const,
			   const Cdescripteur* const) const = 0;
  
  virtual double multiplie(const mat_iter&) const = 0;
  
  // Retourne une valeur pour la prediction a l'exterieur de la sequence
  virtual double horslim() const = 0;
  
  virtual const Op_desc* const choisit(const Cdescripteur* const,
				       const Cdescripteur* const,
				       const Cdescripteur* const) const
  {return this;}
  
  virtual const Op_desc* const choisit(const mat_iter&) const
  {return this;}
  
  // Pour hors limite
  virtual const Op_desc* const choisit() const
  {return this;}
  
protected:
  OP _op;
  
public:
 
  Op_desc() : _op(INC) {};

  virtual ~Op_desc() {};
  //retourne la position APRES le dernier caractere lu
  //         -1 si echec
  virtual int construit_de(const string&, int) = 0;  
  virtual void diffuse(double) = 0;
  
  // copie
  virtual Op_desc* copie() const = 0;
  
  // donnees
  virtual int taille()  const = 0;
  virtual bool est_simple()  const = 0;
  virtual void liste_desc(Tliste<Cdescripteur>&) const = 0;
  virtual bool possede(const Cdescripteur&) const =0;
  virtual bool est_alpha()  const = 0;
  virtual bool operator==(const Op_desc* const) const = 0;
  
  // sorties
  virtual ostream& operator<<(ostream&) const = 0;

  
  friend class Cdesc_comp;
  friend class Op_feuille;
  friend class Op_plus;
  friend class Op_mult;
  friend class Op_mot;
  friend class Op_tom;
  friend class Op_ou_tom;
  friend class Op_ou;
  friend class Op_ou_mot;
  friend class Op_plus_tom;
};


///////////////////////////////////////////////////
// Op_feuille

class Op_feuille : public Op_desc,
		   public Cdescripteur
{
private:

  double _val;
  
  inline double multiplie(const Cdescripteur* const,
			  const Cdescripteur* const,
			  const Cdescripteur* const) const;
  
  inline double multiplie(const mat_iter&) const ;
  
  // Retourne une valeur pour la prediction a l'exterieur de la sequence
  inline double horslim() const ;
  
public:

  inline Op_feuille();
  inline Op_feuille(const Cdescripteur&);
  inline Op_feuille(const Op_feuille&);
  inline Op_feuille(const Cdescripteur&, double);
  inline Op_feuille(char, double);
  
  inline ~Op_feuille();
  void diffuse(double);
  int construit_de(const string&, int);

  // copie
  Op_desc* copie() const;

  // donnees
  inline int taille() const ;
  inline bool est_simple() const ;
  void liste_desc(Tliste<Cdescripteur>&) const ;
  inline bool possede(const Cdescripteur&) const ;
  inline bool est_alpha() const ;
  bool operator==(const Op_desc* const) const;
  
  // sorties
  ostream& operator<<(ostream&) const;
};
 

///////////////////////////////////////////////////
/// Var_Op

class Var_Op {

private:
  Tliste_iter<Op_desc*> it;
  Op_desc** ppop;
  double val;
  
  Var_Op(const Tliste<Op_desc*>&);
  
  friend class Op_plus;
  friend class Op_plus_tom;
  friend class Op_mult;
  friend class Op_mot;
  friend class Op_tom;
};

  
//////////////////////////////////////////////////////
// Op_plus

class Op_plus : public Op_desc
{
private:

  Tliste<Op_desc*> lpop;

  Var_Op* _voo;
  
  inline double multiplie(const Cdescripteur* const,
			  const Cdescripteur* const,
			  const Cdescripteur* const) const ;

  inline double multiplie(const mat_iter&) const ;
  
  // Retourne une valeur pour la prediction a l'exterieur de la sequence
  inline double horslim() const ;
  
public:

  inline Op_plus();
  inline Op_plus(const Op_plus&);
  inline ~Op_plus();

  void diffuse(double);
  int construit_de(const string&, int);

  // copie
  Op_desc* copie() const;

  // donnees
  inline int taille() const ;
  inline bool est_simple() const;
  void liste_desc(Tliste<Cdescripteur>&) const ;
  inline bool possede(const Cdescripteur&) const ;
  inline bool est_alpha() const ;
  bool operator==(const Op_desc* const) const;

  // sorties
  ostream& operator<<(ostream&) const;
};

//////////////////////////////////////////////////////
// Op_plus_tom

class Op_plus_tom : public Op_desc
{
private:

  Tliste<Op_desc*> lpop;

  Var_Op* _voo;
  
  inline double multiplie(const Cdescripteur* const,
			  const Cdescripteur* const,
			  const Cdescripteur* const) const ;

  inline double multiplie(const mat_iter&) const ;
  
  // Retourne une valeur pour la prediction a l'exterieur de la sequence
  inline double horslim() const ;
  
public:

  inline Op_plus_tom();
  inline Op_plus_tom(const Op_plus_tom&);
  inline ~Op_plus_tom();

  void diffuse(double);
  int construit_de(const string&, int);

  // copie
  Op_desc* copie() const;

  // donnees
  inline int taille() const ;
  inline bool est_simple() const;
  void liste_desc(Tliste<Cdescripteur>&) const ;
  inline bool possede(const Cdescripteur&) const ;
  inline bool est_alpha() const ;
  bool operator==(const Op_desc* const) const;

  // sorties
  ostream& operator<<(ostream&) const;
};

//////////////////////////////////////////////////////
// Op_mult

class Op_mult : public Op_desc
{
private:

  Tliste<Op_desc*> lpop;

  Var_Op* _voo;
  
  inline double multiplie(const Cdescripteur* const,
			  const Cdescripteur* const,
			  const Cdescripteur* const) const ;

  inline double multiplie(const mat_iter&) const ;
  
  // Retourne une valeur pour la prediction a l'exterieur de la sequence
  inline double horslim() const ;
  
public:

  inline Op_mult();
  inline Op_mult(const Op_mult&);
  inline ~Op_mult();

  void diffuse(double);
  int construit_de(const string&, int);

  // copie
  Op_desc* copie() const;

  // donnees
  inline int taille() const ;
  inline bool est_simple() const;
  void liste_desc(Tliste<Cdescripteur>&) const ;
  inline bool possede(const Cdescripteur&) const ;
  inline bool est_alpha() const ;
  bool operator==(const Op_desc* const) const;

  // sorties
  ostream& operator<<(ostream&) const;
};

/////////////////////////////////////////////////////
// Op_mot

class Op_mot : public Op_desc
{
private:
  
  Tliste<Op_desc*> lpop;

  Var_Op* _voo;
  
  inline double multiplie(const Cdescripteur* const,
			  const Cdescripteur* const,
			  const Cdescripteur* const) const ;

  inline double multiplie(const mat_iter&) const ;
  
  // Retourne une valeur pour la prediction a l'exterieur de la sequence
  inline double horslim() const ;
  

public:

  inline Op_mot();
  inline Op_mot(const Op_mot&);
  
  inline ~Op_mot();
  void diffuse(double);
  int construit_de(const string&, int);

  // copie
  Op_desc* copie() const;

  // donnees
  inline int taille() const ;
  inline bool est_simple() const;
  void liste_desc(Tliste<Cdescripteur>&) const ;
  inline bool possede(const Cdescripteur&) const ;
  inline bool est_alpha() const ;
  bool operator==(const Op_desc* const) const;

  // sorties
  ostream& operator<<(ostream&) const;
};

/////////////////////////////////////////////////////
// Op_tom

class Op_tom : public Op_desc
{
private:
  
  Tliste<Op_desc*> lpop;

  Var_Op* _voo;
  
  inline double multiplie(const Cdescripteur* const,
			  const Cdescripteur* const,
			  const Cdescripteur* const) const ;

  inline double multiplie(const mat_iter&) const ;
  
  // Retourne une valeur pour la prediction a l'exterieur de la sequence
  inline double horslim() const ;
  
public:

  inline Op_tom();
  inline Op_tom(const Op_tom&);
  
  inline ~Op_tom();
  void diffuse(double);
  int construit_de(const string&, int);

  // copie
  Op_desc* copie() const;

  // donnees
  inline int taille() const ;
  inline bool est_simple() const ;
  void liste_desc(Tliste<Cdescripteur>&) const ;
  inline bool possede(const Cdescripteur&) const ;
  inline bool est_alpha() const ;
  bool operator==(const Op_desc* const) const;

  // sorties
  ostream& operator<<(ostream&) const;
};


//////////////////////////////////////////////////////
//// Couples de pointeurs d'operateurs

class Bi_pop {

private:

  Op_desc* _pop;
  Op_desc* _poc;
    
public: 
  inline Bi_pop();
  inline Bi_pop* copie();
  inline ~Bi_pop();

  inline void nettoie();
  
  friend class Op_ou_tom;
  friend class Op_ou;
  friend class Op_ou_mot;
};

/////////////////////////////////////////////////////
//  Var_op_ou


class Var_Op_ou {

private:
  Tliste_iter<Bi_pop> it;
  Bi_pop *pbi;
  const Op_desc* pop;

  Var_Op_ou(const Tliste<Bi_pop>&);
  
  friend class Op_ou_tom;
  friend class Op_ou;
  friend class Op_ou_mot;
};

  
/////////////////////////////////////////////////////
// Op_ou_tom

class Op_ou_tom : public Op_desc
{
private:  
  
  Tliste<Bi_pop> lbi;

  Var_Op_ou* _voo;
  
  inline double multiplie(const Cdescripteur* const,
			  const Cdescripteur* const,
			  const Cdescripteur* const) const ;

  inline double multiplie(const mat_iter&) const ;
  
  inline const Op_desc* const choisit(const Cdescripteur* const,
				      const Cdescripteur* const,
				      const Cdescripteur* const) const;
  
  inline const Op_desc* const choisit(const mat_iter&) const;
  
  inline const Op_desc* const choisit() const;

  // Retourne une valeur pour la prediction a l'exterieur de la sequence
  inline double horslim() const ;
  
public:

  inline Op_ou_tom();
  inline Op_ou_tom(const Op_ou_tom&);
  
  inline ~Op_ou_tom();
  void diffuse(double);
  int construit_de(const string&, int);

  // copie
  Op_desc* copie() const;

  // donnees
  inline int taille() const ;
  inline bool est_simple() const ;
  void liste_desc(Tliste<Cdescripteur>&) const ;
  inline bool possede(const Cdescripteur&) const ;
  inline bool est_alpha() const ;
  bool operator==(const Op_desc* const) const;

  // sorties
  ostream& operator<<(ostream&) const;
};

/////////////////////////////////////////////////////
// Op_ou

class Op_ou : public Op_desc
{
private:  
  
  Tliste<Bi_pop> lbi;

  Var_Op_ou* _voo;
  
  inline double multiplie(const Cdescripteur* const,
			  const Cdescripteur* const,
			  const Cdescripteur* const) const ;

  inline double multiplie(const mat_iter&) const ;
  
  inline const Op_desc* const choisit(const Cdescripteur* const,
				      const Cdescripteur* const,
				      const Cdescripteur* const) const;
  
  inline const Op_desc* const choisit(const mat_iter&) const;
  
  inline const Op_desc* const choisit() const;

  // Retourne une valeur pour la prediction a l'exterieur de la sequence
  inline double horslim() const ;
  
public:

  inline Op_ou();
  inline Op_ou(const Op_ou&);
  
  inline ~Op_ou();
  void diffuse(double);
  int construit_de(const string&, int);

  // copie
  Op_desc* copie() const;

  // donnees
  inline int taille() const ;
  inline bool est_simple() const ;
  void liste_desc(Tliste<Cdescripteur>&) const ;
  inline bool possede(const Cdescripteur&) const ;
  inline bool est_alpha() const ;
  bool operator==(const Op_desc* const) const;

  // sorties
  ostream& operator<<(ostream&) const;
};

/////////////////////////////////////////////////////
// Op_ou_mot

class Op_ou_mot : public Op_desc
{
private:  
  
  Tliste<Bi_pop> lbi;

  Var_Op_ou* _voo;
  
  inline double multiplie(const Cdescripteur* const,
			  const Cdescripteur* const,
			  const Cdescripteur* const) const ;

  inline double multiplie(const mat_iter&) const ;
  
  inline const Op_desc* const choisit(const Cdescripteur* const,
				      const Cdescripteur* const,
				      const Cdescripteur* const) const;
  
  inline const Op_desc* const choisit(const mat_iter&) const;
  
  // Retourne une valeur pour la prediction a l'exterieur de la sequence
  inline double horslim() const ;

  inline const Op_desc* const choisit() const;

public:

  inline Op_ou_mot();
  inline Op_ou_mot(const Op_ou_mot&);
  
  inline ~Op_ou_mot();
  void diffuse(double);
  int construit_de(const string&, int);

  // copie
  Op_desc* copie() const;

  // donnees
  inline int taille() const ;
  inline bool est_simple() const ;
  void liste_desc(Tliste<Cdescripteur>&) const ;
  inline bool possede(const Cdescripteur&) const ;
  inline bool est_alpha() const ;
  bool operator==(const Op_desc* const) const;

  // sorties
  ostream& operator<<(ostream&) const;
};

#include "Op_desc.inl"

#endif
