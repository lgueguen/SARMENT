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

#include "Op_desc.h"

#include <stdio.h>

using namespace std;

////////////////////////
// Op_feuille
////////////////////////

void Op_feuille::diffuse(double x)
{
  _val*=x;
}

int Op_feuille::construit_de(const string& st, int i)
{
  int l;
  if (isalpha(st[i]) || (st[i]=='!') || (st[i]=='^'))
    g_dsc(st[i++]);
  else
    if ((st[i]=='#') && isdigit(st[i+1])){
      i+=1;
      l=atoi((st.substr(i)).c_str());
      if ((l>=256) || (l<0)){
        cout << "Bad number in " << st << " position " << i <<  endl;
        return -1;
      }
      g_dsc((char)l);
      while (isdigit(st[++i]));
    }
    else{
      cout << "Problem in " << st << " position " << i <<  endl;
      return -1;
    }
  
  if ((st[i]=='(') && (isdigit(st[i+1])
		       || ((st[i+1]=='-') && isdigit(st[i+2])))){
    _val=atof((st.substr(i+1)).c_str());
    if ((i=st.find(')',i+1))==string::npos){
      cout << "Problem in " << st << " position " << i <<  endl;
      return -1;
    }
    i++;
  }
  else
    _val=1;

  return i;
}   

Op_desc* Op_feuille::copie() const
{
  return new Op_feuille(*this);
}

void Op_feuille::liste_desc(Tliste<Cdescripteur>& l)  const
{
  l.aj_fin_diff(*this);
}

ostream& Op_feuille::operator<<(ostream& s) const
{
  Cdescripteur::operator<<(s);
  char tmp[20];
  if (_val!=1.0){
    s << '(';
    snprintf(tmp,20,"%.12g",_val);
    s << tmp;
    s << ')';
  }
  return s;
}

bool Op_feuille::operator==(const Op_desc* const pd) const
{
  return ((!pd)
	  ?0
	  :(pd->_op!=FEU
	    ?0
	    :((Cdescripteur::operator==(*(Op_feuille*)pd)) &&
	      (_val==((Op_feuille*)pd)->_val))
	    )
	  );
}

/////////////////////////////////////////////////////////
///  Var_Op


Var_Op::Var_Op(const Tliste<Op_desc*>& pl)
{
  it.colle(pl);
}

////////////////////////
// Op_plus
////////////////////////

void Op_plus::diffuse(double x)
{
  _voo->it.init();
  while ((_voo->ppop=_voo->it()))
    (*(_voo->ppop))->diffuse(x);
}

