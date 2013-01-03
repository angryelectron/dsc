#!/bin/sh
# Generate graph for heat exchanger efficiency 

RRDB=/home/abythell/sketchbook/dsc/dsctemp.rrd
XDB=/home/abythell/sketchbook/dsc/xtemp.rrd

rrdtool graph /var/spool/dsc/x24.png --end now --start end-86400s -w 800 -h 300 -i -t "Heat Exchanger Efficiency" DEF:tanktemp=$RRDB:tank:AVERAGE DEF:inlettemp=$XDB:inlet:AVERAGE DEF:outlettemp=$XDB:outlet:AVERAGE CDEF:efficiency=outlettemp,tanktemp,\/,100,*  LINE1:inlettemp#0000FF:InletTemp LINE1:outlettemp#FF0000:OutletTemp LINE1:tanktemp#00FF00:TankTemp 1>/dev/null
