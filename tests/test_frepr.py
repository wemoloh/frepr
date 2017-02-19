from ast import literal_eval
from random import getrandbits
from struct import pack, unpack

import frepr

inf = float('inf')
nan = float('nan')

def test_special():
    assert 'inf' == frepr.frepr(inf)
    assert '-inf' == frepr.frepr(-inf)
    assert 'nan' == frepr.frepr(nan)
    assert '0.0' == frepr.frepr(0.0)
    assert '-0.0' == frepr.frepr(-0.0)

def random_double():
    return unpack('d', pack('Q', getrandbits(64)))[0]

def test_random():
    for _ in range(100000):
        d = random_double()
        r = repr(d)
        f = frepr.frepr(d)
        if r in ('inf', '-inf', 'nan'):
            assert r == f
        else:
            assert d == literal_eval(f)
        assert len(r) >= len(f)

def test_install():
    # Normal repr adds a leading zero to the exponent; frepr does not.
    assert repr(1e-6) == '1e-06'
    
    ds = [random_double() for _ in range(10000)]
    rs = [repr(d) for d in ds]
    
    frepr.install()
    try:
        assert repr(1e-6) == '1e-6'
        fs = [repr(d) for d in ds]
    finally:
        frepr.uninstall()
    
    for r, f in zip(rs, fs):
        assert r == f or float(r) == float(f)
