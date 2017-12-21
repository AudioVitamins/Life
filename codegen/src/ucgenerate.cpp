/* Copyright (C) 2016 Andrew Shakinovsky
*/
#include "unlockcode.h"
#include "ucshared.h"

static void
generateCode(int uid, unsigned short expireDays, char prodCode, char version, unsigned char *buf)
{
   // uid (4 bytes)
   buf[0] = (uid >> 24) & 0xFF; 
   buf[1] = (uid >> 16) & 0xFF; 
   buf[2] = (uid >> 8) & 0xFF;
   buf[3] = uid & 0xFF;
   // expire (2 bytes)
   buf[4] = (expireDays >> 8) & 0xFF;
   buf[5] = expireDays & 0xFF;

   buf[6] = prodCode;
   buf[7] = version;

}


static const char * hex = "0123456789ABCDEF";
static void
hexcode(unsigned char *input, char *output, int len)
{   
   unsigned char * pin = input;   
   char * pout = output;
   for (; pin < input + len; pout += 2, pin++) {
      pout[0] = hex[(*pin >> 4) & 0xF];
      pout[1] = hex[*pin & 0xF];      
   }
   pout[0] = 0;

}


void uc_generate(int uid, char prodid, char version, short int valid_days, char *output)
{
   unsigned char keybuf[START_KEY_LEN];
   unsigned char encbuf[FULL_KEY_LEN]; // encoded buf (2nd from last 2 bytes are gen day)(last 2 are checksum)
   
   unsigned short gen_date = days_since_epoch();

   // license expire date
   unsigned short lic_expire = gen_date + valid_days;

   generateCode(uid, lic_expire, prodid, version, keybuf);
   encode(keybuf, gen_date, encbuf);

   unsigned short cs = getChecksum(encbuf);
   encbuf[FULL_KEY_LEN - 2] = (cs >> 8) & 0xff;
   encbuf[FULL_KEY_LEN - 1] = cs & 0xff;
   
   hexcode(encbuf, output, FULL_KEY_LEN);
}
