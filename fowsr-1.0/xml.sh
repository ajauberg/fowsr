#!/bin/sh

# Delete the log and dat file to perform complete read out, then call fowsr

wsr="/usr/bin/fowsr -x"
dat="/var/fowsr.dat"
LOG="/var/fowsr.xml"

rm -f $LOG
rm -f $dat

$wsr

