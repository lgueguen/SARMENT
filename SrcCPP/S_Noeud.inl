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

template<class S>
S_Noeud<S>::S_Noeud() : S(), _no(1)
{
}

template<class S>
S_Noeud<S>::S_Noeud(int) : S(), _no(1)
{
}

template<class S>
S_Noeud<S>::S_Noeud(const S& s) : S(s), _no(1)
{
}

template<class S>
S_Noeud<S>::S_Noeud(const S_Noeud<S>& C) : S(C), _no(1)
{
  Tliste_iter<S_Noeud<S>*> Citer(C._su);
  S_Noeud<S> **xC;

  while ((xC=Citer())){
    _su.aj_deb(*xC);
    (*xC)->_no++;
  }
}

template<class S>
S_Noeud<S>::~S_Noeud()
{
  net_succ();
}

template<class S>
void S_Noeud<S>::net_succ()
{
  S_Noeud<S>* x;

  while ((x=_su.enl_deb())){
    if (--(x->_no)==1)
      if (x->Noeud_normal())
	delete x;
  }
  _su.nettoie();
}

/////////////////////////////
// new/delete
/////////////////////////////

template<class S>
void* S_Noeud<S>::operator new(size_t)
{
  return Stock<10000, S_Noeud<S> >::aj_elem();
}

template<class S>
void S_Noeud<S>::operator delete(void* ad, size_t)
{
  Stock<10000, S_Noeud<S> >::enl_elem(ad);
}

///////////////////////////////
// tests
///////////////////////////////

template<class S>
bool S_Noeud<S>::a_pour_fils(const S_Noeud<S>& n) const
{
  static Tliste_iter<S_Noeud<S>*> iter;

  if (this->Noeud_normal()){
    iter.colle(_su);
    S_Noeud<S> **x;
	
    while ((x=iter()))
      if (n==**x)
	return 1;
  }
  return 0;
}

template<class S>
bool S_Noeud<S>::a_pour_pt_fils(const S_Noeud<S>& p) const
{
  static Tliste_iter<S_Noeud<S>*> iter;

  if (this->Noeud_normal()){
    iter.colle(_su);
    S_Noeud<S> **x;
	
    while ((x=iter()))
      if ((*x)->a_pour_fils(p))
	return 1;
  }
  return 0;
}

template<class S>
bool S_Noeud<S>::a_pour_fils_ou_pt_fils(const S_Noeud<S>& p) const
{
  static Tliste_iter<S_Noeud<S>*> iter;

  if (this->Noeud_normal()){
    iter.colle(_su);
    S_Noeud<S> **x;
	
    while ((x=iter()))
      if ((p==**x) || (*x)->a_pour_fils(p))
	return 1;
  }
  return 0;
}

///////////////////////////////////
// modifications
///////////////////////////////////

template<class S>
void S_Noeud<S>::g_su(S_Noeud<S>& C)
{
  _su.aj_deb(&C);
  C._no++;
}

template<class S>
bool S_Noeud<S>::g_su_diff(const S_Noeud<S>& C, S_Noeud<S>* pc)
{
  if (pc==0){
    cerr << "Probleme d'utilisation de S_Noeud<S>::g_su_diff" << endl;
    cerr << " pour l'objet" <<this;
    return 0;
  }

  bool tem=0;
  static Tliste_iter<S_Noeud<S>*> iter;
  static S_Noeud<S>** xC;

  iter.colle(C._su);
  while ((xC=iter())){
    if (!a_pour_fils_ou_pt_fils(**xC)){
      tem=1;
      pc->g_su(**xC);
    }
  }
  if (tem){
    _su.aj_deb(pc);
    pc->_no++;
  }
  return tem;
}

template<class S>
bool S_Noeud<S>::prend_de__sans(const S_Noeud<S>& C, const S_Noeud<S>& A)
{
  bool tem=0;
  static Tliste_iter<S_Noeud<S>*> iter;
  static S_Noeud<S>** xC;

  iter.colle(C._su);
  while ((xC=iter())){
    if (!A.a_pour_pt_fils(**xC)){
      tem=1;
      g_su(**xC);
    }
  }
  return tem;
}

template<class S>
void S_Noeud<S>::dev_su(S_Noeud<S>& C)
{
  net_succ();
  _su.aj_deb(&C);
  C._no++;
}

template<class S>
void S_Noeud<S>::recup_su_de(S_Noeud<S>& C)
{
  net_succ();

  static Tliste_iter<S_Noeud<S>*> iter;
  static S_Noeud<S>** xC;
	
  iter.colle(C._su);
  while ((xC=iter())){
    _su.aj_deb(*xC);
    (*xC)->_no++;
  }
}

template<class S>
void S_Noeud<S>::operator+=(S_Noeud<S>& C)
{
  S_Noeud<S> **xC, **x;
	
  static Tliste_iter<S_Noeud<S>*> iter;
  static Tliste_iter<S_Noeud<S>*> Citer;
	
  Citer.colle(C._su);
  if (_su.est_vide()){
    while ((xC=Citer())){
      _su.aj_deb(*xC);
      (*xC)->_no++;
    }
  }
  else {
    iter.colle(_su);
    while ((xC=Citer())){
      iter.init();
      while ((x=iter()))
	if ((*xC)->S::operator==(**x))
	  break;
      if (!x){
	_su.aj_deb(*xC);
	(*xC)->_no++;
      }
    }
  }
}

template<class S>
void S_Noeud<S>::operator=(S_Noeud<S>& C)
{
  S::operator=(C);
  recup_su_de(C);
}


