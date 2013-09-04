#!/bin/sh

# Call fowsr and upload data to PWS Weather

wsr="/usr/bin/fowsr -fs"
LOG=/var/log/fowsr/pwsweather.log
ID=$1
PASSWORD=$2

WGET=http://www.pwsweather.com/pwsupdate/pwsupdate.php

WGET="$WGET?action=updateraw&ID=$ID&PASSWORD=$PASSWORD&softwaretype=fowsr&"

rm -f $LOG
$wsr

while read line
do
  WGET2="$WGET`echo $line`"

  echo $WGET2
  wget -O /dev/null "$WGET2"
done < $LOG

