/*
 * FogLAMP south plugin.
 *
 * Copyright (c) 2018 Dianomic Systems
 *
 * Released under the Apache 2.0 Licence
 *
 * Author: Mark Riddoch
 */
#include <csv.h>
#include <plugin_api.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string>
#include <logger.h>
#include <plugin_exception.h>
#include <config_category.h>
#include <version.h>

typedef void (*INGEST_CB)(void *, Reading);

using namespace std;

#define PLUGIN_NAME "CSV-Async"
#define CONFIG	"{\"plugin\" : { \"description\" : \"" PLUGIN_NAME " C south plugin\", " \
			"\"type\" : \"string\", \"default\" : \"" PLUGIN_NAME "\", \"readonly\" : \"true\" }, " \
		"\"asset\" : { \"description\" : \"Asset name\", " \
			"\"type\" : \"string\", \"default\" : \"Vibration\", \"order\": \"1\", \"displayName\": \"Asset Name\" }," \
		"\"file\" : { \"description\" : \"Name of the file to read\", " \
			"\"type\" : \"string\", \"default\" : \"\", \"order\": \"2\", \"displayName\": \"Path Of File\" }" \
		"} "

/**
 * The Csv plugin interface
 */
extern "C" {

/**
 * The plugin information structure
 */
static PLUGIN_INFORMATION info = {
	PLUGIN_NAME,              // Name
	VERSION,                  // Version
	SP_ASYNC, 		  // Flags
	PLUGIN_TYPE_SOUTH,        // Type
	"1.0.0",                  // Interface version
	CONFIG                    // Default configuration
};

/**
 * Return the information about this plugin
 */
PLUGIN_INFORMATION *plugin_info()
{
	return &info;
}

/**
 * Initialise the plugin, called to get the plugin handle
 */
PLUGIN_HANDLE plugin_init(ConfigCategory *config)
{
Csv *csv = new Csv();

	csv->configure(config);

	return (PLUGIN_HANDLE)csv;
}

/**
 * Start the Async handling for the plugin
 */
void plugin_start(PLUGIN_HANDLE *handle)
{
Csv *csv = (Csv *)handle;

	csv->start();
}

/**
 * Register ingest callback
 */
void plugin_register_ingest(PLUGIN_HANDLE *handle, INGEST_CB cb, void *data)
{
Csv *csv = (Csv *)handle;

        if (!handle)
                throw new exception();
        csv->registerIngest(data, cb);
}

/**
 * Poll for a plugin reading
 */
Reading plugin_poll(PLUGIN_HANDLE *handle)
{
Csv *csv = (Csv *)handle;

	return csv->nextValue();
}

/**
 * Reconfigure the plugin
 */
void plugin_reconfigure(PLUGIN_HANDLE *handle, string& newConfig)
{
ConfigCategory	config("csv", newConfig);
Csv		*csv = (Csv *)*handle;

	csv->configure(&config);
}

/**
 * Shutdown the plugin
 */
void plugin_shutdown(PLUGIN_HANDLE *handle)
{
Csv *csv = (Csv *)handle;

	delete csv;
}
};
