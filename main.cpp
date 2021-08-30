#include <bits/stdc++.h>
#include "Data_Book.h"
using namespace std;
#define PI 3.141592
//40 1440 6 2 3 2 2
class Shaft{
public:

    string Material;
    double diameter;
    double safeStress; // units N/mm^2
    double N;
    double length;
    Shaft(){
        //take Material
        cout<<"Select Shaft Material number: \n";
        cout<<"1 for SAE1020\n";
        cout<<"2 for SAE1030\n";
        cout<<"3 for SAE1040\n";
        cout<<"4 for SAE3245\n";
        cout<<"5 for new Material(not in list)\n";
        int Shaft_type;
        cin>>Shaft_type;
        switch(Shaft_type){
            case 1:
                Material="SAE1020";
                break;
            case 2:
                Material="SAE1030";
                break;
            case 3:
                Material="SAE1040";
                break;
            case 4:
                Material="SAE3245";
                break;
            default:{
                cout<<"Enter Material Name";cin>>Material;
                cout<<"Enter So of Material";cin>>safeStress;
                add_Shaft_Strength(Material,safeStress);
            }
        }
        Default_Shaft_Strength();
        safeStress = fetch_Shaft_Strength(Material);
    }
    void Display();
    ~Shaft(){
    }

};

void Shaft::Display(){
    // teeth , module ,Dia,Material,N
    cout<<"Diameter is :"<<diameter<<" mm \n";
    cout<<"Length is :"<<length<<" m \n";
    cout<<"Material is :"<<Material<<"\n";
    cout<<"RPM :"<<N<<"\n";
}

class SpurGear{
public:
    int teeth;
    double module;
    double N;
    string Profile;
    int profileType;
    string Material;
    SpurGear* pointerToMeshGear;
    double Ft;
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
    }
    void Display();
    double Dia(){
        return module*teeth*(1.0);
    }
    ~SpurGear(){

    }
};

void SpurGear::Display(){
    // teeth , module ,Dia,Material,N
    cout<<"No.of Teeth :"<<teeth<<"\n";
    cout<<"module is :"<<module<<" mm \n";
    cout<<"Diameter is :"<<Dia()<<" mm \n";
    cout<<"Material is :"<<Material<<"\n";
    cout<<"RPM :"<<N<<"\n";
}

double calculateCv(const SpurGear& gear){
    double RPM = gear.N;
    if(RPM<1000){
        return 0.4;
    }
    else if(RPM<2000){
        return 0.3;
    }
    else{
        return 0.2;
    }
}

double CalculateY(SpurGear &g){
    int profileType=g.profileType;
    double t=g.teeth*1.0;
    double Y=0;
    switch(profileType){
        case 1:
            Y = 0.39 - (2.16/t);
            break;
        case 2:
            Y = 0.485 - (2.87/t);
            break;
        case 3:
            Y = 0.551 - (2.90/t);
            break;
    }
    return Y;
}

// solving equations
double solve(double S0,SpurGear &gear,double Power){
    double Cv;
    Cv=calculateCv(gear);
    double m;
    double Ks = 1.8 ; // steady and 24 hours continous duty
    double Yg=CalculateY(gear);
    double b=10; // taking b = 10 *m;
    double ans= (Power * Ks*6e6)/(PI*gear.teeth*gear.N*S0*Yg*Cv);
    m = pow(ans,0.333);
    return m;
}

bool isSafe(double m,double S0,double Yg,double Power,SpurGear &gear){
    double Dp = m * gear.teeth;
    double Vp = PI*Dp*gear.N/(60.0*1000);
    double Ft = Power*1.8*1000/(Vp);
    double Cv;
    // find Cv from data book from Vp
    Cv = Cv_values(Vp);
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
    C=C_values(gear.profileType);
    e=0.03;
    double Fi = (21*Vp*(C*e*b + Ft))/(21*Vp+sqrt(C*e*b+Ft));
    // Fd-> Dynamic Load
    double Fd = (Ft + Fi);
    Fb/=Cv;
    if(Fd >= Fb){
        // not safe
        return false;
    }
    double Seb=350;
    // assume Seb
    double Fe = Seb*Yg*b*m;   // endurance force
    if(Fe <= Fd){
        return false;
    }
    else{
        gear.Ft=Ft;
        gear.pointerToMeshGear->Ft=Ft;
        return true;
    }
}

