# Introduction #

Upload weather data to Weather Underground.

# Details #

Weather Underground is an internet based weather service.

Visit Weather Underground and create an account before proceeding with the configuration:

http://www.wunderground.com/members/signup.asp

The input to Weather Underground is a HTTP GET submitted to the following URL:

http://weatherstation.wunderground.com/weatherstation/updateweatherstation.php

**fowsr** supports the following variables read from the weather station and stored in `/var/wunderground.log`:

dateutc=2010-03-14%2005:20:38&winddir=270.0&windspeedmph=0.0&windgustmph=0.0&humidity=32.0&tempf=65.5&rainin=0.4&baromin=29.28<br>
dateutc=2010-03-14%2005:15:38&winddir=270.0&windspeedmph=0.0&windgustmph=0.0&humidity=32.0&tempf=65.5&rainin=0.4&baromin=29.28<br>
dateutc=2010-03-14%2005:10:38&winddir=270.0&windspeedmph=0.0&windgustmph=0.0&humidity=32.0&tempf=65.5&rainin=0.4&baromin=29.28<br>
dateutc=2010-03-14%2005:05:38&winddir=270.0&windspeedmph=0.0&windgustmph=0.0&humidity=33.0&tempf=65.5&rainin=0.4&baromin=29.28<br>
dateutc=2010-03-14%2005:00:38&winddir=270.0&windspeedmph=0.0&windgustmph=0.0&humidity=33.0&tempf=65.7&rainin=0.4&baromin=29.28<br>
dateutc=2010-03-14%2004:55:38&winddir=270.0&windspeedmph=0.0&windgustmph=0.0&humidity=32.0&tempf=65.7&rainin=0.4&baromin=29.28<br>
dateutc=2010-03-14%2004:50:38&winddir=270.0&windspeedmph=0.0&windgustmph=0.0&humidity=33.0&tempf=65.7&rainin=0.4&baromin=29.28<br>

The file can be uploaded to Weather Underground by calling the script <code>/usr/bin/wunderground.sh</code>.<br>
<br>
The script can be called at regular intervals using <a href='http://code.google.com/p/fowsr/wiki/cron'>cron</a>, or called directly from the command line:<br>
<br>
<code>/usr/bin/wunderground.sh &lt;username&gt; &lt;password&gt;</code>

For further details, please see:<br>
<br>
<a href='http://wiki.wunderground.com'>http://wiki.wunderground.com</a>