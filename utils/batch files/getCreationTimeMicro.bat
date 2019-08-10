@echo off
@rem modified from https://superuser.com/a/937401
set a="%~1"
set a=%a:\=\\%
set a=%a:/=\\%
wmic datafile where name=%a% get creationdate | findstr /brc:[0-9]
