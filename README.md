# IP2Location Apache Module

This is a IP2Location Apache Module that enables the user to find the country, region or state, district, city, latitude and longitude,  ZIP/Postal code, time zone, Internet Service Provider (ISP) or company  name, domain name, net speed, area code, weather station code, weather  station name, mobile country code (MCC), mobile network code (MNC) and  carrier brand, elevation, usage type, address type, IAB category and ASN and category by IP address or hostname originates from. The library reads the geo location information
from **IP2Location BIN data** file.

Supported IPv4 and IPv6 address.

For more details, please visit:
[https://www.ip2location.com/developers/apache](https://www.ip2location.com/developers/apache)

# Requirements
1. IP2Location C API library (download from https://github.com/chrislim2888/IP2Location-C-Library)
2. Apache 2.0x
3. GNU make or any compatible make utility.

# Installation
### Linux Build (Debian)
1. Create a new folder called ip2location.

2. Install the development libraries in Debian.
   ```
   apt install apache2-dev
   ```

3. Download IP2Location C library from [here](https://github.com/chrislim2888/IP2Location-C-Library/archive/master.zip) into the ip2location folder.

4. Decompress C library.

   ```
   unzip master.zip && rm master.zip
   ```

5. Get into working directory.

   ```
   cd IP2Location-C-Library-master
   ```

6. Compile and install IP2Location-C-Library.

   ```
   autoreconf -i -v --force
   ./configure
   make
   make install
   ```
   Note: You may need to run ```ldconfig``` to refresh the shared libraries, if needed.

7. Download [IP2Location Apache Module](https://github.com/ip2location/ip2location-apache/archive/refs/heads/master.zip) into the ip2location folder.

8. Decompress IP2Location Apache Module

   ```
   unzip master.zip && rm master.zip
   ```

9. Get into IP2Location Apache Module directory.

   ```
   cd ip2location-apache-master
   ```

10. Compile the IP2Location Apache Module.

   ```
   apxs2 -i -a -L ../IP2Location-C-Library-master/libIP2Location/ -I ../IP2Location-C-Library-master/libIP2Location/ -l IP2Location -c mod_ip2location.c
   ```

12. Run the below command and make sure you see **IP2Location_module (shared)** entry.

   ```
   apache2ctl -M
   ```

13. Add following lines into **/etc/apache2/apache2.conf**

   ```
   <IfModule mod_ip2location.c>
   	IP2LocationEnable On
   	IP2LocationDetectProxy <On|Off>
   	IP2LocationSetmode ALL
   	IP2LocationDBFile <ip2location_binary_db_file_with_fully_qualified_path>
   </IfModule>
   ```



### Windows Build

1. Create a new folder called ip2location.

2. Download IP2Location C library from [here](https://github.com/chrislim2888/IP2Location-C-Library/archive/master.zip) into the ip2location folder.

3. Decompress C library using [7-zip](https://www.7-zip.org/) or other compression tools.

4. Get into working directory.

   ```
   cd IP2Location-C-Library-master
   ```

5. Start compilation.

   ```
   nmake /f Makefile.win
   ```

6. Download and decompress Apache module into the ip2location folder.

7. Navigate to ip2location-apache-master folder.

8. Edit Makefile.win to change the path for C library.

   ```
   IP2LOCATION_CSRC_PATH = C:/ip2location/IP2Location-C-Library-master/libIP2Location
   IP2LOCATION_CLIB_PATH = C:/ip2location/IP2Location-C-Library-master/libIP2Location
   APACHE_INSTALL_PATH = "C:/Program Files/Apache Software Foundation/Apachex.x"
   ```

9. Start compilation.

   ```
   nmake /f Makefile.win
   ```

10. Copy IP2Location_apache.dll generated to the Apache modules directory.

11. Add following lines into httpd.conf

    ```
    <IfModule mod_ip2location.c>
    	IP2LocationEnable <ON|OFF>
    	IP2LocationDetectProxy <ON|OFF>
    	IP2LocationSetmode ALL
    	IP2LocationDBFile <ip2location_binary_db_file_with_fully_qualified_path>
    </IfModule>
    ```



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
1. Add below lines to your domain configuration file (apaches/sites-enabled) or .htaccess:  
   
    ```
    RewriteEngine On
    RewriteCond %{ENV:IP2LOCATION_COUNTRY_SHORT} ^US$
    RewriteRule ^(.*)$ https://www.google.com [L]
    ```
    
2. This will redirect all IP address from United States to https://www.google.com


# Sample BIN Databases
* Download free IP2Location LITE databases at [https://lite.ip2location.com](https://lite.ip2location.com)  
* Download IP2Location sample databases at [https://www.ip2location.com/developers](https://www.ip2location.com/developers)

# IPv4 BIN vs IPv6 BIN
* Use the IPv4 BIN file if you just need to query IPv4 addresses.
* Use the IPv6 BIN file if you need to query BOTH IPv4 and IPv6 addresses.

# Support
Email: support@ip2location.com.  
URL: [https://www.ip2location.com](https://www.ip2location.com)
