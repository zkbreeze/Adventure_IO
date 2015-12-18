#! /usr/bin/python
import Adv
import sys

def docinfo(doc):
  print doc.get_locator()
  i = 0
  while not None:
    p = doc.get_nth_property(i)
    if (p == None):
      break
    i = i+1
    print '  ', p[0], '=', p[1]
  print

dbox = Adv.AdvDatabox()
for n in sys.argv[1:]:
  dbox.add(n)
n = 0
while not None:
  doc = dbox[n]
  n = n+1
  if doc:
    docinfo(doc)
  else:
    break

