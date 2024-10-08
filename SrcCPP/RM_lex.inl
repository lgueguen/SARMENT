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
RM_lex<R>::RM_lex() 
{
}

template<class R>
RM_lex<R>::~RM_lex()
{
  nettoie();
}

template<class R>
RM_lex<R>::RM_lex(const Clexique& L) : _deb()
{
  nettoie();

  Tliste_iter<Cmotif> iterl(L._vocab);
  Cmotif* m;

  while ((m=iterl())){
    _Avoc.aj_fin(new RM_Noeud<R>(m));
    _Nvoc.aj_fin(new RM_Noeud<R>(m));
  }
}

/////////////////////////////////
// modifications
/////////////////////////////////

template<class R>
void RM_lex<R>::nettoie()
{
  Tliste_iter<RM_Noeud<R>* > it(_Avoc);
  RM_Noeud<R>** ppr;
  int lg;

  while ((ppr=it())){    
    if ((lg=(*ppr)->lg())>65000){
      (*ppr)->nettoie_sur(lg);
    }
    delete *ppr;
  }

   _Avoc.nettoie();

   it.colle(_Nvoc);
   while ((ppr=it())){
     if ((lg=(*ppr)->lg())>65000){
       (*ppr)->nettoie_sur(lg);
     }
     delete *ppr;
   }
  

   _Nvoc.nettoie();
}

//////////////////////////////////
// initialisations

template<class R>
void RM_lex<R>::init(const Cdescripteur* const oi,
		     const Cdescripteur* const od, 
		     const Cdescripteur* const of)
{
  Tliste_iter<RM_Noeud<R>* > iter(_Avoc);
  RM_Noeud<R>** pr;

  while ((pr=iter()))
    (*pr)->init(oi, od, of, _deb);
}

template<class R>
void RM_lex<R>::init(const mat_iter& m_i)
{
  Tliste_iter<RM_Noeud<R>* > iter(_Avoc);
  RM_Noeud<R>** pr;

  while ((pr=iter()))
    (*pr)->init(m_i, _deb);
}


template<class R>
void RM_lex<R>::init()
{
  Tliste_iter<RM_Noeud<R>* > iter(_Avoc);
  RM_Noeud<R>** pr;

  while ((pr=iter()))
    (*pr)->init(0,_deb);
}

///////////////////////////////////////////////
//// avancements
template<class R>
void RM_lex<R>::avance()
{
  static Tliste_iter<RM_Noeud<R>* > iter;
  static RM_Noeud<R>** pr;

  iter.colle(_Avoc);
  while ((pr=iter()))
    (*pr)->avance();
  iter.colle(_Nvoc);
  while ((pr=iter()))
    (*pr)->avance();
}

template<class R>
void RM_lex<R>::plus_egale(const Cdescripteur* const di,
			   const Cdescripteur* const dd, 
			   const Cdescripteur* const df)
{
  static Tliste_iter<RM_Noeud<R>* > iterv;
  static RM_Noeud<R> **x;

  iterv.colle(_Avoc);
  while ((x=iterv()))
    (*x)->ajoute_su(di, dd, df);
}

template<class R>
void RM_lex<R>::plus_egale(const mat_iter& m_i)
{
  static Tliste_iter<RM_Noeud<R>* > iterv;
  static RM_Noeud<R> **x;

  iterv.colle(_Avoc);
  while ((x=iterv()))
    (*x)->ajoute_su(m_i);
}


template<class R>
void RM_lex<R>::cumule(const Cdescripteur* const di,
		       const Cdescripteur* const dd,
		       const Cdescripteur* const df)
{
  static Tliste_iter<RM_Noeud<R>* > iterv;
  static RM_Noeud<R> **x;

  iterv.colle(_Nvoc);

  while ((x=iterv()))
    (*x)->ajoute_su(di,dd,df,_Avoc);

  _Avoc.echange(_Nvoc);

  iterv.colle(_Nvoc);
  while ((x=iterv()))
    if ((*x)->no()>1)
      (*x)=new RM_Noeud<R>((R)(**x), (*x)->pd());
}

template<class R>
void RM_lex<R>::cumule(const mat_iter& m_i)
{
  static Tliste_iter<RM_Noeud<R>* > iterv;
  static RM_Noeud<R> **x;

  iterv.colle(_Nvoc);

  while ((x=iterv()))
    (*x)->ajoute_su(m_i,_Avoc);

  _Avoc.echange(_Nvoc);

  iterv.colle(_Nvoc);
  while ((x=iterv()))
    if ((*x)->no()>1)
      (*x)=new RM_Noeud<R>((R)(**x), (*x)->pd());
}

