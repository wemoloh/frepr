frepr
=====
.. image:: https://travis-ci.org/wemoloh/frepr.svg?branch=master
    :target: https://travis-ci.org/wemoloh/frepr

Fast float ``repr()`` for CPython using
`<https://github.com/google/double-conversion>`_.  Approximately 8X faster than
the built-in function.


Quick Start
===========

.. code-block:: python

    >>> import frepr
    >>> frepr.frepr(-1.8397892359791307e+134)
    '-1.8397892359791307e+134'
    >>> from timeit import timeit
    >>> timeit('repr(-1.8397892359791307e+134)')
    2.7296602272476758
    >>> frepr.install()
    >>> timeit('repr(-1.8397892359791307e+134)')
    0.35723431229251545
    >>> frepr.uninstall()


API
===

``frepr.frepr(value)``
----------------------

Returns shortest string representation of a floating point value.  The argument
doesn't have to be a ``float``, as long as it is coercible to one (i.e. it has
a ``__float__()`` method).

``frepr.install()``
-------------------

Monkey-patches the Python runtime.  Subsequent calls to ``repr()`` on floats
will use the faster algorithm.  This is safe to call multiple times.

``frepr.uninstall()``
---------------------

Reverses the monkey-patching done by ``install()``.  Subsequent calls to
``repr()`` will use the original built-in algorithm.  This is safe to call
multiple times, even if ``install()`` was not called first.


Features
========

* On average, approximately 8X faster than the standard function.  (This result
  obtained on Python 3.6.0 64-bit/Windows 7/Intel Core i3 @ 2.93GHz.  Test your
  own system with `tests/perf.py <tests/perf.py>`_).
* Speeds up the standard ``json`` package (and any other library that relies on
  ``repr()``).
* Output is guaranteed to contain the shortest string of decimal digits
  necessary to uniquely identify the original IEEE 754 floating-point value.
* Guaranteed to satisfy ``value == ast.literal_eval(frepr(value))``.
* Tested on CPython 2.6 through 3.6; probably works on other versions too.


Limitations
===========

* Currently, there is no support for alternative Python implementations (e.g.
  PyPy).
* Output can be slightly different from the native function, due to
  floating-point rounding (but see accuracy guarantees above).
* Has no effect on other built-in methods of converting floats to strings (e.g.
  ``str()``, ``str.format()``, ``"%f"``, ``"%g"``).


See Also
========

* `<https://github.com/google/double-conversion>`_ - Core algorithm
  implementation.  This is the same implementation used by Chrome's V8
  JavaScript engine, among others.
* `<https://bugs.python.org/issue12450>`_ - CPython issue from 2011 about this
  idea.
