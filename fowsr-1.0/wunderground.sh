#!/bin/sh

# Call fowsr and upload data to Wunderground

WGET=http://weatherstation.wunderground.com/weatherstation/updateweatherstation.php

wsr=/usr/bin/fowsr -w
LOG=/var/wunderground.log
ID=$1
PASSWORD=$2

WGET="$WGET?action=updateraw&ID=$ID&PASSWORD=$PASSWORD&"

rm -f $LOG
$wsr
while read line
do
  WGET2="$WGET`echo $line`"

  echo $WGET2
  wget -O /dev/null "$WGET2"
done < $LOG

