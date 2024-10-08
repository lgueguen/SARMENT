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
Rmotif_Noeud_val<R>::Rmotif_Noeud_val() : _pt_mot(0)
{
}

template<class R>
Rmotif_Noeud_val<R>::Rmotif_Noeud_val(void*) : _pt_mot(0) 
{
}

template<class R>
Rmotif_Noeud_val<R>::Rmotif_Noeud_val(const Rmotif_Noeud_val<R>& M) 
  : R_Noeud_val<R>(M), _pt_mot(M._pt_mot)
{
}

/////////////////////////
///   initialisations
/////////////////////////

template<class R>
void Rmotif_Noeud_val<R>::init(const Cdescripteur* const pd, 
			       const Cdescripteur* const od, 
			       const Cdescripteur* const of,
			       R_Noeud<R> & demar)
{
  R::init();
  this->g_val(_pt_mot->multiplie(pd, od, of));
  R::avance();
  this->dev_su(demar);
}

template<class R>
void Rmotif_Noeud_val<R>::init(const mat_iter& m_i, 
			       R_Noeud<R> & demar)
{
  R::init();
  this->g_val(_pt_mot->multiplie(m_i));
  R::avance();
  this->dev_su(demar);
}

////////////////////////
// donnees
////////////////////////

template<class R>
const Cmotif* Rmotif_Noeud_val<R>::pt()
{ 
  return (_pt_mot); 
}

//////////////////////////////////
// modifications
//////////////////////////////////

template<class R>
void Rmotif_Noeud_val<R>::g_pt(Cmotif* M)
{
  _pt_mot=M;
}

template<class R>
void Rmotif_Noeud_val<R>::operator=(Rmotif_Noeud_val<R>& mc)
{
  R_Noeud_val<R>::operator=(mc);
  _pt_mot=mc._pt_mot;
}

template<class R>
void Rmotif_Noeud_val<R>::ajoute_su(const Cdescripteur* const pd, 
				   const Cdescripteur* const dd, 
				   const Cdescripteur* const df)
{
  _pt_mot=_pt_mot->su();
  R::avance();
	
  *this+=_pt_mot->multiplie(pd,dd,df);
}

template<class R>
void Rmotif_Noeud_val<R>::ajoute_su(const mat_iter& m_i)
{
  _pt_mot=_pt_mot->su();
  R::avance();
	
  *this+=_pt_mot->multiplie(m_i);
}

template<class R>
void Rmotif_Noeud_val<R>::egale_su(const Cdescripteur* const pd, 
                                   const Cdescripteur* const dd, 
                                   const Cdescripteur* const df)
{
  _pt_mot=_pt_mot->su();
  R::avance();
	
  this->_val=_pt_mot->multiplie(pd,dd,df);
}

template<class R>
void Rmotif_Noeud_val<R>::egale_su(const mat_iter& m_i)
{
  _pt_mot=_pt_mot->su();
  R::avance();
  
  this->g_val(_pt_mot->multiplie(m_i));
}

template<class R>
R_Noeud<R>* Rmotif_Noeud_val<R>::ajoute_su(const Cdescripteur* const pd, 
					  const Cdescripteur* const dd, 
					  const Cdescripteur* const df,
					  R_Noeud_val<R>& c, 
					  R_Noeud<R> * pc)
{
  _pt_mot=_pt_mot->su();
  R::avance();

  if (c.val()>this->val()){
    *pc=c;
    this->dev_su(*pc);
    this->g_val(c.val());
    *this+=_pt_mot->multiplie(pd,dd,df);
    return 0;
  }
		
  *this+=_pt_mot->multiplie(pd,dd,df);

  return pc;
}

template<class R>
R_Noeud<R>* Rmotif_Noeud_val<R>::ajoute_su(const mat_iter& m_i,
					  R_Noeud_val<R>& c, 
					  R_Noeud<R> * pc)
{
  _pt_mot=_pt_mot->su();
  R::avance();

  if (c.val()>this->val()){
    *pc=c;
    this->dev_su(*pc);
    this->g_val(c.val());
    *this+=_pt_mot->multiplie(m_i);
    return 0;
  }
		
  *this+=_pt_mot->multiplie(m_i);

  return pc;
}

