#!/usr/bin/env python
import cgi, os, time,sys
form = cgi.FieldStorage()
val = form.getvalue('val')

s2fName = '/tmp/s2f_fw_xw2'
s2f = open(s2fName,'w+')

s2f.write("%s\n" % val)
s2f.flush()

s2f.close()

html="""
<head>
  <title>Peri Web Server2</title>
</head>
<body>
Text:%s<br/>
<form method="POST" action="stat.py">
  <input name="val" cols="20"></input>
  <input type="submit" value="Entrer">
</form>
</body>
""" % (val)

print html
