/*  Fine Offset Weather Station Reader - Main file

   (C) Arne-Jørgen Auberg (arne.jorgen.auberg@gmail.com)

   This application reads WH1080 compatible devices using the USB port.
   Compatible with all USB stations that can use the EasyWeather app (www.foshk.com)

   The application is written with inspiration from the following projects:

  1)	WeatherStation.py - The pywws poject. http://pywws.googlecode.com

  2)	usbsnoop2libusb.pl - The usbsnoop log file.
	The latest version of the script should be in http://iki.fi/lindi/usb/usbsnoop2libusb.pl

  3)	wwsr.c - Wireless Weather Station Reader
	Michael Pendec (michael.pendec@gmail.com)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include <usb.h>

#include "fowsr.h"

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

/***** libusb functions *****/

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

/***** The CUSB class *****/

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

// Uncomment the following 4 lines for FreeBSD support
//#ifdef LIBUSB_HAS_GET_DRIVER_NP
	signal(SIGTERM, CUSB_Close);

	ret = usb_get_driver_np(devh, 0, buf, sizeof(buf));
	printf("usb_get_driver_np returned %d\n", ret);
	if (ret == 0) {
		printf("interface 0 already claimed by driver \\'%s\\', attempting to detach it\n", buf);
//#ifdef LIBUSB_HAS_DETACH_KERNEL_DRIVER_NP
		ret = usb_detach_kernel_driver_np(devh, 0);
		printf("usb_detach_kernel_driver_np returned %d\n", ret);
//#endif
	}
//#endif
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

void CUSB_Close()
{
	int ret = usb_release_interface(devh, 0);
	assert(ret == 0);
	if (!ret) printf("failed to release interface: %d\n", ret);
	usb_close(devh);
	assert(ret == 0);
	if (!ret) printf("failed to close interface: %d\n", ret);
}

unsigned short CUSB_read_block(unsigned short ptr, char* buf)
{
/*
Read 32 bytes data command	

After sending the read command, the device will send back 32 bytes data wihtin 100ms. 
If not, then it means the command has not been received correctly.
*/
	char buf_1 = (char)(ptr / 256);
	char buf_2 = (char)(ptr & 0xFF);
	char tbuf[8];
	tbuf[0] = 0xA1;		// READ COMMAND
	tbuf[1] = buf_1;	// READ ADDRESS HIGH
	tbuf[2] = buf_2;	// READ ADDRESS LOW
	tbuf[3] = 0x20;		// END MARK
	tbuf[4] = 0xA1;		// READ COMMAND
	tbuf[5] = 0;		// READ ADDRESS HIGH
	tbuf[6] = 0;		// READ ADDRESS LOW
	tbuf[7] = 0x20;		// END MARK

	int ret;
	// Prepare read of 32-byte chunk from position ptr
	ret = usb_control_msg(devh, USB_TYPE_CLASS + USB_RECIP_INTERFACE, 9, 0x200, 0, tbuf, 8, 1000);
	// Read 32-byte chunk and place in buffer buf
	ret = usb_interrupt_read(devh, 0x81, buf, 0x20, 1000);

	return ret;
}

unsigned short CUSB_write_byte(unsigned short ptr, char* buf)
{
/*
Write one byte data to ADDR command	

If data has been received and written correctly, 
the device will return 8 bytes 0xA5 indicating the command has been carried out sucessfully.
*/
	char buf_1 = (char)(ptr / 256);
	char buf_2 = (char)(ptr & 0xFF);
	char tbuf[8];
	tbuf[0] = 0xA2;		// WRITE COMMAND
	tbuf[1] = buf_1;	// WRITE ADDRESS HIGH
	tbuf[2] = buf_2;	// WRITE ADDRESS LOW
	tbuf[3] = 0x20;		// END MARK
	tbuf[4] = 0xA2;		// WRITE COMMAND
	tbuf[5] = *buf;		// DATA TO BE WRITTEN
	tbuf[6] = 0;		// DON'T CARE
	tbuf[7] = 0x20;		// END MARK

	int ret;
	// Prepare write of 32-byte chunk from position ptr
	ret = usb_control_msg(devh, USB_TYPE_CLASS + USB_RECIP_INTERFACE, 9, 0x200, 0, tbuf, 8, 1000);
	// Write 32-byte chunk from buffer buf
	ret = usb_interrupt_write(devh, 0x81, buf, 0x20, 1000);
	// Read 8-byte result and place in buffer tbuf
	ret = usb_interrupt_read(devh, 0x81, tbuf, 0x08, 1000);

	return ret;
}

