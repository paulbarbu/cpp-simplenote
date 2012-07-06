#include "includes/note.hpp"
#include "includes/error.hpp"
#include "jsoncpp/json.h"

#include <string>
#include <vector>

using std::string;
using std::vector;

/*
 * TODO get_json
 * this is already an obj, so I can access key, contents, deleted, etc
 *
 * overload for individual arguments
 *
 * default ctor?
 */

Note::Note(string json_str){
    Json::Value root;
    Json::Reader reader;

    bool parsing_successful = reader.parse(json_str, root);

    if (!parsing_successful){
        string error = "The note could not be parsed: " +
            reader.getFormatedErrorMessages();
        throw ParseError(error.c_str());
    }

    if(root.isMember("key") && root["key"].isString()){
        key = root["key"].asString();
    }

    if(root.isMember("deleted") && root["deleted"].isInt()){
        deleted = root["deleted"].asBool();
    }

    if(root.isMember("modifydate") && root["modifydate"].isString()){
        modifydate = root["modifydate"].asString();
    }
    
    if(root.isMember("createdate") && root["createdate"].isString()){
        createdate = root["createdate"].asString();
    }

    if(root.isMember("syncnum") && root["syncnum"].isInt()){
        syncnum = root["syncnum"].asInt();
    }

    if(root.isMember("version") && root["version"].isInt()){
        version = root["version"].asInt();
    }

    if(root.isMember("minversion") && root["minversion"].isInt()){
        minversion = root["minversion"].asInt();
    }

    if(root.isMember("sharekey") && root["sharekey"].isString()){
        sharekey = root["sharekey"].asString();
    }

    if(root.isMember("publishkey")  && root["publishkey"].isString()){
        publishkey = root["publishkey"].asString();
    }

    if(root.isMember("systemtags") && root["systemtags"].isArray()){
        Json::Value t = root["systemtags"];

        for (unsigned int i=0; i < t.size(); i++){
            systemtags.insert(t[i].asString());
        }
    }

    if(root.isMember("tags") && root["tags"].isArray()){
        Json::Value t = root["tags"];
        
        for (unsigned int i=0; i < t.size(); i++){
            tags.insert(t[i].asString());
        }
    }

    if(root.isMember("content") && root["content"].isString()){
        content = root["content"].asString();
    }
}