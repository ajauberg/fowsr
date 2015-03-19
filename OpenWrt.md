# Introduction #

The **fowsr** application is included as a [package](https://dev.openwrt.org/browser/packages/utils/fowsr) in [OpenWrt](https://openwrt.org/).

# Details #

## Prebuilt snapshots ##

Prebuilt snapshots for different architectures can be found [here](http://downloads.openwrt.org/snapshots/trunk/).

The **fowsr** application is found in the packages directory:

`http://downloads.openwrt.org/snapshots/trunk/<architecture>/packages/fowsr-<version>`

## Custom build ##

Download the OpenWrt build system and generate the binaries as described in this documentation to make a custom build:

http://kamikaze.openwrt.org/docs/openwrt.html#x1-390002

Helpful instructions can also be found [here](http://wiki.openwrt.org/doc/howto/build#building.single.packages)

A quick guide includes the following steps:

  * Check the [prerequisites](http://wiki.openwrt.org/doc/howto/buildroot.exigence#table.of.known.prerequisites.and.their.corresponding.packages) for running the OpenWrt build system.

  * Download the latest OpenWrt trunk:

> `$ svn checkout svn://svn.openwrt.org/openwrt/trunk myopenwrt`
> (For download of different releases see [here](https://dev.openwrt.org/wiki/GetSource).)

  * Enter the openwrt catalog:

> `$ cd myopenwrt`

  * Include packages in the build:

> $ `./scripts/feeds update`

  * Add the **fowsr** package to the build

> $ `./scripts/feeds install fowsr`

  * Start the build system, select your architecture and activate the **fowsr** module:

> $ `make menuconfig`

  * Build the code:

> $ `make`


## Updating the Makefile ##

Perform the following steps if the Makefile in the OpenWrt trunk is out of date or if you want to use an different (newer) version of **fowsr**:

  * Copy this [Makefile](http://fowsr.googlecode.com/svn/trunk/openwrt/Makefile) to the following location in your OpenWrt build system:

`myopenwrt/feeds/packages/utils/fowsr/Makefile`

  * Alternatively, update at least the following fields to match the appropriate release:
    * `PKG_VERSION`
    * `PKG_MD5SUM`

  * Build the updated package:

> $ `make`