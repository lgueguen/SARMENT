#include "Liste.h"
#include "Liste_iter.h"

el_liste* Liste::enl_iter(Liste_iter& i)
{
  if (!_fin) return 0;
  el_liste *a, *e=i._el_c;

  if (!e)
    return 0;
  
  a=e->_so;
  
  if (e==_fin){
    if (a==_fin){
      i._el_c=0;
      _fin=0;
    }
    else
      _fin->_so=a->_so;
  }
  else{
    if (e->_so==_fin){
      _fin=e;
      i._el_c=0;
    }
    e->_so=e->_so->_so;
  }
  
  return a;
}

