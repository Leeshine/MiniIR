#encoding:utf-8
import cPickle as pickle
import sys
import string
import re

max_len=5

def load_dic():
    try:
        f = open(sys.argv[2],'r')
    except:
        print >> sys.stderr,'failed to open file 2'
        sys.exit(1)
    DICT = {}

    for eachLine in f:
        dicStr = eachLine.strip().decode('utf8').encode('utf8')
        DICT[dicStr]=dicStr

    return DICT

def max_match_seg(line,dic):
    chars = line.decode('utf8')
    words = []
    idx =0

    while idx<len(chars):
        matched = False
        for i in xrange(max_len,0,-1):
            cand = chars[idx:idx+i].strip().encode('utf8')
          #  print cand
            if(dic.has_key(cand)):
                words.append(cand)
                matched = True
                break
        if not matched:
            i = 1
            words.append(chars[idx].encode('utf8'))
        idx += i

    return words

if __name__=="__main__":

    try:
        fpi = open(sys.argv[1],'r')
    except:
        print >> sys.stderr,'failed to open file 1'
        sys.exit(1)

    try:
        fpo = open('out.txt','w')
    except:
        print >> sys.stderr,'failed to open file 3'
        sys.exit(1)

    dict = load_dic()
    for line in fpi:
        fpo.write("\n".join(max_match_seg(line.strip(),dict)))

