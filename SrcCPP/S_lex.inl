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
S_lex<S>::S_lex()
{
}

template<class S>
S_lex<S>::S_lex(const Clexique& L)
{
  _vocces.nettoie();
  _demlex.nettoie();

  Tliste_iter<Cmotif> iterl(L._vocab);

  Cmotif* m;
  Smotif_Noeud_val<S> cm;

  while ((m=iterl())){
    cm.g_pt(m);
    cm.g_motif(m);
    _vocces.aj_fin(cm);
    _demlex.aj_fin(cm);
  }
}

template<class S>
S_lex<S>::~S_lex()
{
  _vocces.nettoie();
  _demlex.nettoie();
}

////////////////////////////////
// modifications
///////////////////////////////

template<class S>
void S_lex<S>::nettoie()
{
  _vocces.nettoie();
  _demlex.nettoie();
}

template<class S>
void S_lex<S>::init(const Cdescripteur* const pd, 
		    const Cdescripteur* const od, 
		    const Cdescripteur* const of)
{
  static Tliste_iter<Smotif_Noeud_val<S> > iter;
  static Smotif_Noeud_val<S>* x;
  Tliste_iter<S_Noeud<S> > iterd(_demlex);
  iter.colle(_vocces);

  while ((x=iter()))
    x->init(pd,od, of, *iterd());
}

template<class S>
void S_lex<S>::init(const mat_iter& m_i)
{
  static Tliste_iter<Smotif_Noeud_val<S> > iter;
  static Smotif_Noeud_val<S>* x;
  Tliste_iter<S_Noeud<S> > iterd(_demlex);
  iter.colle(_vocces);

  while ((x=iter()))
    x->init(m_i, *iterd());
}

template<class S>
void S_lex<S>::plus_egale(const Cdescripteur* const pd, 
			  const Cdescripteur* const od, 
			  const Cdescripteur* const of)
{
  static Tliste_iter<Smotif_Noeud_val<S> > iterv;
  static Smotif_Noeud_val<S> *x;
	
  iterv.colle(_vocces);
  while ((x=iterv()))
    x->ajoute_su(pd, od, of);
}

template<class S>
void S_lex<S>::plus_egale(const mat_iter& m_i)
{
  static Tliste_iter<Smotif_Noeud_val<S> > iterv;
  static Smotif_Noeud_val<S> *x;
	
  iterv.colle(_vocces);
  while ((x=iterv()))
    x->ajoute_su(m_i);
}

template<class S>
void S_lex<S>::cumule(const Cdescripteur* const pd,
		      const Cdescripteur* const od,
		      const Cdescripteur* const of,
		      S_Noeud_val<S>& c, S_Noeud<S>*& nv)
{
  static Tliste_iter<Smotif_Noeud_val<S> > iterv;
  static Smotif_Noeud_val<S> *x;
  iterv.colle(_vocces);

  while ((x=iterv()))
    nv = x->ajoute_su(pd,od,of,c,(nv)?nv:new S_Noeud<S>);
}

template<class S>
void S_lex<S>::cumule(const mat_iter& m_i,
		      S_Noeud_val<S>& c, S_Noeud<S>*& nv)
{
  static Tliste_iter<Smotif_Noeud_val<S> > iterv;
  static Smotif_Noeud_val<S> *x;
  iterv.colle(_vocces);

  while ((x=iterv()))
    nv = x->ajoute_su(m_i,c,(nv)?nv:new S_Noeud<S>);
}

template<class S>
void S_lex<S>::cumule_cleq(const Cdescripteur* const pd,
			   const Cdescripteur* const od,
			   const Cdescripteur* const of,
			   S_Noeud_val<S>& c, S_Noeud<S>*& nv)
{
  static Tliste_iter<Smotif_Noeud_val<S> > iterv;
  static Smotif_Noeud_val<S> *x;
  iterv.colle(_vocces);

  while ((x=iterv()))
    nv = x->ajoute_su_cleq(pd,od,of,c,(nv)?nv:new S_Noeud<S>);
}

template<class S>
void S_lex<S>::cumule_cleq(const mat_iter& m_i,
			   S_Noeud_val<S>& c, S_Noeud<S>*& nv)
{
  static Tliste_iter<Smotif_Noeud_val<S> > iterv;
  static Smotif_Noeud_val<S> *x;
  iterv.colle(_vocces);

  while ((x=iterv()))
    nv = x->ajoute_su_cleq(m_i,c,(nv)?nv:new S_Noeud<S>);
}

/////////////////////////
// donnees
/////////////////////////

template<class S>
double S_lex<S>::val_max() const
{
  static Tliste_iter<Smotif_Noeud_val<S> > iter;
  static Smotif_Noeud_val<S>* x;
  double m1, m2;
  iter.colle(_vocces);
  if ((x=iter()))
    m2=x->val();
  while ((x=iter())){
    m1=x->val();
    m2=(m2<m1)?m1:m2;
  }
  return(m2);
}

template<class S>
void S_lex<S>::Noeud_max(S_Noeud_val<S>& ce) const
{
  static Tliste_iter<Smotif_Noeud_val<S> > iter;
  static Smotif_Noeud_val<S> *x;
  iter.colle(_vocces);
  double m1, m2;

  if ((x=iter()))
    m2=x->val();
  while ((x=iter())){
    m1=x->val();
    m2=(m2<m1)?m1:m2;
  }

  iter.init();
  m1=0;
  while ((x=iter()))
    if (m2==x->val()){
      if (!m1){
	m1++;
	ce=*x;
      }
      else 
	ce+=*x;
    }
}


