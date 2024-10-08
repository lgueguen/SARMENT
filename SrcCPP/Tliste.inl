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


//////////////////////////////////////////////////
/// constructeurs/destructeurs
//////////////////////////////////////////////////

template<class T>
Tliste<T>::Tliste() 
{
}

template<class T>
Tliste<T>::~Tliste() 
{ 
  nettoie();
}

template<class T>
Tliste<T>::Tliste(const Tliste<T>& l)
{
  Tliste_iter<T> iter(l);
  T* pt;

  while ((pt=iter()))
    aj_fin(*pt);
}

//////////////////////////////////////////////////
/// modifications
//////////////////////////////////////////////////

template<class T>
void Tliste<T>::nettoie()
{
  el_Tliste<T>* pt;
	
  while ((pt= (el_Tliste<T>*) Liste::enl_deb()))
    delete pt;

  Liste::nettoie();
}

template<class T>
void Tliste<T>::aj_deb(const T& a) 
{ 
  Liste::aj_deb(new el_Tliste<T>(a)); 
}

template<class T>
void Tliste<T>::aj_fin(const T& a) 
{ 
  Liste::aj_fin(new el_Tliste<T>(a)); 
}

template<class T>
void Tliste<T>::aj_fin() 
{
  Liste::aj_fin(new el_Tliste<T>);
}

template<class T>
void Tliste<T>::aj_deb() 
{
  Liste::aj_deb(new el_Tliste<T>);
}


template<class T>
T* Tliste<T>::enl_pt_deb()
{
  el_Tliste<T>* pt = (el_Tliste<T>*) Liste::enl_deb();

  if (pt){
    T* i = &(pt->_ind);
    delete pt;
    return i;
  }
  return 0;
}

template<class T>
T Tliste<T>::enl_deb()
{
  el_Tliste<T>* pt = (el_Tliste<T>*) Liste::enl_deb();

  if (pt){
    T i = pt->_ind;
    delete pt;
    return i;
  }
  return 0;
}

template<class T>
T* Tliste<T>::enl_iter(Tliste_iter<T>& i)
{
  el_Tliste<T>* pt = (el_Tliste<T>*) Liste::enl_iter(i);

  if (pt){
    T* i = &(pt->_ind);
    delete pt;
    return i;
  }
  return 0;
}

template<class T>
void Tliste<T>::tue_deb()
{
  el_Tliste<T>* pt = (el_Tliste<T>*) Liste::enl_deb();

  if (pt)
    delete pt;
}

template<class T>
T Tliste<T>::enl_fin()
{
  el_Tliste<T>* pt = (el_Tliste<T>*) Liste::enl_fin();

  if (pt){
    T i = pt->_ind;
    delete pt;
    return i;
  }
  return 0;
}

template<class T>
void Tliste<T>::tue_fin()
{
  el_Tliste<T>* pt = (el_Tliste<T>*) Liste::enl_fin();

  if (pt)
    delete pt;
}

template<class T>
void Tliste<T>::tue(T* pt)
{
  Tliste_iter<T> iter(*this);
  int i=1;
  T *b;
  el_Tliste<T>* pb;
  
  while ((b=iter()))
    if (pt==b)
      break;
    else
      i++;

  if (b){
    pb=(el_Tliste<T>*) Liste::enl_i(i);
    delete pb;
  }
}
  
template <class T>
bool Tliste<T>::aj_deb_diff(const T& a)
{
  Tliste_iter<T> iter(*this);
  T *b;

  while ((b=iter()))
    if (a==*b)
      return 0;

  aj_deb(a);
  return 1;
}

template <class T>
bool Tliste<T>::aj_fin_diff(const T& a)
{
  Tliste_iter<T> iter(*this);
  T *b;

  while ((b=iter()))
    if (a==*b)
      return 0;

  aj_fin(a);
  return 1;
}

template <class T>
int Tliste<T>::aj_fin_diff(const Tliste<T>& l)
{
  int i=0;
  Tliste_iter<T> iter(l);
  T *b;

  while ((b=iter()))
    if (aj_fin_diff(*b))
      i++;

  return i;
}

template <class T>
void Tliste<T>::operator=(const Tliste<T>& l)
{
  nettoie();
  
  Tliste_iter<T> iter(l);
  T *b;

  while ((b=iter()))
    aj_fin(*b);
}

template <class T>
void Tliste<T>::permute_mm_fin(const Tliste_iter<T>& i1,
			       const Tliste_iter<T>& i2)
{
  Liste::permute_mm_fin(i1.ici(), i2.ici());	
}

