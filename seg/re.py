import re

pattern = re.compile(r'\d+')

s1='1213'
s2='das'

if(pattern.match(s1)):
    print 's1'
if(pattern.match(s2)):
    print 's2'
