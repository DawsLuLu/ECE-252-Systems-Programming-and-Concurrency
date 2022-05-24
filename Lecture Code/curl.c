/**
 * Use curllib to read webpage.
 */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

/**
 * Used as argument to writeCallback().
 */
typedef struct {
	char data[4096]; // storage for received data
	size_t size; // how much has been received
	size_t max; // how big the storage is
} recvBuf_t;

/**
 * Writes data received by curl into mem.
 * @param ptr data received by curl
 * @param size size of 1 data element
 * @param nmemb number of data elements
 * @param userdata pointer to user struct used for storing data
 * @return return bytes read. Must equal size*nmemb or curl_easy_perform()
 * fails.
 */
size_t writeCallback(char *ptr, size_t size, size_t nmemb, void *userdata) {
}

int main(int argc, char *argv[]) {

	// this should be done once and once only
	curl_global_init(CURL_GLOBAL_DEFAULT);

	// can have multiple handles
	CURL *curl = curl_easy_init();
	assert(curl != NULL);

	// set URL
	curl_easy_setopt(curl, CURLOPT_URL, "engadvisor.uwaterloo.ca");

	// set callback function

	// set callback function arg

	// perform request
	// sends data to stdout if no callback function provided
	CURLcode res = curl_easy_perform(curl);
	if(res != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
	} else {
		// add code to print out received data
	}

	// do this one per handle
	curl_easy_cleanup(curl);

	// do this once per program
	curl_global_cleanup();
}

