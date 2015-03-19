# Introduction #

Connect your weather station to the USB port of your Linux box, and upload its contents to the internet.

  * **fowsr** uses the libusb (V0.1) library to read information from the Fine Offset Weather Stations.

  * **fowsr** is installed as `/usr/bin/fowsr`, and may be called directly from this location.

  * The cache file `/var/log/fowsr/fowsr.dat` stores the weather station buffer between the reads. Deleting this file causes the application to re-read all weather station data.

  * The log files `/var/log/fowsr/*.log` contain the weather station output in the requested format.

  * The script files `/usr/bin/*.sh` contain the weather station upload details.

  * [cron](http://code.google.com/p/fowsr/wiki/cron) is used to call the requested script files at regular intervals.


# Details #

The know options are displayed when the fowsr application is called with -h:

```
options
 -f[p|s|w|x|f]	set Logformat for weather data
 	-fp	Logfile in pywws format
	-fs	Logfile in PWS Weather format
	-fw	Logfile in Wunderground format
	-fx	Logfile in XML format
	-ff	Logfile in FHEM log format
 -c	Log to screen (in FHEM-WS3600 format)
 -n<filename>	set full path and name for weather data, may contain
		%-wildcards of the POSIX strftime function and %%s
		for a type specific name part
		default for pywws is: //var//log//fowsr//pywws.log
 -b	Display fixed block
 -d	Display decoded fixed block data
 -r	Dump all weather station records
 -v<Level><Destination>	output debug messages
 	Level: 0-3	0-only errors, 3-all
 	Destination:	(c)onsole, (f)ile (same place as weather data), (b)oth
```

The following listing is an example of the fixed block contents of the weather station:

```
00014BDC | 55 AA AA FF FF FF FF FF 00 00 00 00 49 00 00 00 | Uªª.........I...
00014BEC | 05 20 02 30 09 00 00 00 01 00 00 F0 0F 00 40 9D | ...0.......ð..@
00014C0C | 41 23 C8 00 00 00 46 2D 2C 01 64 80 C8 00 00 00 | A#È...F-,.dÈ...
00014C1C | 64 00 64 80 A3 28 80 25 00 00 00 00 00 B4 00 00 | d.d£(%.....´..
00014C2C | 68 01 00 0A 00 F4 01 12 00 00 00 00 00 00 00 00 | h....ô..........
00014C3C | 00 00 4A 11 60 0F 1C 01 50 00 2A 01 07 80 2A 01 | ..J.`...P.*..*.
00014C4C | 41 80 C9 00 03 00 1F 28 0A 26 5F 28 23 26 63 00 | AÉ....(.&_(#&c.
00014C5C | 85 00 38 01 38 01 55 02 9E 04 B7 06 00 10 07 13 | .8.8.U..·.....
00014C6C | 13 54 10 02 21 07 30 10 04 07 03 01 10 02 21 07 | .T..!.0.......!.
00014C7C | 39 10 05 19 17 59 10 04 09 08 18 10 06 26 16 30 | 9....Y.......&.0
00014C8C | 10 04 20 04 31 10 06 26 16 30 07 01 02 07 09 10 | ....1..&.0......
00014C9C | 07 11 17 11 10 05 26 19 15 10 04 11 06 34 10 06 | ......&......4..
00014CAC | 12 07 04 10 04 11 06 34 10 03 01 04 00 07 01 02 | .......4........
00014CBC | 07 09 10 07 13 00 27 10 07 09 22 13 10 07 09 22 | ......'..."...."
00014CCC | 13 10 07 15 15 49 10 07 15 15 49 10 07 16 23 38 | .....I....I...#8
```


The fixed block above can be decoded as:

```
read_period=5.0
units0=32.0
units_wind_speed=2.0
display_format0=48.0
display_format1=9.0
alarm_enable0=0.0
alarm_enable1=0.0
alarm_enable2=0.0
timezone=1.0
data_refreshed=61440.000
data_count=4080.000
current_pos=40256.000
rel_pressure=1012.800
abs_pressure=1006.400
date_time=2010-07-18 19:50
alarm.hum_in.hi=65.0
alarm.hum_in.lo=35.0
alarm.temp_in.hi=20.000
alarm.temp_in.lo=0.000
alarm.hum_out.hi=70.0
alarm.hum_out.lo=45.0
alarm.temp_out.hi=30.000
alarm.temp_out.lo=-10.000
alarm.windchill.hi=20.000
alarm.windchill.lo=0.000
alarm.dewpoint.hi=10.000
alarm.dewpoint.lo=-10.000
alarm.abs_pressure.hi=1040.300
alarm.abs_pressure.lo=960.000
alarm.rel_pressure.hi=0.000
alarm.rel_pressure.lo=0.000
alarm.wind_ave.bft=0.0
alarm.wind_ave.ms=18.0
alarm.wind_gust.bft=0.0
alarm.wind_gust.ms=10.4
alarm.wind_dir=0.0
alarm.rain.hour=3.000
alarm.rain.day=150.000
alarm.time=12:00
max.hum_in.val=74.0
max.hum_in.date=2010-07-13 13:54
max.hum_out.val=96.0
max.hum_out.date=2010-04-07 03:01
max.temp_in.val=28.400
max.temp_in.date=2010-05-19 17:59
max.temp_out.val=29.800
max.temp_out.date=2010-06-26 16:30
max.windchill.val=29.800
max.windchill.date=2010-06-26 16:30
max.dewpoint.val=20.100
max.dewpoint.date=2010-07-11 17:11
max.abs_pressure.val=1027.100
max.abs_pressure.date=2010-04-11 06:34
max.rel_pressure.val=1033.500
max.rel_pressure.date=2010-04-11 06:34
max.wind_ave.val=9.900
max.wind_ave.date=2007-01-02 07:09
max.wind_gust.val=13.300
max.wind_gust.date=2010-07-13 00:27
max.rain.hour.val=93.600
max.rain.hour.date=2010-07-09 22:13
max.rain.day.val=93.600
max.rain.day.date=2010-07-09 22:13
max.rain.week.val=179.100
max.rain.week.date=2010-07-15 15:49
max.rain.month.val=354.600
max.rain.month.date=2010-07-15 15:49
max.rain.total.val=515.700
max.rain.total.date=2010-07-16 23:38
min.hum_in.val=17.0
min.hum_in.date=2010-02-21 07:30
min.hum_out.val=15.0
min.hum_out.date=2010-02-21 07:39
min.temp_in.val=8.000
min.temp_in.date=2010-04-09 08:18
min.temp_out.val=-0.700
min.temp_out.date=2010-04-20 04:31
min.windchill.val=-6.500
min.windchill.date=2007-01-02 07:09
min.dewpoint.val=0.300
min.dewpoint.date=2010-05-26 19:15
min.abs_pressure.val=973.800
min.abs_pressure.date=2010-06-12 07:04
min.rel_pressure.val=976.300
min.rel_pressure.date=2010-03-01 04:00
```

The following listing is an example of the weather station records:

```
00014CDC | 05 3F D0 00 4F A9 00 42 27 11 1B 00 08 7E 01 00 | .?Ð.O©.B'....~..
00014CEC | 05 3F CF 00 4F A9 00 43 27 14 1B 00 06 7E 01 00 | .?Ï.O©.C'....~..
00014CFC | 05 3F CF 00 4F A9 00 42 27 14 1F 00 06 7E 01 00 | .?Ï.O©.B'....~..
00014D0C | 05 3F CE 00 4F A9 00 41 27 18 29 00 06 7E 01 00 | .?Î.O©.A'.)..~..
00014D1C | 05 3F CE 00 4F A9 00 41 27 11 22 00 08 7E 01 00 | .?Î.O©.A'."..~..
00014D2C | 05 3F CE 00 4F A8 00 42 27 14 22 00 08 7E 01 00 | .?Î.O¨.B'."..~..
00014D3C | 05 3F CD 00 50 A8 00 42 27 11 18 00 06 7E 01 00 | .?Í.P¨.B'....~..
00014D4C | 05 3F CD 00 4F A8 00 42 27 11 1B 00 07 7E 01 00 | .?Í.O¨.B'....~..
00014D5C | 05 3F CD 00 50 A8 00 41 27 0E 18 00 07 7E 01 00 | .?Í.P¨.A'....~..
00014D6C | 05 3F CD 00 50 A8 00 41 27 0E 18 00 06 7E 01 00 | .?Í.P¨.A'....~..
00014D7C | 05 3F CD 00 50 A8 00 42 27 18 22 00 07 7E 01 00 | .?Í.P¨.B'."..~..
00014D8C | 05 3F CD 00 51 A8 00 42 27 18 22 00 08 7E 01 00 | .?Í.Q¨.B'."..~..
00014D9C | 05 3F CD 00 51 A8 00 41 27 11 1F 00 08 7E 01 00 | .?Í.Q¨.A'....~..
00014DAC | 05 3F CD 00 51 A8 00 41 27 11 18 00 08 7E 01 00 | .?Í.Q¨.A'....~..
00014DBC | 05 40 CD 00 51 A8 00 3F 27 0E 18 00 04 7E 01 00 | .@Í.Q¨.?'....~..
00014DCC | 05 40 CD 00 51 A7 00 40 27 11 18 00 08 7E 01 00 | .@Í.Q§.@'....~..
00014DDC | 05 40 CD 00 51 A7 00 3F 27 0A 18 00 06 7E 01 00 | .@Í.Q§.?'....~..
00014DEC | 05 40 CD 00 52 A7 00 3F 27 14 1B 00 08 7E 01 00 | .@Í.R§.?'....~..
00014DFC | 05 40 CD 00 52 A7 00 3F 27 11 1B 00 08 7E 01 00 | .@Í.R§.?'....~..
00014E0C | 05 40 CD 00 52 A8 00 40 27 18 22 00 09 7E 01 00 | .@Í.R¨.@'."..~..
00014E1C | 05 40 CD 00 53 A8 00 3F 27 11 1F 00 08 7E 01 00 | .@Í.S¨.?'....~..
00014E2C | 05 40 CC 00 53 A9 00 3F 27 14 1F 00 08 7E 01 00 | .@Ì.S©.?'....~..
00014E3C | 05 40 CC 00 53 A8 00 3D 27 11 1B 00 08 7E 01 00 | .@Ì.S¨.='....~..
00014E4C | 05 40 CC 00 53 A8 00 3D 27 0E 18 00 08 7E 01 00 | .@Ì.S¨.='....~..
00014E5C | 05 40 CC 00 54 A8 00 3D 27 11 18 00 06 7E 01 00 | .@Ì.T¨.='....~..
00014E6C | 05 41 CC 00 54 A8 00 3E 27 07 11 00 08 7E 01 00 | .AÌ.T¨.>'....~..
00014E7C | 05 41 CC 00 54 A8 00 3D 27 0E 11 00 08 7E 01 00 | .AÌ.T¨.='....~..
00014E8C | 05 41 CD 00 54 A7 00 40 27 0E 18 00 07 7E 01 00 | .AÍ.T§.@'....~..
00014E9C | 05 41 CD 00 54 A7 00 3E 27 0A 14 00 06 7E 01 00 | .AÍ.T§.>'....~..
00014EAC | 05 40 CD 00 54 A7 00 3D 27 0A 11 00 08 7E 01 00 | .@Í.T§.='....~..
00014EBC | 05 41 CD 00 55 A7 00 3C 27 0A 0E 00 08 7E 01 00 | .AÍ.U§.<'....~..
00014ECC | 05 41 CD 00 55 A7 00 3C 27 0A 11 00 06 7E 01 00 | .AÍ.U§.<'....~..
00014EDC | 05 41 CC 00 54 A6 00 3E 27 07 14 00 08 7E 01 00 | .AÌ.T¦.>'....~..
00014EEC | 05 41 CC 00 55 A6 00 3C 27 0A 11 00 06 7E 01 00 | .AÌ.U¦.<'....~..
00014EFC | 05 41 CC 00 55 A6 00 3A 27 0A 11 00 08 7E 01 00 | .AÌ.U¦.:'....~..
00014F0C | 05 41 CC 00 55 A6 00 3C 27 07 11 00 06 7E 01 00 | .AÌ.U¦.<'....~..
00014F1C | 05 41 CC 00 55 A6 00 3B 27 07 0E 00 07 7E 01 00 | .AÌ.U¦.;'....~..
00014F2C | 05 41 CC 00 55 A6 00 3B 27 0A 11 00 06 7E 01 00 | .AÌ.U¦.;'....~..
00014F3C | 05 41 CC 00 55 A7 00 3B 27 0A 14 00 06 7E 01 00 | .AÌ.U§.;'....~..
00014F4C | 05 41 CC 00 55 A7 00 3C 27 11 18 00 0A 7E 01 00 | .AÌ.U§.<'....~..
00014F5C | 05 41 CC 00 55 A8 00 3B 27 0E 18 00 06 7E 01 00 | .AÌ.U¨.;'....~..
00014F6C | 05 41 CC 00 55 A9 00 3C 27 0E 14 00 08 7E 01 00 | .AÌ.U©.<'....~..
00014F7C | 05 41 CC 00 54 AA 00 3A 27 14 1F 00 07 7E 01 00 | .AÌ.Tª.:'....~..
00014F8C | 05 41 CC 00 54 AB 00 3A 27 0E 18 00 08 7E 01 00 | .AÌ.T«.:'....~..
00014F9C | 05 42 CC 00 53 AC 00 3A 27 0E 18 00 08 7E 01 00 | .BÌ.S¬.:'....~..
00014FAC | 05 41 CC 00 54 AD 00 3A 27 11 18 00 08 7E 01 00 | .AÌ.T­.:'....~..
00014FBC | 05 41 CD 00 52 AD 00 39 27 14 1F 00 07 7E 01 00 | .AÍ.R­.9'....~..
00014FCC | 05 41 CD 00 53 AD 00 3A 27 0A 14 00 08 7E 01 00 | .AÍ.S­.:'....~..
00014FDC | 05 41 CD 00 52 AD 00 3A 27 11 1B 00 04 7E 01 00 | .AÍ.R­.:'....~..
00014FEC | 05 41 CD 00 53 AE 00 39 27 0A 11 00 08 7E 01 00 | .AÍ.S®.9'....~..
00014FFC | 05 42 CD 00 52 B0 00 3B 27 0E 14 00 08 7E 01 00 | .BÍ.R°.;'....~..
0001500C | 05 41 CD 00 52 B0 00 3A 27 07 0E 00 08 7E 01 00 | .AÍ.R°.:'....~..
0001501C | 05 41 CD 00 52 B1 00 3C 27 0A 18 00 06 7E 01 00 | .AÍ.R±.<'....~..
0001502C | 05 41 CE 00 51 B2 00 39 27 07 0E 00 08 7E 01 00 | .AÎ.Q².9'....~..
0001503C | 05 42 CE 00 50 B3 00 3A 27 0A 14 00 08 7E 01 00 | .BÎ.P³.:'....~..
0001504C | 05 42 CE 00 50 B4 00 3A 27 0A 0E 00 06 7E 01 00 | .BÎ.P´.:'....~..
0001505C | 05 42 CF 00 50 B3 00 39 27 0E 14 00 09 7E 01 00 | .BÏ.P³.9'....~..
0001506C | 05 42 CF 00 50 B4 00 39 27 11 1B 00 08 7E 01 00 | .BÏ.P´.9'....~..
0001507C | 05 41 CF 00 50 B5 00 37 27 0E 11 00 08 7E 01 00 | .AÏ.Pµ.7'....~..
0001508C | 05 41 CF 00 4F B7 00 38 27 0E 14 00 08 7E 01 00 | .AÏ.O·.8'....~..
0001509C | 05 41 D0 00 4E B7 00 38 27 0E 18 00 08 7E 01 00 | .AÐ.N·.8'....~..
000150AC | 05 41 D0 00 4E B7 00 38 27 0E 14 00 06 7E 01 00 | .AÐ.N·.8'....~..
000150BC | 05 41 D0 00 4E B7 00 39 27 0E 1B 00 08 7E 01 00 | .AÐ.N·.9'....~..
000150CC | 05 41 D1 00 4E B7 00 38 27 0A 14 00 06 7E 01 00 | .AÑ.N·.8'....~..
000150DC | 05 41 D2 00 4D B8 00 36 27 0E 14 00 07 7E 01 00 | .AÒ.M¸.6'....~..
```

Each line represents a record that can be decoded as follows:

  * Minutes since last stored reading (1:240)
  * Indoor relative humidity %        (1:99)    , 0xFF means invalid
  * Indoor temperature °C            (-40:+60) , 0xFFFF means invalid
  * Outdoor relative humidity %       (1:99)    , 0xFF means invalid
  * Outdoor temperature °C         (-40:+60) , 0xFFFF means invalid
  * Absolute pressure hPa        (920:1080), 0xFFFF means invalid
  * Wind average m/s        (0:50)    , 0xFF means invalid
  * Wind gust m/s        (0:50)    , 0xFF means invalid
  * Wind speed
    * Lower 4 bits are the average wind speed high bits
    * Upper 4 bits are the gust wind speed high bits
  * Wind direction
    * Multiply by 22.5 to get ° from north (0-15)
    * 7th bit indicates invalid data
  * Total rainfall mm (0:9999)
  * Status
    * 6th bit indicates loss of contact with sensors
    * 7th bit indicates rainfall overflow


For full details, see the source code: [fowsr.h](http://code.google.com/p/fowsr/source/browse/trunk/fowsr.src/fowsr.h)