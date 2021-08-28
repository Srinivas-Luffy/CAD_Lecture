//
// Created by Asus on 8/28/2021.
//

double CalculateY(int profileType,int &t){
    int Y=0;
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

}