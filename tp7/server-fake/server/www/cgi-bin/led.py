#!/usr/bin/env python
import cgi, os, time,sys
form = cgi.FieldStorage()
val = form.getvalue('val')

s2fName = '/tmp/s2f_fw_xw1'
s2f = open(s2fName,'w+')

s2f.write("%s\n" % val)
s2f.flush()

s2f.close()

html="""
<head>
  <title>Peri Web Server</title>
</head>
<body>
<p><font size="8">Control Arduino</font></p><br/>
LED:%s<br/>
<form method="POST" action="led.py">
  <input name="val" cols="20"></input>
  <input type="submit" value="Entrer">
</form>
</body>
""" % (val)

print html
