#!/bin/sh
# Generate graph for the past week 

RRDB=/home/abythell/sketchbook/dsc/dsctemp.rrd
XDB=/home/abythell/sketchbook/dsc/xtemp.rrd

rrdtool graph /var/spool/dsc/last7.png --end now --start end-604800s -w 800 -h 300 -i -t "Last 7 Days" --vertical-label "Degrees C" DEF:rooftemp=$RRDB:roof:AVERAGE DEF:tanktemp=$RRDB:tank:AVERAGE DEF:state=$RRDB:status:AVERAGE DEF:inlettemp=$XDB:inlet:AVERAGE DEF:outlettemp=$XDB:outlet:AVERAGE CDEF:graphstate=state,10,\* LINE1:rooftemp#ff0000:RoofTemp LINE1:tanktemp#00ff00:TankTemp AREA:graphstate#0000ff:State 1>/dev/null
