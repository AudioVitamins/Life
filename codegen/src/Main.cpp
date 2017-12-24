/* Copyright (C) 2016 Andrew Shakinovsky
*/
#include <stdio.h>
#include "unlockcode.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define ERR_INVALID 2      // parameters are invalid (eg uid checksum failed)
#define ERR_COMMAND 1      // invalid command line

#define USER_ID_ADD 2127456 // added to userid when generating (x 10 shouldn't exceed sizeof int)
/*

*/

#define OPT_GEN            1
#define OPT_LOOKUP         2
#define OPT_USERID         3
#define OPT_VALIDATEUSER   4
#define OPT_INFO           5
#define PROG_NAME "codegen"
void displayHelp()
{
   printf(
      "Usage:\n   " PROG_NAME " [-v] <mode> <options>\n"
      "   -v - Verbose output\n"
      "   Modes:\n"
      "   GEN - generate key - <userid> <prod code> <version> [days valid]\n"
      "   LOOKUP - show info about key - <key>\n"
      "   USERID - generate userid with check digit - <input id number>\n"
      "   VALIDATEID - validate user id's check digit - <user id>\n"
      "   INFO - display days info\n"
      "For GEN, days valid can't exceed %d, and defaults to %d\n", VALID_DAYS, VALID_DAYS);

}

void displayDaysInfo(bool verbose)
{
   if (verbose) {
      printf("Today is %d days from epoch\n"
         "Max valid period %d days\n",
         days_since_epoch(), VALID_DAYS);
   }
   else {
      printf("%d %d\n",
         days_since_epoch(), VALID_DAYS);
   }

}

const char *upper(const char *in, char *out)
{
   strncpy(out, in, 255);
   out[255] = 0;
   char *p = out;
   while (*p) {
      *p = toupper(*p);
      p++;
   }
   return out;
}

#define MAX_OPTS 6
const char *opts[MAX_OPTS] = {};

int getOpt(int argc, char* argv[], bool *verbose)
{
   int ret = 0;
   int curArg = 1;
   int opt = 0;
   static char buf[256];
   const char *v;
   *verbose = false;
   for (curArg=1;curArg < argc;curArg++) {      
      if (ret == 0) { // dont have option yet
         v = upper(argv[curArg], buf);
         if (!strcmp(v, "-V"))
            *verbose = true;
         else if (!strcmp(v, "GEN"))
            ret = OPT_GEN;
         else if (!strcmp(v, "LOOKUP"))
            ret=OPT_LOOKUP;
         else if (!strcmp(v, "USERID"))
            ret= OPT_USERID;
         else if (!strcmp(v, "VALIDATEID"))
            ret= OPT_VALIDATEUSER;
         else if (!strcmp(v, "INFO"))
            ret= OPT_INFO;
      }
      else {
         // rest go into opts
         v = argv[curArg];
         if (opt < MAX_OPTS)
            opts[opt] = v;
         opt++;
      }       
   }

   return ret;
}


int main (int argc, char* argv[])
{
   char keybuf[OUTPUT_KEY_LEN + 1];
   int ret = ERR_COMMAND;
   bool verbose;
   if (argc < 2) {
      displayHelp();
      ret = 0;
      goto end;
   }

   int opt = getOpt(argc, argv, &verbose);
   if (!opt) {
      displayHelp();
      goto end;
   }

   switch (opt) {
   case OPT_INFO:
      displayDaysInfo(verbose);
      ret = 0;
      break;
   case OPT_GEN:
   {
      int uid = 0;
      unsigned short int days = VALID_DAYS;
      char prod, ver;
      if (opts[0]) {
         uid = atoi(opts[0]);
      }
      else goto end;
      if (opts[1]) { // prod code
         prod = (char)atoi(opts[1]);
      }
      else goto end;
      if (opts[2]) { //version
         ver = (char)atoi(opts[2]);
      }
      else goto end;
      if (opts[3]) { // optional days
         days = (unsigned short)atoi(opts[3]);
         if (days > VALID_DAYS) {
            ret = ERR_INVALID;
            printf("Invalid days\n");
            goto end;
         }            
      }
      
      if (!luhnvalid(uid)) {
         printf("Invalid user id\n");
         ret = ERR_INVALID;
         goto end;
      }
      uc_generate(uid, prod, ver, days, keybuf);
      printf("%s\n", keybuf);
      if (verbose) {
         printf("\nGenerated code for uid %d, product %d, version %d, days valid %d\n",
            uid, prod, ver, days);
      }
      ret = 0;
      break;
   }
   case OPT_VALIDATEUSER:
   {
      int uid = 0;
      if (opts[0]) {
         uid = atoi(opts[0]);
      }
      if (!luhnvalid(uid)) 
         printf("INVALID\n");
      else 
         printf("VALID\n");
      if (verbose)
         printf("Uid %d\n", uid);
      ret = 0;
      break;
   }
   case OPT_LOOKUP:
      if (opts[0]) {
         char prod, ver;
         int outid;
         short days;
         const char *errtxt;
         int rc = uc_validate(opts[0], &prod, &ver, &days, &outid);
         switch (rc) {
         case UC_ERR_HEXINVALID:
            printf("CORRUPT\n");
            errtxt = "Corrupted";
            break;
         case UC_ERR_TIMECODEINVALID:
            printf("TAMPERED\n");
            errtxt = "Tampered or expired";
            break;
         case UC_ERR_UIDINVALID:
            printf("BADUID\n");
            errtxt = "User ID invalid";
            break;
         case UC_ERR_EXPIRED:
            printf("EXPIRED\n");
            errtxt = "Expired";
            break;
         default:
            printf("VALID %d %d %d %d\n", days, outid, prod, ver);
            errtxt = "Good";
            break;         
         }
         if (verbose) {
            printf("Status: %s\n"
               "Days left: %d\n"
               "UID: %d\n"
               "Prod: %d\n"
               "Ver: %d\n", errtxt, days, outid, prod, ver);
         }
         ret = 0;
      }
      break;
   case OPT_USERID:
      if (opts[0]) {
         int uid = atoi(opts[0]);
         int outid;
         if (!uid)
            break;
         outid = USER_ID_ADD + uid;
         outid = luhncreate(outid);
         printf("%d\n", outid);
         if (verbose)
            printf("Input ID is %d, output ID is %d", uid, outid);
         ret = 0;
      }
      break;
   }

   
 end:
    if (ret == ERR_INVALID) {
       printf("Invalid parameter input\n");
    }
    else if (ret == ERR_COMMAND) {
       printf("Invalid command line\n");
    }

   return ret;
}
