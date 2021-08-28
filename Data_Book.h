#ifndef CAD_LECTURE_DATA_BOOK_H
#define CAD_LECTURE_DATA_BOOK_H
#include<bits/stdc++.h>
using namespace std;

map<string, int> s_values;

void add_data(string Key, int val){
    s_values.insert(pair<string, int>(Key, val));
}

int fetch_data(string Key){
    cout<<endl<<"Fetching "<<Key<<"Strength: ";
    return s_values[Key];
}


#endif //CAD_LECTURE_DATA_BOOK_H
