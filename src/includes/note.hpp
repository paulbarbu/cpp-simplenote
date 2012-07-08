#ifndef NOTE_H_GUARD
#define NOTE_H_GUARD

#include <string>
#include <set>

class Note{
    private:
        std::string key, modifydate, createdate, sharekey, publishkey;
        int syncnum = -1, version = -1, minversion = -1;
    public:
        std::string content;
        bool deleted=false;
        std::set<std::string> systemtags, tags;

        Note(){}
        Note(const std::string& json_str);

        std::string get_key() const;

        std::string get_json() const;
        /*
         * TODO fix this, think of something to create a note directly with properties
         * maybe a separate method: set_properties with this signature
        Note(const std::string& content,
             const std::set<std::string>& tags=std::set<std::string>(),
             bool pinned=false, bool markdown=false, bool list=false);
             */
};

#endif