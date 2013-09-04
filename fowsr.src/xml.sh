#!/bin/sh

# Delete the log and dat file to perform complete read out, then call fowsr

wsr="/usr/bin/fowsr -fx"
dat="/var/log/fowsr/fowsr.dat"
LOG="/var/log/fowsr/xml.log"

rm -f $LOG
rm -f $dat

$wsr

