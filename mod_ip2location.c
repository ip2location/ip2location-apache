/* Copyright (C) 2005-2014 IP2Location.com
 * All Rights Reserved
 *
 * This library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; If not, see <http://www.gnu.org/licenses/>.
 */

#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h" 
#include "http_log.h"
#include "ap_config.h"
#include "apr_strings.h"
#include "IP2Location.h"


static const int ENV_SET_MODE    = 0x0001;
static const int NOTES_SET_MODE  = 0x0002;
static const int ALL_SET_MODE    = 0x0003;

typedef struct {
	int enabled;
	int detectProxy;
	int setMode; 
	char* dbFile;
	IP2Location* ip2locObj;
} ip2location_server_config;

module AP_MODULE_DECLARE_DATA IP2Location_module;

static apr_status_t ip2location_cleanup(void *cfgdata) {
   // cleanup code here, if needed
   return APR_SUCCESS;
}

static void ip2location_child_init(apr_pool_t *p, server_rec *s) {
   apr_pool_cleanup_register(p, NULL, ip2location_cleanup, ip2location_cleanup);
}

static int ip2location_post_read_request(request_rec *r) {
	char* ipaddr;
	ip2location_server_config* config;
	IP2LocationRecord* record;
	char buff[20];
	
	config = (ip2location_server_config*) ap_get_module_config(r->server->module_config, &IP2Location_module);
	
	if(!config->enabled)
		return OK;

	if(config->detectProxy){
		if(apr_table_get(r->headers_in, "Client-IP")) {
			ipaddr = (char *)apr_table_get(r->headers_in, "Client-IP");
		}
		else if(apr_table_get(r->headers_in, "X-Forwarded-For")) {
			ipaddr = (char *)apr_table_get(r->headers_in, "X-Forwarded-For");
		}
		else if(apr_table_get(r->headers_in, "X-Forwarded-IP")) {
			ipaddr = (char *)apr_table_get(r->headers_in, "X-Forwarded-IP");
		}
		else if(apr_table_get(r->headers_in, "Forwarded-For")) {
			ipaddr = (char *)apr_table_get(r->headers_in, "Forwarded-For");
		}
		else if(apr_table_get(r->headers_in, "X-Forwarded")) {
			ipaddr = (char *)apr_table_get(r->headers_in, "X-Forwarded");
		}
		else if(apr_table_get(r->headers_in, "Via")) {
			ipaddr = (char *)apr_table_get(r->headers_in, "Via");
		}
		else {
			#if (((AP_SERVER_MAJORVERSION_NUMBER == 2) && (AP_SERVER_MINORVERSION_NUMBER >= 4)) || (AP_SERVER_MAJORVERSION_NUMBER > 2))
				ipaddr = r->connection->client_ip;
			#else
				ipaddr = r->connection->remote_ip;
			#endif
		}
	}
	else{
		#if (((AP_SERVER_MAJORVERSION_NUMBER == 2) && (AP_SERVER_MINORVERSION_NUMBER >= 4)) || (AP_SERVER_MAJORVERSION_NUMBER > 2))
			ipaddr = r->connection->client_ip;
		#else	
			ipaddr = r->connection->remote_ip;
		#endif	
	}

	record = IP2Location_get_all(config->ip2locObj, ipaddr);

	if(record) {
		if(config->setMode & ENV_SET_MODE) {
			apr_table_set(r->subprocess_env, "IP2LOCATION_COUNTRY_SHORT", record->country_short); 
			apr_table_set(r->subprocess_env, "IP2LOCATION_COUNTRY_LONG", record->country_long); 
			apr_table_set(r->subprocess_env, "IP2LOCATION_REGION", record->region); 
			apr_table_set(r->subprocess_env, "IP2LOCATION_CITY", record->city); 
			apr_table_set(r->subprocess_env, "IP2LOCATION_ISP", record->isp); 
			sprintf(buff, "%f", record->latitude);
			apr_table_set(r->subprocess_env, "IP2LOCATION_LATITUDE", buff); 
			sprintf(buff, "%f", record->longitude);
			apr_table_set(r->subprocess_env, "IP2LOCATION_LONGITUDE", buff); 
			apr_table_set(r->subprocess_env, "IP2LOCATION_DOMAIN", record->domain); 
			apr_table_set(r->subprocess_env, "IP2LOCATION_ZIPCODE", record->zipcode); 
			apr_table_set(r->subprocess_env, "IP2LOCATION_TIMEZONE", record->timezone); 
			apr_table_set(r->subprocess_env, "IP2LOCATION_NETSPEED", record->netspeed);
			apr_table_set(r->subprocess_env, "IP2LOCATION_IDDCODE", record->iddcode);
			apr_table_set(r->subprocess_env, "IP2LOCATION_AREACODE", record->areacode);
			apr_table_set(r->subprocess_env, "IP2LOCATION_WEATHERSTATIONCODE", record->weatherstationcode);
			apr_table_set(r->subprocess_env, "IP2LOCATION_WEATHERSTATIONNAME", record->weatherstationname);
			apr_table_set(r->subprocess_env, "IP2LOCATION_MCC", record->mcc);
			apr_table_set(r->subprocess_env, "IP2LOCATION_MNC", record->mnc);
			apr_table_set(r->subprocess_env, "IP2LOCATION_MOBILEBRAND", record->mobilebrand);
			sprintf(buff, "%f", record->elevation);
			apr_table_set(r->subprocess_env, "IP2LOCATION_ELEVATION", buff); 
			apr_table_set(r->subprocess_env, "IP2LOCATION_USAGETYPE", record->usagetype);
		}
		if(config->setMode & NOTES_SET_MODE) {
			apr_table_set(r->notes, "IP2LOCATION_COUNTRY_SHORT", record->country_short); 
			apr_table_set(r->notes, "IP2LOCATION_COUNTRY_LONG", record->country_long); 
			apr_table_set(r->notes, "IP2LOCATION_REGION", record->region); 
			apr_table_set(r->notes, "IP2LOCATION_CITY", record->city); 
			apr_table_set(r->notes, "IP2LOCATION_ISP", record->isp); 
			sprintf(buff, "%f", record->latitude);
			apr_table_set(r->notes, "IP2LOCATION_LATITUDE", buff); 
			sprintf(buff, "%f", record->longitude);
			apr_table_set(r->notes, "IP2LOCATION_LONGITUDE", buff); 
			apr_table_set(r->notes, "IP2LOCATION_DOMAIN", record->domain); 
			apr_table_set(r->notes, "IP2LOCATION_ZIPCODE", record->zipcode); 
			apr_table_set(r->notes, "IP2LOCATION_TIMEZONE", record->timezone); 
			apr_table_set(r->notes, "IP2LOCATION_NETSPEED", record->netspeed);
			apr_table_set(r->notes, "IP2LOCATION_IDDCODE", record->iddcode);
			apr_table_set(r->notes, "IP2LOCATION_AREACODE", record->areacode);
			apr_table_set(r->notes, "IP2LOCATION_WEATHERSTATIONCODE", record->weatherstationcode);
			apr_table_set(r->notes, "IP2LOCATION_WEATHERSTATIONNAME", record->weatherstationname);
			apr_table_set(r->notes, "IP2LOCATION_MCC", record->mcc);
			apr_table_set(r->notes, "IP2LOCATION_MNC", record->mnc);
			apr_table_set(r->notes, "IP2LOCATION_MOBILEBRAND", record->mobilebrand);
			sprintf(buff, "%f", record->elevation);
			apr_table_set(r->notes, "IP2LOCATION_ELEVATION", buff); 
			apr_table_set(r->notes, "IP2LOCATION_USAGETYPE", record->usagetype);
		}
	
		IP2Location_free_record(record);		
	}
	
	return OK;
}

