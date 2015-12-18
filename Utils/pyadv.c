/***********************************************************
Copyright (C) 2000,2001,2002,2006
Shinobu Yoshimura, University of Tokyo,
the Japan Society for the Promotion of Science (JSPS)
All Rights Reserved
***********************************************************/

#include <Python.h>
#include <sysmodule.h>
#include <Adv/AdvDocument.h>

/*------------------------------------------------------------*/

typedef struct {
  PyObject_HEAD
  AdvDocFile* dfile;
  bool closed;
} PyAdvDfile_Object;

staticforward PyTypeObject PyAdvDfile_Type;

#define PyAdvDfile_Check(v)   ((v)->ob_type == &PyAdvDfile_Type)
#define PyAdvDfile_Get(v)     (((PyAdvDfile_Object *)(v))->dfile)

static PyObject *
PyAdvDfile_New(AdvDocFile* dfile)
{
  PyAdvDfile_Object *self;

  self = (PyAdvDfile_Object *)PyObject_NEW(PyAdvDfile_Object, &PyAdvDfile_Type);
  if (self == NULL)
    return NULL;
  self->dfile = dfile;
  self->closed = false;
  return (PyObject *)self;
}

static void 
PyAdvDfile_dealloc(PyAdvDfile_Object *self)
{
  if (!self->closed)
    {
      adv_dio_file_close(self->dfile);
    }
  PyMem_DEL(self);
}

static int
PyAdvDfile_compare(PyAdvDfile_Object *self, PyAdvDfile_Object *v)
{
  if (self->dfile == v->dfile) return 0;
  if (self->dfile > v->dfile) return -1;
  return 1;
}

static PyObject *
PyAdvDfile_repr(PyAdvDfile_Object *self)
{
  char buf[100];

  sprintf(buf, "<AdvDatafile at %lx>", (long)PyAdvDfile_Get(self));
  return PyString_FromString(buf);
}

static char PyAdvDfile_Type__doc__[] = "This is the adv datafile Objects";

static PyTypeObject PyAdvDfile_Type =
{
  PyObject_HEAD_INIT(&PyType_Type)
  0,						  /*ob_size*/
  "AdvDatafile",				  /*tp_name*/
  sizeof(PyAdvDfile_Object),			  /*tp_basicsize*/
  0,						  /*tp_itemsize*/
  (destructor)PyAdvDfile_dealloc,		  /*tp_dealloc*/
  (printfunc)0,					  /*tp_print*/
  (getattrfunc)0,				  /*tp_getattr*/
  (setattrfunc)0,				  /*tp_setattr*/
  (cmpfunc)PyAdvDfile_compare,			  /*tp_compare*/
  (reprfunc)PyAdvDfile_repr,			  /*tp_repr*/
  0,						  /*tp_as_number*/
  0,						  /*tp_as_sequence*/
  0,						  /*tp_as_mapping*/
  (hashfunc)0,					  /*tp_hash*/
  (ternaryfunc)0,				  /*tp_call*/
  (reprfunc)0,					  /*tp_str*/
  0L,0L,0L,0L,
  PyAdvDfile_Type__doc__
};

/*------------------------------------------------------------*/

typedef struct {
  PyObject_HEAD
  AdvDocument* doc;
  bool closed;
} PyAdvDoc_Object;

staticforward PyTypeObject PyAdvDoc_Type;

#define PyAdvDoc_Check(v)   ((v)->ob_type == &PyAdvDoc_Type)
#define PyAdvDoc_Get(v)     (((PyAdvDoc_Object *)(v))->doc)

static PyObject *
PyAdvDoc_New(AdvDocument* doc)
{
  PyAdvDoc_Object *self;

  self = (PyAdvDoc_Object*)PyObject_NEW(PyAdvDoc_Object, &PyAdvDoc_Type);
  if (self == NULL)
    return NULL;
  self->doc = doc;
  self->closed = false;
  return (PyObject *)self;
}

static void 
PyAdvDoc_dealloc(PyAdvDoc_Object *self)
{
  if (!self->closed)
    {
      adv_dio_close(self->doc);
    }
  PyMem_DEL(self);
}

static int
PyAdvDoc_compare(PyAdvDoc_Object *self, PyAdvDoc_Object *v)
{
  if (self->doc == v->doc) return 0;
  if (self->doc > v->doc) return -1;
  return 1;
}

