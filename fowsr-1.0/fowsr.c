/*  Fine Offset Weather Station Reader

 This application reads WH1080 compatible devices using the USB port.
 Compatible with all USB stations that can use the EasyWeather app (www.foshk.com)


 This file is generated with usbsnoop2libusb.pl from a usbsnoop log file.
 Latest version of the script should be in http://iki.fi/lindi/usb/usbsnoop2libusb.pl

 * wwsr - Wireless Weather Station Reader
 * 2007 dec 19, Michael Pendec (michael.pendec@gmail.com)
 * Version 0.5
 * 2008 jan 24 Svend Skafte (svend@skafte.net)
 * 2008 sep 28 Adam Pribyl (covex@lowlevel.cz)
 * Modifications for different firmware version(?)
 
 WeatherStation.py - get data from WH1080 compatible weather stations

 Derived from wwsr.c by Michael Pendec (michael.pendec@gmail.com) and
 wwsrdump.c by Svend Skafte (svend@skafte.net), modified by Dave Wells.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <ctype.h>
#include <time.h>
#include <usb.h>

#include "fowsr.h"

void release_usb_device(int dummy) {
    int ret;
    ret = usb_release_interface(devh, 0);
    if (!ret)
	printf("failed to release interface: %d\n", ret);
    usb_close(devh);
    if (!ret)
	printf("failed to close interface: %d\n", ret);
    exit(1);
}

void list_devices() {
    struct usb_bus *bus;
    for (bus = usb_get_busses(); bus; bus = bus->next) {
	struct usb_device *dev;

	for (dev = bus->devices; dev; dev = dev->next)
	    printf("0x%04x 0x%04x\n",
		   dev->descriptor.idVendor,
		   dev->descriptor.idProduct);
    }
}

struct usb_device *find_device(int vendor, int product) {
    struct usb_bus *bus;
    
    for (bus = usb_get_busses(); bus; bus = bus->next) {
	struct usb_device *dev;
	
	for (dev = bus->devices; dev; dev = dev->next) {
	    if (dev->descriptor.idVendor == vendor
		&& dev->descriptor.idProduct == product)
		return dev;
	}
    }
    return NULL;
}

void print_bytes(char *address, int length) {
	int i = 0; //used to keep track of line lengths
	char *line = (char*)address; //used to print char version of data
	unsigned char ch; // also used to print char version of data
	printf("%08X | ", (int)address); //Print the address we are pulling from
	while (length-- > 0) {
		printf("%02X ", (unsigned char)*address++); //Print each char
		if (!(++i % 16) || (length == 0 && i % 16)) { //If we come to the end of a line...
			//If this is the last line, print some fillers.
			if (length == 0) { while (i++ % 16) { printf("__ "); } }
			printf("| ");
			while (line < address) {  // Print the character version
				ch = *line++;
				printf("%c", (ch < 33 || ch == 255) ? 0x2E : ch);
			}
			// If we are not on the last line, prefix the next line with the address.
			if (length > 0) { printf("\n%08X | ", (int)address); }
		}
	}
	puts("");
}

int CUSB_Open(int vendor, int product)
{
	int ret;
	char buf[1000];

	usb_init();
	usb_set_debug(255);
	usb_find_busses();
	usb_find_devices();

	dev = find_device(vendor, product);
	assert(dev);

	devh = usb_open(dev);
	assert(devh);

	signal(SIGTERM, release_usb_device);

	ret = usb_get_driver_np(devh, 0, buf, sizeof(buf));
	printf("usb_get_driver_np returned %d\n", ret);
	if (ret == 0) {
		printf("interface 0 already claimed by driver \\'%s\\', attempting to detach it\n", buf);
		ret = usb_detach_kernel_driver_np(devh, 0);
		printf("usb_detach_kernel_driver_np returned %d\n", ret);
	}
	ret = usb_claim_interface(devh, 0);
	if (ret != 0) {
		printf("claim failed with error %d\n", ret);
			exit(1);
	}

	ret = usb_set_altinterface(devh, 0);
	assert(ret >= 0);

	ret = usb_get_descriptor(devh, 1, 0, buf, 0x12);
	ret = usb_get_descriptor(devh, 2, 0, buf, 0x09);
	ret = usb_get_descriptor(devh, 2, 0, buf, 0x22);
	ret = usb_release_interface(devh, 0);
	if (ret != 0) printf("failed to release interface before set_configuration: %d\n", ret);
	ret = usb_set_configuration(devh, 1);
	ret = usb_claim_interface(devh, 0);
	if (ret != 0) printf("claim after set_configuration failed with error %d\n", ret);
	ret = usb_set_altinterface(devh, 0);
	ret = usb_control_msg(devh, USB_TYPE_CLASS + USB_RECIP_INTERFACE, 0xa, 0, 0, buf, 0, 1000);
	ret = usb_get_descriptor(devh, 0x22, 0, buf, 0x74);

	return ret;
}

int CUSB_Close()
{
	int ret = usb_release_interface(devh, 0);
	assert(ret == 0);
	ret = usb_close(devh);
	assert(ret == 0);
	return ret;
}

void CWS_serialize(char isStoring)
{
	int n;
	char fname[] = "//var//weather.dat";	// cache file
	FILE* f;
	if (isStoring == ISWRITING) {
		if (f=fopen(fname,"wb")) {
			n=fwrite(&m_timestamp,sizeof(m_timestamp),1,f);
			n=fwrite(m_buf,sizeof(m_buf[0]),WS_BUFFER_SIZE,f);
		}
		print_bytes((char *)&m_timestamp, sizeof(time_t));
	} else {
		if (f=fopen(fname,"rb")) {
			n=fread(&m_previous_timestamp,sizeof(m_previous_timestamp),1,f);
			n=fread(m_buf,sizeof(m_buf[0]),WS_BUFFER_SIZE,f);
		}
		print_bytes((char *)&m_previous_timestamp, sizeof(time_t));
	};
	if (f) fclose(f);
}

int CWS_Open()
{
	CWS_serialize(ISREADING);	// Read cache file

	int vendor = 0x1941;
	int product = 0x8021; 

	CUSB_Open(vendor, product);

	return(0);
}

int CWS_Close()
{
	CWS_serialize(ISWRITING);	// Write cache file

	CUSB_Close();

	return(0);
}

int CWS_Read()
{
// Read fixed block
// - Get current_pos
// - Get data_count
// Read data_count records backwards from current_pos
// Calculate timestamp and break if already read
// Step 0x10 in the range 0x10000 to 0x100, wrap at 0x100
// USB is read in 0x20 byte chunks, so read at odd positions only, or on even position on first read

	m_timestamp = time(NULL);	// Set to current time
	time_t timestamp = m_timestamp;	// Set to current time

	int n=CWS_read_fixed_block();

	unsigned short data_count;
	data_count = CWS_unsigned_short(&m_buf[WS_DATA_COUNT]);
	unsigned short current_pos;
	current_pos = CWS_unsigned_short(&m_buf[WS_CURRENT_POS]);

	int i;
	for (i=0;i<data_count;i++)
	{
		n=0;
		if ((current_pos % 0x20) == 0x10) {
			// Read previous and current record on odd positions
			n=CWS_read_block(CWS_dec_ptr(current_pos),&m_buf[CWS_dec_ptr(current_pos)]);
		} else if (i == 0) {
			// Read current and next record on first read on even position
			n=CWS_read_block(current_pos,&m_buf[current_pos]);
		};

		timestamp -= m_buf[current_pos+WS_DELAY]*60;	// Update timestamp
		if (difftime(timestamp, m_previous_timestamp) < 0)
			break;	// All new records read

		current_pos=CWS_dec_ptr(current_pos);
	};

	// Dump all weather station records
	print_bytes(&m_buf[0x100], WS_BUFFER_SIZE-0x100);

	return(0);
}

int CWS_Write(char arg,char* fname)
{
// - Get current_pos
// - Get data_count
// Read data_count records backwards from current_pos. 
// Calculate timestamp and break if already written
// Step 0x10 in the range 0x10000 to 0x100
// Store output file in requested format

	time_t timestamp = m_timestamp;	// Set to current time

	unsigned short data_count;
	data_count = CWS_unsigned_short(&m_buf[WS_DATA_COUNT]);
	unsigned short current_pos;
	current_pos = CWS_unsigned_short(&m_buf[WS_CURRENT_POS]);

	FILE* f;
	if (f=fopen(fname,"a+s")) {

		unsigned short i;
		for (i=0;i<data_count;i++)
		{
			timestamp -= m_buf[current_pos+WS_DELAY]*60;	// Update timestamp
			if (difftime(timestamp, m_previous_timestamp) < 0)
				break;	// All new records written

			int n,j;
			char s1[1000]={0},s2[1000]={0};

			switch (arg) {
				case 'p':
					// Save in pywws raw format
					n=strftime(s1,100,"%Y-%m-%d %H:%M", gmtime(&timestamp));
					for (j=1;j<11;j++) {
						strcat(s1,",");
		
						CWS_decode(&m_buf[current_pos+ws_format[j].pos],
								ws_format[j].ws_type,
								ws_format[j].scale,
								0.0,
								s2);
		
						strcat(s1,s2);
					};
				break;
				case 'w':
					// Save in Wunderground format
					n=strftime(s1,100,"dateutc=%Y-%m-%d%20%H:%M:%S", gmtime(&timestamp));
					for (j=0;j<7;j++) {
						strcat(s1,"&");
						strcat(s1,wug_format[j].name);
						strcat(s1,"=");
		
						CWS_decode(&m_buf[current_pos+wug_format[j].pos],
								wug_format[j].ws_type,
								wug_format[j].scale,
								wug_format[j].offset,
								s2);
		
						strcat(s1,s2);
					};
				break;
				default:
					printf("Unknown log file format.\n");
			};

			strcat(s1,"\n");
			fputs(s1,f);

			current_pos=CWS_dec_ptr(current_pos);
		};
		fclose(f);
	};

	return(0);
}

unsigned short CWS_dec_ptr(unsigned short ptr)
{
	// Step backwards through buffer.
	ptr -= WS_BUFFER_RECORD;
	if (ptr < WS_BUFFER_START)
		// Start is reached, step to end of buffer.
		ptr = WS_BUFFER_END;
	return ptr;
}

unsigned short CWS_read_block(unsigned short ptr, char* buf)
{
        char buf_1 = (char)(ptr / 256) & 0xFF;
        char buf_2 = (char)(ptr & 0xFF);
	char tbuf[8];
	tbuf[0] = 0xA1;
	tbuf[1] = buf_1;
	tbuf[2] = buf_2;
	tbuf[3] = 0x20;
	tbuf[4] = 0xA1;
	tbuf[5] = buf_1;
	tbuf[6] = buf_2;
	tbuf[7] = 0x20;

	int ret;
	// Prepare read of 32-byte chunk from position ptr
	ret = usb_control_msg(devh, USB_TYPE_CLASS + USB_RECIP_INTERFACE, 9, 0x200, 0, tbuf, 8, 1000);
	// Read 32-byte chunk and place in buffer buf
	ret = usb_interrupt_read(devh, 0x81, buf, 0x20, 1000);

	return ret;
}

unsigned short CWS_read_fixed_block()
{
	// Read fixed block in 32 byte chunks
	unsigned short i;
	for (i=0x0000;i<0x0100;i+=0x0020)
		CWS_read_block(i, &m_buf[i]);

	// Display fixed block
	print_bytes(m_buf, 0x100);

	// Dump decoded fixed block data
	char s1[1000]={0},s2[1000]={0};
	for (i=0;i<88;i++) {
		strcpy(s1,ws_format[i].name);
		strcat(s1,"=");

		CWS_decode(&m_buf[ws_format[i].pos],
				ws_format[i].ws_type,
				ws_format[i].scale,
				0.0,
				s2);

		strcat(s1,s2);
		strcat(s1,"\n");
		printf(s1);
	}

	// Check for valid data
	if (((m_buf[0] == (char)0x55) && (m_buf[1] == (char)0xAA)) ||
	((m_buf[0] == (char)0xFF) && (m_buf[1] == (char)0xFF)))
		return(i);
	
	printf("Fixed block is not valid.\n");
	exit(1);
}

unsigned char CWS_bcd_decode(unsigned char byte)
{
        unsigned char hi = (byte / 16) & 0x0F;
        unsigned char lo = byte & 0x0F;
        return (hi * 10) + lo;
}

unsigned short CWS_unsigned_short(char* raw)
{
	return raw[0] + (raw[1] * 256);
}

signed short CWS_signed_short(char* raw)
{
	return raw[0] + (raw[1] * 256);
}

int CWS_decode(char* raw, enum ws_types ws_type, float scale, float offset, char* result)
{
	int n;
	float fresult;
	unsigned char year, month, day, hour, minute;
	switch(ws_type){
		case ub:
			fresult = (unsigned char)raw[0] * scale + offset;
			n=sprintf(result,"%.1f", fresult);
		break;
		case sb:
			fresult = (signed char)raw[0] * scale + offset;
			n=sprintf(result,"%.1f", fresult);
		break;
		case us:
			fresult = CWS_unsigned_short(raw) * scale + offset;
			n=sprintf(result,"%.1f", fresult);
		break;
		case ss:
			fresult = CWS_signed_short(raw) * scale + offset;
			n=sprintf(result,"%.1f", fresult);
		break;
		case dt:
			year = CWS_bcd_decode(raw[0]);
			month = CWS_bcd_decode(raw[1]);
			day = CWS_bcd_decode(raw[2]);
			hour = CWS_bcd_decode(raw[3]);
			minute = CWS_bcd_decode(raw[4]);
			n=sprintf(result,"%4d-%02d-%02d %02d:%02d", year + 2000, month, day, hour, minute);
		break;
		case tt:
			n=sprintf(result,"%02d:%02d", CWS_bcd_decode(raw[0]), CWS_bcd_decode(raw[1]));
		break;
		case pb:
			fresult = (unsigned char)raw[0];
			n=sprintf(result,"%02x", fresult);
		break;
		case wa:
			// wind average - 12 bits split across a byte and a nibble
			fresult = raw[0] + ((raw[2] & 0x0F) * 256);
			fresult = fresult * scale + offset;
			n=sprintf(result,"%.1f", fresult);
		break;
		case wg:
			// wind gust - 12 bits split across a byte and a nibble
			fresult = raw[0] + ((raw[1] & 0xF0) * 16);
			fresult = fresult * scale + offset;
			n=sprintf(result,"%.1f", fresult);
		break;
		default:
			printf("Unknown type %u",ws_type);
	}
	return n;
}


int main(int argc, char **argv) {

	char c;
	while ((c = getopt (argc, argv, "pw?h")) != -1)
	{
		switch (c)
		{
			case 'p':
				CWS_Open();
				CWS_Read();
				CWS_Write(c,"//var//pywws.log");
				CWS_Close();
			break;
			case 'w':
				CWS_Open();
				CWS_Read();
				CWS_Write(c,"//var//wunderground.log");
				CWS_Close();
			break;
			default:
				printf("Fine Offset Weather Station Reader v1.0\n");
				printf("(C) 2010 Arne-Jørgen Auberg (arne.jorgen.auberg@gmail.com)\n\n");
				printf("See http://fowsr.googlecode.com for more information\n");
				printf("Credits to Michael Pendec, Jim Easterbrook, Timo Juhani Lindfors\n\n");
				printf("options\n");
				printf(" -p	Logfile in pywws format\n");
				printf(" -w	Logfile in Wunderground format\n\n");
				exit (0);
		}
	}

	return 0;
}