unsigned short CUSB_write_block(unsigned short ptr, char* buf)
{
/*
Write 32 bytes data command	
	
After sending the write command, the 32 bytes data load must be sent to device within 500ms. 
Once the 32 bytes data have been written to the EEPROM sucessfully, 
then the device will return 8 bytes of 0xA5 telling that the data has been written correctly.
*/
	char buf_1 = (char)(ptr / 256);
	char buf_2 = (char)(ptr & 0xFF);
	char tbuf[8];
	tbuf[0] = 0xA0;		// WRITE COMMAND
	tbuf[1] = buf_1;	// WRITE ADDRESS HIGH
	tbuf[2] = buf_2;	// WRITE ADDRESS LOW
	tbuf[3] = 0x20;		// END MARK
	tbuf[4] = 0xA0;		// WRITE COMMAND
	tbuf[5] = 0;		// DON'T CARE
	tbuf[6] = 0;		// DON'T CARE
	tbuf[7] = 0x20;		// END MARK

	int ret;
	// Prepare write of 32-byte chunk from position ptr
	ret = usb_control_msg(devh, USB_TYPE_CLASS + USB_RECIP_INTERFACE, 9, 0x200, 0, tbuf, 8, 1000);
	// Write 32-byte chunk from buffer buf
	ret = usb_interrupt_write(devh, 0x81, buf, 0x20, 1000);
	// Read 8-byte result and place in buffer tbuf
	ret = usb_interrupt_read(devh, 0x81, tbuf, 0x08, 1000);

	return ret;
}

/***** The CWS class *****/

void CWS_Cache(char isStoring)
{
	int n;
	char fname[] = "//var//fowsr.dat";	// cache file
	FILE* f;
	if (isStoring == WS_CACHE_READ) {
		if (f=fopen(fname,"rb")) {
			n=fread(&m_previous_timestamp,sizeof(m_previous_timestamp),1,f);
			n=fread(m_buf,sizeof(m_buf[0]),WS_BUFFER_SIZE,f);
		}
		print_bytes((char *)&m_previous_timestamp, sizeof(time_t));
	} else {	// WS_CACHE_WRITE
		if (f=fopen(fname,"wb")) {
			n=fwrite(&m_timestamp,sizeof(m_timestamp),1,f);
			n=fwrite(m_buf,sizeof(m_buf[0]),WS_BUFFER_SIZE,f);
		}
		print_bytes((char *)&m_timestamp, sizeof(time_t));
	};
	if (f) fclose(f);
}

