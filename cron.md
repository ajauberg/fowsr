# Introduction #

Call your scripts at regular intervals to upload data.


# Details #

**cron** is a Linux process that performs tasks at regular intervals. The following statement will cause the script `/usr/bin/fowsr.sh` to be called every 5 minutes, passing the parameters PWS station name and password:

`*/5 * * * * /usr/bin/fowsr.sh <PWS station name> <password>`

Enter the following command to add the line above to cron on your Linux system:

`crontab -e`