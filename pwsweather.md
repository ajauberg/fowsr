# Introduction #

Upload weather data to PWS Weather.

# Details #

PWS Weather is an internet based weather service.

Visit PWS Weather and create an account before proceeding with the configuration:

http://www.pwsweather.com/register.php

The input to PWS Weather is a HTTP GET submitted to the following URL:

http://www.pwsweather.com/pwsupdate/pwsupdate.php

**fowsr** supports the following variables read from the weather station and stored in `/var/pwsweather.log`:

dateutc=2000-12-01+15%3A20%3A01&winddir=225&windspeedmph=0.0&windgustmph=0.0&tempf=34.88&rainin=0.06&dailyrainin=0.06&monthrainin=1.02&yearrainin=18.26&baromin=29.49&dewptf=30.16&humidity=83<br>

The file can be uploaded to PWS Weather by calling the script <code>/usr/bin/pwsweather.sh</code>.<br>
<br>
The script can be called at regular intervals using <a href='http://code.google.com/p/fowsr/wiki/cron'>cron</a>, or called directly from the command line:<br>
<br>
<code>/usr/bin/pwsweather.sh &lt;username&gt; &lt;password&gt;</code>

For further details, please see:<br>
<br>
<a href='http://www.pwsweather.com/faqs.php'>http://www.pwsweather.com/faqs.php</a>