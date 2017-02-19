#include "Python.h"
#include "double-conversion.h"

using namespace double_conversion;

const int flags =
    DoubleToStringConverter::EMIT_POSITIVE_EXPONENT_SIGN |
    DoubleToStringConverter::EMIT_TRAILING_DECIMAL_POINT |
    DoubleToStringConverter::EMIT_TRAILING_ZERO_AFTER_POINT;

static DoubleToStringConverter conv(flags, "inf", "nan", 'e', -4, 16, 0, 0);

static PyObject *dtoa(double f) {
    // max string length would be for something like
    // -1.1111111111111111e+100
    // (24 characters)
    char buf[28]; 
    StringBuilder sb(buf, 28);
    conv.ToShortest(f, &sb);
    return PyString_FromStringAndSize(buf, (Py_ssize_t)sb.position());
}

static PyObject *frepr(PyObject *self, PyObject *pyf) {
    double f = PyFloat_AsDouble(pyf);
    if (f == -1 && PyErr_Occurred()) {
        return NULL;
    }
    return dtoa(f);
}

static PyObject *float_repr(PyFloatObject *v) {
    return dtoa(PyFloat_AS_DOUBLE(v));
}

static reprfunc orig_float_repr = NULL;

static PyObject *install(PyObject *self, PyObject *unused) {
    PyFloat_Type.tp_repr = (reprfunc)float_repr;
    Py_RETURN_NONE;
}

static PyObject *uninstall(PyObject *self, PyObject *unused) {
    PyFloat_Type.tp_repr = orig_float_repr;
    Py_RETURN_NONE;
}

static PyMethodDef methods[] = {
  {"frepr", (PyCFunction)frepr, METH_O, 0},
  {"install", (PyCFunction)install, METH_NOARGS, 0},
  {"uninstall", (PyCFunction)uninstall, METH_NOARGS, 0},
  {0, 0, 0, 0}
};

PyMODINIT_FUNC initfrepr(void)
{
    orig_float_repr = PyFloat_Type.tp_repr;
    Py_InitModule("frepr", methods);
}
