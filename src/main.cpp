#include "includes/simplenote.hpp"
#include "includes/note.hpp"
#include "includes/error.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int main(){
    ifstream credentials(".credentials.in");

    string email, pass, note_key;

    getline(credentials, email);
    getline(credentials, pass);

//     try{
        Simplenote a(email, pass);
//     }
//     catch(Error& e){
//         cout<<"1The exception was:"<<e.what();
//     }
    
    set<string> tags = {"abcd", "fcoo"};
    Note foo, x, y;
    foo.content = "notecontent-afterGETdoPOST";
    foo.tags = tags;

    //x = a.create_note(foo);

    /*int variable;
    cout<<"\n\nWAITING!!!\n\n";
    cin>>variable;*/

    try{

        getline(credentials, note_key);
        x = a.get_note(note_key);
    }
    catch(Error& e){
        cout<<"The exception was:"<<e.what();
    }

    
    //x = a.create_note(foo);

    cout<<"\n"<<x.get_json()<<"\n";
    y = x;

    y.content = y.content + "zz";

    cout<<"new:"<<y.get_json();


    x = a.update(y);
    
    cout<<"\nDeleted: "<<x.deleted;
    set<string>::iterator i;

    for(i=x.tags.begin(); i!=x.tags.end(); i++){
        cout<<*i;
    }
    for(i=x.systemtags.begin(); i!=x.systemtags.end(); i++){
        cout<<*i;
    }

    credentials.close();
}
