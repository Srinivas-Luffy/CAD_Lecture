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

//Cv values start
double Cv_values(double Vp){
    double Cv=0;
    if(Vp<=5){
        //class 1, grade: 8 - 9
        return 3/(3+Vp);
    }
    else if(Vp<=7.5){
        //class 1, grade: 7 - 8
        return 3/(3+Vp);
    }
    else if(Vp<=15){
        //class 2 grade: 5 - 6
        return 4.5/(4.5+Vp);
    }
    else if(Vp<=20){
        //class 3, grade: 4 - 5
        return 6/(6+Vp);
    }
    else{
        //Lapped gear, grade: 7 - 8
        return 5.5/(5.5+pow(Vp,0.5));
    }
}
//Cv values end

//C values start
int C_values(int profileType){

    switch(profileType){
        case 1:{
            return 11500;
        }
        case 2:{
            return 11800;
        }
        case 3:{
            return 12300;
        }
    }

   // return Cv;
}

//C values end


#endif //CAD_LECTURE_DATA_BOOK_H
