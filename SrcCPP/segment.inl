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

#include "motif.h"

/////////////////////////
/////// construction

Csegment::Csegment() :  _pos_d(0), _val(0), _pos_f(-1)
{
}

Csegment::Csegment(void*) :  _pos_d(0), _val(0), _pos_f(-1)
{
}

Csegment::Csegment(const Csegment& s) : 
  _pos_d(s._pos_d), _val(s._val), _pos_f(s._pos_f)
{
  if (s.a_un_mot())
    {
      Tliste_iter<Cdesc_comp> iter=s._dsc_deb;
      Cdesc_comp* d;
      
      while ((d=iter(s._dsc_deb)))
	_mot.aj_fin(*d);
      ajuste();
    }
}


Csegment::Csegment(const int posd, const int posf, const double v) :
  _pos_d(posd), _val(v), _pos_f(posf)
{
}

Csegment::~Csegment()
{
  nettoie();
}

void Csegment::nettoie()
{
  _mot.nettoie();
}

////////////////////////
/////// modifications

void Csegment::operator=(const Csegment& s)
{
  if (s.est_vide())
    vide();
  else {
    _pos_d=s._pos_d;
    _pos_f=s._pos_f;
    _val=s._val;
		
    _mot.nettoie();
    
    if (s.a_un_mot()){
      Tliste_iter<Cdesc_comp> iter=s._dsc_deb;
      Cdesc_comp* d;

      while ((d=iter(s._dsc_deb)))
	_mot.aj_fin(*d);
      
      ajuste();
    }
  }
}

void Csegment::vide_mot()
{
  _mot.nettoie();
}

void Csegment::vide()
{
  _val=0;
  g_pos_d(0);
  g_pos_f(-1);
  _mot.nettoie();
}

void Csegment::ajuste()
{
  if (!a_un_mot())
    return;

  _dsc_deb.colle(_mot);
  _dsc_fin=_dsc_deb;

  int j= (pos_f() - pos_d()) % lg_mot();
  for(;j>0;j--) _dsc_fin++;
}

/////  sur une donnee membre

void Csegment::recup_mot(const Cmotif& m)
{
  //  Cdesc_comp *pd;
  _mot.nettoie();
  
  _mot.aj_fin(m);
  //  pd=_mot.ind_fin();
  //  (*pd)=
  Cmotif* s=m.su();

  while (s!=&m){
    _mot.aj_fin(*s);
    s=s->su();
  }
  ajuste();
}

void Csegment::recup_mot(const Tliste_iter<Cdesc_comp>& i)
{
  _mot.nettoie();
  Tliste_iter<Cdesc_comp> it(i);
  Cdesc_comp* p;
  while((p=it(i)))
    _mot.aj_fin(*p);
  ajuste();
}

void Csegment::g_pos_d(const Cposition& p)
{
  _pos_d=p;
}

void Csegment::g_pos_d(const int p)
{
  _pos_d=p;
}

void Csegment::g_pos_f(const Cposition& p)
{
  _pos_f=p;
}

void Csegment::g_pos_f(const int p)
{
  _pos_f=p;
}

void Csegment::g_val(const double x) 
{ 
  _val=x;
}

/// deplacement d'un bord

double Csegment::all_d(const Cdescripteur* const pd,
		       const Cdescripteur* const po,
		       const Cdescripteur* const pf)
{
  _pos_d--;
  int i, l=lg_mot();
  double v;
  if (l==0){
    cerr << "appel illicite de double Csegment::all_d(";
    cerr << "const Cdescripteur* const,";
    cerr << "const Cdescripteur* const,";
    cerr << "const Cdescripteur* const)" << endl;
    return _val=0;
  }

  for (i=1;i<l;i++)
    _dsc_deb++;
  Cdesc_comp* d=~_dsc_deb;

  v=d->multiplie(pd, po,pf);
  _val+=v;
  return v;
}