static const char* set_ip2location_enable(cmd_parms *cmd, void *dummy, int arg) {
	ip2location_server_config* config = (ip2location_server_config*) ap_get_module_config(cmd->server->module_config, &IP2Location_module);
	
	if(!config) 
		return NULL;
	
	config->enabled = arg;
	
	return NULL;
}

static const char* set_ip2location_dbfile(cmd_parms* cmd, void* dummy, const char* dbFile, int arg) {
  ip2location_server_config* config = (ip2location_server_config*) ap_get_module_config(cmd->server->module_config, &IP2Location_module);
	if(!config) 
		return NULL;
		
	config->dbFile = apr_pstrdup(cmd->pool, dbFile);
	if(config->enabled) {
		config->ip2locObj = IP2Location_open(config->dbFile);	
		
		if(!config->ip2locObj)
			return "Error opening dbFile!";
	}

	return NULL; 
}

static const char* set_ip2location_set_mode(cmd_parms* cmd, void* dummy, const char* mode, int arg) {
	ip2location_server_config* config = (ip2location_server_config*) ap_get_module_config(cmd->server->module_config, &IP2Location_module);
	
	if(!config) 
		return NULL;
	
	if(strcmp(mode, "ALL") == 0) 	
		config->setMode = ALL_SET_MODE;

	else if(strcmp(mode, "ENV") == 0) 	
		config->setMode = ENV_SET_MODE;

	else if(strcmp(mode, "NOTES") == 0)
		config->setMode = NOTES_SET_MODE; 	

	else
		return "Invalid mode for IP2LocationSetMode";
	
	return NULL; 
}

