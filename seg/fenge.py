#encoding:utf-8

char = [u'，',u'。',u'？',u'！']

try:
    f=open('testfile','r')
    Str=f.read().decode('utf8')
    Str=Str.replace(' ','')
    Str=Str.replace('\n','')
    Str=Str.strip()
finally:
    f.close()

b = u'l'
for c in char:
    Str=Str.replace(c,b)
Str=Str.split(b)
p=open('re','w')
for tr in Str:
    tr=tr+'\n'
    p.write(tr.encode('utf8'))
