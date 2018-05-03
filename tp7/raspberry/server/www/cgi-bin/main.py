#!/usr/bin/env python
import cgi
import math
import os
f = open('./log_in', 'r')
s = f.readlines()
begin = 0
end =len(s)
if end > 360 :
    begin = end - 360

s2fName = '/tmp/s2f_JL'
if not os.path.exists(s2fName):
    os.mkfifo(s2fName)
s2f = open(s2fName,'w+')
s2f.write(str(end))
s2f.flush()
val = s[end-1].split(",",1)[1]

print """
<head>
  <title>Peri Web Server</title>
</head>
<h1 align="center">Brightness Sensor_JL</h1>
<p align="center">Enter a LED number</p>
<body align="center">LeDs oh lalala:<br/>
<form method="POST" action="led.py">
  <input name="val" cols="20"></input>
  <input type="submit" value="Entrer">
</form>
<head>
   <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
   <script type="text/javascript">
      google.charts.load('current', {'packages':['gauge','corechart']});
      google.charts.setOnLoadCallback(drawChart);

      function drawChart() {

        var data = google.visualization.arrayToDataTable([
          ['Label', 'Value'],
          ['Brightness',"""
print val
print """],
        ]);

        var options = {
          width: 200, height: 200,
          redFrom: 850, redTo: 1000,
          yellowFrom:500, yellowTo:850,
          minorTicks: 5,
          max: 1000
        };

        var chart = new google.visualization.Gauge(document.getElementById('gauge_chart'));

        chart.draw(data, options);
      }
    </script>


</head>

<body>
    <meta http-equiv="refresh" content="3">

    <div id="gauge_chart" style="width: 200px; height: 200px"></div>
    <div id="area_chart" style="width: 100%; height: 500px;"></div>
        <script type="text/javascript">
		google.charts.setOnLoadCallback(drawChart);
    
    function drawChart() {
    var data = google.visualization.arrayToDataTable([
      ['TIME', 'Val'],
      """ 

for i in range (begin,end):
    print "[new Date(%s), %s]," %(s[i].split(",",1)[0], s[i].split(",",1)[1],)

print """\
    ]);
    
    var options = {
    title: 'Lumiere Value of the Capture lalala',
    hAxis: {title: 'time(seconds)',  titleTextStyle: {color: '#333'}},
    vAxis: {minValue: 0}
    };
    
    var chart = new google.visualization.AreaChart(document.getElementById('area_chart'));
    chart.draw(data, options);
    }
	</script>
  </body>
</body>
"""
