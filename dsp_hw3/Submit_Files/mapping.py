#!/usr/bin/python2
# -*- coding: utf-8 -*-

import sys
import codecs

from_file = open(sys.argv[1])
to_file = open(sys.argv[2],'w')

from_list=[]
to_map={}

from_map = from_file.read().splitlines()

for line in from_map:
    word,tmp_spell = line.split(' ')
    spells = tmp_spell.split('/')
    to_map[word] = word  #directly mapping
    for spell in spells: #bopomofo mapping
        #spell[:3] is the first
        if spell[:3] in to_map:
            to_map[spell[:3]].extend(word)
        else:
            to_map[spell[:3]] = []
            to_map[spell[:3]].extend(word)

for key in to_map:
    to_file.write('{}'.format(key))
    for idx in range(0,len(to_map[key]),3):
        to_file.write(' {}'.format(''.join(to_map[key][idx:idx+3])))
    to_file.write('\n')

from_file.close()
to_file.close()
