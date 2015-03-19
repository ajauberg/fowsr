# Introduction #

Create a local XML-format log file.


# Details #

**fowsr** supports the following variables read from the weather station and stored in `/var/fowsr.xml`:

```
<ws>
  <wsd date="2010-07-18 17:04:47" delay="5.0" hum_in="49.0" temp_in="25.300" hum_out="62.0" temp_out="22.500" abs_pressure="1006.300" wind_ave="2.7" wind_gust="3.4" wind_dir="180.0" rain="141.900" status="00">
  <wsd date="2010-07-18 16:59:47" delay="5.0" hum_in="49.0" temp_in="25.300" hum_out="59.0" temp_out="22.200" abs_pressure="1006.400" wind_ave="3.4" wind_gust="5.1" wind_dir="225.0" rain="141.900" status="00">
  <wsd date="2010-07-18 16:54:47" delay="5.0" hum_in="49.0" temp_in="25.300" hum_out="59.0" temp_out="22.100" abs_pressure="1006.500" wind_ave="3.4" wind_gust="4.8" wind_dir="180.0" rain="141.900" status="00">
  <wsd date="2010-07-18 16:49:47" delay="5.0" hum_in="49.0" temp_in="25.300" hum_out="57.0" temp_out="23.100" abs_pressure="1006.100" wind_ave="2.7" wind_gust="4.1" wind_dir="180.0" rain="141.900" status="00">
  <wsd date="2010-07-18 16:44:47" delay="5.0" hum_in="48.0" temp_in="25.300" hum_out="54.0" temp_out="22.900" abs_pressure="1006.300" wind_ave="3.1" wind_gust="4.1" wind_dir="157.5" rain="141.900" status="00">
  <wsd date="2010-07-18 16:39:47" delay="5.0" hum_in="48.0" temp_in="25.300" hum_out="53.0" temp_out="22.700" abs_pressure="1006.400" wind_ave="3.1" wind_gust="4.8" wind_dir="180.0" rain="141.900" status="00">
  <wsd date="2010-07-18 16:34:47" delay="5.0" hum_in="48.0" temp_in="25.200" hum_out="52.0" temp_out="22.600" abs_pressure="1006.500" wind_ave="4.1" wind_gust="5.8" wind_dir="180.0" rain="141.900" status="00">
  <wsd date="2010-07-18 16:29:47" delay="5.0" hum_in="48.0" temp_in="25.200" hum_out="54.0" temp_out="22.400" abs_pressure="1006.400" wind_ave="4.1" wind_gust="6.5" wind_dir="180.0" rain="141.900" status="00">
  <wsd date="2010-07-18 16:24:47" delay="5.0" hum_in="48.0" temp_in="25.200" hum_out="54.0" temp_out="22.100" abs_pressure="1006.400" wind_ave="4.1" wind_gust="5.8" wind_dir="180.0" rain="141.900" status="00">
  <wsd date="2010-07-18 16:19:47" delay="5.0" hum_in="48.0" temp_in="25.200" hum_out="55.0" temp_out="21.900" abs_pressure="1006.400" wind_ave="3.7" wind_gust="6.1" wind_dir="180.0" rain="141.900" status="00">
</ws>
```


The file can be created by calling the script `/usr/bin/xml.sh`.

The script can be called at regular intervals using cron, or called directly from the command line:

`/usr/bin/xml.sh`