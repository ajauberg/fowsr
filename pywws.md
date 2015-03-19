# Introduction #

Upload weather data to pywws.

# Details #

pywws is a stand alone weather station collection and data presentation software. The **fowsr** application can be used to read the weather station, and pywws can be used for presenting the weather data.

The input for pywws is a comma separated file, and is typically sent to pywws over FTP.

**fowsr** supports the following variables read from the weather station and stored in a comma separated file called `/var/pywws.log`:

2010-03-14 17:50:18,5.0,24.0,24.6,37.0,20.7,992.2,0.0,0.0,12.0,0.3,00<br>
2010-03-14 17:45:18,5.0,24.0,24.6,37.0,20.8,992.2,0.0,0.0,12.0,0.3,00<br>
2010-03-14 17:40:18,5.0,24.0,24.6,37.0,20.8,992.3,0.0,0.0,12.0,0.3,00<br>
2010-03-14 17:35:18,5.0,24.0,24.6,37.0,20.8,992.3,0.0,0.0,12.0,0.3,00<br>
2010-03-14 17:30:18,5.0,24.0,24.6,37.0,20.8,992.2,0.0,0.0,12.0,0.3,00<br>
2010-03-14 17:25:18,5.0,24.0,24.5,38.0,20.9,992.1,0.0,0.0,12.0,0.3,00<br>
2010-03-14 17:20:18,5.0,24.0,24.5,38.0,20.9,992.1,0.0,0.0,12.0,0.3,00<br>

The log file can be uploaded by calling the script <code>/usr/bin/pywws.sh</code>.<br>
<br>
The script can be called at regular intervals using <a href='http://code.google.com/p/fowsr/wiki/cron'>cron</a>, or called directly from the command line:<br>
<br>
<code>/usr/bin/pywws.sh &lt;username&gt; &lt;password&gt;</code>

For furher details, please see:<br>
<br>
<a href='http://pywws.googlecode.com'>http://pywws.googlecode.com</a>