void CWS_print_decoded_data()
{
	int i;
	char s1[1000]={0},s2[1000]={0};
	for (i=WS_LOWER_FIXED_BLOCK_START;i<WS_LOWER_FIXED_BLOCK_END;i++) {
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
}

int CWS_Open()
{
	CWS_Cache(WS_CACHE_READ);	// Read cache file

	int vendor = 0x1941;
	int product = 0x8021; 

	CUSB_Open(vendor, product);

	return(0);
}

int CWS_Close()
{
	CWS_Cache(WS_CACHE_WRITE);	// Write cache file

	CUSB_Close();

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

unsigned short CWS_read_fixed_block()
{
	// Read fixed block in 32 byte chunks
	unsigned short i;
	for (i=WS_FIXED_BLOCK_START;i<WS_FIXED_BLOCK_SIZE;i+=WS_BUFFER_CHUNK)
		CUSB_read_block(i, &m_buf[i]);

	// Check for valid data
	if (((m_buf[0] == (unsigned char)0x55) && (m_buf[1] == (unsigned char)0xAA)) ||
	((m_buf[0] == (unsigned char)0xFF) && (m_buf[1] == (unsigned char)0xFF)))
		return(i);
	
	printf("Fixed block is not valid.\n");
	exit(1);
}

char CWS_calculate_rain_period(char done, unsigned short pos, unsigned short begin, unsigned short end)
{
	if (done) // Already done?
		return 1;

	unsigned short result;
	unsigned int begin_rain, end_rain;

	begin_rain = CWS_unsigned_short(&m_buf[begin]);
	end_rain   = CWS_unsigned_short(&m_buf[end]);
	if (begin_rain > end_rain) {	// Test for wrap around in rain counter
		end_rain += 0x10000;	// Make sure that end rain counter always is the largest
	}

	result = (end_rain - begin_rain) % 0x10000;	// Squeeze the result back to a short

	m_buf[pos]	= result % 0x100;		// Lo byte
	m_buf[pos+1]	= (result / 256) % 0x100;	// Hi byte

	return 1;
}

unsigned int CWS_calculate_rain(unsigned short current_pos, unsigned short data_count, unsigned short start)
{
	// Initialize rain variables
	m_buf[WS_RAIN_HOUR]	= 0;	m_buf[WS_RAIN_HOUR +1]	= 0;
	m_buf[WS_RAIN_DAY]	= 0;	m_buf[WS_RAIN_DAY  +1]	= 0;
	m_buf[WS_RAIN_WEEK]	= 0;	m_buf[WS_RAIN_WEEK +1]	= 0;
	m_buf[WS_RAIN_MONTH]	= 0;	m_buf[WS_RAIN_MONTH+1]	= 0;

	// Flags set when calculation is done
	char bhour	= 0;
	char bday	= 0;
	char bweek	= 0;
	char bmonth	= 0;

	// Set the different time periods
	time_t hour	=       60*60;
	time_t day	=    24*60*60;
	time_t week	=  7*24*60*60;
	time_t month	= 30*24*60*60;

	unsigned short initial_pos = current_pos;
	time_t timestamp = m_timestamp;	// Set to current time

	unsigned short i;
	for (i=start;i<data_count;i++) {	// Calculate backwards through buffer, not all values will be calculated if buffer is too short
		if        (difftime(m_timestamp,timestamp) > month) {
			bmonth = CWS_calculate_rain_period(bmonth, WS_RAIN_MONTH, current_pos+WS_RAIN, initial_pos+WS_RAIN);

		} else if (difftime(m_timestamp,timestamp) > week) {
			bweek = CWS_calculate_rain_period(bweek, WS_RAIN_WEEK,    current_pos+WS_RAIN, initial_pos+WS_RAIN);

		} else if (difftime(m_timestamp,timestamp) > day) {
			bday = CWS_calculate_rain_period(bday, WS_RAIN_DAY,       current_pos+WS_RAIN, initial_pos+WS_RAIN);

		} else if (difftime(m_timestamp,timestamp) > hour) {
			bhour = CWS_calculate_rain_period(bhour, WS_RAIN_HOUR,    current_pos+WS_RAIN, initial_pos+WS_RAIN);

		}

		timestamp -= m_buf[current_pos+WS_DELAY]*60;	// Update timestamp

		current_pos=CWS_dec_ptr(current_pos);
	}

	return (0);
}

float CWS_dew_point(char* raw, float scale, float offset)
{
	float temp = CWS_signed_short(raw+WS_TEMPERATURE_OUT) * scale + offset;
	float hum = raw[WS_HUMIDITY_OUT];

	// Compute dew point, using formula from
	// http://en.wikipedia.org/wiki/Dew_point.
	float a = 17.27;
	float b = 237.7;

	float gamma = ((a * temp) / (b + temp)) + log(hum / 100);

	return (b * gamma) / (a - gamma);
}

unsigned char CWS_bcd_decode(unsigned char byte)
{
        unsigned char lo = byte & 0x0F;
        unsigned char hi = byte / 16;
        return (lo + (hi * 10));
}

unsigned short CWS_unsigned_short(char* raw)
{
        unsigned char lo = (unsigned char)raw[0];
        unsigned char hi = (unsigned char)raw[1];
	return lo + (hi * 256);
}

signed short CWS_signed_short(char* raw)
{
        unsigned char lo = (unsigned char)raw[0];
        unsigned char hi = (unsigned char)raw[1];
	unsigned short us = lo + (hi * 256);
	if (us >= 0x8000)	// Test for sign bit
		return -(us - 0x8000);	// Negative value
	else
		return us;		// Positive value
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
			fresult = (unsigned char)raw[0];
			if (fresult >= 0x80)	// Test for sign bit
				fresult = -(fresult - 0x80);	// Negative value
			fresult = fresult * scale + offset;
			n=sprintf(result,"%.1f", fresult);
		break;
		case us:
			fresult = CWS_unsigned_short(raw) * scale + offset;
			n=sprintf(result,"%.3f", fresult);
		break;
		case ss:
			fresult = CWS_signed_short(raw) * scale + offset;
			n=sprintf(result,"%.3f", fresult);
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
			fresult = (unsigned char)raw[0] + (((unsigned char)raw[2] & 0x0F) * 256);
			fresult = fresult * scale + offset;
			n=sprintf(result,"%.1f", fresult);
		break;
		case wg:
			// wind gust - 12 bits split across a byte and a nibble
			fresult = (unsigned char)raw[0] + (((unsigned char)raw[1] & 0xF0) * 16);
			fresult = fresult * scale + offset;
			n=sprintf(result,"%.1f", fresult);
		break;
		case dp:
			// Scale outside temperature and calculate dew point
			fresult = CWS_dew_point(raw, scale, offset);
			n=sprintf(result,"%.1f", fresult);
		break;
		default:
			printf("Unknown type %u",ws_type);
	}
	return n;
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
		if ((current_pos % WS_BUFFER_CHUNK) == WS_BUFFER_RECORD) {
			// Read previous and current record on odd positions
			n=CUSB_read_block(CWS_dec_ptr(current_pos),&m_buf[CWS_dec_ptr(current_pos)]);
		} else if (i == 0) {
			// Read current and next record on first read on even position
			n=CUSB_read_block(current_pos,&m_buf[current_pos]);
		};

		timestamp -= m_buf[current_pos+WS_DELAY]*60;	// Update timestamp
		if (difftime(timestamp, m_previous_timestamp) < 0)
			break;	// All new records read

		current_pos=CWS_dec_ptr(current_pos);
	};

	return(0);
}

