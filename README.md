# SARMENT
Partitionning libraries in python (from C++ through SWIG)

## Usage

Callable packages are in main directory SARMENT, and some off them
call SWIG made packages in Modules (built at compilation), that call
dynamic libraries .so in Modules.

Either from SARMENT directory, or after having put the SARMENT
directory in your PYTHONPATH, simply import as needed. 

## Compilation 

Everything should be done through a "make" command from directory
SARMENT.

"make clean"  to clean binaries.

"make clean_all" to clean binaries & SWIG made files.

### Organization

Dynamic librairy binaries are compiled from *.h, *.cpp, *.inl, *_wrap.cxx
files in SrcCPP.

*_wrap.cxx and Modules/*.py are built with SWIG, using *.i
configuration files.

### Python.h

Python development library is required (used as "Python.h" in the
sources). 

In SrcCPP/makefile, the existence of this directory is tested in
/usr/include/python3.x with x equals 9, 10 or 11. If the dev directory
is elsewhere, adapt this file.


