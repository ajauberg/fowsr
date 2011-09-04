#
# Copyright (C) 2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=fowsr
PKG_VERSION:=1.0
PKG_RELEASE:=20110904

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_RELEASE).tar.gz
PKG_SOURCE_URL:=http://fowsr.googlecode.com/files
PKG_MD5SUM:=3dbe93a59d85bf89f9c1d20d54f5e983
PKG_CAT:=zcat

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/fowsr
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=+libusb
  TITLE:=Fine Offset Wireless Weather Station Reader
  URL:=http://fowsr.googlecode.com/
endef

define Package/fowsr/description
	fowsr is a USB Data Collector application for WH1080 compatible 
weather stations.
	Output formats includes XML, Wunderground.com and pywws.
	See http://fowsr.googlecode.com for further information.
endef

define Build/Configure
	$(call Build/Configure/Default, \
		--with-linux-headers="$(LINUX_DIR)" \
	)
endef

define Package/fowsr/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/fowsr $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/fowsr.sh $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/pwsweather.sh $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/wunderground.sh $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/pywws.sh $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/xml.sh $(1)/usr/bin/
endef

$(eval $(call BuildPackage,fowsr))
