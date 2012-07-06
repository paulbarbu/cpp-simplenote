/**
 * @file
 *
 * @author Barbu Paul - Gheorghe
 */
#include "includes/simplenote.hpp"
#include "includes/error.hpp"
#include "includes/note.hpp"
#include "includes/helpers.hpp"
#include "includes/base64.h"
#include "jsoncpp/json.h" //TODO remove

#include <curl/curl.h>

#include <string>
#include <set>

using std::string;
using std::set;

/**
 * TODO:
 * add tests
 *
 * add docs
 */

//TODO: remove these
#include <iostream>
using std::cout;

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

    string email_query_str = "&email=" + (string)email;

    //because the token is set at this point we can modify the urls to contain
    //the token, same for the email
    data_url += token + email_query_str;
    index_url += token + email_query_str;
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
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, get_curl_string_data) ||
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
 * Setter method for the User-Agent header used by cURL
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

/**
 * Create a note
 * TODO modify this to take a Note object
 * update the docs
 *
 * Create the note object by putting together the necessary JSON, which is sent
 * to Simplenote for processing, the actual note creation takes place at Simplenote
 * 
 * @throw InitError if a cURL setup error occurs
 * @throw
 * 
 * @param string content the note's text body
 * @param set<string> tags a set of user defined tags, if these are provided
 * the user may filter the notes in the Simplenote web interface by tags
 * @param bool pinned if set to tre the note will appear at the top in the web
 * interface of Simplenote
 * @param bool markdown if the text body of the note is written in Markdown and
 * this parameter is true then the note will be displayed formatted in the web
 * interface
 * @param bool list if this is true the note will appear as a to do list for
 * the premium users
 *
 * @return Note the newly created note
 */
Note Simplenote::create_note(string content, set<string> tags,
                   bool pinned, bool markdown, bool list){
    string json_response;
    // TODO: maybe I could abstract away the Json parts into the Note object?
    Json::Value note, user_tags, system_tags;

    note["content"] = content;
    
    if(tags.size()){
        set<string>::iterator i;
        
        for(i=tags.begin(); i != tags.end(); i++){
            user_tags.append(*i);
        }
        
        note["tags"] = user_tags;
    }

    if(pinned){
        system_tags.append("pinned");
    }

    if(markdown){
        system_tags.append("markdown");
    }

    if(list){
        system_tags.append("list");
    }

    if(system_tags.size()){
        note["systemtags"] = system_tags;
    }

    Json::FastWriter writer;
    string req_body = writer.write(note);
    
    bool setup = curl_easy_setopt(handle, CURLOPT_URL, data_url.c_str()) ||
        curl_easy_setopt(handle, CURLOPT_POSTFIELDS, req_body.c_str()) ||
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, get_curl_string_data) ||
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, &json_response);

    if(setup){
        throw InitError(err_buffer);
    }
    
    CURLcode retval = curl_easy_perform(handle);

    if(CURLE_OK != retval){
        throw FetchError(err_buffer);
    }

    if (json_response.empty()){
        throw CreateError("Check your parameters, no note created!");
    }

    cout<<json_response; // FIXME REMOVE

    Note new_note(json_response);
    
    return new_note;
}

void Simplenote::debug(){
}

Simplenote::~Simplenote(){
    curl_easy_cleanup(handle);
}