/* Fine Offset Weather Station Reader - Header file

   (C) Arne-Jørgen Auberg (arne.jorgen.auberg@gmail.com)

  - Wireless Weather Station Data Block Definition
  - Wireless Weather Station Record Format Definition
  - Wunderground Record Format

  - CUSB class for open, initialize and close of USB interface
  - CWS class for open, read, write and close of WS buffer to selected log file format
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <ctype.h>
#include <time.h>
#include <usb.h>

#define ISREADING	0
#define ISWRITING	1

// Table for decoding raw weather station data.
// Each key specifies a (pos, type, scale) tuple that is understood by decode().
// See http://www.jim-easterbrook.me.uk/weather/mm/ for description of data

#define WS_BUFFER_SIZE		0x10010	// Size of total buffer
#define WS_BUFFER_START		0x100	// Size of fixed block, start of up to 4080 buffer records
#define WS_BUFFER_END		0xFFF0	// Last buffer record
#define WS_BUFFER_RECORD	0x10	// Size of one buffer record
#define WS_BUFFER_CHUNK		0x20	// Size of chunk received over USB

#define WS_DELAY		0	// Position of delay parameter
#define WS_RAIN			13	// Position of rain parameter
#define WS_DATA_COUNT		27	// Position of data_count parameter
#define WS_CURRENT_POS		30	// Position of current_pos parameter

#define WS_RAIN_HOUR		0x10000	// Position of hourly calculated rain
#define WS_RAIN_DAY		0x10002	// Position of daily calculated rain
#define WS_RAIN_WEEK		0x10004	// Position of weekly calculated rain
#define WS_RAIN_MONTH		0x10006	// Position of monthly calculated rain

enum ws_types {ub,sb,us,ss,dt,tt,pb,wa,wg};

struct ws_record {
	char name[22];
	int pos;
	enum ws_types ws_type;
	float scale;
} ws_format[] = {
// Up to 4080 records with this format
	{"delay"	,  0, ub,  1.0}, // Minutes since last stored reading
	{"hum_in"       ,  1, ub,  1.0},
	{"temp_in"      ,  2, ss,  0.1}, // Multiply by 0.1 to get °C
	{"hum_out"      ,  4, ub,  1.0},
	{"temp_out"     ,  5, ss,  0.1}, // Multiply by 0.1 to get °C
	{"abs_pressure" ,  7, us,  0.1}, // Multiply by 0.1 to get hPa
	{"wind_ave"     ,  9, wa,  0.1}, // Multiply by 0.1 to get m/s
	{"wind_gust"    , 10, wg,  0.1}, // Multiply by 0.1 to get m/s
	// 11, wind speed, high bits     // Lower 4 bits are the average wind speed high bits, upper 4 bits are the gust wind speed high bits.
	{"wind_dir"     , 12, ub,  1.0}, // Multiply by 22.5 to get ° from north
	{"rain"         , 13, us,  0.3}, // Multiply by 0.3 to get mm
	{"status"       , 15, pb,  1.0}, // 7th bit indicates loss of contact with sensors
// The lower fixed block
	{"read_period"   , 16, ub, 1.0}, // Minutes between each stored reading
	{"timezone"      , 24, sb, 1.0}, // Hours offset from Central European Time, so in the UK this should be set to -1. In stations without a radio controlled clock this is always zero
	{"data_count"    , 27, us, 1.0}, // Number of stored readings. Starts at zero, rises to 4080
	{"current_pos"   , 30, us, 1.0}, // Address of the stored reading currently being created. Starts at 256, rises to 65520 in steps of 16, then loops back to 256. The data at this address is updated every 48 seconds or so, until the read period is reached. Then the address is incremented and the next record becomes current.
	{"rel_pressure"  , 32, us, 0.1}, // Current relative (sea level) atmospheric pressure, multiply by 0.1 to get hPa
	{"abs_pressure"  , 34, us, 0.1}, // Current absolute atmospheric pressure, multiply by 0.1 to get hPa
	{"date_time"     , 43, dt, 1.0}, // Current date & time
// Alarm settings
	{"alarm.hum_in.hi"       , 48, ub,  1.0}, {"alarm.hum_in.lo"       , 49, ub, 1.0},
	{"alarm.temp_in.hi"      , 50, ss,  0.1}, {"alarm.temp_in.lo"      , 52, ss, 0.1}, // Multiply by 0.1 to get °C
	{"alarm.hum_out.hi"      , 54, ub,  1.0}, {"alarm.hum_out.lo"      , 55, ub, 1.0},
	{"alarm.temp_out.hi"     , 56, ss,  0.1}, {"alarm.temp_out.lo"     , 58, ss, 0.1}, // Multiply by 0.1 to get °C
	{"alarm.windchill.hi"    , 60, ss,  0.1}, {"alarm.windchill.lo"    , 62, ss, 0.1}, // Multiply by 0.1 to get °C
	{"alarm.dewpoint.hi"     , 64, ss,  0.1}, {"alarm.dewpoint.lo"     , 66, ss, 0.1}, // Multiply by 0.1 to get °C
	{"alarm.abs_pressure.hi" , 68, ss,  0.1}, {"alarm.abs_pressure.lo" , 70, ss, 0.1}, // Multiply by 0.1 to get hPa
	{"alarm.rel_pressure.hi" , 72, ss,  0.1}, {"alarm.rel_pressure.lo" , 74, ss, 0.1}, // Multiply by 0.1 to get hPa
	{"alarm.wind_ave.bft"    , 76, ub,  1.0}, {"alarm.wind_ave.ms"     , 77, ub, 0.1}, // Multiply by 0.1 to get m/s
	{"alarm.wind_gust.bft"   , 79, ub,  1.0}, {"alarm.wind_gust.ms"    , 80, ub, 0.1}, // Multiply by 0.1 to get m/s
	{"alarm.wind_dir"        , 82, ub, 22.5},                                          // Multiply by 22.5 to get ° from north
	{"alarm.rain.hour"       , 83, us,  0.3}, {"alarm.rain.day"        , 85, us, 0.3}, // Multiply by 0.3 to get mm
	{"alarm.time"            , 87, tt,  1.0},
// Maximums with timestamps
	{"max.hum_in.val"       ,  98, ub, 1.0}, {"max.hum_in.date"       , 141, dt, 1.0},
	{"max.hum_out.val"      , 100, ub, 1.0}, {"max.hum_out.date"      , 151, dt, 1.0},
	{"max.temp_in.val"      , 102, ss, 0.1}, {"max.temp_in.date"      , 161, dt, 1.0}, // Multiply by 0.1 to get °C
	{"max.temp_out.val"     , 106, ss, 0.1}, {"max.temp_out.date"     , 171, dt, 1.0}, // Multiply by 0.1 to get °C
	{"max.windchill.val"    , 110, ss, 0.1}, {"max.windchill.date"    , 181, dt, 1.0}, // Multiply by 0.1 to get °C
	{"max.dewpoint.val"     , 114, ss, 0.1}, {"max.dewpoint.date"     , 191, dt, 1.0}, // Multiply by 0.1 to get °C
	{"max.abs_pressure.val" , 118, us, 0.1}, {"max.abs_pressure.date" , 201, dt, 1.0}, // Multiply by 0.1 to get hPa
	{"max.rel_pressure.val" , 122, us, 0.1}, {"max.rel_pressure.date" , 211, dt, 1.0}, // Multiply by 0.1 to get hPa
	{"max.wind_ave.val"     , 126, us, 0.1}, {"max.wind_ave.date"     , 221, dt, 1.0}, // Multiply by 0.1 to get m/s
	{"max.wind_gust.val"    , 128, us, 0.1}, {"max.wind_gust.date"    , 226, dt, 1.0}, // Multiply by 0.1 to get m/s
	{"max.rain.hour.val"    , 130, us, 0.3}, {"max.rain.hour.date"    , 231, dt, 1.0}, // Multiply by 0.3 to get mm
	{"max.rain.day.val"     , 132, us, 0.3}, {"max.rain.day.date"     , 236, dt, 1.0}, // Multiply by 0.3 to get mm
	{"max.rain.week.val"    , 134, us, 0.3}, {"max.rain.week.date"    , 241, dt, 1.0}, // Multiply by 0.3 to get mm
	{"max.rain.month.val"   , 136, us, 0.3}, {"max.rain.month.date"   , 246, dt, 1.0}, // Multiply by 0.3 to get mm
	{"max.rain.total.val"   , 138, us, 0.3}, {"max.rain.total.date"   , 251, dt, 1.0}, // Multiply by 0.3 to get mm
// Minimums with timestamps
	{"min.hum_in.val"       ,  99, ub, 1.0}, {"min.hum_in.date"       , 146, dt, 1.0},
	{"min.hum_out.val"      , 101, ub, 1.0}, {"min.hum_out.date"      , 156, dt, 1.0},
	{"min.temp_in.val"      , 104, ss, 0.1}, {"min.temp_in.date"      , 166, dt, 1.0}, // Multiply by 0.1 to get °C
	{"min.temp_out.val"     , 108, ss, 0.1}, {"min.temp_out.date"     , 176, dt, 1.0}, // Multiply by 0.1 to get °C
	{"min.windchill.val"    , 112, ss, 0.1}, {"min.windchill.date"    , 186, dt, 1.0}, // Multiply by 0.1 to get °C
	{"min.dewpoint.val"     , 116, ss, 0.1}, {"min.dewpoint.date"     , 196, dt, 1.0}, // Multiply by 0.1 to get °C
	{"min.abs_pressure.val" , 120, us, 0.1}, {"min.abs_pressure.date" , 206, dt, 1.0}, // Multiply by 0.1 to get hPa
	{"min.rel_pressure.val" , 124, us, 0.1}, {"min.rel_pressure.date" , 216, dt, 1.0}, // Multiply by 0.1 to get hPa
// Calculated rainfall, must be callculated prior to every record
	{"rain.hour"    , WS_RAIN_HOUR , us, 0.3}, // Multiply by 0.3 to get mm
	{"rain.day"     , WS_RAIN_DAY  , us, 0.3}, // Multiply by 0.3 to get mm
	{"rain.week"    , WS_RAIN_WEEK , us, 0.3}, // Multiply by 0.3 to get mm
	{"rain.month"   , WS_RAIN_MONTH, us, 0.3}  // Multiply by 0.3 to get mm
};


// Table for creating Wunderground format
// Each key specifies a (pos, type, scale, offset) tuple that is understood by decode().
// See http://weatherstation.wunderground.com/weatherstation/updateweatherstation.php

struct wug_record {
	char name[13];
	int pos;
	enum ws_types ws_type;
	float scale;
	float offset;
} wug_format[] = {
	// (name, pos, type, scale, offset)
	// action [action=updateraw]
	// ID [ID as registered by wunderground.com]
	// PASSWORD [PASSWORD registered with this ID]
	// dateutc - [YYYY-MM-DD HH:MM:SS (mysql format)]
	{"winddir"      , 12, ub,  22.5,        0.0},	// - [0-360]
	{"windspeedmph" ,  9, wa,   0.22369363, 0.0},	// - [mph]
	{"windgustmph"  , 10, wg,   0.22369363, 0.0},	// - [windgustmph]
	{"humidity"     ,  4, ub,   1.0,        0.0},	// - [%]
	{"tempf"        ,  5, ss,   0.18,      32.0},	// - [temperature F]
//	{"rainin"       , 13, us,   0.39370079, 0.0},	// - [rain in]
	{"rainin"       ,256, us,   0.39370079, 0.0},	// - [rain in]
	{"dailyrainin"  ,258, us,   0.39370079, 0.0},	// dailyrainin - [daily rain in accumulated]
	{"baromin"      ,  7, us,   0.002953,   0.0}	// - [barom in]
	// dewptf - [dewpoint F]
	// weather - [text] -- metar style (+RA)
	// clouds - [text] -- SKC, FEW, SCT, BKN, OVC
	// softwaretype - [text] ie: vws or weatherdisplay
};


// libusb structures and functions
struct usb_dev_handle *devh;
struct usb_device *dev;

void list_devices();
struct usb_device *find_device(int vendor, int product);
void print_bytes(char *address, int len);

// USB class
int CUSB_Open(int vendor, int product);
int CUSB_Close();

// Weather Station class
void CWS_serialize(char isStoring);

int CWS_Open();
int CWS_Close();
int CWS_Read();
int CWS_Write(char arg, char* fname);

unsigned short CWS_dec_ptr(unsigned short ptr);
unsigned short CWS_read_block(unsigned short ptr, char* buf);
unsigned short CWS_read_fixed_block();	

int CWS_calculate_rain(unsigned short current_pos, unsigned short data_count, unsigned short start);
float CWS_dew_point(signed short temp, unsigned char hum);

unsigned char CWS_bcd_decode(unsigned char byte);
unsigned short CWS_unsigned_short(char* raw);
signed short CWS_signed_short(char* raw);
int CWS_decode(char* raw, enum ws_types ws_type, float scale, float offset, char* result);

// Weather Station properties
char m_buf[WS_BUFFER_SIZE] = {0};	// Raw WS data

time_t m_previous_timestamp = 0;	// Previous readout
time_t m_timestamp = 0;			// Current readout

