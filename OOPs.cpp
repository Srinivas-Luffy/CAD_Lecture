#include<bits/stdc++.h>
using namespace std;

class student{
public:
    string name;
    int age;
    bool gender;
    void printInfo(){
        cout<<this->name<<" "<<this->age<<" "<<this->gender;

    }

};

int main(){
    student arr[3];
    //for(int i=0;i<3;i++){
        cin>>arr[0].name;
        cin>>arr[0].age;
        cin>>arr[0].gender;
       // arr[i].printInfo();
    //}
    arr[1](arr[0]);
    cout<<arr[1].name<<endl;

    return 0;
}

