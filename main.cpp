#include <bits/stdc++.h>
#include "Data_Book.h"
using namespace std;
#define PI 3.141592

class SpurGear{
public:
    int teeth;
    double module;
    double N;
    string Profile;
    int profileType;
    string Material;
    SpurGear* pointerToMeshGear;
    SpurGear(double RPM){
        N=RPM;
        cout<<" Select the tooth profile \n";
        cout<<" type\n";
        cout<<" 1 for 14.5 degree Full Depth  (prefferd for smooth operations)\n";
        cout<<" 2 for 20 degree Full Depth\n";
        cout<<" 3 for 20 degree stub teeth (prefferd when there is space constraint)\n";
        bool flag = true;
        while(flag){
            cin>>profileType;
                switch(profileType){
                    case 1:
                        Profile="14.5 degree Full Depth";
                        flag = false;
                        break;

                    case 2:
                        Profile="20 degree Full Depth";
                        flag = false;
                    break;
                case 3:
                    Profile="20 degree stub teeth";
                        flag = false;
                    break;
                default:
                    cout<<"please enter valid type\n";
            }
        }
        cout<<"Select Material number: \n";
        cout<<"1 for Cast_Iron_Untreated\n";
        cout<<"2 for Cast_Iron_Heat_Treated\n";
        cout<<"3 for SAE1045_Heat_Treated\n";
        cout<<"4 for SAE1050_Heat_Treated\n";
        cout<<"5 for SAE2320_Case_Hardened\n";
        cout<<"6 for SAE3245_Heat_Treated\n";
        cout<<"7 for SAE6145_Heat_Treated\n";
        cout<<"8 for new Material(not in list)\n";
        int type;
        double So;
        cin>>type;
        switch(type){
            case 1:
                Material="Cast_Iron_Untreated";
                break;
            case 2:
                Material="Cast_Iron_Heat_Treated";
                break;
            case 3:
                Material="SAE1045_Heat_Treated";
                break;
            case 4:
                Material="SAE1050_Heat_Treated";
                break;
            case 5:
                Material="SAE2320_Case_Hardened";
                break;
            case 6:
                Material="SAE3245_Heat_Treated";
                break;
            case 7:
                Material="SAE6145_Heat_Treated";
                break;
            default:{
                cout<<"Enter Material Name";cin>>Material;
                cout<<"Enter So of Material";cin>>So;
                add_So_data(Material,So);
            }
        }
        //cout<<"Hi"<<endl;
    }
    double Dia(){
        return module*teeth*(1.0);
    }
    ~SpurGear(){

    }
};

double calculateCv(const SpurGear& gear){
    double RPM = gear.N;
    if(RPM<1000){
        return 0.4;
    }
    else if(RPM<2000){
        return 0.3;
    }
    else{
        return 0.25;
    }
}

double CalculateY(SpurGear &g){
    int profileType=g.profileType;
    int t=g.teeth;
    double Y=0;
    switch(profileType){
        case 1:
            Y = 0.39 - 2.16/t;
            break;
        case 2:
            Y = 0.485 - 2.87/t;
            break;
        case 3:
            Y = 0.551 - 2.90/t;
            break;
    }
    return Y;
}

// solving equations

double solve(double S0,SpurGear &gear,double Power){
    double Cv;
    Cv=calculateCv(gear);
    double m;
    double Ks = 1.25 ; // steady and 24 hours continous duty
    double Yg=CalculateY(gear);
    double b=10; // taking b = 10 *m;
    double ans= (Power * Ks)/(PI*gear.teeth*gear.N*S0*Yg*b);
    m = pow(ans,0.333);
    return m;
}


bool isSafe(double m,double S0,double Yg,double Power,SpurGear &gear){
    double Dp = m * gear.teeth;
    double Vp = PI*Dp*gear.N/(60.0);
    double Ft = Power*1.25/(Vp);
    double Cv;
    // find Cv from data book from Vp
    double b = Ft /(S0*Yg*m*Cv);
    if(b>=8.5*m && b<=12.5*m){
        b=ceil(b);
    }
    else if(b<8.5*m){
        b=ceil(8.5*m);
    }
    else{
        b=floor(12.5*m);
    }
    double Fb = (S0*Yg*m*b*Cv);
    double C,e;
    //get C & e from data book
    double Fi = (21*Vp*(C*e*b + Ft*1000))/(21*Vp+sqrt(C*e*b+Ft*1000));
    double Fd = (Ft + Fi);
    if(Fd >= Fb){
        // not safe
        return false;
    }
    double Seb;
    // assume Seb
    double Fe = Seb*Yg*b*m;   // endurance force
    if(Fe <= Fd){
        return false;
    }
    else{
        return true;
    }
}

void checkWeaker(SpurGear &g,SpurGear &p,double Power){

    //from data book
    double Sog = fetch_So_data(g.Material);
    double Sop = fetch_So_data(p.Material);


    double Yg=CalculateY(g);
    double Yp=CalculateY(p);

    if(Sog*Yg < Sop*Yp){
        //design is based on gear
        double m = solve(Sog,g,Power);
        //binary search for higher m
        m = Preffered_m(m);
        while(!isSafe(m,Sog,Yg,Power,g)){
            m = Preffered_m(m);
        }
        g.module=m;
        g.pointerToMeshGear->module = m;
    }
    else{
        //design is based on pinion
        double m =solve(Sop,p,Power);
        //binary search for higher m
        m = Preffered_m(m);
        while(!isSafe(m,Sop,Yg,Power,p)){
            m = Preffered_m(m);
        }
        p.module=m;
        p.pointerToMeshGear->module = m;
    }
}

int main()
{

    double Power;

    double RPM;

    double reductionRatio;

    cout<<" enter power in KW, RPM of motor, reduction ratio\n";

    cin>>Power>>RPM>>reductionRatio;

    double VR1 = 6; // velocity ratio;

    SpurGear p1(RPM);

    SpurGear g1(RPM/VR1);

    g1.pointerToMeshGear = &p1;

    p1.pointerToMeshGear = &g1;

    p1.teeth = 25;

    g1.teeth = p1.teeth*VR1;

    checkWeaker(g1,p1,Power);

    SpurGear p2(RPM/VR1);

    p2.teeth=30;

    //SpurGear g2();

    return 0;
}

