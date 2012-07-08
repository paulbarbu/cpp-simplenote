#include "includes/note.hpp"
#include "includes/error.hpp"
#include "jsoncpp/json.h"

#include <string>
#include <set>
#include <sstream>
#include <ctime>

using std::string;
using std::set;
using std::stringstream;

//TODO REMOVE
#include <iostream>
using std::cout;

/**
 * Note constructor
 *
 * @throw ParseError if the json_str cannot be parsed by jsoncpp
 * 
 * @param string json_str the JSON string from which the Note members will be
 * populated
 */
Note::Note(const string& json_str){
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
        stringstream _mdate;
        _mdate<<root["modifydate"].asString();
        
        _mdate>>modifydate;
    }
    else{
        modifydate = std::time(NULL);
    }
    
    if(root.isMember("createdate") && root["createdate"].isString()){
        stringstream _cdate;
        _cdate<<root["createdate"].asString();

        _cdate>>createdate;
    }
    else{
        createdate = modifydate;
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

/**
 * Overloaded Note constructor
 *
 * @param string content the note's text body
 * @param set<string> tags a set of user defined tags, if these are provided
 * the user may filter the notes in the Simplenote web interface by tags
 * @param bool pinned if set to true the note will appear at the top in the web
 * interface of Simplenote
 * @param bool markdown if the text body of the note is written in Markdown and
 * this parameter is true then the note will be displayed formatted in the web
 * interface
 * @param bool list if this is true the note will appear as a to do list for
 * the premium users of the iOS app
 *
Note::Note(const string& content, const set<string>& tags, bool pinned, bool markdown,
           bool list){

    Json::Value note, _tags, _systemtags;

    note["content"] = content;

    if(tags.size()){
        set<string>::iterator i;

        for(i=tags.begin(); i != tags.end(); i++){
            _tags.append(*i);
        }

        note["tags"] = _tags;
    }

    if(pinned){
        _systemtags.append("pinned");
    }

    if(markdown){
        _systemtags.append("markdown");
    }

    if(list){
        _systemtags.append("list");
    }

    if(_systemtags.size()){
        note["systemtags"] = _systemtags;
    }
}
*/

/**
 * Get the JSON string for the Note object
 *
 * @param bool pub if this parameter is true then only the parts of the
 * note that the user or the program can change are used
 * (those set by the Simplenote servers are excluded)
 *
 * @return string the JSON string created using the values of members of the
 * Note object
 */
string Note::get_json(bool pub) const {
    Json::Value note;
    Json::FastWriter writer;
    set<string>::iterator i;

    if(!pub && !key.empty()){
        note["key"] = key;
    }

    if(deleted){
        note["deleted"] = 1;
    }
    else{
        note["deleted"] = 0;
    }

    if(-1 != modifydate){
        stringstream _mdate;
        
        _mdate<<modifydate;
        note["modifydate"] = _mdate.str();
    }

    if(-1 != createdate){
        stringstream _cdate;
        
        _cdate<<createdate;
        note["createdate"] = _cdate.str();
    }
    
    if(!pub && -1 != syncnum){
        note["syncnum"] = syncnum;
    }

    if(!pub && -1 != version){
        note["version"] = version;
    }

    if(!pub && -1 != minversion){
        note["minversion"] = minversion;
    }

    if(!pub && !sharekey.empty()){
        note["sharekey"] = sharekey;
    }
    
    if(!pub && !publishkey.empty()){
        note["publishkey"] = publishkey;
    }

    if(systemtags.size()){
        Json::Value _systemtags;

        for(i=systemtags.begin(); i != systemtags.end(); i++){
            _systemtags.append(*i);
        }
        
        note["systemtags"] = _systemtags;
    }

    if(tags.size()){
        Json::Value _tags;

        for(i=tags.begin(); i != tags.end(); i++){
            _tags.append(*i);
        }
        
        note["tags"] = _tags;
    }
    
    note["content"] = content;

    return writer.write(note);
}

string Note::get_key() const {
    return key;
}