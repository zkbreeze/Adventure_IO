#! /usr/bin/python
import Adv
import sys

def check(doc):
    print 'checking', doc.get_locator(), '...'
    content_type_key = 'content_type'
    content_type_val = doc.get_property('content_type')
    if not content_type_val:
	content_type_key = 'content-type'
	content_type_val = doc.get_property('content-type')
	if content_type_val:
	    print '! Use content_type instead of content-type'
	else:
	    content_type_key = None
	    print '! There is no content_type'

    if content_type_val == 'FEMGenericCondition':
	print '! FEMGenericCondition is obsolute. Use FEGenericAttribute'
    elif content_type_val == 'node':
	print '! %s=node is obsolute. Use content_type=Node instead.' % content_type_key
    elif content_type_val == 'element':
	print '! %s=element is obsolute. Use content_type=Element instead.' % content_type_key

    if (content_type_val == 'FEMGenericCondition'
	or content_type_val == 'FEGenericAttribute'):
	label_val = doc.get_property('label')
	if (label_val == 'DirichletBoundaryCondition'
	    or label_val=='NeumannBoundaryCondition'):
	    print '! label=%s is wrong. Write phisical name here.' % label_val
    
    if content_type_val == 'FEGenericAttribute':
	fega_type_val = doc.get_property('fega_type')
	if (fega_type_val != 'AllNodeConstant'
	    and fega_type_val != 'AllNodeVariable'
	    and fega_type_val != 'AllElementConstant'
	    and fega_type_val != 'AllElementVariable'
	    and fega_type_val != 'NodeConstant'
	    and fega_type_val != 'NodeVariable'
	    and fega_type_val != 'ElementConstant'
	    and fega_type_val != 'ElementVariable'):
	    print '! fega_type=%s is invalid type.' % fega_type_val
      

dbox = Adv.AdvDatabox()
for n in sys.argv[1:]:
  dbox.add(n)
n = 0
while not None:
  doc = dbox[n]
  n = n+1
  if doc:
    check(doc)
  else:
    break

