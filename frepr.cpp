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
    
    #if PY_MAJOR_VERSION < 3
    return PyString_FromStringAndSize(buf, (Py_ssize_t)sb.position());
    #else
    return PyUnicode_DecodeUTF8(buf, (Py_ssize_t)sb.position(), "strict");
    #endif
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

#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef moduledef = {
    #if PY_VERSION_HEX < 0x03020000
    { PyObject_HEAD_INIT(NULL) NULL, 0, NULL },
    #else
    PyModuleDef_HEAD_INIT,
    #endif
    "frepr",
    NULL, /* m_doc */
    -1, /* m_size */
    methods, /* m_methods */
    NULL, /* m_reload */
    NULL, /* m_traverse */
    NULL, /* m_clear */
    NULL /* m_free */
};
#endif

#if PY_MAJOR_VERSION < 3
PyMODINIT_FUNC initfrepr(void)
#else
PyMODINIT_FUNC PyInit_frepr(void)
#endif
{
    orig_float_repr = PyFloat_Type.tp_repr;
    
    #if PY_MAJOR_VERSION < 3
    Py_InitModule("frepr", methods);
    #else
    return PyModule_Create(&moduledef);
    #endif
}
