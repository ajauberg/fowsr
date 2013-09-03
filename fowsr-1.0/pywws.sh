#!/bin/sh

# Call fowsr and upload data to pywws

wsr="/usr/bin/fowsr -p"
LOG=/var/pywws.log
UID=$1
PWD=$2
server=www.pywws.com

WPUT="$LOG ftp://$UID:$PWD@$server"

rm -f $LOG
$wsr

wput $WPUT
