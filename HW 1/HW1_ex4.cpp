#include<iostream>
using namespace std;
#include<fstream>
#include<cstring>

int preflight() {     
    ifstream file("payload.dat");
    double weight = 1431;
    double center = 36.21;
    int num_rows = 7;
    string payload[num_rows];
    for (int row = 0; row < num_rows; ++row) { 
        getline(file, payload[row]);
        if (stod(payload[row])<0) {
            cout<<"Data error (weight or load < 0)"<<endl;
            return -1;
        }
        weight+=stod(payload[row]);
    }
    weight+=stod(payload[6])*5; //converts gal into lbs (*5 because 1* already added in the for loop)
    double balance = (stod(payload[0])+stod(payload[1]))*37+(stod(payload[2])+stod(payload[3]))*73+stod(payload[4])*95+stod(payload[5])*123+stod(payload[6])*6*48;
    center+=balance/weight;
    //conditions
    if (center < 34 || center > 48){
        cout<<"You cannot fly because of the center of gravity"<<endl;
        cout<<"Center of gravity = "+to_string(center)+" inches"<<endl;
        return 0;

    } else if (weight < 1500 || weight > 2400) {
        cout<<"You cannot fly because of the weight"<<endl;
        cout<<"Overall aircraft weight = "+to_string(weight)+" lbs"<<endl;
        return 0;

    } else {
        cout<<"This plane is ready to fly"<<endl;
        cout<<"Center of gravity = "+to_string(center)+" inches"<<endl;
        cout<<"Overall aircraft weight = "+to_string(weight)+" lbs"<<endl;
        cout<<"Moment = "+to_string(balance)+" lbs.inch"<<endl;
        return 1;
    }
}

int main() {
    preflight();
    return 0;
}