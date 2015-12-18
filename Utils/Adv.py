import pyadv
import os
import stat
from pyadv import adv_fdio_file_create, adv_fdio_create

class AdvDatafile:
  OpenError = 'cannot open file'

  def __init__(self, filename, mode='r'):
    self.dfile = None
    self.filename = filename
    try:
        s = os.stat(filename)
    except:
        s = None
    if mode == 'r':
        if s and stat.S_ISREG(s[0]):
            self.dfile = pyadv.adv_fdio_file_open(filename)
    #      del self.__method__['create_document']
    elif mode == 'c':
	if (not s) or (s and stat.S_ISREG(s[0])):
	    self.dfile = pyadv.adv_fdio_file_create(filename)
    if (self.dfile == None):
      raise self.OpenError      

  def __len__(self):
    n = 0
    while pyadv.adv_fdio_open_nth(self.dfile, n):
      n = n + 1
    return n

  def __getitem__(self, index):
    doc = pyadv.adv_fdio_open_nth(self.dfile, index)
    if (doc != None):
      return AdvDocument(doc);
    else:
      return None

  def create_document(self, magic = 'DID'):
    _doc = pyadv.adv_fdio_create(self.dfile, pyadv.adv_dio_make_documentid(magic))
    return AdvDocument(_doc)

class AdvDocument:
  InvalidDocument = 'Invalid Document'

  def get_locator(self):
    return pyadv.adv_dio_get_locator(self.doc);

  def get_documentid(self):
    return pyadv.adv_dio_get_documentid(self.doc);

  def get_size(self):
    return pyadv.adv_dio_get_size(self.doc);

  def get_property(self, key):
    return pyadv.adv_dio_get_property(self.doc, key);

  def get_property_int32(self, key):
    return pyadv.adv_dio_get_property_int32(self.doc, key);

  def get_property_float64(self, key):
    return pyadv.adv_dio_get_property_float64(self.doc, key);

  def get_nth_property(self, index):
    return pyadv.adv_dio_get_nth_property(self.doc, index);

  def read_int8(self, off):
    return pyadv.adv_dio_read_int8(self.doc, off);

  def read_int32(self, off):
    return pyadv.adv_dio_read_int32(self.doc, off);

  def read_int32v(self, off, count):
    return pyadv.adv_dio_read_int32v(self.doc, off, count);

  def read_float64(self, off):
    return pyadv.adv_dio_read_float64(self.doc, off);

  def read_float64v(self, off, count):
    return pyadv.adv_dio_read_float64v(self.doc, off, count);

  def read_octet(self, off, count):
    return pyadv.adv_dio_read_octet(self.doc, off, count);

  def write_octet(self, off, octet):
    return pyadv.adv_dio_write_octet(self.doc, off, octet);

  def read_format(self, off, format):
    i = 0
    ret = [0]
    while (i < len(format)):
      atom = format[i:i+2]
      i = i+2
      f = self.format_func[atom]
      if (f != None):
        r = f(off)
        off = off + r[0]
        ret[0] = ret[0] + r[0]
        ret.append(r[1])
    return tuple(ret)

  def check_property(self, prop):
    for k in prop.keys():
      v = self.get_property(k)
      if (v == None or v != prop[k]):
        return None
    return 1

  def set_property(self, key, val):
    return pyadv.adv_dio_set_property(self.doc, key, '%s' % val)

  def write_int32(self, off, val):
    inc = 0
    if type(val) == type(()):
      for v in val:
        inc = inc + pyadv.adv_dio_write_int32(self.doc, off+inc, v)
    else:
      inc = pyadv.adv_dio_write_int32(self.doc, off, val)
    return inc

  def write_float64(self, off, val):
    inc = 0
    if type(val) == type(()):
      for v in val:
        inc = inc + pyadv.adv_dio_write_float64(self.doc, off+inc, v)
    else:
      inc = pyadv.adv_dio_write_float64(self.doc, off, val)
    return inc

  def __init__(self, doc):
    if type(doc) == type(''):
      doc = pyadv.adv_dio_open_by_locator(doc)
    if not doc:
      self = None
      raise self.InvalidDocument
    else:
      self.doc = doc
    self.format_func = { 'i1' : self.read_int8,
                         'i4' : self.read_int32,
                         'f8' : self.read_float64 }

  def close(self):
    if self.__dict__.has_key('doc'):
        del self.doc

  def __del__(self):
    self.close()

class AdvDatabox:
  NoSuchDocument = 'No such Document'

  def find_by_documentid(self, arg):
    doc = pyadv.adv_dbox_find_by_documentid(self.dbox, arg)
    return doc

  def find_by_property(self, prev, arg):
    doc = None
    n = 0
    while not None:
      d = pyadv.adv_dbox_open_nth(self.dbox, n)
      if not d:
        break
      doc = AdvDocument(d)
      n = n+1
      if doc.check_property(arg):
        if prev and doc.get_documentid() == prev.get_documentid():
          prev = None
          doc = None
        else:
          break
    return doc

  def add(self, file):
    pyadv.adv_dbox_add(self.dbox, file)

  def __getitem__(self, index):
    d = pyadv.adv_dbox_open_nth(self.dbox, index)
    if d:
	return AdvDocument(d)
    return None

  def __len__(self):
    n = 0
    while pyadv.adv_dbox_open_nth(self.dbox, n):
      n = n + 1
    return n

  def __init__(self):
    self.dbox = pyadv.adv_dbox_new()
