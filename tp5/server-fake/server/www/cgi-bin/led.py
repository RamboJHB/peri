#!/usr/bin/env python
import cgi, os, time,sys
form = cgi.FieldStorage()
val = form.getvalue('val')

s2fName = '/tmp/s2f_JL'
f2sName = '/tmp/f2s_JL'
f2s = open(f2sName,'r',0)
s2f = open(s2fName,'w',0)


s2f.write("%s\n" % val)
s2f.flush()
res = f2s.readline()
f2s.flush()
f2s.close()
s2f.close()

html="""
<head>
  <title>Peri Web Server</title>
  <META HTTP-EQUIV="Refresh" CONTENT="1; URL=/cgi-bin/main.py">
</head>
<h1>TP4_JL</h1>
<p></p>
<body>
LeDs Set oh lalala:<br/>
<form method="POST" action="led.py">
  <input name="val" cols="20"></input>
  <input type="submit" value="Entrer">
 <p> set LED =  %s BP = %s </p>
</form>
</body>
""" %(val,res,)

print html
