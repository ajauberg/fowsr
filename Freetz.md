# Introduction #

The **fowsr** application is included as a [package](http://freetz.org/browser/trunk/make/fowsr) in [Freetz](http://freetz.org/wiki#).

# Details #

## Custom build ##

Download the Freetz build system and generate the binaries using the following steps:

  * Install the [prerequisites](http://freetz.org/wiki/help/howtos/common/install#NotwendigePakete) for running the Freetz build system.

  * Download the latest Freetz trunk:

> `$ svn co http://svn.freetz.org/trunk freetz`

  * Enter the freetz catalog:

> `$ cd freetz`

  * Start the build menu:

> $ `make menuconfig`

  * Select your box, and select the 'fowsr' and 'wget' packages to be included in the image. Exit the build menu and save the changes.

  * Build the code:

> $ `make`

## Installation ##

  * Follow [these](http://freetz.org/wiki/help/howtos/common/newbie.en) instructions to upload the image to the unit.

  * Log in to the console, default uid/pwd is root/freetz, and make the configuration files writable as stated [here](http://freetz.org/wiki/FAQ.en#Settingsarenotavailableatcurrentsecuritylevel).

  * Log in to the WEB interface, default uid/pwd is freetz/freetz, start crond, and edit crontab with the script to be called as indicated [here](http://code.google.com/p/fowsr/wiki/cron).