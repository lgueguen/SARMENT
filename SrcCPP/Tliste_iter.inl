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

///////////////////////////////////////
// construction/desctruction
///////////////////////////////////////

template<class T>
Tliste_iter<T>::Tliste_iter() : Liste_iter() 
{  }

template<class T>
Tliste_iter<T>::Tliste_iter(const Tliste<T>& s) : Liste_iter(s) 
{  }

template<class T>
Tliste_iter<T>::Tliste_iter(const Tliste_iter<T>& s) : Liste_iter(s) 
{  }

//////////////////////////////////////
// modifications
//////////////////////////////////////

template<class T>
void Tliste_iter<T>::init() 
{ 
  Liste_iter::init(); 
}

template<class T>
void Tliste_iter<T>::operator=(const Tliste_iter<T>& a) 
{ 
  Liste_iter::operator=(a);
}

template<class T>
void Tliste_iter<T>::colle(const Tliste<T>& s) 
{ 
Liste_iter::colle(s); 
}

template<class T>
void Tliste_iter<T>::annule() 
{ 
  Liste_iter::annule();
}

//////////////////////////////////////
// tests/calculs
//////////////////////////////////////

////////////// sur les iterateurs

template<class T>
bool Tliste_iter<T>::operator==(const Tliste_iter<T>& a) const
{ 
  return Liste_iter::operator==(a);
}
  
template<class T>
bool Tliste_iter<T>::meme_liste(const Tliste_iter<T>& a) const
{ 
  return Liste_iter::meme_liste(a);
}

/////////////// sur les objets de la liste

template<class T>
bool Tliste_iter<T>::operator==(const T* x) const
{
  T* y=Tliste_iter<T>::operator~();
  return (x==y);
}

template<class T>
T* Tliste_iter<T>::operator()()
{
  el_Tliste<T>* x;
  x=(el_Tliste<T>*) Liste_iter::operator()();
  return (x)?&x->_ind:0;
}

template<class T>
T* Tliste_iter<T>::operator++()
{
  return &((el_Tliste<T>*)Liste_iter::operator++())->_ind;
}

template<class T>
T* Tliste_iter<T>::operator++(int)
{
  return &((el_Tliste<T>*)Liste_iter::operator++())->_ind;
}

template<class T>
T* Tliste_iter<T>::operator()(const Tliste_iter<T>& a)
{
  el_Tliste<T>* x;
  x=(el_Tliste<T>*) Liste_iter::operator()(a);
  return (x)?&x->_ind:0;
}

template<class T>
T* Tliste_iter<T>::operator~() const
{
  el_Tliste<T>* x;
  x=(el_Tliste<T>*) Liste_iter::operator~();
  return (x)?&x->_ind:0;
}
