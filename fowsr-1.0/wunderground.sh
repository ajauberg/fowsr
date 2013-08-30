#!/bin/sh

# Call fowsr and upload data to Wunderground

wsr="/usr/bin/fowsr -fw"
LOG=/var/wunderground.log
ID=$1
PASSWORD=$2

WGET=http://weatherstation.wunderground.com/weatherstation/updateweatherstation.php

WGET="$WGET?action=updateraw&ID=$ID&PASSWORD=$PASSWORD&softwaretype=fowsr&"

rm -f $LOG
$wsr

while read line
do
  WGET2="$WGET`echo $line`"

  echo $WGET2
  wget -O /dev/null "$WGET2"
done < $LOG