static PyObject *
PyAdvDoc_repr(PyAdvDoc_Object *self)
{
  char buf[1000];

  sprintf(buf, "<AdvDocument at %lx : %s>", (long)PyAdvDoc_Get(self), adv_dio_get_locator(self->doc));
  return PyString_FromString(buf);
}

static char PyAdvDoc_Type__doc__[] = "This is the adv document Objects";

static PyTypeObject PyAdvDoc_Type =
{
  PyObject_HEAD_INIT(&PyType_Type)
  0,						  /*ob_size*/
  "AdvDocument",				  /*tp_name*/
  sizeof(PyAdvDoc_Object),			  /*tp_basicsize*/
  0,						  /*tp_itemsize*/
  (destructor)PyAdvDoc_dealloc,			  /*tp_dealloc*/
  (printfunc)0,					  /*tp_print*/
  (getattrfunc)0,				  /*tp_getattr*/
  (setattrfunc)0,				  /*tp_setattr*/
  (cmpfunc)PyAdvDoc_compare,			  /*tp_compare*/
  (reprfunc)PyAdvDoc_repr,			  /*tp_repr*/
  0,						  /*tp_as_number*/
  0,						  /*tp_as_sequence*/
  0,						  /*tp_as_mapping*/
  (hashfunc)0,					  /*tp_hash*/
  (ternaryfunc)0,				  /*tp_call*/
  (reprfunc)0,					  /*tp_str*/
  0L,0L,0L,0L,
  PyAdvDoc_Type__doc__
};

/*------------------------------------------------------------*/

typedef struct {
  PyObject_HEAD
  AdvDatabox* dbox;
  bool closed;
} PyAdvDbox_Object;

staticforward PyTypeObject PyAdvDbox_Type;

#define PyAdvDbox_Check(v)   ((v)->ob_type == &PyAdvDbox_Type)
#define PyAdvDbox_Get(v)     (((PyAdvDbox_Object *)(v))->dbox)

static PyObject *
PyAdvDbox_New(AdvDatabox* dbox)
{
  PyAdvDbox_Object *self;

  self = (PyAdvDbox_Object*)PyObject_NEW(PyAdvDbox_Object, &PyAdvDbox_Type);
  if (self == NULL)
    return NULL;
  self->dbox = dbox;
  self->closed = false;
  return (PyObject *)self;
}

static void 
PyAdvDbox_dealloc(PyAdvDbox_Object *self)
{
  if (!self->closed)
    adv_dbox_close(self->dbox);
  PyMem_DEL(self);
}

static int
PyAdvDbox_compare(PyAdvDbox_Object *self, PyAdvDbox_Object *v)
{
  if (self->dbox == v->dbox) return 0;
  if (self->dbox > v->dbox) return -1;
  return 1;
}

static PyObject *
PyAdvDbox_repr(PyAdvDbox_Object *self)
{
  char buf[1000];

  sprintf(buf, "<AdvDatabox at %lx>", (long)PyAdvDbox_Get(self));
  return PyString_FromString(buf);
}

static char PyAdvDbox_Type__doc__[] = "This is the adv databox Objects";

static PyTypeObject PyAdvDbox_Type =
{
  PyObject_HEAD_INIT(&PyType_Type)
  0,						  /*ob_size*/
  "AdvDatabox",					  /*tp_name*/
  sizeof(PyAdvDbox_Object),			  /*tp_basicsize*/
  0,						  /*tp_itemsize*/
  (destructor)PyAdvDbox_dealloc,		  /*tp_dealloc*/
  (printfunc)0,					  /*tp_print*/
  (getattrfunc)0,				  /*tp_getattr*/
  (setattrfunc)0,				  /*tp_setattr*/
  (cmpfunc)PyAdvDbox_compare,			  /*tp_compare*/
  (reprfunc)PyAdvDbox_repr,			  /*tp_repr*/
  0,						  /*tp_as_number*/
  0,						  /*tp_as_sequence*/
  0,						  /*tp_as_mapping*/
  (hashfunc)0,					  /*tp_hash*/
  (ternaryfunc)0,				  /*tp_call*/
  (reprfunc)0,					  /*tp_str*/
  0L,0L,0L,0L,
  PyAdvDbox_Type__doc__
};