double Csegment::rac_d(const Cdescripteur* const pd,
		       const Cdescripteur* const po,
		       const Cdescripteur* const pf)
{
  _pos_d++;
  if (!a_un_mot()){
    cerr << "appel illicite de double Csegment::rac_d(";
    cerr << "const Cdescripteur* const,";
    cerr << "const Cdescripteur* const,";
    cerr << "const Cdescripteur* const)" << endl;
    return _val=0;
  }
  Cdesc_comp* d=++_dsc_deb;
  double v=d->multiplie(pd,po,pf);
  _val-=v;
  return -v;
}

double Csegment::all_f(const Cdescripteur* const pd,
		       const Cdescripteur* const po,
		       const Cdescripteur* const pf)
{
  _pos_f++;
  if (!a_un_mot()){
    cerr << "appel illicite de double Csegment::all_f(";
    cerr << "const Cdescripteur* const,";
    cerr << "const Cdescripteur* const,";
    cerr << "const Cdescripteur* const)" << endl;
    return _val=0;
  }
  _dsc_fin++;
  Cdesc_comp *d=~_dsc_fin;
  double v=d->multiplie(pd,po,pf);
  _val+=v;
  return v;
}

double Csegment::rac_f(const Cdescripteur* const pd,
		       const Cdescripteur* const po,
		       const Cdescripteur* const pf)
{
  _pos_f--;
  if (!a_un_mot()){
    cerr << "appel illicite de double Csegment::rac_fin(";
    cerr << "const Cdescripteur* const,";
    cerr << "const Cdescripteur* const,";
    cerr << "const Cdescripteur* const)" << endl;
    return _val=0;
  }
  int i, l=lg_mot();
  Cdesc_comp* d=~_dsc_fin;
  for (i=1;i<l;i++)
    _dsc_fin++;
  double v=d->multiplie(pd,po,pf);
  _val-=v;
  return -v;
}

double Csegment::all_d(const Cdescripteur* const pd, 
		       const Cdescripteur* const od, 
		       const Cdescripteur* const of,
		       const Cdescripteur* const fin)
{
  const Cdescripteur* pt;
  Tliste_iter<Cdesc_comp> it;
  if (est_vide())
    return 0;
  if (!a_un_mot()){
    cerr << "appel illicite de double Csegment::all_deb(const ";
    cerr << "Cdescripteur* const, const Cdescripteur* const, ";
    cerr << "const Cdescripteur* const)" << endl;
    return _val=0;
  }

  int i,l=lg_mot();
  i=0;
  for (pt=od;pt<=of;++pt)
    ++i;
  _pos_d-=i;

  i%=l;
  for (;i<l;i++)
    ++_dsc_deb;
		
  it=_dsc_deb;
  double v=0;
  for (pt=od;pt<=of;++pt)
    v+=(++it)->multiplie(pd, pt, fin);
  _val+=v;
  return v;
}

double Csegment::rac_d(const Cdescripteur* const pd, 
		       const Cdescripteur* const od, 
		       const Cdescripteur* const of,
		       const Cdescripteur* const fin)
{
  const Cdescripteur* pt;
  if (est_vide())
    return 0;
  if (!a_un_mot()){
    cerr << "appel illicite de double Csegment::rac_deb(const ";
    cerr << "Cdescripteur* const, const Cdescripteur* const, ";
    cerr << "const Cdescripteur* const)" << endl;
    return _val=0;
  }

  double v=0;
  for (pt=od;pt<=of;++pt){
    ++_pos_d;
    v+=(++_dsc_deb)->multiplie(pd, pt, fin);
  }
  _val-=v;
  return -v;
}

