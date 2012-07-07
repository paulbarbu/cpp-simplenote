#ifndef SIMPLENOTE_H_GUARD
#define SIMPLENOTE_H_GUARD

#include "note.hpp"

#include <curl/curl.h>

#include <string>
#include <set>

/**
 * The main Simplenote class
 */
class Simplenote{
    private:
        CURL *handle;
        
        char *err_buffer;
        
        std::string token, user_agent = "c++api_for_simplenote",
            login_url = "https://simple-note.appspot.com/api/login",
            data_url = "https://simple-note.appspot.com/api2/data?auth=",
            index_url = "https://simple-note.appspot.com/api2/index?auth=";
        
        std::string create_request_body(const std::string& email,
                                        const std::string& password);
        
        void init();
        void authenticate(const std::string& req_body);
    public:
        Simplenote(const std::string& email, const std::string& password);
        void debug();
        void set_user_agent(const std::string& ua);
        Note create_note(Note n);
        ~Simplenote();
};

#endif