#!/usr/bin/env python
import cgi
import math
import os
f = open('./log_in', 'r')
s = f.readlines()
begin = 0
end =len(s)
if end > 60 :
    begin = end - 60

s2fName = '/tmp/s2f_JL'
if not os.path.exists(s2fName):
    os.mkfifo(s2fName)
s2f = open(s2fName,'w+')
s2f.write(str(end))
s2f.flush()

print """
    <head>
    <title>Peri Web Server</title>
    </head>
    <h1 align="center">TP5_JL</h1>
    <p align="center">Enter a LED number</p>
    <body align="center">LeDs oh lalala:<br/>
    <form method="POST" action="led.py">
    <input name="val" cols="20"></input>
    <input type="submit" value="Entrer">
    </form>
    <head>
    <meta http-equiv="refresh" content="5">
    <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
    <script type="text/javascript">
    google.charts.load('current', {'packages':['corechart']});
    google.charts.setOnLoadCallback(drawChart);
    
    function drawChart() {
    var data = google.visualization.arrayToDataTable([
    ['TIME', 'Val']"""
for i in range (begin,end):
    print " ,[%s]" %(s[i],)

print """\
    ]);
    
    var options = {
    title: 'Lumiere Value of the Capture lalala',
    hAxis: {title: 'time(seconds)',  titleTextStyle: {color: '#333'}},
    vAxis: {minValue: 0}
    };
    
    var chart = new google.visualization.AreaChart(document.getElementById('chart_div'));
    chart.draw(data, options);
    }
    </script>
    </head>
    <body>
    <div id="chart_div" style="width: 100%; height: 500px;"></div>
    </body>
    """


