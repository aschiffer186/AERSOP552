#include<iostream>
using namespace std;

#include<cmath>

//This looks good
void fly(double latA, double longA, double latB, double longB) {

    if (latA<-90 || latA>90) {
        cout<<"Please correct the starting latitude"<<endl;

    } else if (latB<-90 || latB>90) {
        cout<<"Please correct the end latitude"<<endl;

    } else if (longA<-180 || longA>180) {
        cout<<"Please correct the starting longitude"<<endl;

    } else if (longB<-180 || longB>180) {
        cout<<"Please correct the end longitude"<<endl;
        
    } else {
        double R, a, d, theta;
        R=3440;
        a=sin((M_PI/180)*(latB-latA)/2)*sin((M_PI/180)*(latB-latA)/2)+cos((M_PI/180)*latA)*cos((M_PI/180)*latB)*sin((M_PI/180)*(longB-longA)/2)*sin((M_PI/180)*(longB-longA)/2);
        d=2*R*atan2(sqrt(a),sqrt(1-a));
        theta=(180/M_PI)*atan2(sin((M_PI/180)*(longB-longA))*cos((M_PI/180)*latB),cos((M_PI/180)*latA)*sin((M_PI/180)*latB)-sin((M_PI/180)*latA)*cos((M_PI/180)*latB)*cos((M_PI/180)*(longA-longB)));
        if (theta<0) {
            theta+=360;
        } else if (theta>=360) {
            theta-=360; 
        }
        cout<<"Fly heading "+to_string(theta)+" degrees for "+to_string(d)+" nautical miles"<<endl;
    }
}

int main() {
    double latA, latB, longA, longB;

    /*cout<<"Enter your starting latitude: ";
    cin>>latA;
    
    cout<<"Enter your starting longitude: ";
    cin>>longA;

    cout<<"Enter your end latitude: ";
    cin>>latB;

    cout<<"Enter your end longitude: ";
    cin>>longB;*/

    latA=42.279594;
    longA=-83.732124;
    latB=42.361145;
    longB=-71.057083;

    fly(latA,longA,latB,longB);
    return 0;
}