double Csegment::all_f(const Cdescripteur* const pd, 
			 const Cdescripteur* const od, 
			 const Cdescripteur* const of,
			 const Cdescripteur* const fin)
{
  const Cdescripteur* pt;
  if (est_vide())
    return 0;
  if (!a_un_mot()){
    cerr << "appel illicite de double Csegment::all_fin(const ";
    cerr << " Cdescripteur* const, const Cdescripteur* const,";
    cerr << " const Cdescripteur* const)" << endl;
    return _val=0;
  }

  double v=0;
  for (pt=od;pt<=of;++pt){
    ++_pos_f;
    v+=(++_dsc_fin)->multiplie(pd, pt, fin);
  }
  _val+=v;
  return v;
}

double Csegment::rac_f(const Cdescripteur* const pd, 
			 const Cdescripteur* const od, 
			 const Cdescripteur* const of,
			 const Cdescripteur* const fin)
{
  const Cdescripteur* pt;
  Tliste_iter<Cdesc_comp> it;
  if (est_vide())
    return 0;
  if (!a_un_mot()){
    cerr << "appel illicite de double Csegment::rac_fin(const ";
    cerr << "Cdescripteur* const, const Cdescripteur* const,";
    cerr << " const Cdescripteur* const)" << endl;
    return _val=0;
  }

  int i,l=lg_mot();
  i=0;
  for (pt=od;pt<=of;++pt)
    ++i;
  _pos_f-=i;

  i%=l;
  for (;i<l;i++)
    ++_dsc_fin;
		
  it=_dsc_fin;
  double v=0;
  for (pt=od;pt<=of;++pt)
    v+=(++it)->multiplie(pd, pt, fin);
  _val-=v;
  return -v;
}


//////////////////////
//     tests       ///

bool Csegment::est_vide() const
{
  return (_pos_d>_pos_f);
}

bool Csegment::a_un_mot() const
{
  return (!_mot.est_vide());
}

bool Csegment::operator==(const Csegment& s) const
{
  bool tem=(_val==s._val) && (_pos_d==s._pos_d)
    && (s._pos_f==_pos_f);

  if (tem){
    if (!a_un_mot())
      return (!s.a_un_mot());
    Tliste_iter<Cdesc_comp> iter(_dsc_deb), iterp(s._dsc_deb);
    Cdesc_comp *d, *dp;
    d=iter(_dsc_deb);
    dp=iterp(s._dsc_deb);
    while (tem && d && dp){
      tem=(*d==*dp);
      d=iter(_dsc_deb);
      dp=iterp(s._dsc_deb);
    }
    return(tem && !(d || dp));
  }
  else
    return 0;
}

bool Csegment::operator!=(const Csegment& s) const
{
  return !Csegment::operator==(s);
}

bool Csegment::deb_entre(const Cposition& d, const Cposition& f) const
{
  return _pos_d.entre(d,f);
}

//////////////////////////
///     calculs        ///

Cdesc_comp& Csegment::operator[](int i) const
{
  if (!a_un_mot()){
    cerr << "appel illicite de Cdesc_comp& Csegment::operator[](int) ";
    cerr << "const" << endl;
    exit(1);
  }

  Tliste_iter<Cdesc_comp> iter=_dsc_deb;

  int j;
  if (i>=pos_d()){
    j=(i - pos_d()) % lg_mot();
    for(;j>0;j--) iter++;
  }
  else{
    j=lg_mot()-(pos_d()-i) % lg_mot();
    for(;j>0;j--) iter++;
  }
  return *~iter;

}

double Csegment::val() const 
{
  return _val;
}

int Csegment::lg_mot() const
{
  return _mot.nb_ind();
}

int Csegment::pos_f() const
{
  return _pos_f.pos();
}

int Csegment::pos_d() const
{
  return _pos_d.pos();
}

int Csegment::taille() const
{
  return pos_f()-pos_d()+1;
}

double Csegment::compact() const
{
  return (est_vide())?0:val()/taille();
}

    
//////////////////////////////
//// pour les R_Noeuds   /////

bool Csegment::Noeud_normal() const 
{
  return 1;
}


