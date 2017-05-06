# -*- coding: utf-8 -*-
import cPickle
import os

options = ('', '-s 4 -s 7')
idx2tok = ['>>dummy<<', '\xe4\xb9\xa0\xe5\xa4\xa7\xe5\xa4\xa7', '\xe8\x80\x83\xe5\xaf\x9f', '\xe5\xa4\xa7\xe7\xb1\xb3', '\xe5\xa4\xa9\xe7\x84\xb6', '\xe5\xaf\x8c\xe7\xa1\x92', '\xe7\xb1\xb3', '\xe8\x80\xb3', '\xe5\x87\xba\xe5\x8f\xa3', '\xe5\x88\xb6\xe9\x80\xa0']
fidx2ngram = ['>>dummy<<', (1,), (2,), (3,), (4,), (5,), (6,), (1, 2), (2, 3), (3, 4)]

_dir = os.path.join(
    os.path.dirname(os.path.realpath(__file__)),
    'stub')
if not os.path.exists(_dir):
    os.makedirs(_dir)

cPickle.dump(options, open(os.path.join(_dir, '1.pickle'), 'wb'))
cPickle.dump(options, open(os.path.join(_dir, '2.pickle'), 'wb'))
cPickle.dump(options, open(os.path.join(_dir, '3.pickle'), 'wb'))
