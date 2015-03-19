<b><a href='http://code.google.com/p/fowsr/wiki/FineOffsetWeatherStationReader'>fowsr</a></b> is a Linux application that reads the wireless weather stations
  * [WH1080](http://www.foshk.com/Weather_Professional/WH1080.htm) / [WH1081](http://www.foshk.com/Weather_Professional/WH1081.htm) / [WH1090](http://www.foshk.com/Weather_Professional/WH1090.htm) / [WH1091](http://www.foshk.com/Weather_Professional/WH1091.htm) / [WH2080](http://www.foshk.com/Weather_Professional/WH2080.htm) / [WH2081](http://www.foshk.com/Weather_Professional/WH2081.htm)
  * Watson W-8681
  * Scientific Sales Pro Touch Screen Weather Station
  * TOPCOM NATIONAL GEOGRAPHIC 265NE
  * PCE-FWS 20
  * ... and other similar USB devices from [Fine Offset Electronics Co., LTD.](http://www.foshk.com) compatible with the [EasyWeather application](http://code.google.com/p/fowsr/wiki/Compatibility)

The result is a weather history log file that can be uploaded to a central server for further processing. Example script files for uploads is included. So far the following formats are supported:
  * [Weather Underground](http://code.google.com/p/fowsr/wiki/Wunderground)
  * [PWS Weather](http://code.google.com/p/fowsr/wiki/pwsweather)
  * [pywws](http://code.google.com/p/fowsr/wiki/pywws)
  * [XML](http://code.google.com/p/fowsr/wiki/xml)

**fowsr** performs a complete read out of the weather station memory using its USB port, and stores the result in a cache file to speed up later read-outs. Rain data is then calculated per hour, day, week and month if data for these periods exist. No further data processing is performed. This makes **fowsr** very small and well suited for running in embedded devices at remote locations. Adding a 3G USB modem to an embedded device such as the NSLU2 or Fritz!Box makes it possible to receive weather data from remote weather stations with very limited hardware.

**fowsr** is available as an [OpenWrt package](http://code.google.com/p/fowsr/wiki/OpenWrt), supporting a number of different processors and architectures out-of-the-box.

**fowsr** is available as a [Freetz package](http://code.google.com/p/fowsr/wiki/Freetz). This makes it available for all [Fritz!Box](http://www.avm.de/en/Produkte/FRITZBox/index.html) devices with a USB host port.


&lt;wiki:gadget url="http://fowsr.googlecode.com/svn/trunk/googlegadget/pws\_bars.xml" title="Example charts with input from fowsr" width="400" height="750" border="0"/&gt;
