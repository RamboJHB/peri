#!/usr/bin/env python
import cgi, os, time,sys
tab = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]

f2sName = '/tmp/f2s_fw_xw1'
f2s = open(f2sName,'r',0)
datafile = '/tmp/datafile.txt'

res = f2s.readline()
date = f2s.readline()

f2s.close()
fs = open(datafile,'r',0)
for i in range(0,14):
	tab[i] = int(fs.readline())

fs.close()
print """
<html>
 <head>
 <script type="text/javascript" src="https://www.google.com/jsapi"></script>
 <script type="text/javascript">
 google.load("visualization", "1", {packages:["corechart"]});
 google.setOnLoadCallback(drawChart);
 function drawChart() {
 var data = google.visualization.arrayToDataTable([
 ['time', 'value']"""

for i in range(0,14):
 print " ,['%d', %d]" % (i,tab[i])
print """\
 ]);
 var options = {
 title: 'lumiere',
 hAxis: {title: 'time', titleTextStyle: {color: '#333'}},
 vAxis: {minValue: 0, maxValue: 1030}
 };
 var chart = new google.visualization.AreaChart(document.getElementById('chart_div'));
 chart.draw(data, options);
 }
 </script>
 <META HTTP-EQUIV="Refresh" CONTENT="1; URL=/cgi-bin/cap.py">
 </head>
 <body>
 <div id="chart_div" style="width: 600; height: 300px;"></div>
 VAR:%d<br/>
 DATE:%s<br/>
 </body>
</html>
""" % (int(res),date)
fs = open(datafile,'w',0)
for i in range(1,14):
	fs.write(str(tab[i]))
	fs.write('\n')
fs.write(str(res))
fs.write('\n')
fs.close
