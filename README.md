# IP2Location Apache Module 8.0.0

This is a IP2Location Apache Module that enables the user to find the country, region, city, latitude, longitude, 
zip code, time zone, ISP, domain name, connection type, area code, weather, mobile network, elevation, 
usage type by IP address or hostname originates from. The library reads the geo location information
from **IP2Location BIN data** file.

Supported IPv4 and IPv6 address.

For more details, please visit:
[http://www.ip2location.com/developers/apache](http://www.ip2location.com/developers/apache)

# Requirements
1. IP2Location C API library ( download from http://www.ip2location.com/developers/c )
2. Apache 2.0x
3. GNU make or any compatible make utility

# Installation
### Linux Build
Depending on installed Apache devel, APXS is either as "apxs" or "apxs2" available

1) apxs -i -a -L<path_to_ip2location_c_api_compiled_library> -I<path_to_ip2location_c_api_src> -lIP2Location -c mod_ip2location.c
	e.g. apxs -i -a -L ../ip2location-c-8.0.0/libIP2Location/ -I ../ip2location-c-x.x.x/libIP2Location/ -l IP2Location -c mod_ip2location.c

### Windows Build
1. open Makefile.win and configure macros as below:  
   `IP2LOCATION_CSRC_PATH = <path_to_ip2location_c_api_src>`  
   e.g. `IP2LOCATION_CSRC_PATH = ../C-IP2Location-x.x.x/libIP2Location`  

`IP2LOCATION_CLIB_PATH = <path_to_ip2location_c_api_compiled_library>`  
e.g. `IP2LOCATION_CLIB_PATH = ../C-IP2Location-x.x.x/libIP2Location`  

`APACHE_INSTALL_PATH   = <path_to_apache_installation>`  
e.g. `APACHE_INSTALL_PATH   = "C:/Program Files/Apache Group/Apache2"`  

1. nmake /f Makefile.win
2. copy the resulting IP2Location_apache.dll to Apache modules path

# Apache Configuration
1. load the compiled IP2Location module by adding these lines in httpd.conf  

    `LoadModule IP2Location_module <compiled_ip2location_dll_file_with_fully_qualified_path>`  
    `<IfModule mod_ip2location.c>`  
    `IP2LocationEnable <On|Off>`  
    `# ENV will set server variables`  
    `# NOTES will set apache notes`  
    `# ALL will set both`  
    `IP2LocationSetmode <ALL|ENV|NOTES>`  
    `IP2LocationDBFile <ip2location_binary_db_file_with_fully_qualified_path>`  
    `IP2LocationDetectProxy <On|Off>`  
    `</IfModule>`  

2. restart Apache server to take effect of the changes

# Testing
### PHP Testing
from internet browser, load mod_ip2location_test.php

### Apache Rewrite Testing
1. add below lines to Apache configuration file httpd.conf:
    `RewriteEngine On`  
    `RewriteCond %{ENV:IP2LOCATION_COUNTRY_SHORT} ^UK$`  
    `RewriteRule ^(.*)$ http://www.google.co.uk [L]`  
2. this will redirect all ip address from United Kingdom to http://www.google.co.uk


# Sample BIN Databases
* Download free IP2Location LITE databases at [http://lite.ip2location.com](http://lite.ip2location.com)  
* Download IP2Location sample databases at [http://www.ip2location.com/developers](http://www.ip2location.com/developers)

# Support
Email: support@ip2location.com.  
URL: [http://www.ip2location.com](http://www.ip2location.com)