/*------------------------------------------------------------*/

typedef struct {
  PyObject_HEAD
  octet* b;
  int len;
} OctetSeq_Object;

staticforward PyTypeObject OctetSeq_Type;

#define OctetSeq_Check(v)   ((v)->ob_type == &OctetSeq_Type)
#define OctetSeq_Get(v)     (((OctetSeq_Object*)(v))->b)

static PyObject *
OctetSeq_New(int len)
{
  OctetSeq_Object *self;

  self = (OctetSeq_Object*)PyObject_NEW(OctetSeq_Object, &OctetSeq_Type);
  if (self == NULL)
    return NULL;
  self->b = (octet*)malloc(sizeof(octet)*len);
  self->len = len;
  return (PyObject *)self;
}

static void 
OctetSeq_dealloc(OctetSeq_Object *self)
{
  free(self->b);
  self->b = NULL;
  PyMem_DEL(self);
}

static int
OctetSeq_compare(OctetSeq_Object *self, OctetSeq_Object *v)
{
  if (self->b == v->b) return 0;
  if (self->b > v->b) return -1;
  return 1;
}

static PyObject *
OctetSeq_repr(OctetSeq_Object *self)
{
  char buf[1000];

  sprintf(buf, "<OctetSequence at %lx length %d>", (long)OctetSeq_Get(self), self->len);
  return PyString_FromString(buf);
}

int
OctetSeq_length(OctetSeq_Object *self)
{
  return self->len;
}

static PyObject *
OctetSeq_item(OctetSeq_Object *self, int n)
{
  return PyInt_FromLong(self->b[n]);
}

static char OctetSeq_Type__doc__[] = "This is the adv databox Objects";

static PySequenceMethods OctetSeq_SeqMethod =
{
  (inquiry)OctetSeq_length,			  /* sq_length */
  NULL,						  /* sq_concat */
  NULL,						  /* sq_repeat */
  (intargfunc)OctetSeq_item,			  /* sq_item */
  NULL,						  /* sq_slice */
  NULL,						  /* sq_ass_item */
  NULL,						  /* sq_ass_slice */
};

static PyTypeObject OctetSeq_Type =
{
  PyObject_HEAD_INIT(&PyType_Type)
  0,						  /*ob_size*/
  "OctetSequence",				  /*tp_name*/
  sizeof(OctetSeq_Object),			  /*tp_basicsize*/
  0,						  /*tp_itemsize*/
  (destructor)OctetSeq_dealloc,		  /*tp_dealloc*/
  (printfunc)0,					  /*tp_print*/
  (getattrfunc)0,				  /*tp_getattr*/
  (setattrfunc)0,				  /*tp_setattr*/
  (cmpfunc)OctetSeq_compare,			  /*tp_compare*/
  (reprfunc)OctetSeq_repr,			  /*tp_repr*/
  0,						  /*tp_as_number*/
  &OctetSeq_SeqMethod,				  /*tp_as_sequence*/
  0,						  /*tp_as_mapping*/
  (hashfunc)0,					  /*tp_hash*/
  (ternaryfunc)0,				  /*tp_call*/
  (reprfunc)0,					  /*tp_str*/
  0L,0L,0L,0L,
  OctetSeq_Type__doc__
};
/*------------------------------------------------------------*/

static
PyObject*
_adv_fdio_file_open(PyObject* self, PyObject* args)
{
  char* filename;
  AdvDocFile* dfile;
  PyArg_ParseTuple(args, "s:adv_fdio_file_open", &filename);
  dfile = adv_dio_file_open(filename, "r");
  if (dfile)
    return PyAdvDfile_New(dfile);
  Py_INCREF(Py_None);
  return Py_None;
}

static
PyObject*
_adv_fdio_file_create(PyObject* self, PyObject* args)
{
  char* filename;
  AdvDocFile* dfile;
  PyArg_ParseTuple(args, "s:adv_fdio_file_create", &filename);
  dfile = adv_dio_file_open(filename, "c");
  if (dfile)
    return PyAdvDfile_New(dfile);
  Py_INCREF(Py_None);
  return Py_None;
}

static
PyObject*
_adv_fdio_file_close(PyObject* self, PyObject* args)
{
  PyAdvDfile_Object* obj;
  PyArg_ParseTuple(args, "O!:adv_fdio_file_clese", &PyAdvDfile_Type, &obj);
  if (!obj->closed)
    {
      adv_dio_file_close(obj->dfile);
      obj->closed = true;
    }
  Py_INCREF(Py_None);
  return Py_None;
}

