#ifndef SIMPLENOTE_H_GUARD
#define SIMPLENOTE_H_GUARD

#include <curl/curl.h>

#include <string>
#include <exception>

/**
 * The main SimpleNote class
 */
class Simplenote{
    private:
        CURL *handle;
        
        char *err_buffer;
        
        std::string token, user_agent = "c++api_for_simplenote",
            login_url = "https://simple-note.appspot.com/api/login",
            data_url = "https://simple-note.appspot.com/api2/data?auth=",
            index_url = "https://simple-note.appspot.com/api2/index?auth=";
        
        std::string create_request_body(std::string email, std::string password);
        
        void init();
        void authenticate(std::string req_body);
    public:
        Simplenote(const char *email, const char *password);
        void debug();
        void set_user_agent(std::string ua);
        ~Simplenote();
};

class SimplenoteError : public std::exception{
    protected:
        std::string msg;
    public:
        SimplenoteError(){
            msg = "An unknown error occurred!";
        }

        SimplenoteError(const char *user_msg){
            msg = user_msg;
        }
        
        virtual const char *what(){
            return msg.c_str();
        }
};

class InitError : public SimplenoteError{
    public:
        InitError(){
            msg = "An error occurred during initialization!";
        }
        
        InitError(const char *user_msg) : SimplenoteError(user_msg){}
};

class FetchError : public SimplenoteError{
    public:
        FetchError(){
            msg = "An error occurred while fetching data!";
        }

        FetchError(const char *user_msg) : SimplenoteError(user_msg){}
};

class AuthenticationError : public SimplenoteError{
    public:
        AuthenticationError(){
            msg = "An error occurred while fetching data!";
        }

        AuthenticationError(const char *user_msg) : SimplenoteError(user_msg){}
};

#endif