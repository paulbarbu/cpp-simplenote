#ifndef ERROR_H_GUARD
#define ERROR_H_GUARD

class Error : public std::exception{
    protected:
        std::string msg;
    public:
        Error(){
            msg = "An unknown error occurred!";
        }

        Error(const char *user_msg){
            msg = user_msg;
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
            msg = "An error occurred while fetching data!";
        }

        FetchError(const char *user_msg) : Error(user_msg){}
};

class AuthenticationError : public Error{
    public:
        AuthenticationError(){
            msg = "An error occurred while fetching data!";
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

class ParseError : public Error{
    public:
        ParseError(){
            msg = "An error occurred while parsing the note!";
        }

        ParseError(const char *user_msg) : Error(user_msg){}
};

#endif