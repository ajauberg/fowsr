#!/bin/sh

# Call fowsr and upload data to pywws

wsr=/usr/bin/uwws -p
LOG=/var/pywws.log
UID=$1
PWD=$2

WPUT="$LOG ftp://$UID:$PWD@<www.myser.com>/<mypath>"

rm -f $LOG
$wsr

$WPUT