void checkWeaker(SpurGear &g,SpurGear &p,double Power){
    Default_So_Data();
    //from data book
    double Sog = fetch_So_data(g.Material);
    double Sop = fetch_So_data(p.Material);

    double Yg=CalculateY(g);
    double Yp=CalculateY(p);

    if(Sog*Yg < Sop*Yp){
        //design is based on gear
        cout<<endl<<"Gear Weak"<<endl;
        double m = solve(Sog,g,Power);
        //binary search for higher m
        m = Preffered_m(m);
        while(!isSafe(m,Sog,Yg,Power,g)){
            if(m>=50) break;
            m+=0.25;
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
        while(!isSafe(m,Sop,Yp,Power,p)){
            if(m>=50) break;
            m+=0.25;
            m = Preffered_m(m);
        }
        p.module=m;
        p.pointerToMeshGear->module = m;
    }
}

void designShaft(Shaft &s,double Power,double Ft1,double Ft2){
    double l=0.6;//total length of shaft , units in m
    s.length=l;
    double N=s.N;//Speed of shaft in rotations per minute
    double leftDist=0.2; // left pulley distance from left end
    double rightDist=0.2;// Right pulley distance from left end
    //here shaft is subjected to bending and twisting (torsion)
    // shaft is designed based on bending and torsion
    // p = 2*PI*N*T/60
    double T = Power*60.0/(2*PI*N*1.0); // KNm
    double M = max(leftDist * Ft1,(l-rightDist)*Ft2); // bending moment //KNm
    double Teq = sqrt(T*T + M*M); // Equivalent twisting moment
    double dia ; // Diameter of shaft
    // Teq = (PI * Dia^3 *Tou)/16
    dia = pow(Teq * 1000 * 16 /(PI* s.safeStress),0.333);
    s.diameter=Preffered_Shaft_Dia(dia);
}

int main()
{
    double Power;
    double RPM;
    double reductionRatio;
    cout<<" enter power in KW, RPM of motor, reduction ratio\n";
    cin>>Power>>RPM>>reductionRatio;
    double VR1 = min(6.0,reductionRatio); // velocity ratio;
    SpurGear p1(RPM);
    SpurGear g1(RPM/VR1);
    g1.pointerToMeshGear = &p1;
    p1.pointerToMeshGear = &g1;
    p1.teeth = 25;
    g1.teeth = p1.teeth*VR1;
    checkWeaker(g1,p1,Power);

    if(reductionRatio>6){
        SpurGear p2(g1.N);
        p2.teeth=30;
        double VR2=reductionRatio/VR1;
        VR2=ceil(VR2);
        SpurGear g2(p2.N/VR2);
        g2.teeth = p2.teeth*VR2;
        g2.pointerToMeshGear = &p2;
        p2.pointerToMeshGear = &g2;
        checkWeaker(g2,p2,Power*0.98);
        Shaft s;
        s.N=g1.N;
        designShaft(s,Power,p1.Ft,p2.Ft);
        cout<<"pinion 2 has\n";
        p2.Display();
        cout<<"\n";
        cout<<"gear 2 has\n";
        g2.Display();
        cout<<"\n";
        cout<<"Shaft :"<<endl;
        s.Display();
        cout<<endl;
    }
    cout<<"pinion 1 has\n";
    p1.Display();
    cout<<"\n";
    cout<<"gear 1 has\n";
    g1.Display();
    cout<<"\n";

    return 0;
}

