#!/bin/bash
#
# Use RRDTool to monitor temperatures from the DSC via serial port
# (C)2009 Andrew Bythell <abythell@ieee.org>
# $Id: dsc-monitor.sh,v 1.4 2010-02-14 18:29:14 abythell Exp $
#
# Roof and Tank temperatures are recorded into one database, inlet and outlet temperatures
# are recorded into another 
#
# Run this script every 1 minute from a cronjob.  The rrdtool expects data every 5 minutes, 
# but running the cron job at this interval could lead to missing data points:
# */1 * * * * dsc-monitor.sh

#
# User Settings 
#
PORT=/dev/ttyUSB0
BAUD=9600
RRDB=/home/abythell/sketchbook/dsc/dsctemp.rrd
XDB=/home/abythell/sketchbook/dsc/xtemp.rrd
XML=/var/spool/dsc/solar.xml
TEMPLATE=/home/abythell/sketchbook/dsc/solar-template.xml

#create RR databases if it doesn't exist.
if [ ! -e $RRDB ]
then
	rrdtool create $RRDB --start `date +%s` --step 300 DS:status:GAUGE:600:0:3 DS:roof:GAUGE:600:-50:150 DS:tank:GAUGE:600:0:100 RRA:AVERAGE:0.5:1:105120 RRA:MAX:0.5:288:365 RRA:MIN:0.5:288:365
fi
if [ ! -e $XDB ]
then
	rrdtool create $XDB --start `date +%s` --step 300 DS:inlet:GAUGE:600:-50:150 DS:outlet:GAUGE:600:0:100 RRA:AVERAGE:0.5:1:105120 RRA:MAX:0.5:288:365 RRA:MIN:0.5:288:365
fi

#get data from serial port - will reset the arduino the first time it's run
/bin/stty -F $PORT cs8 $BAUD ignbrk -brkint -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -clocal -hupcl
read -e STATUS ROOF TANK INLET OUTLET < $PORT

#update database.  data from serial port is in hex (to allow 100C+ temperatures to fit into 1 byte) - convert to decimal first
rrdtool update $RRDB `date +%s`:$((16#$STATUS)):$((16#$ROOF)):$((16#$TANK))
rrdtool update $XDB `date +%s`:$((16#$INLET)):$((16#$OUTLET))

#generate XML file for google gadget
DATE=`date +%r-%Z`
EFF=$((100*16#$OUTLET/16#$TANK))
sed -e "s/ROOF/$((16#$ROOF))/" -e "s/TANK/$((16#$TANK))/" -e "s/INLET/$((16#$INLET))/" -e "s/OUTLET/$((16#$OUTLET))/" -e "s/EFFICIENCY/$EFF/" -e "s/DATE/$DATE/" -e "s/STATE/$STATUS/" $TEMPLATE > $XML
