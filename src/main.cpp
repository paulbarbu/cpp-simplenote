#include "includes/simplenote.hpp"
#include "includes/note.hpp"

#include <iostream>

using namespace std;

int main(){
    Simplenote a("", "");

    set<string> tags = {"abd", "foo"};
    Note foo, x;
    foo.content = "notecontent";
    foo.tags = tags;

    x = a.create_note(foo);

    cout<<"\nDeleted: "<<x.deleted;
    set<string>::iterator i;

    for(i=x.tags.begin(); i!=x.tags.end(); i++){
        cout<<*i;
    }
    for(i=x.systemtags.begin(); i!=x.systemtags.end(); i++){
        cout<<*i;
    }
}
