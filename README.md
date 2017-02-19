# frepr

[![Build Status](https://travis-ci.org/wemoloh/frepr.svg?branch=master)](https://travis-ci.org/wemoloh/frepr)

Fast float repr for CPython using https://github.com/google/double-conversion.

## Usage

```python
>>> import frepr
>>> frepr.frepr(11111111111111111.)
'1.1111111111111112e+16'
>>> from timeit import timeit
>>> timeit('repr(11111111111111111.)')
2.317620810403102
>>> frepr.install()
>>> timeit('repr(11111111111111111.)')
0.732992553966163
>>> frepr.uninstall()
```

## Requirements

- CPython 2.7 (other versions coming soon)