static
PyObject*
_adv_fdio_open_nth(PyObject* self, PyObject* args)
{
  PyAdvDfile_Object* obj;
  int n;
  AdvDocument* doc;
  
  PyArg_ParseTuple(args, "O!i:adv_fdio_open_nth", &PyAdvDfile_Type, &obj, &n);
  doc = adv_dio_open_nth(obj->dfile, n);
  if (doc)
    return PyAdvDoc_New(doc);

  Py_INCREF(Py_None);
  return Py_None;
}

static
PyObject*
_adv_fdio_create(PyObject* self, PyObject* args)
{
  PyAdvDfile_Object* obj;
  const char* did;
  AdvDocument* doc;
  
  PyArg_ParseTuple(args, "O!s:adv_fdio_create", &PyAdvDfile_Type, &obj, &did);
  if (!did)
    did = adv_dio_make_documentid("DID");
  doc = adv_dio_create(obj->dfile, did);
  if (doc)
    return PyAdvDoc_New(doc);

  Py_INCREF(Py_None);
  return Py_None;
}

/*------------------------------------------------------------*/

static
PyObject*
_adv_dio_open_by_locator(PyObject* self, PyObject* args)
{
  char* locator;
  AdvDocument* doc;
  
  PyArg_ParseTuple(args, "s:adv_dio_open_by_locator", &locator);
  if (locator)
    {
      doc = adv_dio_open_by_locator(locator);
      if (doc)
	return PyAdvDoc_New(doc);
    }
  Py_INCREF(Py_None);
  return Py_None;
}

/*------------------------------------------------------------*/

static
PyObject*
_adv_dio_make_documentid(PyObject* self, PyObject* args)
{
  char* token;
  const char* did;
  
  PyArg_ParseTuple(args, "s:adv_dio_make_documentid", &token);
  if (!token)
    token = "DID";
  did = adv_dio_make_documentid(token);
  if (did)
    return PyString_FromString(did);

  Py_INCREF(Py_None);
  return Py_None;
}

static
PyObject*
_adv_dio_get_locator(PyObject* self, PyObject* args)
{
  PyAdvDoc_Object* obj;
  const char* loc;
  
  PyArg_ParseTuple(args, "O!:adv_dio_get_locator", &PyAdvDoc_Type, &obj);
  loc = adv_dio_get_locator(obj->doc);
  if (loc)
    return PyString_FromString(loc);

  Py_INCREF(Py_None);
  return Py_None;
}

static
PyObject*
_adv_dio_get_documentid(PyObject* self, PyObject* args)
{
  PyAdvDoc_Object* obj;
  const char* did;
  
  PyArg_ParseTuple(args, "O!:adv_dio_get_documentid", &PyAdvDoc_Type, &obj);
  did = adv_dio_get_documentid(obj->doc);
  if (did)
    return PyString_FromString(did);

  Py_INCREF(Py_None);
  return Py_None;
}

static
PyObject*
_adv_dio_get_size(PyObject* self, PyObject* args)
{
  PyAdvDoc_Object* obj;
  adv_off_t size;
  
  PyArg_ParseTuple(args, "O!:adv_dio_get_size", &PyAdvDoc_Type, &obj);
  size = adv_dio_get_size(obj->doc);
  return PyInt_FromLong(size);
}

static
PyObject*
_adv_dio_get_property(PyObject* self, PyObject* args)
{
  PyAdvDoc_Object* obj;
  char* key;
  const char* val;
  
  PyArg_ParseTuple(args, "O!s:adv_dio_get_property", &PyAdvDoc_Type, &obj, &key);
  val = adv_dio_get_property(obj->doc, key);
  if (val)
    return PyString_FromString(val);

  Py_INCREF(Py_None);
  return Py_None;
}

static
PyObject*
_adv_dio_get_property_int32(PyObject* self, PyObject* args)
{
  PyAdvDoc_Object* obj;
  char* key;
  int32 val;
  bool b;
  
  PyArg_ParseTuple(args, "O!s:adv_dio_get_property_int32", &PyAdvDoc_Type, &obj, &key);
  b = adv_dio_get_property_int32(obj->doc, key, &val);
  if (b)
    return PyInt_FromLong(val);

  Py_INCREF(Py_None);
  return Py_None;
}

