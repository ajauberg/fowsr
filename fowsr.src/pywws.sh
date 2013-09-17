#!/bin/sh

# Call fowsr and upload data to pywws

wsr="/usr/bin/fowsr -fp"
LOG=/var/log/fowsr/pywws.log
UID=$1
PWD=$2
server=www.pywws.com

WPUT="$LOG ftp://$UID:$PWD@$server"

rm -f $LOG
$wsr

wput $WPUT