template <class T>
void Tliste<T>::trie(T** tab, int G, int D)
{
  T *pt, *pu;
  int d,g;

  if (D<=G){
    return;
  }
  pt=tab[D];
  g=G-1;
  d=D;
  
  do{
    while ((*tab[++g])<*pt);
    while (((*pt)<*tab[--d]) && (d>G));
    if (g<d){
      pu=tab[d];
      tab[d]=tab[g];
      tab[g]=pu;
    }
  } while (g<d);
  pu=tab[D];
  tab[D]=tab[g];
  tab[g]=pu;
  trie(tab, G, g-1);
  trie(tab, g+1, D);
}

template <class T>
void Tliste<T>::trie()
{
  int i,l;
  if ((l=nb_ind())<=1)
    return;
  T** tab;
  tab=new T*[l];
  Tliste_iter<T> iter(*this);
  T* pt;

  i=0;
  while ((pt=iter()))
    tab[i++]=pt;

  // tri dichotomique de tab
  trie(tab, 0, l-1);

  // mise a jour de la liste

  for (i=0; i<l; i++)
    aj_fin(*tab[i]);
  for (i=0; i<l; i++)
    enl_deb();

  delete[] tab;
}

template <class T>
void Tliste<T>::echange(Tliste<T>& l)
{
  Liste::echange(l);
}

///////////////////////////////////////////////////
//  donneess

template <class T>
T* Tliste<T>::max()
{
  T* pt,*pm;
  
  Tliste_iter<T> iter(*this);
  if (!(pm=iter()))
    return 0;
  
  while ((pt=iter()))
    if ((*pt)>(*pm))
      pm=pt;
  return pm;
}
    
template <class T>
T* Tliste<T>::min()
{
  T* pt,*pm;
  
  Tliste_iter<T> iter(*this);
  if (!(pm=iter()))
    return 0;
  
  while ((pt=iter()))
    if ((*pt)<(*pm))
      pm=pt;
  return pm;
}

//////////////////////////////////////////////////
/// donnees
//////////////////////////////////////////////////

template<class T>
T* Tliste<T>::ind_deb() const
{
  el_Tliste<T>* pt = (el_Tliste<T>*) Liste::ind_deb();

  return (pt)?&pt->_ind:0;
}

template<class T>
T* Tliste<T>::ind_i(int i) const
{
  el_Tliste<T>* pt = (el_Tliste<T>*) Liste::ind_i(i);

  return (pt)?&pt->_ind:0;
}

template<class T>
T* Tliste<T>::ind_fin() const
{
  el_Tliste<T>* pt = (el_Tliste<T>*) Liste::ind_fin();

  return (pt)?&pt->_ind:0;
}


template <class T>
T* Tliste<T>::possede(T* const a) const{
  Tliste_iter<T> iter(*this);
  T *b;

  while ((b=iter()))
    if (*a==*b)
      return b;
  return 0;
}

template <class T>
T* Tliste<T>::possede(const T& a) const{
  Tliste_iter<T> iter(*this);
  T *b;

  while ((b=iter()))
    if (a==*b)
      return b;
  return 0;
}

////////////////////////////////////
// tests/calculs
////////////////////////////////////

template <class T>
bool Tliste<T>::est_vide() const 
{ 
return Liste::est_vide(); 
}

template <class T>
int Tliste<T>::nb_ind() const
{
  Tliste_iter<T> iter(*this);
  int i=0;

  while (iter())
    i++;
	
  return i;
}

template <class T>
int Tliste<T>::nb_diff(const Tliste<T>& l) const
{
  return nb_ind()+l.nb_ind()-nb_meme(l)-l.nb_meme(*this);
}

template <class T>
bool Tliste<T>::operator==(const Tliste<T>& l) const
{
  Tliste_iter<T> it(*this);
  Tliste_iter<T> it_l(l);

  T *x, *xl;

  while ((x=it())){
    if (!(xl=it_l()))
      return 0;
    if (*xl!=*x)
      return 0;
  }
  if (it_l())
    return 0;
  return 1;
}

template <class T>
bool Tliste<T>::mm_el(const Tliste<T>& L) const
{
  int i,l;
  if ((l=nb_ind())!=L.nb_ind())
    return 0;
  if (l==0)
    return 1;

  bool* tab=new bool[l];
  for (i=0;i<l;i++)
    tab[i]=0;
  
  Tliste_iter<T> it(*this);
  Tliste_iter<T> it_l(L);
  T *x, *xl;

  while ((x=it())){
    it_l.init();
    i=0;
    while ((xl=it_l())){
      if ((*xl==*x) && !tab[i]){
	tab[i]=1;
	break;
      }
      i++;
    }
    if (!xl){
      delete[] tab;
      return 0;
    }
  }

  for (i=0;i<l;i++)
    if (!tab[i]){
      delete[] tab;
      return 0;
    }

  delete[] tab;
  return 1;
}

template <class T>
int Tliste<T>::nb_meme(const Tliste<T>& l) const
{
  Tliste_iter<T> it(*this);

  T *x;
  int i=0;

  while ((x=it()))
    if (l.possede(x))
      i++;

  return i;
}

  
