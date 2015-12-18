#!/usr/bin/python
import Adv
import sys

def disp_element(doc):
  npe = doc.get_property_int32("num_nodes_per_element")
  items = doc.get_property_int32("num_items")
  print items
  off = 0
  for i in range(0,items):
    ret = doc.read_int32v(off, npe)
    for j in ret[1:]:
      print j,
    off = off + ret[0]
    print

def disp_node(doc):
  dim = doc.get_property_int32("dimension")
  items = doc.get_property_int32("num_items")
  print items
  off = 0
  for i in range(0,items):
    ret = doc.read_float64v(off, dim)
    for j in ret[1:]:
      print j,
    off = off + ret[0]
    print

try:
  dfile = Adv.AdvDatafile(sys.argv[1])
except Adv.AdvDatafile.OpenError:
  print "Cannot open file `%s'" % sys.argv[1]
  sys.exit(1)

dbox = Adv.AdvDatabox()
dbox.add(sys.argv[1])

try:
  disp_element(dbox.find_by_property(None, {'content_type' : 'Element'}))
  disp_node(dbox.find_by_property(None, {'content_type' : 'Node'}))
  doc = dbox.find_by_property(None, {'content_type' : 'FEGenericAttribute',
                                     'label' : 'DirichletBoundaryCondition'})
  while doc:
    off = 0
    items = doc.get_property_int32('num_items')
    print items
    for i in range(0,items):
      r = doc.read_int32(off)
      off = off + r[0]
      s = doc.read_format(off, doc.get_property("format"))
      off = off + s[0]
      print r[1],
      for j in s[1:]:
        print j,
      print
    doc = dbox.find_by_property(doc, {'content_type' : 'FEGenericAttribute',
                                      'label' : 'DirichletBoundaryCondition'})
  
except:
  sys.exit(1)
