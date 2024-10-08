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

#include "parti_simp.h"

#include "parti_comp.h"
#include "segment.h"

#include "Tliste_iter.h"
#include <stdio.h>

// #include "Fonctions_sg.h"
// #include "Fonctions_ps.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Cparti_simp::Cparti_simp() : _vmax(0), _npmax(0)
{
}

Cparti_simp::Cparti_simp(void*) : _vmax(0),    _npmax(0)
{
}

Cparti_simp::~Cparti_simp()
{
  nettoie();
}

Cparti_simp::Cparti_simp(const Cparti_simp& p)
  : _vmax(p._vmax), _npmax(p._npmax)
{
  cerr << "appel inopine de Cparti_simp::Cparti_simp(";
  cerr << "const Cparti_simp& P)" << endl;
}

void Cparti_simp::nettoie()
{
  _hist.nettoie();
  _vmax=0;
  _npmax=0;
}

/////////////////////////////////////////
// modifications
/////////////////////////////////////////


Cpartition* Cparti_simp::extrait_prem() const
{
  return _hist.ind_deb();
}

//////////////////////////////////////////////
// recuperations
//////////////////////////////////////////////

// void Cparti_simp::select_min(const Parti_comp& P, 
// 				  Fonction_sg& ptf)
// {
//   Tliste_iter<Part_equ> iter(P._hist);
//   Part_equ* Sp;
//   double x;
//   int i=0;
//   while (Sp=iter()){
//     _hist.aj_fin();
//     x=_hist.ind_fin()->select_min(*Sp, ptf);
//     if (++i==10){
//       cout << '|';
//       i=0;
//     }
//     else
//       cout << '.';
//     cout.flush();
//   }
//   cout << endl;
//   _vmax=P.val_max();
//   _npmax=P.np_max();
// }

// void Cparti_simp::select_max(const Parti_comp& P, 
// 				  Fonction_sg& ptf)
// {
//   Tliste_iter<Part_equ> iter(P._hist);
//   Part_equ* Sp;
//   double x;
//   int i=0;

//   while (Sp=iter()){
//     _hist.aj_fin();
//     x=_hist.ind_fin()->select_max(*Sp, ptf);
//     if (++i==10){
//       cout << '|';
//       i=0;
//     }
//     else
//       cout << '.';
//     cout.flush();
//   }
//   cout << endl;
//   _vmax=P.val_max();
//   _npmax=P.np_max();
// }

void Cparti_simp::recup(const Parti_comp& P)
{
  nettoie();
  Tliste_iter<Part_equ> iter(P._hist);
  Part_equ* Sp;

  while ((Sp=iter())){
    _hist.aj_fin();
    _hist.ind_fin()->recup(*Sp);
  }

  _npmax=P.np_max();
  _vmax=P.val_max();
}

bool Cparti_simp::recup(const string& ns)
{
  nettoie();
  if (ns.empty())
    return 0;

  ifstream flot(ns.c_str());
  if (!flot){
    cout << "pb a l'ouverture de " << ns << endl;
    return 0;
  }
  bool tem=recup(flot);
  flot.close();
  if (!tem){
    cout << "pb dans le fichier " << ns << endl;
    nettoie();
  }
  return tem;
}

bool Cparti_simp::recup(ifstream& flot)
{
  bool tem;
  Cpartition* sq;
  string st;
  int i,j;
  
  nettoie();
  tem=0;

  while (!flot.eof()){
    
    _hist.aj_fin();
    sq=_hist.ind_fin();

    if (sq->recup(flot))
      tem=1;
    else{
      _hist.tue_fin();
      getline(flot,st);
      if (((j=st.find("MAX"))!=string::npos) &&
	  ((i=st.find("--->"))!=string::npos)){
	_npmax=atoi((st.substr(j+4)).c_str());
	_vmax=atof((st.substr(i+4)).c_str());
	tem=1;
      }
      else
	tem=0;
      break;
    }
  }
  return (tem && !(_hist.est_vide()));
}


///////////////////////////////////////////////////
//  calculs/donnees
///////////////////////////////////////////////////

int Cparti_simp::nb_seq() const
{
  int i=0;
  Tliste_iter<Cpartition> iter(_hist);

  while(iter()) i++;

  return i;
}

Cpartition* Cparti_simp::dern_seq() const 
{
  return _hist.ind_fin();
}

Cpartition* Cparti_simp::prem_seq() const 
{
  return _hist.ind_deb();
}

double Cparti_simp::val_max() const
{
  return _vmax;
}

int Cparti_simp::np_max() const
{
  return _npmax;
}

/////////////////////////////////////////////
///       sorties textes 
/////////////////////////////////////////////

ostream& Cparti_simp::operator<<(ostream& s) const
{
  Cpartition* S_h;
  Tliste_iter<Cpartition> iter(_hist);
  char tmp[20];
  
  while ((S_h=iter()))
    S_h->operator<<(s);

  s << "MAX";
  if (_npmax>0){
    sprintf(tmp,"%d",_npmax);
    s << '(' << tmp << ')';
    }
  s << " ---> ";
  sprintf(tmp,"%.8f",_vmax);
  s << tmp << endl;

  return s;
}

Cpartition* Cparti_simp::seq(int i) const 
{
  if ((i>=1) && (i<=nb_seq()))
    return _hist.ind_i(i);
  else
    return 0;
}



