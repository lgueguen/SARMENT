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

#include "desc_comp.h"
#include <stdio.h>

Transit::Transit() : _num(0), _tx(-10000)
{
}

Transit::Transit(int n, double x) : _num(n), _tx(x)
{
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

Cdesc_comp::Cdesc_comp() : pop(0), _num(0),
			   _prec(0), t_prec(0)
{
}

Cdesc_comp::Cdesc_comp(const Cdescripteur& d): _num(0)
					     ,_prec(0)
					     ,t_prec(0)
{
  pop=new Op_feuille(d);
}

Cdesc_comp::Cdesc_comp(const Cdescripteur& d, double v): _num(0),
                                                         _prec(0),t_prec(0)
{
  pop=new Op_feuille(d,v);
}

Cdesc_comp::Cdesc_comp(const Cdesc_comp& d)
{
  pop=(d.pop)?d.pop->copie():0;
  _num=d._num;

  t_prec=d.t_prec;
  int i;
  _prec=new Transit[t_prec];
  for (i=0;i<t_prec;i++)
    _prec[i]=d._prec[i];
}

Cdesc_comp::Cdesc_comp(char d, double v): _prec(0),t_prec(0)
{
  pop=new Op_feuille(d,v);
}

Cdesc_comp::~Cdesc_comp()
{
  nettoie();
}

///////// constructions


int Cdesc_comp::construit_de(const string& st, int n, int num)
{
  _num=num;
  int i=n;
  if (pop)
    delete pop;
  
  switch(st[i]){
  case '`':
    if ((pop=new Op_mot())==0){
      cerr << "Probleme allocation dans Cdesc_comp::construit_de()"
	   << endl;
      i=-1;
    }
    else
      i++;
    break;
  case '{':
    if ((pop=new Op_tom())==0){
      cerr << "Probleme allocation dans Cdesc_comp::construit_de()"
	   << endl;
      i=-1;
    }
    else
      i++;
    break;
  case '+':
    switch(st[i+1]){
    case '(':
      if ((pop=new Op_plus())==0){
	cerr << "Probleme allocation dans Cdesc_comp::construit_de()"
	     << endl;
	i=-1;
      }
      else
	i+=2;
      break;
    case '{':
      if ((pop=new Op_plus_tom())==0){
	cerr << "Probleme allocation dans Cdesc_comp::construit_de()"
	     << endl;
	i=-1;
      }
      else
	i+=2;
      break;
    default:
      i=-1;
      break;
    }
    break;
  case '|':
    switch(st[i+1]){
    case '{':
      if ((pop=new Op_ou_tom())==0){
	cerr << "Probleme allocation dans Cdesc_comp::construit_de()"
	     << endl;
	i=-1;
      }
      else
	i+=2;
      break;
    case '(':
      if ((pop=new Op_ou())==0){
	cerr << "Probleme allocation dans Cdesc_comp::construit_de()"
	     << endl;
	i=-1;
      }
      else
	i+=2;
      break;
    case '`':
      if ((pop=new Op_ou_mot())==0){
	cerr << "Probleme allocation dans Cdesc_comp::construit_de()"
	     << endl;
	i=-1;
      }
      else
	i+=2;
      break;
    default:
      i=-1;
      break;
    }
    break;
  case '*':
    if ((pop=new Op_mult())==0){
      cerr << "Probleme allocation dans Cdesc_comp::construit_de()"
	   << endl;
      i=-1;
    }
    else
      i+=2;
    break;
  default:
    if ((pop=new Op_feuille())==0){
      cerr << "Probleme allocation dans Cdesc_comp::construit_de()"
	   << endl;
      i=-1;
    }
    break;
  }
  if ((i==-1) || (i=pop->construit_de(st,i))==-1){
    if (pop)
      delete pop;
    pop=0;
  }
  return i;
}

void Cdesc_comp::cree_prec(Tliste<int>& L)
{
  
  int n=L.nb_ind();
  if (n<=0)
    return;
  
  Transit *preca=_prec;
  int t_preca=t_prec;
  
  t_prec=n;
  if ((_prec=new Transit[n])==0){
    cerr << "pb alloc dans Cdesc_comp::cree_prec()"
	 << endl;
    exit(0);
  }
  
  Tliste_iter<int> it(L);
  int i,j, *pi;

  for (i=0;i<n;i++)
    if ((pi=it())){
      _prec[i].g_num(*pi);
      for (j=0;j<t_preca;j++)
        if (preca[j].num()==*pi){
          _prec[i].g_tx(preca[j].tx());
          break;
        }
    }

  delete[] preca;
}

bool Cdesc_comp::g_trans_de(int n, double x)
{
  int i;
  for (i=0;i<t_prec;i++)
    if (_prec[i].num()==n){
      _prec[i].g_tx(x);
      return 1;
    }
  return 0;
}

////// sorties ///////////////////////

/////ecritures

ostream& Cdesc_comp::operator<<(ostream& s) const
{
  if (pop)
    pop->operator<<(s);
  return s;
}
	
ostream& Cdesc_comp::tab(ostream& s) const
{
  int i;
  char tmp[30];
  for (i=0;i<t_prec;i++){
    sprintf(tmp,"%d",_prec[i].num());
    s << tmp << ",";
    sprintf(tmp,"%d",num());
    s << tmp << "\t";
    sprintf(tmp,"%.12g",_prec[i].tx());
    s << tmp << "\n";
  }
    return s;
}

