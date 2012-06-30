#include <string>

using std::string;

/**
 * This is a callback function for cURL
 *
 * For details see CURLOPT_WRITEFUNCTION on
 * http://curl.haxx.se/libcurl/c/curl_easy_setopt.html
 *
 * @param char* ptr the response body
 * @param void* userdata pointer to string that will be populated with the response body
 *
 * @return size_t the number of bytes taken care of
 */
size_t get_curl_data(char *ptr, size_t size, size_t nmemb, void *userdata){
    string *data = (string *) userdata;

    data->assign(ptr);

    return size * nmemb;
}