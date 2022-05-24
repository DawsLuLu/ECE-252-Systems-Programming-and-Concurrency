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
size_t curlWriteCallback(char *ptr, size_t size, size_t nmemb, void *userdata) {
	// calc total bytes
	size_t bytes = size * nmemb;

	// cast arg to recvBuf_t * variable
	recvBuf_t *buf = userdata;
 
	// copy ptr data to buffer data member (make sure it fits first)
	assert(buf->size + bytes <= buf->max);
	memcpy(buf->data, ptr, bytes);

	// increment buffer size
	buf->size += bytes;

	// return bytes written
	return bytes;
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
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteCallback); 

	// set callback function arg
    recvBuf_t buf = { "", 0, 4096 };
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);
    
	// perform request
    CURLcode res = curl_easy_perform(curl);
    if(res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
    } else {
		// add code to print out received data
		// print buf data as string (make sure it is null-terminated)
		if(buf.size < buf.max) {
			buf.data[buf.size] = '\0';
		} else {
			buf.data[buf.max - 1] = '\0';
		}
		printf("%s\n", buf.data);
    }

	// do this one per handle
    curl_easy_cleanup(curl);

	// do this once per program
    curl_global_cleanup();
}