/***** The CWF class *****/

int CWF_Write(char arg,char* fname)
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

		// Header
		switch (arg) {
			case 'x':
				fputs("<ws>\n",f);
			break;
		};

		// Body
		char s1[1000]={0},s2[1000]={0};
		unsigned short i;
		for (i=0;i<data_count;i++)
		{
			timestamp -= m_buf[current_pos+WS_DELAY]*60;	// Update timestamp
			if (difftime(timestamp, m_previous_timestamp) < 0)
				break;	// All new records written

			CWS_calculate_rain(current_pos, data_count, i);

			int n,j;
			switch (arg) {
				case 'p':
					// Save in pywws raw format
					n=strftime(s1,100,"%Y-%m-%d %H:%M:%S", gmtime(&timestamp));
					for (j=0;j<WS_PYWWS_RECORDS;j++) {
						strcat(s1,",");
		
						CWS_decode(&m_buf[current_pos+pywws_format[j].pos],
								pywws_format[j].ws_type,
								pywws_format[j].scale,
								0.0,
								s2);
		
						strcat(s1,s2);
					};
				break;
				case 's':
					// Save in PWS Weather format
					n=strftime(s1,100,"dateutc=%Y-%m-%d+%H\%3A%M\%3A%S", gmtime(&timestamp));
					for (j=0;j<WS_PWS_RECORDS;j++) {
						strcat(s1,"&");
						strcat(s1,pws_format[j].name);
						strcat(s1,"=");

						if (j==WS_PWS_HOURLY_RAIN || j==WS_PWS_DAILY_RAIN) {
							CWS_decode(&m_buf[pws_format[j].pos],
									pws_format[j].ws_type,
									pws_format[j].scale,
									pws_format[j].offset,
									s2);
						} else {
							CWS_decode(&m_buf[current_pos+pws_format[j].pos],
									pws_format[j].ws_type,
									pws_format[j].scale,
									pws_format[j].offset,
									s2);
						}

						strcat(s1,s2);
					};
				break;
				case 'w':
					// Save in Wunderground format
					n=strftime(s1,100,"dateutc=%Y-%m-%d %H:%M:%S", gmtime(&timestamp));
					// Calculate relative pressure
          wug_format[WS_WUG_PRESSURE].offset+=(CWS_unsigned_short(m_buf+WS_CURR_REL_PRESSURE)-CWS_unsigned_short(m_buf+WS_CURR_ABS_PRESSURE));
					for (j=0;j<WS_WUG_RECORDS;j++) {
						strcat(s1,"&");
						strcat(s1,wug_format[j].name);
						strcat(s1,"=");

						if (j==WS_WUG_HOURLY_RAIN || j==WS_WUG_DAILY_RAIN) {
							CWS_decode(&m_buf[wug_format[j].pos],
									wug_format[j].ws_type,
									wug_format[j].scale,
									wug_format[j].offset,
									s2);
						} else {
							CWS_decode(&m_buf[wug_format[j].pos+current_pos],
									wug_format[j].ws_type,
									wug_format[j].scale,
									wug_format[j].offset,
									s2);
						}

						strcat(s1,s2);
					};
				break;
				case 'x':
					// Save in XML format
					n=strftime(s1,100,"  <wsd date=\"%Y-%m-%d %H:%M:%S\"", gmtime(&timestamp));
					for (j=0;j<WS_RECORDS;j++) {
						strcat(s1," ");
						strcat(s1,ws_format[j].name);
						strcat(s1,"=\"");

						CWS_decode(&m_buf[current_pos+ws_format[j].pos],
								ws_format[j].ws_type,
								ws_format[j].scale,
								0.0,
								s2);

						strcat(s1,s2);
						strcat(s1,"\"");
					};
					strcat(s1,">");
				break;
				default:
					printf("Unknown log file format.\n");
			};

			strcat(s1,"\n");
			fputs(s1,f);

			current_pos=CWS_dec_ptr(current_pos);
		};

		// Footer
		switch (arg) {
			case 'x':
				fputs("</ws>\n",f);
			break;
		};

		fclose(f);
	};

	return(0);
}