static
PyObject*
_adv_dio_get_property_float64(PyObject* self, PyObject* args)
{
  PyAdvDoc_Object* obj;
  char* key;
  float64 val;
  bool b;
  
  PyArg_ParseTuple(args, "O!s:adv_dio_get_property_float64", &PyAdvDoc_Type, &obj, &key);
  b = adv_dio_get_property_float64(obj->doc, key, &val);
  if (b)
    return PyFloat_FromDouble(val);

  Py_INCREF(Py_None);
  return Py_None;
}

static
PyObject*
_adv_dio_get_nth_property(PyObject* self, PyObject* args)
{
  PyAdvDoc_Object* obj;
  int i;
  char key[1024];
  char val[4096];
  bool b;
  PyObject* tuple;
  
  PyArg_ParseTuple(args, "O!i:adv_dio_get_nth_property", &PyAdvDoc_Type, &obj, &i);
  b = adv_dio_get_nth_property(obj->doc, i, key, sizeof(key), val, sizeof(val));
  if (b)
    {
      tuple = PyTuple_New(2);
      PyTuple_SetItem(tuple, 0, PyString_FromString(key));
      PyTuple_SetItem(tuple, 1, PyString_FromString(val));
      return tuple;
    }

  Py_INCREF(Py_None);
  return Py_None;
}

/*-----------------------------------------------------------*/

static
PyObject*
_adv_dio_read_int8(PyObject* self, PyObject* args)
{
  PyAdvDoc_Object* obj;
  int off;
  int inc;
  int8 val;
  PyObject* tuple;
  
  PyArg_ParseTuple(args, "O!i:adv_dio_read_int8", &PyAdvDoc_Type, &obj, &off);
  inc = adv_dio_read_int8(obj->doc, off, &val);
  /* printf("off %d val %d\n", (int)off, val); */

  tuple = PyTuple_New(2);
  PyTuple_SetItem(tuple, 0, PyInt_FromLong(inc));
  PyTuple_SetItem(tuple, 1, PyInt_FromLong(val));

  return tuple;
}

static
PyObject*
_adv_dio_read_int32(PyObject* self, PyObject* args)
{
  PyAdvDoc_Object* obj;
  int off;
  int inc;
  int32 val;
  PyObject* tuple;
  
  PyArg_ParseTuple(args, "O!i:adv_dio_read_int32", &PyAdvDoc_Type, &obj, &off);
  inc = adv_dio_read_int32(obj->doc, off, &val);
  /* printf("off %d val %d\n", (int)off, val); */

  tuple = PyTuple_New(2);
  PyTuple_SetItem(tuple, 0, PyInt_FromLong(inc));
  PyTuple_SetItem(tuple, 1, PyInt_FromLong(val));

  return tuple;
}

static
PyObject*
_adv_dio_read_int32v(PyObject* self, PyObject* args)
{
  int i;
  PyAdvDoc_Object* obj;
  int off;
  int count;
  int inc;
  int32* val;
  PyObject* tuple;
  
  PyArg_ParseTuple(args, "O!ii:adv_dio_read_int32v", &PyAdvDoc_Type, &obj, &off, &count);
  tuple = PyTuple_New(count+1);
  val = (int32*)malloc(sizeof(int32)*count);
  inc = adv_dio_read_int32v(obj->doc, off, count, val);
  PyTuple_SetItem(tuple, 0, PyInt_FromLong(inc));
  for (i = 0; i < count; i++)
    PyTuple_SetItem(tuple, i+1, PyInt_FromLong(val[i]));
  free(val);

  return tuple;
}

static
PyObject*
_adv_dio_read_float64(PyObject* self, PyObject* args)
{
  PyAdvDoc_Object* obj;
  int off;
  int inc;
  float64 val;
  PyObject* tuple;
  
  PyArg_ParseTuple(args, "O!i:adv_dio_read_float64", &PyAdvDoc_Type, &obj, &off);
  inc = adv_dio_read_float64(obj->doc, off, &val);

  tuple = PyTuple_New(2);
  PyTuple_SetItem(tuple, 0, PyInt_FromLong(inc));
  PyTuple_SetItem(tuple, 1, PyFloat_FromDouble(val));

  return tuple;
}

