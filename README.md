# IP2Location Apache Module

This is a IP2Location Apache Module that enables the user to find the country, region, city, latitude, longitude, 
zip code, time zone, ISP, domain name, connection type, area code, weather, mobile network, elevation, 
usage type by IP address or hostname originates from. The library reads the geo location information
from **IP2Location BIN data** file.

Supported IPv4 and IPv6 address.

For more details, please visit:
[https://www.ip2location.com/developers/apache](https://www.ip2location.com/developers/apache)

# Requirements
1. IP2Location C API library ( download from https://www.ip2location.com/developers/c )
2. Apache 2.0x
3. GNU make or any compatible make utility

# Installation
### Linux Build
Depending on  Apache devel package, APXS is either as **apxs** or **apxs2** available.

```bash
apxs -i -a -L ../ip2location-c_x.y.z/libIP2Location/ -I ../ip2location-c-x.y.z/libIP2Location/ -l IP2Location -c mod_ip2location.c
```



### Windows Build
1. open Makefile.win and configure macros as below:    

   ```
   IP2LOCATION_CSRC_PATH = ../ip2location-c_x.y.z/libIP2Location
   IP2LOCATION_CLIB_PATH = ../ip2;ocation-c_x.x.x/libIP2Location
   APACHE_INSTALL_PATH   = PATH_TO_APACHE_INSTALLATION_FOLDER
   ```

2. Compile with Nmake.

   ```
   nmake /f Makefile.win
   ```

3. Copy the **IP2Location_apache.dll** generated to Apache modules folder.

   

# Apache Configuration
1. To load IP2Location module in Apache,  add the following lines in httpd.conf.

    ```
    LoadModule IP2Location_module FULL_PATH_TO_IP2LOCATION_MODULE
    
    <IfModule mod_ip2location.c>
    IP2LocationEnable On 
    # ENV will set server variables
    # NOTES will set apache notes 
    # ALL will set both
    IP2LocationSetmode ALL
    IP2LocationDBFile PATH_TO_IP2LOCATION_BIN_DATABASE
    IP2LocationDetectProxy On
    </IfModule>
    ```

    

    Reference:

    | Name                   | Value           | Description                                                  |
    | ---------------------- | --------------- | ------------------------------------------------------------ |
    | IP2LocationEnable      | On\|Off         | Enable or disable IP2Location module.                        |
    | IP2LocationSetmode     | ENV\|NOTES\|ALL | Choose where to display the geolocation variables. ENV = Server variables, NOTES = Apache notes, ALL = Both |
    | IP2LocationDBFile      | Path            | The full path to a IP2Location BIN database                  |
    | IP2LocationDetectProxy | On\|Off         | If you website is hosted behind a reverse proxy server, turn this on to detect the correct IP address |

    

2. Restart Apache server to take effect of the changes

**Notes**
If you are getting startup errors where Apache cannot locate the module files, please set your environment path as below:

```
$ LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib/apache2/modules/
$ export LD_LIBRARY_PATH
```

# Testing
### PHP Testing
From internet browser, load mod_ip2location_test.php

### Apache Rewrite Testing
1. Add below lines to Apache configuration file httpd.conf:  
    
    ```
    RewriteEngine On
    RewriteCond %{ENV:IP2LOCATION_COUNTRY_SHORT} ^UK$
    RewriteRule ^(.*)$ http://www.google.co.uk [L]
    ```
    
2. This will redirect all IP address from United Kingdom to http://www.google.co.uk


# Sample BIN Databases
* Download free IP2Location LITE databases at [https://lite.ip2location.com](https://lite.ip2location.com)  
* Download IP2Location sample databases at [https://www.ip2location.com/developers](https://www.ip2location.com/developers)

# IPv4 BIN vs IPv6 BIN
* Use the IPv4 BIN file if you just need to query IPv4 addresses.
* Use the IPv6 BIN file if you need to query BOTH IPv4 and IPv6 addresses.

# Support
Email: support@ip2location.com.  
URL: [https://www.ip2location.com](https://www.ip2location.com)