int main(int argc, char **argv) {

	int bflag	= 0;	// Display fixed block
	int dflag	= 0;	// Dump decoded fixed block data
	int rflag	= 0;	// Dump all weather station records

	int readflag	= 0;	// Read the weather station or use the cache file.
	int pflag	= 0;	// Create /var/pywws.log
	int sflag	= 0;	// Create /var/pwsweather.log
	int wflag	= 0;	// Create /var/wunderground.log
	int xflag	= 0;	// Create /var/fowsr.xml

	int c;
	while ((c = getopt (argc, argv, "bdrpswx")) != -1)
	{
		switch (c)
		{
			case 'b':	// Display fixed block
				bflag	= 1;
			break;
			case 'd':	// Dump decoded fixed block data
				dflag	= 1;
			break;
			case 'r':	// Dump all weather station records
				rflag	= 1;
			break;
			case 'p':
				readflag	= 1;
				pflag		= 1;
			break;
			case 's':
				readflag	= 1;
				sflag		= 1;
			break;
			case 'w':
				readflag	= 1;
				wflag		= 1;
			break;
			case 'x':
				readflag	= 1;
				xflag		= 1;
			break;
			case '?':
				printf("\n");
				printf("Fine Offset Weather Station Reader v1.0\n\n");
				printf("(C) 2010 Arne-Jørgen Auberg (arne.jorgen.auberg@gmail.com)\n");
				printf("Credits to Michael Pendec, Jim Easterbrook, Timo Juhani Lindfors\n\n");
				printf("See http://fowsr.googlecode.com for more information\n\n");
				printf("options\n");
				printf(" -p	Logfile in pywws format\n");
				printf(" -s	Logfile in PWS Weather format\n");
				printf(" -w	Logfile in Wunderground format\n");
				printf(" -x	Logfile in XML format\n");
				printf(" -b	Display fixed block\n");
				printf(" -d	Display decoded fixed block data\n");
				printf(" -r	Dump all weather station records\n\n");
				exit (0);
			default:
				abort();
		}
	}

	
	CWS_Open();	// Read the cache file and open the weather station

	if (readflag)
		CWS_Read();	// Read the weather station

	// Write the log files
	if (pflag)
		CWF_Write('p',"//var//pywws.log");
	if (sflag)
		CWF_Write('s',"//var//pwsweather.log");
	if (wflag)
		CWF_Write('w',"//var//wunderground.log");
	if (xflag)
		CWF_Write('x',"//var//fowsr.xml");

	if (bflag)	// Display fixed block
		print_bytes(m_buf, WS_FIXED_BLOCK_SIZE);
	if (dflag)	// Dump decoded fixed block data
		CWS_print_decoded_data();
	if (rflag)	// Dump all weather station records
		print_bytes(&m_buf[WS_BUFFER_START], WS_BUFFER_SIZE-WS_BUFFER_START);

	CWS_Close();	// Write the cache file and close the weather station
	
	return 0;
}
