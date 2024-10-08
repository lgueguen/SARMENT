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

template<int T, class W>
Stock<T,W>::Stock(){
  if (T<=0){
    cerr << "mauvaise gestion du ramasse-miettes" << endl;
    cerr << "void Stock<W>::Stock()" << endl;
    exit(1);
  }
}

template<int T, class W>
Stock<T,W>::~Stock(){
  detruit();
}


template<int T, class W>
void Stock<T,W>::detruit()
{
  _Li.nettoie();
  _Dispo.nettoie();
}

template<int T, class W>
W* Stock<T,W>::aj_elem(const W& a)
{
  W* w=(W*)_Dispo.enl_deb();
	
  if (!w){
    _Li.aj_fin();
    Paquet<T, W>* p=_Li.ind_fin();
    int i;
    W* x=p->elem();
    for (i=0;i<T;i++)
      _Dispo.aj_fin((void*)x++);
    w=(W*)_Dispo.enl_deb();
  }
	
  *w=a;
  return w;
}

template<int T,class W>
W* Stock<T,W>::aj_elem()
{
  W* w=(W*)_Dispo.enl_deb();

  if (!w){
    _Li.aj_fin();
    Paquet<T, W>* p=_Li.ind_fin();
    int i;
    W* x=p->elem();
    for (i=0;i<T;i++)
      _Dispo.aj_fin((void*)x++);
    w=(W*)_Dispo.enl_deb();
  }
	
  return w;
}

template<int T, class W>
void Stock<T,W>::enl_elem(void* p_a)
{
  _Dispo.aj_deb(p_a);
}


