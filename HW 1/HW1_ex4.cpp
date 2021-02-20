#include<iostream>
using namespace std;
#include<fstream>
#include<cstring>

//Note, it's also an error if pilot weight or fuel = 0; you only check if things are < 0
//You need to consider the fact that between cg = 35 and cg = 38.5, the maximum weight isn't 
//constnat, it's a function of cg. 

//On Piazza he said not to use string.


int preflight() {     
    ifstream file("payload.dat");
    double weight = 1431;
    double center = 36.21; //This is the lever arm of the airfram 
    int num_rows = 7;
    //instead of suing an array of strings you'll need to do something like 
    //double pilot;
    //file >> pilot;
    //double front_pax;
    //file >> front_pax;
    //... for all of the weights
    
    //You'll end up getting rid of this loop
    //You'll need to maually add up the weights
    //double weight =  pilot + front_pax + ...
    string payload[num_rows];
    for (int row = 0; row < num_rows; ++row) { 
        getline(file, payload[row]);
        if (stod(payload[row])<0) {
            cout<<"Data error (weight or load < 0)"<<endl;
            return -1;
        }
        weight+=stod(payload[row]);
    }

    //You'll also need to check each number individually 
    //if (pilot <= 0) print an error; return -1;
    //if (front_pax < 0) print an error; return -1;
    //... 

    weight+=stod(payload[6])*5; //converts gal into lbs (*5 because 1* already added in the for loop)
    //This will also become something like
    //double balance = (pilot + front_pax)*37 + ... for the appropriate weights and lever arms
    double balance = (stod(payload[0])+stod(payload[1]))*37+(stod(payload[2])+stod(payload[3]))*73+stod(payload[4])*95+stod(payload[5])*123+stod(payload[6])*6*48;
    center+=balance/weight; //I think this ends up working out, but send me your final code so I can double check it.
    if (center < 34 || center > 48){
        cout<<"You cannot fly because of the center of gravity"<<endl;
        cout<<"Center of gravity = "+to_string(center)+" inches"<<endl;
        return 0;

    } else if (weight < 1500 || weight > 2400) {
        cout<<"You cannot fly because of the weight"<<endl;
        cout<<"Overall aircraft weight = "+to_string(weight)+" lbs"<<endl;
        return 0;
    }/*
        need something here thats like 
        else if (center >= 35 && cg <= 38.5)
        {
            if (weight > weight_max(cg)) <-- note in this comment weight_max(cg) refers to a mathematical function 
                                             that you need to determine by looking at the c.g. chart (it should be a line).
                                             In code this will look like weight > a*(cg-35) + b where you need to determine a and b
                                             from the char.
            {
                print an error
                return 0;
            } 
        }
    */ 
    else {
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