static
PyObject*
_adv_dio_read_float64v(PyObject* self, PyObject* args)
{
  int i;
  PyAdvDoc_Object* obj;
  int off;
  int count;
  int inc;
  float64* val;
  PyObject* tuple;
  
  PyArg_ParseTuple(args, "O!ii:adv_dio_read_int64v", &PyAdvDoc_Type, &obj, &off, &count);
  tuple = PyTuple_New(count+1);
  val = (float64*)malloc(sizeof(float64)*count);
  inc = adv_dio_read_float64v(obj->doc, off, count, val);
  PyTuple_SetItem(tuple, 0, PyInt_FromLong(inc));
  for (i = 0; i < count; i++)
    PyTuple_SetItem(tuple, i+1, PyFloat_FromDouble(val[i]));
  free(val);

  return tuple;
}

static
PyObject*
_adv_dio_read_octet(PyObject* self, PyObject* args)
{
  PyAdvDoc_Object* obj;
  int off;
  int count;
  int inc;
  OctetSeq_Object* seq;
  
  PyArg_ParseTuple(args, "O!ii:adv_dio_read_octet", &PyAdvDoc_Type, &obj, &off, &count);
  seq = (OctetSeq_Object*)OctetSeq_New(count);
  inc = adv_dio_read_octet(obj->doc, off, count, seq->b);
  return (PyObject*)seq;
  /*  return PyInt_FromLong(inc); */
}

/*------------------------------------------------------------*/

static
PyObject*
_adv_dio_set_property(PyObject* self, PyObject* args)
{
  PyAdvDoc_Object* obj;
  char* key;
  char* val;
  
  PyArg_ParseTuple(args, "O!ss:adv_dio_get_property", &PyAdvDoc_Type, &obj, &key, &val);
  adv_dio_set_property(obj->doc, key, val);

  Py_INCREF(Py_None);
  return Py_None;
}

static
PyObject*
_adv_dio_unset_nth_property(PyObject* self, PyObject* args)
{
  PyAdvDoc_Object* obj;
  int n;
  
  PyArg_ParseTuple(args, "O!i:adv_dio_get_property", &PyAdvDoc_Type, &obj, &n);
  adv_dio_unset_nth_property(obj->doc, n);

  Py_INCREF(Py_None);
  return Py_None;
}

/*-----------------------------------------------------------*/

static
PyObject*
_adv_dio_write_int32(PyObject* self, PyObject* args)
{
  PyAdvDoc_Object* obj;
  int off;
  int inc;
  int val;
  
  PyArg_ParseTuple(args, "O!ii:adv_dio_write_int32", &PyAdvDoc_Type, &obj, &off, &val);
  inc = adv_dio_write_int32(obj->doc, off, val);

  return PyInt_FromLong(inc);
}

static
PyObject*
_adv_dio_write_float64(PyObject* self, PyObject* args)
{
  PyAdvDoc_Object* obj;
  int off;
  int inc;
  double val;
  
  PyArg_ParseTuple(args, "O!id:adv_dio_write_float64", &PyAdvDoc_Type, &obj, &off, &val);
  inc = adv_dio_write_float64(obj->doc, off, val);

  return PyInt_FromLong(inc);
}

static
PyObject*
_adv_dio_write_octet(PyObject* self, PyObject* args)
{
  PyAdvDoc_Object* obj;
  int off;
  int count;
  int inc;
  OctetSeq_Object* seq;
  
  PyArg_ParseTuple(args, "O!iO:adv_dio_write_octet", &PyAdvDoc_Type, &obj, &off, &seq);
  inc = adv_dio_write_octet(obj->doc, off, seq->len, seq->b);
  return PyInt_FromLong(inc);
}

/*------------------------------------------------------------*/

static
PyObject*
_adv_dbox_new(PyObject* self, PyObject* args)
{
  AdvDatabox* dbox;
  dbox = adv_dbox_new();
  return PyAdvDbox_New(dbox);
}

static
PyObject*
_adv_dbox_add(PyObject* obj, PyObject* args)
{
  char* filename;
  PyAdvDbox_Object* self;

  PyArg_ParseTuple(args, "O!s:adv_dbox_add", &PyAdvDbox_Type, &self, &filename);
  adv_dbox_add(self->dbox, filename);

  Py_INCREF(Py_None);
  return Py_None;
}

