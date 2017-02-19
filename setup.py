from glob import glob
from os.path import join
from setuptools import setup, Extension

DBLCONV = 'double-conversion/double-conversion'

sources = glob(join(DBLCONV, '*.cc'))
sources.append('frepr.cpp')

setup(
    name='frepr',
    version='0.1.0',
    description='Fast float repr',
    author='Bart Robinson',
    author_email='bart@bartrobinson.com',
    license='BSD 3-Clause License',
    url='https://github.com/wemoloh/frepr',
    classifiers=[
        'Development Status :: 3 - Alpha',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: BSD License',
        'Programming Language :: Python :: 2',
        'Programming Language :: Python :: 3'
    ],
    ext_modules=[
        Extension('frepr',
            sources=sources,
            include_dirs=[DBLCONV]
        )
    ]
)
