#!/bin/sh
# Generate graph for energy efficiency 

RRDB=/home/abythell/sketchbook/dsc/dsctemp.rrd
XDB=/home/abythell/sketchbook/dsc/xtemp.rrd
GRAPH=/var/spool/dsc/e24.png

rrdtool graph $GRAPH --end now --start end-86400s -w 800 -h 300 -i -t "Energy Usage" DEF:tanktemp=$RRDB:tank:AVERAGE DEF:inlettemp=$XDB:inlet:AVERAGE DEF:outlettemp=$XDB:outlet:AVERAGE CDEF:energy=outlettemp,inlettemp,-  AREA:energy#00FF00:TempIncrease LINE1:inlettemp#FF0000:InletTemp LINE1:outlettemp#0000FF:OutletTemp 1>/dev/null
