#ifndef CAD_LECTURE_DATA_BOOK_H
#define CAD_LECTURE_DATA_BOOK_H
#include<bits/stdc++.h>
using namespace std;

// So Values start
map<string, int> So_values;
void Default_So_Data(){
    So_values["Cast_Iron_Untreated"] = 140;
    So_values["Cast_Iron_Heat_Treated"] = 196;
    So_values["SAE1045_Heat_Treated"] = 210;
    So_values["SAE1050_Heat_Treated"] = 245;
    So_values["SAE2320_Case_Hardened"] = 350;
    So_values["SAE3245_Heat_Treated"] = 456;
    So_values["SAE6145_Heat_Treated"] = 473;
    So_values["Ph_Bronze_SAE65"] = 84;
}
void add_So_data(string Key, int val){
    So_values.insert(pair<string, int>(Key, val));
}
int fetch_So_data(string Key){
    return So_values[Key];
}
//So values end

//module values start
double module[18] = {1, 1.25, 1.5, 2, 2.5, 3, 4, 5, 6, 8, 10, 12, 16, 20, 25, 32, 40, 50};
double Preffered_m(double m){
    int l=0,mid,r=17;
    double ans=50;
    while(l<=r){
        mid = (l+r)/2;
        if(module[mid]>m){
            ans = module[mid];
            r = mid-1;
        }
        else{
            l=mid+1;
        }
    }
return ans;
}
//module values end

#endif //CAD_LECTURE_DATA_BOOK_H