static const char* set_ip2location_detect_proxy(cmd_parms *cmd, void *dummy, int arg) {
	ip2location_server_config* config = (ip2location_server_config*) ap_get_module_config(cmd->server->module_config, &IP2Location_module);
	
	if(!config) 
		return NULL;
	
	config->detectProxy = arg;
	
	return NULL;
}

static void* ip2location_create_svr_conf(apr_pool_t* pool, server_rec* svr) {
	ip2location_server_config* svr_cfg = apr_pcalloc(pool, sizeof(ip2location_server_config));
	
	svr_cfg->enabled = 0;
	svr_cfg->dbFile = NULL;
	svr_cfg->setMode = ALL_SET_MODE;
	svr_cfg->detectProxy = 0;
	svr_cfg->ip2locObj = NULL;
	return svr_cfg ;
}

static const command_rec ip2location_cmds[] = {
	AP_INIT_FLAG( "IP2LocationEnable", set_ip2location_enable, NULL, OR_FILEINFO, "Turn on mod_ip2location"),
	AP_INIT_TAKE1( "IP2LocationDBFile", (const char *(*)()) set_ip2location_dbfile, NULL, OR_FILEINFO, "File path to DB file"),
	AP_INIT_TAKE1( "IP2LocationSetMode", (const char *(*)()) set_ip2location_set_mode, NULL, OR_FILEINFO, "Set scope mode"),
	AP_INIT_TAKE1( "IP2LocationDetectProxy", (const char *(*)()) set_ip2location_detect_proxy, NULL, OR_FILEINFO, "Detect proxy headers"),
	{NULL} 
};

static void ip2location_register_hooks(apr_pool_t *p) {
	ap_hook_post_read_request( ip2location_post_read_request, NULL, NULL, APR_HOOK_MIDDLE );
	ap_hook_child_init(        ip2location_child_init, NULL, NULL, APR_HOOK_MIDDLE );
}

// API hooks
module AP_MODULE_DECLARE_DATA IP2Location_module = {
	STANDARD20_MODULE_STUFF, 
	NULL,                        /* create per-dir    config structures */
	NULL,                        /* merge  per-dir    config structures */
	ip2location_create_svr_conf, /* create per-server config structures */
	NULL,                        /* merge  per-server config structures */
	ip2location_cmds,            /* table of config file commands       */
	ip2location_register_hooks   /* register hooks                      */
};