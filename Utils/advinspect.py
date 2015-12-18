#! /usr/bin/python
import Adv
import sys

class DocPrinter:
  def __init__(self, doc):
    self.doc = doc
  
  def Print(self):
    print doc.get_locator()
    i = 0
    while not None:
      p = self.doc.get_nth_property(i)
      if (p == None):
        break
      i = i+1
      print '  ', p[0], '=', p[1]
    print

class NodeDocPrinter(DocPrinter):
  def Print(self):
    DocPrinter.Print(self)
    fmt = self.doc.get_property('format')
    items = self.doc.get_property_int32('num_items')
    off = 0
    for i in range(0, items):
      r = self.doc.read_format(off, fmt)
      off = off+r[0]
      for j in r[1:]:
        print j,
      print
    
class ElemDocPrinter(DocPrinter):
  def Print(self):
    DocPrinter.Print(self)
    fmt = self.doc.get_property('format')
    items = self.doc.get_property_int32('num_items')
    off = 0
    for i in range(0, items):
      r = self.doc.read_format(off, fmt)
      off = off+r[0]
      for j in r[1:]:
        print j,
      print
    
def CreateDocPrinter(doc):
  if doc.check_property({'content_type' : 'Node'}):
    return NodeDocPrinter(doc)
  elif doc.check_property({'content_type' : 'Element'}):
    return ElemDocPrinter(doc)
  elif doc.check_property({'content_type' : 'FEGenericAttribute'}):
    return ElemDocPrinter(doc)
  return DocPrinter(doc)


if not sys.argv[1]:
  sys.exit(0)

try:
  doc = Adv.AdvDocument(sys.argv[1])
except:
  print "Invalid Locator `%s'" % sys.argv[1]
  sys.exit(0)

CreateDocPrinter(doc).Print()

