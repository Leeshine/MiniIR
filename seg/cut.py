#encoding:utf-8

def load_stop():
    try:
        f = open('stop','r')
    except:
        print "failed to open stop file"
        sys.exit(1)

    STOP = {}

    for line in f:
        stop = line.strip().decode('utf8')
        STOP[stop] = stop

    return STOP

def cut(sentence,stop):

    sentence = sentence.replace(' ','')
    sentence = sentence.replace('\n','')
    sentence = sentence.strip()

    b=u'l'
    for c in sentence:
        if(stop.has_key(c)):
            sentence=sentence.replace(c,b)
    sentence = sentence.split(b)

    return sentence

if __name__=="__main__":

    try:
        fpi = open('testfile','r')
    except:
        print "fail to open file1"
        exit(1)

    try:
        fpo = open('CutResult.txt','w')
    except:
        print "fail to open file2"
        exit(1)

    stop = load_stop()
    line_num = 0
    for line in fpi:
        line_num += 1
        if(line_num > 4):
            Str = cut(line.decode('utf8'),stop)
            for tr in Str:
                tr = tr+'\n'
                if (tr != '\n'):
                    fpo.write(tr.encode('utf8'))
