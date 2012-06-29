/**
 * @file
 *
 * @author Barbu Paul - Gheorghe
 */
#include "includes/simplenote.hpp"

#include <iostream>
#include <string>

#include <curl/curl.h>

using std::string;
using std::cout; //TODO: remove

/**
 * TODO:
 * check how to compile a library
 */

/**
 * Explicit constructor for SimpleNote
 *
 * @param char* email the user's Simplenote email
 * @param char* password the user's password for Simplenote
 */
Simplenote::Simplenote(const char *email, const char *password){
    _email = email;
    _password = password;
}

/**
 * Temporary method
 */
void Simplenote::debug(){
    cout<<"Email: "<<_email<<"\nPassword: "<<_password<<"\n";
}