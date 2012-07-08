#ifndef NOTE_H_GUARD
#define NOTE_H_GUARD

#include <string>
#include <set>
#include <ctime>

class Note{
    private:
        std::string key, sharekey, publishkey, content;
        int syncnum = -1, version = -1, minversion = -1;
        std::time_t modifydate = -1, createdate = -1;
        bool deleted=false;
        std::set<std::string> systemtags, tags;
    public:

        Note(){}
        Note(const std::string& json_str);

        std::string get_key() const;
        void set_content(const std::string& c){
            modifydate = std::time(NULL);
            content = c;
        }

        std::string get_content() const {
            return content;
        }

        void trash(){
            modifydate = std::time(NULL);
            deleted = true;
        }

        void untrash(){
            modifydate = std::time(NULL);
            deleted = false;
        }

        void set_systemtags(std::set<std::string> systags){
            modifydate = std::time(NULL);
            systemtags = systags;
        }
        
        std::set<std::string> get_systemtags() const {
            return systemtags;
        }

        void set_tags(std::set<std::string> t){
            modifydate = std::time(NULL);
            tags = t;
        }

        std::set<std::string> get_tags() const {
            return tags;
        }

        std::string get_json(bool pub=false) const;
        /*
         * TODO fix this, think of something to create a note directly with properties
         * maybe a separate method: set_properties with this signature
        Note(const std::string& content,
             const std::set<std::string>& tags=std::set<std::string>(),
             bool pinned=false, bool markdown=false, bool list=false);
             */
};

#endif