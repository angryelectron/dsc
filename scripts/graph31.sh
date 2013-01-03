#!/bin/sh
# Generate graph for the past month 

RRDB=/home/abythell/sketchbook/dsc/dsctemp.rrd
XDB=/home/abythell/sketchbook/dsc/xtemp.rrd
FILENAME=/var/spool/dsc/last31.png

rrdtool graph $FILENAME --end now --start end-2678400s -w 800 -h 300 -i -t "Past Month" --vertical-label "Degrees C" DEF:rooftemp=$RRDB:roof:AVERAGE DEF:tanktemp=$RRDB:tank:AVERAGE DEF:state=$RRDB:status:AVERAGE DEF:inlettemp=$XDB:inlet:AVERAGE DEF:outlettemp=$XDB:outlet:AVERAGE CDEF:graphstate=state,10,\* LINE1:rooftemp#ff0000:RoofTemp LINE1:tanktemp#00ff00:TankTemp AREA:graphstate#0000ff:State 1>/dev/null
