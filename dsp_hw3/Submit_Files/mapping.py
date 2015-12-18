#!/usr/bin/python2
# -*- coding: utf-8 -*-

import sys
import locale
import codecs

from_file = open(sys.argv[1])
to_file = open(sys.argv[2],'w')

from_list=[]
to_map={}

from_map = from_file.read().splitlines()

for line in from_map:
    word,tmp_spell = line.split(' ')
    spells = tmp_spell.split('/')
    locale.setlocale(locale.LC_ALL,'zh_TW.BIG5')
    to_file.write('{} {}\n'.format(word,word))
    for spell in spells:
        if(spell[0:2]) in to_map:
            to_map[spell[0:2]].append(word)
        else:
            to_map[spell[0:2]] = []
            to_map[spell[0:2]].append(word)
for key in to_map:
    to_file.write('{} {}\n'.format(key,' '.join(w for w in to_map[key])))

from_file.close()
to_file.close()
