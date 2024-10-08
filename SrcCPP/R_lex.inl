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

template<class R>
R_lex<R>::R_lex() 
{
}

template<class R>
R_lex<R>::~R_lex()
{
  _vocces.nettoie();
  _demlex.nettoie();
}

template<class R>
R_lex<R>::R_lex(const Clexique& L)
{
  _vocces.nettoie();
  _demlex.nettoie();

  Tliste_iter<Cmotif> iterl(L._vocab);

  Cmotif* m;
  Rmotif_Noeud_val<R> cm;

  while ((m=iterl())){
    cm.g_pt(m);
    cm.g_motif(m);
    _vocces.aj_fin(cm);
    _demlex.aj_fin(cm);
  }
}

/////////////////////////////////
// modifications
/////////////////////////////////

template<class R>
void R_lex<R>::nettoie()
{
  _vocces.nettoie();
  _demlex.nettoie();
}

//////////////////////////////////
// initialisations

template<class R>
void R_lex<R>::init(const Cdescripteur* const pd, 
		    const Cdescripteur* const od, 
		    const Cdescripteur* const of)
{
  static Tliste_iter<Rmotif_Noeud_val<R> > iter;
  static Rmotif_Noeud_val<R>* x;
  Tliste_iter<R_Noeud<R> > iterd(_demlex);
  iter.colle(_vocces);

  while ((x=iter()))
    x->init(pd, od, of, *iterd());
}

template<class R>
void R_lex<R>::init(const mat_iter& m_i)
{
  static Tliste_iter<Rmotif_Noeud_val<R> > iter;
  static Rmotif_Noeud_val<R>* x;
  Tliste_iter<R_Noeud<R> > iterd(_demlex);
  iter.colle(_vocces);

  while ((x=iter()))
    x->init(m_i, *iterd());
}

template<class R>
void R_lex<R>::plus_egale(const Cdescripteur* const pd, 
			  const Cdescripteur* const dd, 
			  const Cdescripteur* const df)
{
  static Tliste_iter<Rmotif_Noeud_val<R> > iterv;
  static Rmotif_Noeud_val<R> *x;
	
  iterv.colle(_vocces);
  while ((x=iterv()))
    x->ajoute_su(pd, dd, df);
}

template<class R>
void R_lex<R>::plus_egale(const mat_iter& m_i)
{
  static Tliste_iter<Rmotif_Noeud_val<R> > iterv;
  static Rmotif_Noeud_val<R> *x;
	
  iterv.colle(_vocces);
  while ((x=iterv()))
    x->ajoute_su(m_i);
}

template<class R>
void R_lex<R>::egale(const mat_iter& m_i)
{
  static Tliste_iter<Rmotif_Noeud_val<R> > iterv;
  static Rmotif_Noeud_val<R> *x;
	
  iterv.colle(_vocces);
  while ((x=iterv()))
    x->egale_su(m_i);
}

template<class R>
void R_lex<R>::egale(const Cdescripteur* const pd, 
                     const Cdescripteur* const dd, 
                     const Cdescripteur* const df)
{
  static Tliste_iter<Rmotif_Noeud_val<R> > iterv;
  static Rmotif_Noeud_val<R> *x;
	
  iterv.colle(_vocces);
  while ((x=iterv()))
    x->egale_su(pd, dd, df);
}

template<class R>
void R_lex<R>::operator+=(const double d)
{
  static Tliste_iter<Rmotif_Noeud_val<R> > iterv;
  static Rmotif_Noeud_val<R> *x;
	
  iterv.colle(_vocces);
  while ((x=iterv()))
    (*x)+=d;
}


template<class R>
void R_lex<R>::cumule(const Cdescripteur* const pd,
		      const Cdescripteur* const dd,
		      const Cdescripteur* const df,
		      R_Noeud_val<R>& c, 
		      R_Noeud<R>*& nv)
{
  static Tliste_iter<Rmotif_Noeud_val<R> > iterv;
  static Rmotif_Noeud_val<R> *x;
  iterv.colle(_vocces);

  while ((x=iterv()))
    nv = x->ajoute_su(pd,dd,df,c,(nv)?nv:new R_Noeud<R>);
}

template<class R>
void R_lex<R>::cumule(const mat_iter& m_i,
		      R_Noeud_val<R>& c, 
		      R_Noeud<R>*& nv)
{
  static Tliste_iter<Rmotif_Noeud_val<R> > iterv;
  static Rmotif_Noeud_val<R> *x;
  iterv.colle(_vocces);

  while ((x=iterv()))
    nv = x->ajoute_su(m_i,c,(nv)?nv:new R_Noeud<R>);
}

/////////////////////////
// donnees
/////////////////////////

template<class R>
double R_lex<R>::val_max() const
{
  static Tliste_iter<Rmotif_Noeud_val<R> > iter;
  static Rmotif_Noeud_val<R>* x;
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

template<class R>
void R_lex<R>::Noeud_max(R_Noeud_val<R>& cv) const
{
  static Tliste_iter<Rmotif_Noeud_val<R> > iter;
  static Rmotif_Noeud_val<R> *x, *cm;
  double m1, m2;

  iter.colle(_vocces);
  if ((x=iter())){
    cm=x;
    m2=x->val();
  }
  while ((x=iter())){
    m1=x->val();
    if (m2<m1){
      cm=x;
      m2=m1;
    }
    else 
      if (m2==m1)
	if (x->su()->pos()<cm->su()->pos())
	  cm=x;
  }
  cv=*cm;
}
