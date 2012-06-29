#ifndef SIMPLENOTE_H_GUARD
#define SIMPLENOTE_H_GUARD

#include <string>

/**
 * The main SimpleNote class
 */
class Simplenote{
    private:
        std::string token, _email, _password;
    public:
        Simplenote(const char *email, const char *password);
        void debug();
};

#endif