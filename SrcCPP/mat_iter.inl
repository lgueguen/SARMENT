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

#include <math.h>

mat_iter::mat_iter() : _p_mat(0), _p_vec(0)
{
}

mat_iter::mat_iter(const Fmatrice& m) : _p_mat(&m), _p_vec(m.deb())
{
}

mat_iter::mat_iter(const Fmatrice& pm,
		   Fmatrice::vector<double>* const pv)
  : _p_mat(&pm), _p_vec(pv)
{
}

mat_iter::mat_iter(const Fmatrice& pm,
		   int i)
  : _p_mat(&pm), _p_vec(pm.deb()+i)
{
}

mat_iter::mat_iter(const mat_iter& m) : 
  _p_mat(m._p_mat), _p_vec(m._p_vec)
{
}

mat_iter::~mat_iter()
{
}


///////////////////////////
//  modifications

inline void mat_iter::init()
{
  if (_p_mat)
    _p_vec=_p_mat->deb();
}

inline void mat_iter::finit()
{
  if (_p_mat)
    _p_vec=_p_mat->fin();
}

inline void mat_iter::init(int i)
{
  if (!_p_mat)
    _p_vec=0;
  else
    if (i>=_p_mat->taille())
      _p_vec=_p_mat->fin();
    else 
      if (i<=0)
	_p_vec=_p_mat->deb();
      else{
	_p_vec=&(*_p_mat)[i];
      }
}

inline void mat_iter::operator=(const mat_iter& m)
{
  _p_mat=m._p_mat;
  _p_vec=m._p_vec;
}

inline void mat_iter::colle(const Fmatrice& m)
{
  _p_mat=&m;
  _p_vec=m.deb();
}


//////////////////////////
// iteration

mat_iter mat_iter::operator+(int i) const
{
  return mat_iter(*_p_mat, _p_vec+i);
}


bool mat_iter::operator++()
{
  if (!_p_mat)
    return 0;
  if (_p_vec>=_p_mat->fin())
    return 0;
  _p_vec++;
  return 1;
}

bool mat_iter::operator()()
{
  if (!_p_mat)
    return 0;
  if (_p_vec>=_p_mat->fin())
    return 0;
  _p_vec++;
  return 1;
}

bool mat_iter::operator()(const mat_iter& m_i)
{
  if (_p_vec>=m_i._p_vec)
    return 0;
  _p_vec++;
  return 1;
}

bool mat_iter::operator--()
{
  if (!_p_mat)
    return 0;
  if (_p_vec<=_p_mat->deb())
    return 0;
  _p_vec--;
  return 1;
}

bool mat_iter::operator++(int)
{
  if (!_p_mat)
    return 0;
  if (_p_vec>=_p_mat->fin())
    return 0;
  _p_vec++;
  return 1;
}

bool mat_iter::operator+=(int i)
{
  if (!_p_mat)
    return 0;
  if (_p_vec>=_p_mat->fin())
    return 0;
  _p_vec+=i;
  if (_p_vec>_p_mat->fin())
    _p_vec=_p_mat->fin();
  return 1;
}

bool mat_iter::operator-=(int i)
{
  if (!_p_mat)
    return 0;
  if (_p_vec<=_p_mat->deb())
    return 0;
  _p_vec-=i;
  if (_p_vec<_p_mat->deb())
    _p_vec=_p_mat->deb();
  return 1;
}

bool mat_iter::operator--(int)
{
  if (!_p_mat)
    return 0;
  if (_p_vec<=_p_mat->deb())
    return 0;
  _p_vec--;
  return 1;
}

///////////////////////////////
// donnees

const Fmatrice* mat_iter::mat() const
{
  return _p_mat;
}

double mat_iter::operator()(const int i) const
{
  return (!_p_mat || ((i<0) || (i>=_p_mat->nval())))
    ?0
    :(*_p_vec)[i];
}

double mat_iter::somme(const Cdescripteur& d) const
{
  static int i;
  static double v;
  v=0;
  for (i=0;i<_p_mat->nval();i++)
    v+=((d>=_p_mat->dsc(i))
      ?(*_p_vec)[i]
      :0);
  return v;
}

double mat_iter::log_sum_exp() const
{
  static int i;
  static double v,max;

  if (_p_vec==0)
    return 0;
  max=(*_p_vec)[0];
  for (i=1;i<_p_mat->_nval;i++)
    if (max<(*_p_vec)[i])
      max=(*_p_vec)[i];
  v=0;
  for (i=0;i<_p_mat->_nval;i++)
    v+=exp((*_p_vec)[i]-max);
  return max+log(v);
}

///////////////////////////////
// tests

bool mat_iter::fin() const
{
  return (_p_mat && _p_vec==_p_mat->_fin)
    ?1
    :0;
}

bool mat_iter::deb() const
{
  return (_p_mat && _p_vec==_p_mat->_seq)
    ?1
    :0;
}

bool mat_iter::operator<=(const mat_iter& m) const
{
  return (
	  (!_p_mat && !m._p_mat)
	  ?0
	  :((!_p_mat || !m._p_mat)
	    ?1
	    :_p_vec<=m._p_vec));
}

bool mat_iter::operator<(const mat_iter& m) const
{
  return (
	  (!_p_mat && !m._p_mat)
	  ?0
	  :((!_p_mat || !m._p_mat)
	    ?1
	    :_p_vec<m._p_vec));
}

bool mat_iter::operator>=(const mat_iter& m) const
{
  return (
	  (!_p_mat && !m._p_mat)
	  ?0
	  :((!_p_mat || !m._p_mat)
	    ?1
	    :_p_vec>=m._p_vec));
}

bool mat_iter::operator>(const mat_iter& m) const
{
  return (
	  (!_p_mat && !m._p_mat)
	  ?0
	  :((!_p_mat || !m._p_mat)
	    ?1
	    :_p_vec>m._p_vec));
}

bool mat_iter::operator!=(const mat_iter& m) const
{
  return (
	  (!_p_mat && !m._p_mat)
	  ?0
	  :((!_p_mat || !m._p_mat)
	    ?1
	    :_p_vec!=m._p_vec));
}

bool mat_iter::operator==(const mat_iter& m) const
{
  return (
	  (!_p_mat && !m._p_mat)
	  ?1
	  :((!_p_mat || !m._p_mat)
	    ?0
	    :_p_vec==m._p_vec));
}

