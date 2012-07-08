#ifndef ERROR_H_GUARD
#define ERROR_H_GUARD

#include <exception>
#include <cstring>

class Error : public std::exception{
    protected:
        std::string msg;
    public:
        Error(){
            msg = "An unknown error occurred!";
        }

        Error(const char *user_msg){
            if(user_msg && strlen(user_msg)){
                msg = user_msg;
            }
        }

        virtual const char *what(){
            return msg.c_str();
        }
};

class InitError : public Error{
    public:
        InitError(){
            msg = "An error occurred during initialization!";
        }

        InitError(const char *user_msg) : Error(user_msg){}
};

class FetchError : public Error{
    public:
        FetchError(){
            msg = "An error occurred while fetching data"
                ", it's worth to check your network connection!";
        }

        FetchError(const char *user_msg) : Error(user_msg){}
};

class AuthenticationError : public Error{
    public:
        AuthenticationError(){
            msg = "An error occurred while authenticating!";
        }

        AuthenticationError(const char *user_msg) : Error(user_msg){}
};

class CreateError : public Error{
    public:
        CreateError(){
            msg = "An error occurred while creating the note!";
        }

        CreateError(const char *user_msg) : Error(user_msg){}
};

class UpdateError : public Error{
    public:
        UpdateError(){
            msg = "An update error has occurred!";
        }

        UpdateError(const char *user_msg) : Error(user_msg){}
};

class ParseError : public Error{
    public:
        ParseError(){
            msg = "A parse error has occurred!";
        }

        ParseError(const char *user_msg) : Error(user_msg){}
};

#endif