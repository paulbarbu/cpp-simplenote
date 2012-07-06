#ifndef NOTE_H_GUARD
#define NOTE_H_GUARD

#include <string>
#include <set>

class Note{
    private:
        std::string jsonify(); // TODO
    public:
        Note(std::string json_str);
        std::string key, modifydate, createdate, sharekey, publishkey, content;
        bool deleted;
        int syncnum, version, minversion;
        std::set<std::string> systemtags, tags;
};

#endif