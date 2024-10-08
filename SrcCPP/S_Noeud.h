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

#if !defined _S_NOEUD_H
#define _S_NOEUD_H

#include "Tliste.h"
#include "Tliste_iter.h"

#include "Stock.h"

template<class S>
class S_Noeud : public S
{
private:
  Tliste<S_Noeud<S>*> _su;
  unsigned int _no;

  // pour le nettoyage des successeurs
  void net_succ();

public:

  inline S_Noeud();
  inline S_Noeud(int);
  inline S_Noeud(const S_Noeud<S>&);
  inline S_Noeud(const S&);
  inline ~S_Noeud();

  //////////////
  // new/delete
  //////////////

  //!!!! gérés par Stock
  inline void* operator new(size_t); 
  inline void operator delete(void*, size_t); 

  //////////////////
  // tests
  //////////////////

  inline bool a_pour_fils(const S_Noeud<S>&) const;
  inline bool a_pour_pt_fils(const S_Noeud<S>&) const;
  inline bool a_pour_fils_ou_pt_fils(const S_Noeud<S>&) const;

  //////////////////////
  // modifications
  //////////////////////

  inline void dev_su(S_Noeud<S>&);
  //n'aura plus que S_Noeud<S> comme noeud suivant

  inline void g_su(S_Noeud<S>&);	
  //ajoute a ses noeuds suivants S_Noeud<S>_1

  //////

  inline void operator=(S_Noeud<S>&);
  //!!! necessite S::operator=()

  inline void recup_su_de(S_Noeud<S>&);	
  //n'aura plus que les suivants de S_Noeud<S>_1 comme suivants

  inline void operator+=(S_Noeud<S>&);
  //ajoute sans redondance a ses suivants ceux de S_Noeud<S>_1 

  inline bool g_su_diff(const S_Noeud<S>&, S_Noeud<S>*);	
  // On ajoute aux suivants de S_Noeud<S>_2
  //    chaque suivant de S_Noeud<S>_1 qui 
  //    n'est pas dans les suivants (ou suivants de suivants !)
  //    de this.
  // S'il y a eu ajout, on ajoute S_Noeud<S>_2
  //    aux suivants de this et on retourne 1.
  // On retourne 0 sinon.
	
  inline bool prend_de__sans(const S_Noeud<S>&, const S_Noeud<S>&);
  //Prend les noeuds suivants de S_Noeud<S>_1
  //    qui ne sont pas dans les suivants de S_Noeud<S>_2.
  // Retourne 1 s'il y en a
  // retourne 0 sinon
		
  friend class Part_equ;
};

#include "S_Noeud.inl"

#endif 
