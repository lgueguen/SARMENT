# Copyright 2004 Laurent GUEGUEN
# 
# This file is part of Sarment.
# 
# Sarment is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# 
# Sarment is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with Sarment; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#############################
## C++

CCC= g++

FSH = -shared -fPIC 
FSHD= -bundle -framework Python

OPS= -g

SRCDIR = ../SrcCPP

VPATH = .:$(SRCDIR)

######################

define faitmodule
@if [ `uname` = Darwin ]; then \
cd $(SRCDIR); set -x; $(CCC) $(OPS) $(FSHD) $(^F) -o ../Modules/$@ ; \
else \
cd $(SRCDIR); set -x; $(CCC) $(OPS) $(FSH)  $(^F) -o ../Modules/$@; \
fi
endef

############################
# MACROS
#############################

ELO= desc_comp.o motif.o Op_desc.o descripteur.o Liste.o
SEQUENCEO = sequence.o Fsequence.o random.o
MATRICEO = matrice.o Fmatrice.o random.o

#######################
#   cibles
#######################

all: _Csequence.so _Fsequence.so _Cmatrice.so \
	_Fmatrice.so _Clexique.so _Cpartition.so \
	_part_equ.so _parti_comp.so _Cparti_simp.so \
        _Cdesc_comp.so

.PHONY: all

.PRECIOUS: %_wrap.cxx

%_wrap.cxx: %.i
	$(MAKE) -C $(SRCDIR) $(@F)

%_wrap.o: %_wrap.cxx 
	$(MAKE) -C $(SRCDIR) $(@F)

%.o: %.cpp
	$(MAKE) -C $(SRCDIR) $(@F)

_Cdesc_comp.so: $(addprefix $(SRCDIR)/, $(ELO)  desc_comp_wrap.o)
	$(faitmodule)

_Csequence.so: $(addprefix $(SRCDIR)/, $(SEQUENCEO) sequence_wrap.o)
	$(faitmodule)

_Fsequence.so: $(addprefix $(SRCDIR)/, Fsequence.o Fsequence_wrap.o random.o)
	$(faitmodule)

_Cmatrice.so: $(addprefix $(SRCDIR)/, $(MATRICEO) matrice_wrap.o lexique.o $(ELO))
	$(faitmodule)

_Fmatrice.so: $(addprefix $(SRCDIR)/, Fmatrice_wrap.o Fmatrice.o descripteur.o random.o)
	$(faitmodule)

_Clexique.so: $(addprefix $(SRCDIR)/, lexique_wrap.o lexique.o  $(MATRICEO) $(SEQUENCEO) $(ELO))
	$(faitmodule)

_Cpartition.so: $(addprefix $(SRCDIR)/, partition.o partition_wrap.o segment.o $(ELO) part_equ_iter.o)
	$(faitmodule)

_part_equ.so: $(addprefix $(SRCDIR)/, part_equ.o part_equ_wrap.o posopere.o part_equ_iter.o segment.o $(ELO))
	$(faitmodule)

_parti_comp.so: $(addprefix $(SRCDIR)/, parti_comp.o parti_comp_wrap.o Arret.o part_equ.o lexique.o segment.o $(MATRICEO) $(ELO) posopere.o part_equ_iter.o)
	$(faitmodule)

_Cparti_simp.so: $(addprefix $(SRCDIR)/,parti_simp.o parti_simp_wrap.o partition.o $(ELO) segment.o part_equ_iter.o parti_comp.o Arret.o part_equ.o lexique.o posopere.o $(MATRICEO))
	$(faitmodule)