template<class R>
void RM_lex<R>::backward(const Cdescripteur* const di,
			 const Cdescripteur* const dd,
			 const Cdescripteur* const df)
{
  static Tliste_iter<RM_Noeud<R>* > iter;
  static RM_Noeud<R> **x;

  iter.colle(_Avoc);
  while ((x=iter()))
    (*x)->ajoute_pred(di, dd, df);

  iter.colle(_Nvoc);
  while ((x=iter()))
    (*x)->backward(_Avoc);
  
  _Avoc.echange(_Nvoc); 
}

template<class R>
void RM_lex<R>::backward(const mat_iter& m_i)
{
  static Tliste_iter<RM_Noeud<R>* > iter;
  static RM_Noeud<R> **x;

  iter.colle(_Avoc);
  while ((x=iter()))
    (*x)->ajoute_pred(m_i);

  iter.colle(_Nvoc);
  while ((x=iter()))
    (*x)->backward(_Avoc);
  
  _Avoc.echange(_Nvoc); 
}


template<class R>
void RM_lex<R>::forward(const Cdescripteur* const di,
			const Cdescripteur* const dd,
			const Cdescripteur* const df)
{
  static Tliste_iter<RM_Noeud<R>* > itera,iterb;
  static RM_Noeud<R> **x;

  iterb.colle(_Nvoc);
  while ((x=iterb())){
    (*x)->forward(_Avoc);
    (*x)->ajoute_pred(di, dd, df);
  }

  _Avoc.echange(_Nvoc); 
}

template<class R>
void RM_lex<R>::forward(const mat_iter& m_i)
{
  static Tliste_iter<RM_Noeud<R>* > itera,iterb;
  static RM_Noeud<R> **x;

  iterb.colle(_Nvoc);
  while ((x=iterb())){
    (*x)->forward(_Avoc);
    (*x)->ajoute_pred(m_i);
  }

  _Avoc.echange(_Nvoc); 
}

/////////////////////////
// donnees
/////////////////////////

template<class R>
double RM_lex<R>::val_max() const
{
  Tliste_iter<RM_Noeud<R>* > iter(_Avoc);
  RM_Noeud<R>** ppr;
  double m1, m2;

  if ((ppr=iter()))
    m2=*ppr->val();
  while ((ppr=iter())){
    m1=*ppr->val();
    m2=(m2<m1)?m1:m2;
  }
  return(m2);
}

/////////////////////////////////////
// copie des valeurs dans des listes

template<class R>
void RM_lex<R>::aj_val(Tliste<double>& l) const
{
  static Tliste_iter<RM_Noeud<R>* > iter;
  static RM_Noeud<R> **pr;

  iter.colle(_Avoc);
  while ((pr=iter()))
    l.aj_fin((*pr)->val());
} 

template<class R>
void RM_lex<R>::maj_val(Tliste<double>& l) const
{
  static Tliste_iter<RM_Noeud<R>* > iter;
  static Tliste_iter<double> itl;
  static double *pl;
  static RM_Noeud<R> **pr;

  iter.colle(_Avoc);
  itl.colle(l);
  while ((pl=itl())){
    pr=iter++;
    (*pl)=(*pr)->val();
  }
} 

////////////////////////////////////////
// reference a des noeuds

template<class R>
RM_Noeud<R>* RM_lex<R>::prem_Noeud() const
{
  return *_Avoc.ind_deb();
}

template<class R>
RM_Noeud<R>* RM_lex<R>::Noeud_max() const
{
  static Tliste_iter<RM_Noeud<R>* > iter;
  static RM_Noeud<R> **ppr, *prm;
  static double m1, m2;

  iter.colle(_Avoc);
  
  if ((ppr=iter())){
    prm=*ppr;
    m2=(*ppr)->val();
  }
  while ((ppr=iter())){
    m1=(*ppr)->val();
    if (m2<m1){
      m2=m1;
      prm=*ppr;
    }
    else{
      if (m2==m1)
	if ((*ppr)->su()->pos()< prm->su()->pos())
	  prm=*ppr;
    }
  }
  
  return prm;
}
