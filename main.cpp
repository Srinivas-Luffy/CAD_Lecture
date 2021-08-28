#include <iostream>
#include"Data_Book.h"
using namespace std;
int main() {

    cout<<endl<<fetch_data("iron");

    add_data("Brass",100);
    cout<<endl<<fetch_data("Brass");

    return 0;
}

