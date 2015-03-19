# Introduction #

Fonera is built on Openwrt, and many packages from the Openwrt repository will run on the Fonera. A special build environment is however needed to compile the packages.

# Details #

## Custom build ##

Download the [Fonera build system](http://trac.fonosfera.org/fon-ng/wiki/build) and generate the binaries using the following steps:

  * Install the requirements for running the Fonera build system.

  * Download the latest Fonera trunk:

> `$ svn co http://svn.fonosfera.org/fon-ng/trunk/ fonera`

  * Enter the fonera catalog:

> `$ cd fonera`

  * Update and install FON feeds:

> `$ chmod +x install.sh`

> `$ ./install.sh`

> `$ cd openwrt`

  * Update OpenWRT feeds:

> `$ ./scripts/feeds update -a`

  * Install the fowsr package:

> `$ ./scripts/feeds install fowsr`

  * Start the build system:

> `$ make menuconfig`

  * Select your box and select 'fowsr' and 'wget' to be included in the image. Exit the build system and save the changes.

  * Build the code:

> `$ make`