int Op_plus::construit_de(const string& st, int i)
{
  if ((st.find(')',i)==string::npos) || (st[i]==')')){
    cout << "Problem in " << st << " position " << i <<  endl;
    return -1;
  }
  
  Op_desc **ppop;
  
  while (st[i]!=')'){
    while (st[i]==','){
      i++;
      if (i>=st.length()){
	cout << "Problem at the end of " << st << endl;
	return -1;
      }
    }
    lpop.aj_fin();
    ppop=lpop.ind_fin();
    switch(st[i]){
    case '`':
      if (((*ppop)=new Op_mot())==0){
	cerr << "Allocation problem in Op_plus::construit_de()"
	     << endl;
	i=-1;
      }
      else
	i++;
      break;
    case '{':
      if (((*ppop)=new Op_tom())==0){
	cerr << "Allocation problem in Op_plus::construit_de()"
	     << endl;
	i=-1;
      }
      else
	i++;
      break;
    case '+':
      switch(st[i+1]){
      case '(':
	if (((*ppop)=new Op_plus())==0){
	  cerr << "Allocation problem in Op_plus::construit_de()"
	       << endl;
	  i=-1;
	}
	else
	  i+=2;
	break;
      case '{':
	if (((*ppop)=new Op_plus_tom())==0){
	  cerr << "Allocation problem in Op_plus::construit_de()"
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
	if (((*ppop)=new Op_ou_tom())==0){
	  cerr << "Allocation problem in Op_plus::construit_de()"
	       << endl;
	  i=-1;
	}
	else
	  i+=2;
	break;
      case '(':
	if (((*ppop)=new Op_ou())==0){
	  cerr << "Allocation problem in Op_plus::construit_de()"
	       << endl;
	  i=-1;
	}
	else
	  i+=2;
	break;
      case '`':
	if (((*ppop)=new Op_ou_mot())==0){
	  cerr << "Allocation problem in Op_plus::construit_de()"
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
      if (((*ppop)=new Op_mult())==0){
	cerr << "Allocation problem in Op_plus::construit_de()"
	     << endl;
	i=-1;
      }
      else
	i+=2;
      break;
    default:
      if (((*ppop)=new Op_feuille())==0){
	cerr << "Allocation problem in Op_plus::construit_de()"
	     << endl;
	i=-1;
      }
      break;
    }
    if (i!=-1)
      i=(*ppop)->construit_de(st,i);

    if (i==-1){
      if (*ppop)
	delete (*ppop);
      lpop.tue_fin();
      return -1;
    }
    
    if (i>=st.length()){
      cout << "Problem at the end of  " << st << endl;
      delete *ppop;
      lpop.tue_fin();
      return -1;
    }
  }
  i++;
  if ((st[i]=='(') && (isdigit(st[i+1])
		       || ((st[i+1]=='-') && isdigit(st[i+2])))){
    diffuse(atof((st.substr(i+1)).c_str()));
    if ((i=st.find(')',i+1))==string::npos){
      cout << "Problem in " << st << endl;
      return -1;
    }
    i++;
  }
  return i;
}

Op_desc* Op_plus::copie()  const
{
  return new Op_plus(*this);
}

void Op_plus::liste_desc(Tliste<Cdescripteur>& l)  const
{
  _voo->it.init();
  while ((_voo->ppop=_voo->it()))
    (*(_voo->ppop))->liste_desc(l);
}

ostream& Op_plus::operator<<(ostream& s) const
{
  _voo->it.init();
  if (lpop.nb_ind()>1)
    s << "+(";

  while ((_voo->ppop=_voo->it()))
    (*(_voo->ppop))->operator<<(s);
  
  if (lpop.nb_ind() > 1)
    s << ')';
  
  return s;
}

bool Op_plus::operator==(const Op_desc* const pd) const
{
  if (!pd || (pd->_op!=PLUS))
    return 0;

  Op_plus* pp=(Op_plus*)pd;
  
  int i,l;
  if ((l=lpop.nb_ind())!=pp->lpop.nb_ind())
    return 0;
  if (l==0)
    return 1;

  bool* tab=new bool[l];
  for (i=0;i<l;i++)
    tab[i]=0;
  
  Tliste_iter<Op_desc*> it(lpop);
  Tliste_iter<Op_desc*> it_l(pp->lpop);
  Op_desc **x, **xl;

  while ((x=it())){
    it_l.init();
    i=0;
    while ((xl=it_l())){
      if ((**x==*xl) && !tab[i]){
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
  
////////////////////////
// Op_plus_tom
////////////////////////

void Op_plus_tom::diffuse(double x)
{
  _voo->it.init();
  while ((_voo->ppop=_voo->it()))
    (*(_voo->ppop))->diffuse(x);
}

int Op_plus_tom::construit_de(const string& st, int i)
{
  if ((st.find('}',i)==string::npos) || (st[i]=='}')){
    cout << "Problem in " << st << endl;
    return -1;
  }
  
  Op_desc **ppop;
  
  while (st[i]!='}'){
    lpop.aj_deb();
    ppop=lpop.ind_deb();
    switch(st[i]){
    case '`':
      if (((*ppop)=new Op_mot())==0){
	cerr << "Allocation problem in Op_plus_tom::construit_de()"
	     << endl;
	i=-1;
      }
      else
	i++;
      break;
    case '{':
      if (((*ppop)=new Op_tom())==0){
	cerr << "Allocation problem in Op_plus_tom::construit_de()"
	     << endl;
	i=-1;
      }
      else
	i++;
      break;
    case '+':
      switch(st[i+1]){
      case '(':
	if (((*ppop)=new Op_plus())==0){
	  cerr << "Allocation problem in Op_plus_tom::construit_de()"
	       << endl;
	  i=-1;
	}
	else
	  i+=2;
	break;
      case '{':
	if (((*ppop)=new Op_plus_tom())==0){
	  cerr << "Allocation problem in Op_plus_tom::construit_de()"
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
	if (((*ppop)=new Op_ou_tom())==0){
	  cerr << "Allocation problem in Op_plus_tom::construit_de()"
	       << endl;
	  i=-1;
	}
	else
	  i+=2;
	break;
      case '(':
	if (((*ppop)=new Op_ou())==0){
	  cerr << "Allocation problem in Op_plus_tom::construit_de()"
	       << endl;
	  i=-1;
	}
	else
	  i+=2;
	break;
      case '`':
	if (((*ppop)=new Op_ou_mot())==0){
	  cerr << "Allocation problem in Op_plus_tom::construit_de()"
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
      if (((*ppop)=new Op_mult())==0){
	cerr << "Allocation problem in Op_plus_tom::construit_de()"
	     << endl;
	i=-1;
      }
      else
	i+=2;
      break;
    default:
      if (((*ppop)=new Op_feuille())==0){
	cerr << "Allocation problem in Op_plus_tom::construit_de()"
	     << endl;
	i=-1;
      }
      break;
    }
    if (i!=-1)
      i=(*ppop)->construit_de(st,i);

    if (i==-1){
      if (*ppop)
	delete (*ppop);
      lpop.tue_fin();
      return -1;
    }
    
    if (i>=st.length()){
      cout << "Problem at the end of  " << st << endl;
      if (*ppop)
	delete *ppop;
      lpop.tue_deb();
      return -1;
    }
  }
  
  i++;
  if ((st[i]=='(') && (isdigit(st[i+1])
		       || ((st[i+1]=='-') && isdigit(st[i+2])))){
    diffuse(atof((st.substr(i+1)).c_str()));
    if ((i=st.find(')',i+1))==string::npos){
      cout << "Problem in " << st << endl;
      return -1;
    }
    i++;
  }
  return i;
}

Op_desc* Op_plus_tom::copie()  const
{
  return new Op_plus_tom(*this);
}

void Op_plus_tom::liste_desc(Tliste<Cdescripteur>& l)  const
{
  _voo->it.init();
  while ((_voo->ppop=_voo->it()))
    (*(_voo->ppop))->liste_desc(l);
}

ostream& Op_plus_tom::operator<<(ostream& s) const
{
  int n=lpop.nb_ind();
  int i;

  if (n>1)
    s << "+{";

  for (i=n;i>=1;i--)
    (*(lpop.ind_i(i)))->operator<<(s);
  
  if (lpop.nb_ind() > 1)
    s << '}';
  
  return s;
}

bool Op_plus_tom::operator==(const Op_desc* const pd) const
{
  if (!pd || (pd->_op!=PLUS_TOM))
    return 0;

  Op_plus_tom* pp=(Op_plus_tom*)pd;
  
  Tliste_iter<Op_desc*> it(lpop);
  Tliste_iter<Op_desc*> it_l(pp->lpop);
  Op_desc **x, **xl;

  while ((x=it())){
    if (!(xl=it_l()))
      return 0;
    if (!(**x==*xl))
      return 0;
  }
  if (it_l())
    return 0;
  return 1;
}
  
////////////////////////
// Op_mult
////////////////////////

void Op_mult::diffuse(double x)
{

  if ((_voo->ppop=lpop.ind_deb()))
    (*(_voo->ppop))->diffuse(x);
}

int Op_mult::construit_de(const string& st, int i)
{
  if ((st.find(')',i)==string::npos) || (st[i]==')')){
    cout << "Problem in " << st << endl;
    return -1;
  }
  
  Op_desc **ppop;
  
  while (st[i]!=')'){
    while (st[i]==','){
      i++;
      if (i>=st.length()){
	cout << "Problem at the end of  " << st << endl;
	return -1;
      }
    }
    lpop.aj_fin();
    ppop=lpop.ind_fin();
    switch(st[i]){
    case '`':
      if (((*ppop)=new Op_mot())==0){
	cerr << "Allocation problem in Op_mult::construit_de()"
	     << endl;
	i=-1;
      }
      else
	i++;
      break;
    case '{':
      if (((*ppop)=new Op_tom())==0){
	cerr << "Allocation problem in Op_mult::construit_de()"
	     << endl;
	i=-1;
      }
      else
	i++;
      break;
    case '+':
      switch(st[i+1]){
      case '(':
	if (((*ppop)=new Op_plus())==0){
	  cerr << "Allocation problem in Op_mult::construit_de()"
	       << endl;
	  i=-1;
	}
	else
	  i+=2;
	break;
      case '{':
	if (((*ppop)=new Op_plus_tom())==0){
	  cerr << "Allocation problem in Op_mult::construit_de()"
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
	if (((*ppop)=new Op_ou_tom())==0){
	  cerr << "Allocation problem in Op_mult::construit_de()"
	       << endl;
	  i=-1;
	}
	else
	  i+=2;
	break;
      case '(':
	if (((*ppop)=new Op_ou())==0){
	  cerr << "Allocation problem in Op_mult::construit_de()"
	       << endl;
	  i=-1;
	}
	else
	  i+=2;
	break;
      case '`':
	if (((*ppop)=new Op_ou_mot())==0){
	  cerr << "Allocation problem in Op_mult::construit_de()"
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
      if (((*ppop)=new Op_mult())==0){
	cerr << "Allocation problem in Op_mult::construit_de()"
	     << endl;
	i=-1;
      }
      else
	i+=2;
      break;
    default:
      if (((*ppop)=new Op_feuille())==0){
	cerr << "Allocation problem in Op_mult::construit_de()"
	     << endl;
	i=-1;
      }
      break;
    }
    if (i!=-1)
      i=(*ppop)->construit_de(st,i);

    if (i==-1){
      if (*ppop)
	delete (*ppop);
      lpop.tue_fin();
      return -1;
    }
    
    if (i>=st.length()){
      cout << "Problem at the end of  " << st << endl;
      delete *ppop;
      lpop.tue_fin();
      return -1;
    }
  }
  i++;
  if ((st[i]=='(') && (isdigit(st[i+1])
		       || ((st[i+1]=='-') && isdigit(st[i+2])))){
    diffuse(atof((st.substr(i+1)).c_str()));
    if ((i=st.find(')',i+1))==string::npos){
      cout << "Problem in " << st << endl;
      return -1;
    }
    i++;
  }
  return i;
}

Op_desc* Op_mult::copie()  const
{
  return new Op_mult(*this);
}

void Op_mult::liste_desc(Tliste<Cdescripteur>& l)  const
{
  _voo->it.init();
  while ((_voo->ppop=_voo->it()))
    (*(_voo->ppop))->liste_desc(l);
}

ostream& Op_mult::operator<<(ostream& s) const
{
  _voo->it.init();
  if (lpop.nb_ind()>1)
    s << "*(";

  while ((_voo->ppop=_voo->it()))
    (*(_voo->ppop))->operator<<(s);
  
  if (lpop.nb_ind() > 1)
    s << ')';
  
  return s;
}

bool Op_mult::operator==(const Op_desc* const pd) const
{
  if (!pd || (pd->_op!=MULT))
    return 0;

  Op_mult* pp=(Op_mult*)pd;
  
  int i,l;
  if ((l=lpop.nb_ind())!=pp->lpop.nb_ind())
    return 0;
  if (l==0)
    return 1;

  bool* tab=new bool[l];
  for (i=0;i<l;i++)
    tab[i]=0;
  
  Tliste_iter<Op_desc*> it(lpop);
  Tliste_iter<Op_desc*> it_l(pp->lpop);
  Op_desc **x, **xl;

  while ((x=it())){
    it_l.init();
    i=0;
    while ((xl=it_l())){
      if ((**x==*xl) && !tab[i]){
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
  
////////////////////////
// Op_mot
////////////////////////

void Op_mot::diffuse(double x)
{
  
  if ((_voo->ppop=lpop.ind_deb()))
    (*(_voo->ppop))->diffuse(x);
}


int Op_mot::construit_de(const string& st, int i)
{
  if ((st.find('\'',i)==string::npos) || (st[i]=='\'')){
    cout << "Problem in " << st << endl;
    return -1;
  }
  Op_desc **ppop;
  
  while (st[i]!='\''){
    lpop.aj_fin();
    ppop=lpop.ind_fin();
    switch(st[i]){
    case '`':
      if (((*ppop)=new Op_mot())==0){
	cerr << "Allocation problem in Op_mot::construit_de()"
	     << endl;
	i=-1;
      }
      else
	i++;
      break;
    case '{':
      if (((*ppop)=new Op_tom())==0){
	cerr << "Allocation problem in Op_mot::construit_de()"
	     << endl;
	i=-1;
      }
      else
	i++;
      break;
    case '+':
      switch(st[i+1]){
      case '(':
	if (((*ppop)=new Op_plus())==0){
	  cerr << "Allocation problem in Op_mot::construit_de()"
	       << endl;
	  i=-1;
	}
	else
	  i+=2;
	break;
      case '{':
	if (((*ppop)=new Op_plus_tom())==0){
	  cerr << "Allocation problem in Op_mot::construit_de()"
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
	if (((*ppop)=new Op_ou_tom())==0){
	  cerr << "Allocation problem in Op_mot::construit_de()"
	       << endl;
	  i=-1;
	}
	else
	  i+=2;
	break;
      case '(':
	if (((*ppop)=new Op_ou())==0){
	  cerr << "Allocation problem in Op_mot::construit_de()"
	       << endl;
	  i=-1;
	}
	else
	  i+=2;
	break;
      case '`':
	if (((*ppop)=new Op_ou_mot())==0){
	  cerr << "Allocation problem in Op_mot::construit_de()"
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
      if (((*ppop)=new Op_mult())==0){
	cerr << "Allocation problem in Op_mot::construit_de()"
	     << endl;
	i=-1;
      }
      else
	i+=2;
      break;
    default:
      if (((*ppop)=new Op_feuille())==0){
	cerr << "Allocation problem in Op_mot::construit_de()"
	     << endl;
	i=-1;
      }
      break;
    }
    if (i!=-1)
      i=(*ppop)->construit_de(st,i);

    if (i==-1){
      if (*ppop)
	delete (*ppop);
      lpop.tue_fin();
      return -1;
    }
    
    if (i>=st.length()){
      cout << "Problem at the end of  " << st << endl;
      if (*ppop)
	delete *ppop;
      lpop.tue_fin();
      return -1;
    }
  }
  i++;
  if ((st[i]=='(') && (isdigit(st[i+1])
		       || ((st[i+1]=='-') && isdigit(st[i+2])))){
    diffuse(atof((st.substr(i+1)).c_str()));
    if ((i=st.find(')',i+1))==string::npos){
      cout << "Problem in " << st << endl;
      return -1;
    }
    i++;
  }

  return i;
}   

Op_desc* Op_mot::copie()  const
{
  return new Op_mot(*this);
}

void Op_mot::liste_desc(Tliste<Cdescripteur>& l)  const
{
  _voo->it.init();
  while ((_voo->ppop=_voo->it()))
    (*(_voo->ppop))->liste_desc(l);
}

ostream& Op_mot::operator<<(ostream& s) const
{
  _voo->it.init();
    
  s << '`';

  while ((_voo->ppop=_voo->it())){
      (*(_voo->ppop))->operator<<(s);
  }
  s << '\'';
  
  return s;
}

bool Op_mot::operator==(const Op_desc* const pd) const
{
  if (!pd || (pd->_op!=MOT))
    return 0;

  Op_mot* pp=(Op_mot*)pd;
  
  Tliste_iter<Op_desc*> it(lpop);
  Tliste_iter<Op_desc*> it_l(pp->lpop);
  Op_desc **x, **xl;

  while ((x=it())){
    if (!(xl=it_l()))
      return 0;
    if (!(**x==*xl))
      return 0;
  }
  if (it_l())
    return 0;
  return 1;
}

////////////////////////
// Op_tom
////////////////////////

void Op_tom::diffuse(double x)
{
  if ((_voo->ppop=lpop.ind_deb()))
    (*(_voo->ppop))->diffuse(x);
}


int Op_tom::construit_de(const string& st, int i)
{
  if ((st.find('}',i)==string::npos) || (st[i]=='}')){
    cout << "Problem in " << st << endl;
    return -1;
  }
  Op_desc **ppop;
  
  while (st[i]!='}'){
    lpop.aj_deb();
    ppop=lpop.ind_deb();
    switch(st[i]){
    case '`':
      if (((*ppop)=new Op_mot())==0){
	cerr << "Allocation problem in Op_tom::construit_de()"
	     << endl;
	i=-1;
      }
      else
	i++;
      break;
    case '{':
      if (((*ppop)=new Op_tom())==0){
	cerr << "Allocation problem in Op_tom::construit_de()"
	     << endl;
	i=-1;
      }
      else
	i++;
      break;
    case '+':
      switch(st[i+1]){
      case '(':
	if (((*ppop)=new Op_plus())==0){
	  cerr << "Allocation problem in Op_tom::construit_de()"
	       << endl;
	  i=-1;
	}
	else
	  i+=2;
	break;
      case '{':
	if (((*ppop)=new Op_plus_tom())==0){
	  cerr << "Allocation problem in Op_tom::construit_de()"
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
	if (((*ppop)=new Op_ou_tom())==0){
	  cerr << "Allocation problem in Op_tom::construit_de()"
	       << endl;
	  i=-1;
	}
	else
	  i+=2;
	break;
      case '(':
	if (((*ppop)=new Op_ou())==0){
	  cerr << "Allocation problem in Op_tom::construit_de()"
	       << endl;
	  i=-1;
	}
	else
	  i+=2;
	break;
      case '`':
	if (((*ppop)=new Op_ou_mot())==0){
	  cerr << "Allocation problem in Op_tom::construit_de()"
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
      if (((*ppop)=new Op_mult())==0){
	cerr << "Allocation problem in Op_tom::construit_de()"
	     << endl;
	i=-1;
      }
      else
	i+=2;
      break;
    default:
      if (((*ppop)=new Op_feuille())==0){
	cerr << "Allocation problem in Op_tom::construit_de()"
	     << endl;
	i=-1;
      }
      break;
    }
    if (i!=-1)
      i=(*ppop)->construit_de(st,i);

    if (i==-1){
      if (*ppop)
	delete (*ppop);
      lpop.tue_deb();
      return -1;
    }
        
    if (i>=st.length()){
      cout << "Problem at the end of  " << st << endl;
      if (*ppop)
	delete *ppop;
      lpop.tue_deb();
      return -1;
    }
  }
  i++;
  if ((st[i]=='(') && (isdigit(st[i+1])
		       || ((st[i+1]=='-')
			   && isdigit(st[i+2])))){
    diffuse(atof((st.substr(i+1)).c_str()));
    if ((i=st.find(')',i+1))==string::npos){
      cout << "Problem in " << st << endl;
      return -1;
    }
    i++;
  }

  return i;
}   

Op_desc* Op_tom::copie()  const
{
  return new Op_tom(*this);
}

void Op_tom::liste_desc(Tliste<Cdescripteur>& l)  const
{
  _voo->it.init();
  while ((_voo->ppop=_voo->it()))
    (*(_voo->ppop))->liste_desc(l);
}

ostream& Op_tom::operator<<(ostream& s) const
{
  int n=lpop.nb_ind();
  int i;

  s << '{';
  
  for (i=n;i>=1;i--)
    (*(lpop.ind_i(i)))->operator<<(s);

  s << '}';
  
  return s;
}

bool Op_tom::operator==(const Op_desc* const pd) const
{
  if (!pd || (pd->_op!=TOM))
    return 0;

  Op_tom* pp=(Op_tom*)pd;
  
  Tliste_iter<Op_desc*> it(lpop);
  Tliste_iter<Op_desc*> it_l(pp->lpop);
  Op_desc **x, **xl;

  while ((x=it())){
    if (!(xl=it_l()))
      return 0;
    if (!(**x==*xl))
      return 0;
  }
  if (it_l())
    return 0;
  return 1;
}

//////////////////////////////////////////
// Var_op_ou

Var_Op_ou::Var_Op_ou(const Tliste<Bi_pop>& pl)
{
  it.colle(pl);
}

////////////////////////
// Op_ou_tom
////////////////////////

void Op_ou_tom::diffuse(double x)
{
  _voo->it.init();
  while ((_voo->pbi=_voo->it()))
    _voo->pbi->_pop->diffuse(x);
}


int Op_ou_tom::construit_de(const string& st, int i)
{
  if ((st.find('}',i)==string::npos) || (st[i]=='}')){
    cout << "Problem in " << st << endl;
    return -1;
  }
  Bi_pop *pbi;
  Op_desc** pop;
  
  bool b=1;
  
  while (st[i]!='}'){
    if (b){
      lbi.aj_fin();
      pbi=lbi.ind_fin();
    }
    pop=b?&(pbi->_poc):&(pbi->_pop);
    b=!b;

    switch(st[i]){
    case '`':
      if (((*pop)=new Op_mot())==0){
	cerr << "Allocation problem in Op_ou_tom::construit_de()"
	     << endl;
	i=-1;
      }
      else
	i++;
      break;
    case '{':
      if (((*pop)=new Op_tom())==0){
	cerr << "Allocation problem in Op_ou_tom::construit_de()"
	     << endl;
	i=-1;
      }
      else
	i++;
      break;
    case '+':
      switch(st[i+1]){
      case '(':
	if (((*pop)=new Op_plus())==0){
	  cerr << "Allocation problem in Op_ou_tom::construit_de()"
	       << endl;
	  i=-1;
	}
	else
	  i+=2;
	break;
      case '{':
	if (((*pop)=new Op_plus_tom())==0){
	  cerr << "Allocation problem in Op_ou_tom::construit_de()"
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
	if (((*pop)=new Op_ou_tom())==0){
	  cerr << "Allocation problem in Op_ou_tom::construit_de()"
	       << endl;
	  i=-1;
	}
	else
	  i+=2;
	break;
      case '(':
	if (((*pop)=new Op_ou())==0){
	  cerr << "Allocation problem in Op_ou_tom::construit_de()"
	       << endl;
	  i=-1;
	}
	else
	  i+=2;
	break;
      case '`':
	if (((*pop)=new Op_ou_mot())==0){
	  cerr << "Allocation problem in Op_ou_tom::construit_de()"
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
      if (((*pop)=new Op_mult())==0){
	cerr << "Allocation problem in Op_ou_tom::construit_de()"
	     << endl;
	i=-1;
      }
      else
	i+=2;
      break;
    default:
      if (((*pop)=new Op_feuille())==0){
	cerr << "Allocation problem in Op_ou_tom::construit_de()"
	     << endl;
	i=-1;
      }
      break;
    }
    if (i!=-1)
      i=(*pop)->construit_de(st,i);

    if (i==-1){
      pbi->nettoie();
      lbi.tue_fin();
      return -1;
    }      
    
    if (i>=st.length()){
      cout << "Problem at the end of  " << st << endl;
      pbi->nettoie();
      lbi.tue_fin();
      return -1;
    }
  }
  if (!b){
    cout << "Missing descriptor in " << st << endl;
    pbi->nettoie();
    lbi.tue_fin();
    return -1;
  }
  i++;
  if ((st[i]=='(') && (isdigit(st[i+1])
		       || ((st[i+1]=='-')
			   && isdigit(st[i+2])))){
    diffuse(atof((st.substr(i+1)).c_str()));
    if ((i=st.find(')',i+1))==string::npos){
      cout << "Problem in " << st << endl;
      return -1;
    }
    i++;
  }

  return i;
}   

Op_desc* Op_ou_tom::copie()  const
{
  return new Op_ou_tom(*this);
}

void Op_ou_tom::liste_desc(Tliste<Cdescripteur>& l)  const
{
  _voo->it.init();

  while ((_voo->pbi=_voo->it())){
    _voo->pbi->_poc->liste_desc(l);
    _voo->pbi->_pop->liste_desc(l);
  }
}

ostream& Op_ou_tom::operator<<(ostream& s) const
{
  _voo->it.init();

  s << "|{";
  
  while ((_voo->pbi=_voo->it())){
    _voo->pbi->_poc->operator<<(s);
    _voo->pbi->_pop->operator<<(s);
  }
  
  s << '}';
  return s;
}

bool Op_ou_tom::operator==(const Op_desc* const pd) const
{
  cerr << "Op_ou_tom::operator==(const Op_desc* const pd) const pas fait" << endl;
  return 0;
}

////////////////////////
// Op_ou
////////////////////////

void Op_ou::diffuse(double x)
{
  _voo->it.init();

  while ((_voo->pbi=_voo->it()))
    _voo->pbi->_pop->diffuse(x);
}


int Op_ou::construit_de(const string& st, int i)
{
  if ((st.find(')',i)==string::npos) || (st[i]==')')){
    cout << "Problem in " << st << endl;
    return -1;
  }
  Bi_pop *pbi;
  Op_desc** pop;
  
  bool b=1;
  
  while (st[i]!=')'){
    if (b){
      lbi.aj_fin();
      pbi=lbi.ind_fin();
    }
    pop=b?&(pbi->_poc):&(pbi->_pop);
    b=!b;

    switch(st[i]){
    case '`':
      if (((*pop)=new Op_mot())==0){
	cerr << "Allocation problem in Op_ou::construit_de()"
	     << endl;
	i=-1;
      }
      else
	i++;
      break;
    case '{':
      if (((*pop)=new Op_tom())==0){
	cerr << "Allocation problem in Op_ou::construit_de()"
	     << endl;
	i=-1;
      }
      else
	i++;
      break;
    case '+':
      switch(st[i+1]){
      case '(':
	if (((*pop)=new Op_plus())==0){
	  cerr << "Allocation problem in Op_ou::construit_de()"
	       << endl;
	  i=-1;
	}
	else
	  i+=2;
	break;
      case '{':
	if (((*pop)=new Op_plus())==0){
	  cerr << "Allocation problem in Op_ou::construit_de()"
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
	if (((*pop)=new Op_ou_tom())==0){
	  cerr << "Allocation problem in Op_ou::construit_de()"
	       << endl;
	  i=-1;
	}
	else
	  i+=2;
	break;
      case '(':
	if (((*pop)=new Op_ou())==0){
	  cerr << "Allocation problem in Op_ou::construit_de()"
	       << endl;
	  i=-1;
	}
	else
	  i+=2;
	break;
      case '`':
	if (((*pop)=new Op_ou_mot())==0){
	  cerr << "Allocation problem in Op_ou::construit_de()"
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
      if (((*pop)=new Op_mult())==0){
	cerr << "Allocation problem in Op_ou::construit_de()"
	     << endl;
	i=-1;
      }
      else
	i+=2;
      break;
    default:
      if (((*pop)=new Op_feuille())==0){
	cerr << "Allocation problem in Op_ou::construit_de()"
	     << endl;
	i=-1;
      }
      break;
    }
    if (i!=-1)
      i=(*pop)->construit_de(st,i);

    if (i==-1){
      pbi->nettoie();
      lbi.tue_fin();
      return -1;
    }      
     
    if (i>=st.length()){
      cout << "Problem at the end of  " << st << endl;
      pbi->nettoie();
      lbi.tue_fin();
      return -1;
    }
  }
  if (!b){
    cout << "Missing descriptor in " << st << endl;
    pbi->nettoie();
    lbi.tue_fin();
    return -1;
  }
  i++;
  if ((st[i]=='(') && (isdigit(st[i+1])
		       || ((st[i+1]=='-')
			   && isdigit(st[i+2])))){
    diffuse(atof((st.substr(i+1)).c_str()));
    if ((i=st.find(')',i+1))==string::npos){
      cout << "Problem in " << st << endl;
      return -1;
    }
    i++;
  }

  return i;
}   

Op_desc* Op_ou::copie()  const
{
  return new Op_ou(*this);
}

void Op_ou::liste_desc(Tliste<Cdescripteur>& l)  const
{
  _voo->it.init();

  while ((_voo->pbi=_voo->it())){
    _voo->pbi->_poc->liste_desc(l);
    _voo->pbi->_pop->liste_desc(l);
  }
}

ostream& Op_ou::operator<<(ostream& s) const
{
  _voo->it.init();

  s << "|(";
  
  while ((_voo->pbi=_voo->it())){
    _voo->pbi->_poc->operator<<(s);
    _voo->pbi->_pop->operator<<(s);
  }
  
  s << ')';
  return s;
}

bool Op_ou::operator==(const Op_desc* const pd) const
{
  cerr << "Op_ou::operator==(const Op_desc* const pd) const pas fait" << endl;
  return 0;
}

////////////////////////
// Op_ou_mot
////////////////////////

void Op_ou_mot::diffuse(double x)
{
  _voo->it.init();

  while ((_voo->pbi=_voo->it()))
    _voo->pbi->_pop->diffuse(x);
}


int Op_ou_mot::construit_de(const string& st, int i)
{
  if ((st.find('\'',i)==string::npos) || (st[i]=='\'')){
    cout << "Problem in " << st << endl;
    return -1;
  }
  Bi_pop *pbi;
  Op_desc** pop;
  
  bool b=1;
  
  while (st[i]!='\''){
    if (b){
      lbi.aj_fin();
      pbi=lbi.ind_fin();
    }
    pop=b?&(pbi->_poc):&(pbi->_pop);
    b=!b;

    switch(st[i]){
    case '`':
      if (((*pop)=new Op_mot())==0){
	cerr << "Allocation problem in Op_ou_mot::construit_de()"
	     << endl;
	i=-1;
      }
      else
	i++;
      break;
    case '{':
      if (((*pop)=new Op_tom())==0){
	cerr << "Allocation problem in Op_ou_mot::construit_de()"
	     << endl;
	i=-1;
      }
      else
	i++;
      break;
    case '+':
      switch(st[i+1]){
      case '(':
	if (((*pop)=new Op_plus())==0){
	  cerr << "Allocation problem in Op_ou_mot::construit_de()"
	       << endl;
	  i=-1;
	}
	else
	  i+=2;
	break;
      case '{':
	if (((*pop)=new Op_plus())==0){
	  cerr << "Allocation problem in Op_ou_mot::construit_de()"
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
	if (((*pop)=new Op_ou_tom())==0){
	  cerr << "Allocation problem in Op_ou_mot::construit_de()"
	       << endl;
	  i=-1;
	}
	else
	  i+=2;
	break;
      case '`':
	if (((*pop)=new Op_ou_mot())==0){
	  cerr << "Allocation problem in Op_ou_mot::construit_de()"
	       << endl;
	  i=-1;
	}
	else
	  i+=2;
	break;
      case '(':
	if (((*pop)=new Op_ou())==0){
	  cerr << "Allocation problem in Op_ou_mot::construit_de()"
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
      if (((*pop)=new Op_mult())==0){
	cerr << "Allocation problem in Op_ou_mot::construit_de()"
	     << endl;
	i=-1;
      }
      else
	i+=2;
      break;
    default:
      if (((*pop)=new Op_feuille())==0){
	cerr << "Allocation problem in Op_ou_mot::construit_de()"
	     << endl;
	i=-1;
      }
      break;
    }
    if (i!=-1)
      i=(*pop)->construit_de(st,i);

    if (i==-1){
      pbi->nettoie();
      lbi.tue_fin();
      return -1;
    }      
    
    if (i>=st.length()){
      cout << "Problem at the end of  " << st << endl;
      pbi->nettoie();
      lbi.tue_fin();
      return -1;
    }
  }
  if (!b){
    cout << "Missing descriptor in " << st << endl;
    pbi->nettoie();
    lbi.tue_fin();
    return -1;
  }
  i++;
  if ((st[i]=='(') && (isdigit(st[i+1])
		       || ((st[i+1]=='-')
			   && isdigit(st[i+2])))){
    diffuse(atof((st.substr(i+1)).c_str()));
    if ((i=st.find(')',i+1))==string::npos){
      cout << "Problem in " << st << endl;
      return -1;
    }
    i++;
  }

  return i;
}   

Op_desc* Op_ou_mot::copie()  const
{
  return new Op_ou_mot(*this);
}

void Op_ou_mot::liste_desc(Tliste<Cdescripteur>& l)  const
{
  _voo->it.init();

  while ((_voo->pbi=_voo->it())){
    _voo->pbi->_poc->liste_desc(l);
    _voo->pbi->_pop->liste_desc(l);
  }
}

ostream& Op_ou_mot::operator<<(ostream& s) const
{
  _voo->it.init();

  s << "|`";
  
  while ((_voo->pbi=_voo->it())){
    _voo->pbi->_poc->operator<<(s);
    _voo->pbi->_pop->operator<<(s);
  }
  
  s << '\'';
  return s;
}

bool Op_ou_mot::operator==(const Op_desc* const pd) const
{
  cerr << "Op_ou_mot::operator==(const Op_desc* const pd) const pas fait" << endl;
  return 0;
}

