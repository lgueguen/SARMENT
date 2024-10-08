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
Smotif_Noeud_val<S>::Smotif_Noeud_val() : _pt_mot(0)
{
}

template<class S>
Smotif_Noeud_val<S>::Smotif_Noeud_val(int) : _pt_mot(0)
{
}

template<class S>
Smotif_Noeud_val<S>::Smotif_Noeud_val(const Smotif_Noeud_val<S>& m) 
  : S_Noeud_val<S>(m), _pt_mot(m._pt_mot)
{
}

///////////////////////////////
// initialisations
///////////////////////////////

template<class S>
void Smotif_Noeud_val<S>::init(const Cdescripteur* const pd,
			       const Cdescripteur* const od,
			       const Cdescripteur* const of, 
			       S_Noeud<S>& demar)
{
  S::init();
  _pt_mot=(Cmotif*)((demar.motif())?demar.motif():_pt_mot);
  this->g_val(_pt_mot->multiplie(pd,od,of));
  this->dev_su(demar);
  S::avance();
}

template<class S>
void Smotif_Noeud_val<S>::init(const mat_iter& m_i,
			       S_Noeud<S>& demar)
{
  S::init();
  _pt_mot=(Cmotif*)((demar.motif())?demar.motif():_pt_mot);
  this->g_val(_pt_mot->multiplie(m_i));
  this->dev_su(demar);
  S::avance();
}

//////////////////////////
// donnees
//////////////////////////

template<class S>
Cmotif* Smotif_Noeud_val<S>::pt()
{ 
  return (_pt_mot); 
}

///////////////////////////
// modifications
///////////////////////////

template<class S>
void Smotif_Noeud_val<S>::g_pt(Cmotif* m)
{
  _pt_mot=m;
}

template<class S>
void Smotif_Noeud_val<S>::operator=(Smotif_Noeud_val<S>& mc)
{
  S_Noeud_val<S>::operator=(mc);
  _pt_mot=mc._pt_mot;
}

template<class S>
void Smotif_Noeud_val<S>::ajoute_su(const Cdescripteur* const pd,
				   const Cdescripteur* const od,
				   const Cdescripteur* const of)
{
  _pt_mot=_pt_mot->su();
  S::avance();
	
  *this+=_pt_mot->multiplie(pd, od, of);
}

template<class S>
void Smotif_Noeud_val<S>::ajoute_su(const mat_iter& m_i)
{
  _pt_mot=_pt_mot->su();
  S::avance();
	
  *this+=_pt_mot->multiplie(m_i);
}

template<class S>
S_Noeud<S>* Smotif_Noeud_val<S>::ajoute_su(const Cdescripteur* const pd,
					  const Cdescripteur* const od,
					  const Cdescripteur* const of,
					  S_Noeud_val<S>& c, 
					  S_Noeud<S>* pc)
{
  _pt_mot=_pt_mot->su();
  S::avance();

  if (c.val()>=this->val()){
    *pc=c;
    if (c.val()>this->val()){
      this->dev_su(*pc);
      this->g_val(c.val());
    }
    else
      this->g_su(*pc);

    *this+=_pt_mot->multiplie(pd, od, of);
    return 0;
  }
		
  *this+=_pt_mot->multiplie(pd, od, of);
  return pc;
}

template<class S>
S_Noeud<S>* Smotif_Noeud_val<S>::ajoute_su(const mat_iter& m_i,
					  S_Noeud_val<S>& c, 
					  S_Noeud<S>* pc)
{
  _pt_mot=_pt_mot->su();
  S::avance();

  if (c.val()>=this->val()){
    *pc=c;
    if (c.val()>this->val()){
      this->dev_su(*pc);
      this->g_val(c.val());
    }
    else
      this->g_su(*pc);

    *this+=_pt_mot->multiplie(m_i);
    return 0;
  }
		
  *this+=_pt_mot->multiplie(m_i);
  return pc;
}

template<class S>
S_Noeud<S>* Smotif_Noeud_val<S>::ajoute_su_cleq(const Cdescripteur* const pd,
					       const Cdescripteur* const od,
					       const Cdescripteur* const of,
					       S_Noeud_val<S>& c, 
					       S_Noeud<S>* pc)
{
  _pt_mot=_pt_mot->su();
  S::avance();

  if (c.val()>=this->val()){
    if (c.val()>this->val()){
      pc->recup_su_de(c);
      pc->surj_de__sur(c,_pt_mot);
      this->dev_su(*pc);
      this->g_val(c.val());
      *this+=_pt_mot->multiplie(pd, od, of);
      return 0;
    }
    else
      if (pc->prend_de__sans(c, *this)){
	pc->surj_de__sur(c,_pt_mot);
	this->g_su(*pc);
	*this+=_pt_mot->multiplie(pd, od, of);
	return 0;
      }
  }			
  *this+=_pt_mot->multiplie(pd, od, of);
  return pc;
}

template<class S>
S_Noeud<S>* Smotif_Noeud_val<S>::ajoute_su_cleq(const mat_iter& m_i,
					       S_Noeud_val<S>& c, 
					       S_Noeud<S>* pc)
{
  _pt_mot=_pt_mot->su();
  S::avance();

  if (c.val()>=this->val()){
    if (c.val()>this->val()){
      pc->recup_su_de(c);
      pc->surj_de__sur(c,_pt_mot);
      this->dev_su(*pc);
      this->g_val(c.val());
      *this+=_pt_mot->multiplie(m_i);
      return 0;
    }
    else
      if (pc->prend_de__sans(c, *this)){
	pc->surj_de__sur(c,_pt_mot);
	this->g_su(*pc);
	*this+=_pt_mot->multiplie(m_i);
	return 0;
      }
  }			
  *this+=_pt_mot->multiplie(m_i);
  return pc;
}