static
PyObject*
_adv_dbox_close(PyObject* obj, PyObject* args)
{
  char* filename;
  PyAdvDbox_Object* self;

  PyArg_ParseTuple(args, "O!:adv_dbox_close", &PyAdvDbox_Type, &self);
  if (!self->closed)
    {
      adv_dbox_close(self->dbox);
      self->closed = true;
    }

  Py_INCREF(Py_None);
  return Py_None;
}

static
PyObject*
_adv_dbox_find_by_documentid(PyObject* obj, PyObject* args)
{
  char* did;
  PyAdvDbox_Object* self;
  AdvDocument* doc;

  PyArg_ParseTuple(args, "O!s:adv_dbox_find_by_documentid", &PyAdvDbox_Type, &self, &did);
  doc = adv_dbox_find_by_documentid(self->dbox, did);
  if (doc)
    return PyAdvDoc_New(doc);

  Py_INCREF(Py_None);
  return Py_None;
}

static
PyObject*
_adv_dbox_open_nth(PyObject* obj, PyObject* args)
{
  int n;
  PyAdvDbox_Object* self;
  AdvDocument* doc;

  PyArg_ParseTuple(args, "O!i:adv_dbox_open_nth", &PyAdvDbox_Type, &self, &n);
  doc = adv_dbox_open_nth(self->dbox, n);
  if (doc)
    return PyAdvDoc_New(doc);

  Py_INCREF(Py_None);
  return Py_None;
}

/*------------------------------------------------------------*/

static PyMethodDef PyAdvDfile_Methods[] =
{
  { "adv_fdio_file_open", _adv_fdio_file_open, 1 },
  { "adv_fdio_file_create", _adv_fdio_file_create, 1 },
  { "adv_fdio_file_close", _adv_fdio_file_close, 1 },
  { "adv_fdio_open_nth", _adv_fdio_open_nth, 1 },
  { "adv_fdio_create", _adv_fdio_create, 1 },

  { "adv_dio_make_documentid", _adv_dio_make_documentid, 1 },

  { "adv_dio_open_by_locator", _adv_dio_open_by_locator, 1 },
  { "adv_dio_get_locator", _adv_dio_get_locator, 1 },
  { "adv_dio_get_documentid", _adv_dio_get_documentid, 1 },
  { "adv_dio_get_size", _adv_dio_get_size, 1 },
  { "adv_dio_get_property", _adv_dio_get_property, 1 },
  { "adv_dio_get_property_int32", _adv_dio_get_property_int32, 1 },
  { "adv_dio_get_property_float64", _adv_dio_get_property_float64, 1 },
  { "adv_dio_get_nth_property", _adv_dio_get_nth_property, 1 },
  { "adv_dio_read_int8", _adv_dio_read_int8, 1 },
  { "adv_dio_read_int32", _adv_dio_read_int32, 1 },
  { "adv_dio_read_int32v", _adv_dio_read_int32v, 1 },
  { "adv_dio_read_float64", _adv_dio_read_float64, 1 },
  { "adv_dio_read_float64v", _adv_dio_read_float64v, 1 },
  { "adv_dio_read_octet", _adv_dio_read_octet, 1 },

  { "adv_dio_set_property", _adv_dio_set_property, 1 },
  { "adv_dio_unset_nth_property", _adv_dio_unset_nth_property, 1 },
  { "adv_dio_write_int32", _adv_dio_write_int32, 1 },
  { "adv_dio_write_float64", _adv_dio_write_float64, 1 },
  { "adv_dio_write_octet", _adv_dio_write_octet, 1 },

  { "adv_dbox_new", _adv_dbox_new, 1 },
  { "adv_dbox_close", _adv_dbox_close, 1 },
  { "adv_dbox_add", _adv_dbox_add, 1 },
  { "adv_dbox_find_by_documentid", _adv_dbox_find_by_documentid, 1 },
  { "adv_dbox_open_nth", _adv_dbox_open_nth, 1 },

  { NULL, NULL }
};

void
initpyadv()
{
  int ac = 1;
  char* avv[] = { "pyadv" };
  char** av = avv;
  Py_InitModule("pyadv", PyAdvDfile_Methods);
  adv_dio_init(&ac, &av);
}
