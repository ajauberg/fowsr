# Introduction #

Follow the steps below for installing fowsr from a prebuilt OpenWrt package.


# Details #

## Preparation ##

- Decide which service to use for displaying the weather data, and create an account for the weather station:

  * [Weather Underground](http://www.wunderground.com/members/signup.asp)
  * [PWS Weather](http://www.pwsweather.com/register.php)

- Install the EasyWeather application, and connect the weather station to your PC:

  * Configure the time zone
  * Set the sampling interval to 5 minutes

- Connect the Weather Station to the OpenWrt device

- Log in to the OpenWrt device, either using the web interface or the command line interface

- Verify that the OpenWrt package feed is available in the opkg configuration:

  * Type 'cat /etc/opkg.conf', and verify that the domain 'downloads.openwrt.org' is included in one of the feeds if using the command line interface
  * Verify that the domain 'downloads.openwrt.org' is included in one of the feeds if using the web interface

- Verify that cron is started on your OpenWrt device


## Installation ##

- Update the package lists

Type 'opkg update' from the command line, or click the 'Update package list' link in the web interface

- Install fowsr

Type 'opkg install fowsr' from the command line, or click the 'Install' link in the web interface


## Configuration ##

- Choose the correct upload script, and fill in the username and password for registering the station:

  * Use `*/5 * * * * /usr/bin/wunderground.sh <username> <password>` for uploading weather data to Weather Underground every 5 minutes
  * Use `*/5 * * * * /usr/bin/pwsweather.sh <username> <password>` for uploading weather data to PWS Weather every 5 minutes
  * Use `*/5 * * * * /usr/bin/fowsr.sh <username> <password>` for uploading weather data to both services every 5 minutes

- Add the selected upload string to cron

  * Type 'crontab -e' from the command line and add the string using the editor, or add the string by clicking the 'cron' link in the WEB page


## Testing ##

- Check the weather service, and verify that weather data is showing up.