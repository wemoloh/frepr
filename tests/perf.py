from __future__ import print_function

from random import getrandbits
from struct import pack, unpack
from timeit import timeit

import frepr

def random_double():
    return unpack('d', pack('Q', getrandbits(64)))[0]

values = []

def repr_all():
    for value in values:
        repr(value)

def main(nvalues, nruns):
    print('Creating {} random double values...'.format(nvalues))
    global values
    values = [random_double() for _ in range(nvalues)]
    
    print('Applying native repr() to all values {} times...'.format(nruns))
    t_repr = timeit(repr_all, number=nruns)
    print('{} seconds'.format(t_repr))
    
    print('Applying new repr() to all values {} times...'.format(nruns))
    frepr.install()
    t_frepr = timeit(repr_all, number=nruns)
    frepr.uninstall()
    print('{} seconds'.format(t_frepr))
    
    print('New repr() is {} times faster.'.format(t_repr / t_frepr))

if __name__ == '__main__':
    main(1000000, 10)
