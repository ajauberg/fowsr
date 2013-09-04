#!/bin/sh

# Call fowsr with multiple parameters

PWSLOG=/var/log/fowsr/pwsweather.log
rm -f $PWSLOG

WUGLOG=/var/log/fowsr/wunderground.log
rm -f $WUGLOG

wsr="/usr/bin/fowsr -fw -fs"
$wsr

# Upload data to Wunderground

ID=$1
PASSWORD=$2
WGET=http://weatherstation.wunderground.com/weatherstation/updateweatherstation.php
WGET="$WGET?action=updateraw&ID=$ID&PASSWORD=$PASSWORD&softwaretype=fowsr&"

while read line
do
  WGET2="$WGET`echo $line`"
  echo $WGET2
  wget -O /dev/null "$WGET2"
done < $WUGLOG

# Upload data to PWS Weather

ID=$1
PASSWORD=$2
WGET=http://www.pwsweather.com/pwsupdate/pwsupdate.php
WGET="$WGET?action=updateraw&ID=$ID&PASSWORD=$PASSWORD&softwaretype=fowsr&"

while read line
do
  WGET2="$WGET`echo $line`"
  echo $WGET2
  wget -O /dev/null "$WGET2"
done < $PWSLOG

