#!/usr/bin/env python

html="""
<head>
  <title>Peri Web Server</title>
</head>
<body>
<p><font size="8">Control Arduino</font></p><br/>
LED:<br/>
<form method="POST" action="led.py">
  <input name="val" cols="20"></input>
  <input type="submit" value="Entrer">
</form>
</body>
"""

print html
