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

/////////////////////////////////////
// Transit

int Transit::num() const
{
  return _num;
}

double Transit::tx() const
{
  return _tx;
}

void Transit::g_num(int n)
{
  _num=n;
}

void Transit::g_tx(double x)
{
  _tx=x;
}

void Transit::operator=(const Transit& t)
{
  _tx=t._tx;
  _num=t._num;
}

//////////////////////////////
// modifications

void Cdesc_comp::nettoie()
{
  if (pop)
    delete pop;
  pop=0;
  if (_prec)
    delete[] _prec;
  t_prec=0;
}

//////////////////////////////////////
// donnees

int Cdesc_comp::taille() const
{
  return (pop)?pop->taille():0;
}

int Cdesc_comp::num() const
{
  return _num;
}

void Cdesc_comp::liste_desc(Tliste<Cdescripteur>& li) const
{
  if (pop)
    pop->liste_desc(li);
}

bool Cdesc_comp::possede(const Cdescripteur& c) const
{
  return (pop)?pop->possede(c):0;
}

bool Cdesc_comp::est_alpha() const
{
  return (pop)?pop->est_alpha():0;
}

///////////////////////////////////////////
//	affectations

void Cdesc_comp::operator=(const Cdesc_comp& d)
{
  if (pop)
    delete pop;
  pop=d.pop->copie();
}

void Cdesc_comp::operator=(const Cdescripteur& d)
{
  if (pop)
    delete pop;
  pop=new Op_feuille(d);
}

void Cdesc_comp::g_num(int i)
{
  _num=i;
}

void Cdesc_comp::diffuse(double x)
{
  if (pop)
    pop->diffuse(x);
}
  
///////////////  comparaisons ///////////////

/// Cdescripteur

double Cdesc_comp::multiplie(const Cdescripteur* const poi ,
			     const Cdescripteur* const pod, 
			     const Cdescripteur* const pof) const
{
  return (pop)?pop->multiplie(poi,pod,pof):0;
}

/// dans une matrice

double Cdesc_comp::multiplie(const mat_iter& m_i) const
{
  return (pop)?pop->multiplie(m_i):0;
}


///		entre Cdesc_comp

bool Cdesc_comp::operator==(const Cdesc_comp& d) const
{
  return ((pop)
	  ?pop->operator==(d.pop)
	  :((d.pop)
	    ?0
	    :1)
	  );
}

bool Cdesc_comp::operator!=(const Cdesc_comp& d) const
{
  return !(operator==(d));
}

bool Cdesc_comp::est_simple() const
{
  return (!pop)?0:pop->est_simple();
}

double Cdesc_comp::auto_trans() const
{
  return trans_de(_num);
}

double Cdesc_comp::trans_de(int i) const
{
  int n=0;
  while (n<t_prec){
    if (_prec[n].num()==i)
      return _prec[n].tx();
    n++;
  }
  return -10000;
}

