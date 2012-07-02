/**
 * @file
 *
 * @author Barbu Paul - Gheorghe
 */
#include "includes/simplenote.hpp"
#include "includes/helpers.hpp"
#include "includes/base64.h"

#include <curl/curl.h>

#include <cstring>
#include <string>

using std::string;

//TODO: remove these
#include <iostream>
using std::cout;

/**
 * TODO:
 * add tests
 * 
 * add docs
 */

/**
 * Explicit constructor for Simplenote that authenticates the user upon object
 * creation
 *
 * In this case the use of lazy loading is not a good choice
 * because the variables that would have been on the stack
 * (user's email and password) can be inspected.
 * By authenticating the user right away enables this vulnerability for a short
 * period of time, until the token is received.
 *
 * @throw InitError if the setup of cURL fails
 * @throw FetchError if no data can be fetched from Simplenote
 * @throw AuthenticationError if after connecting to Simplenote, no token is
 * received
 *
 * @param char* email the user's Simplenote email
 * @param char* password the user's password for Simplenote
 */
Simplenote::Simplenote(const char *email, const char *password){
    // this function call may throw InitError, so don't catch it
    // because the user should know if the object initialization failed
    init();

    string request_body = create_request_body((string)email, (string)password);

    // this function call may throw InitError or FetchError, so don't catch here
    // because the user should know if the object initialization failed
    authenticate(request_body);
}

/**
 * Creates a request body to be used for authentication
 *
 * The request body must compile with the Simplenote requirements, namely to
 * have a form similar to a URL query and to be base64 encoded
 *
 * @param char* email the user's Simplenote email
 * @param char* password the user's password for Simplenote
 *
 * @return string the base64 encoded request body
 */
string Simplenote::create_request_body(string email, string password){
    const string body = "email=" + email + "&password=" + password;

    return base64_encode(reinterpret_cast<const unsigned char*>(body.c_str()),
                        body.length());
}

/**
 * This function sets the cURL options for the object being created
 * @throw InitError if the setup fails
 */
void Simplenote::init(){
    handle = curl_easy_init();

    if(!handle){
        throw InitError("Could not initialize the cURL handler!");
    }

    bool setup = curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, err_buffer) ||
        #ifdef DEBUG
        curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L) ||
        #endif
        curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1L) ||
        curl_easy_setopt(handle, CURLOPT_POSTFIELDSIZE, -1) ||
        curl_easy_setopt(handle, CURLOPT_USERAGENT, user_agent.c_str()) ||
        curl_easy_setopt(handle, CURLOPT_MAXREDIRS, 0L) ||
        curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 1L) ||
        curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 2);

    /**
     * setup will have a value of 0 if cURL was set up successfully,
     * else it will have a different value, see: man libcurl-errors
     * for a list of error codes and why 0 represents success.
     */
    if(setup){
        throw InitError(err_buffer);
    }
}


/**
 * Tries to get the API token in order to use it for the actual note handling
 * This is in fact the authentication process as seen by the Simplenote API
 *
 * @throw InitError if a cURL setup error occurs
 * @throw FetchError if no data can be fetched from Simplenote
 * @throw AuthenticationError if after connecting to Simplenote, no token is
 * received
 *
 * @param string req_body the request body which consists of the email and
 * password base64 encode3d as returned by Simplenote::create_request_body
 */
void Simplenote::authenticate(string req_body){
    /**
     * setup will have a value of 0 if cURL was set up successfully,
     * else it will have a different value, see: man libcurl-errors
     * for a list of error codes and why 0 represents success.
     */

    bool setup = curl_easy_setopt(handle, CURLOPT_URL, login_url.c_str()) ||
        curl_easy_setopt(handle, CURLOPT_POSTFIELDS, req_body.c_str()) ||
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, get_curl_data) ||
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, &token);

    if(setup){
        throw InitError(err_buffer);
    }

    CURLcode retval = curl_easy_perform(handle);

    if(CURLE_OK != retval){
        throw FetchError(err_buffer);
    }

    if(token.empty()){
        throw AuthenticationError("Could not authenticate, check your \
            credentials and try again later!");
    }
}

/**
 * Setter method for the user-Agent header used by cURL
 *
 * @param string ua the new user agent
 */
void Simplenote::set_user_agent(string ua){
    user_agent = ua;

    CURLcode setup = curl_easy_setopt(handle, CURLOPT_USERAGENT, user_agent.c_str());

    if(!setup){
        throw InitError(err_buffer);
    }
}

void Simplenote::debug(){
}

Simplenote::~Simplenote(){
    curl_easy_cleanup(handle);
}