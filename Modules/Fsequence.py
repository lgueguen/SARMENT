# This file was automatically generated by SWIG (https://www.swig.org).
# Version 4.1.0
#
# Do not make changes to this file unless you know what you are doing - modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
# Import the low-level C/C++ module
if __package__ or "." in __name__:
    from . import _Fsequence
else:
    import _Fsequence

try:
    import builtins as __builtin__
except ImportError:
    import __builtin__

def _swig_repr(self):
    try:
        strthis = "proxy of " + self.this.__repr__()
    except __builtin__.Exception:
        strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)


def _swig_setattr_nondynamic_instance_variable(set):
    def set_instance_attr(self, name, value):
        if name == "this":
            set(self, name, value)
        elif name == "thisown":
            self.this.own(value)
        elif hasattr(self, name) and isinstance(getattr(type(self), name), property):
            set(self, name, value)
        else:
            raise AttributeError("You cannot add instance attributes to %s" % self)
    return set_instance_attr


def _swig_setattr_nondynamic_class_variable(set):
    def set_class_attr(cls, name, value):
        if hasattr(cls, name) and not isinstance(getattr(cls, name), property):
            set(cls, name, value)
        else:
            raise AttributeError("You cannot add class attributes to %s" % cls)
    return set_class_attr


def _swig_add_metaclass(metaclass):
    """Class decorator for adding a metaclass to a SWIG wrapped class - a slimmed down version of six.add_metaclass"""
    def wrapper(cls):
        return metaclass(cls.__name__, cls.__bases__, cls.__dict__.copy())
    return wrapper


class _SwigNonDynamicMeta(type):
    """Meta class to enforce nondynamic attributes (no new attributes) for a class"""
    __setattr__ = _swig_setattr_nondynamic_class_variable(type.__setattr__)


class Fsequence(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")
    __repr__ = _swig_repr

    def __init__(self):
        _Fsequence.Fsequence_swiginit(self, _Fsequence.new_Fsequence())
    __swig_destroy__ = _Fsequence.delete_Fsequence

    def melange(self, arg2):
        return _Fsequence.Fsequence_melange(self, arg2)

    def termine(self, arg2):
        return _Fsequence.Fsequence_termine(self, arg2)

    def vtaille(self):
        return _Fsequence.Fsequence_vtaille(self)

    def __getitem__(self, i):
        return _Fsequence.Fsequence___getitem__(self, i)

    def __setitem__(self, i, c):
        return _Fsequence.Fsequence___setitem__(self, i, c)

    def __str__(self):
        return _Fsequence.Fsequence___str__(self)

    def __len__(self):
        return _Fsequence.Fsequence___len__(self)

    def recup_rel(self, *args):
        return _Fsequence.Fsequence_recup_rel(self, *args)

# Register Fsequence in _Fsequence:
_Fsequence.Fsequence_swigregister(Fsequence)

