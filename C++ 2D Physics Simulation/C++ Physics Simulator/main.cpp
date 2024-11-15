#include <iostream>
#include <chrono>
#include <fstream>
#include "particle.h"
#include <random>

using namespace std;

#define DURATION 800

int main()
{
    for(float y=-4; y<4; y++) {
        for(float x=-4; x<4; x++) {
            new Particle(x,y,0,0,0.4,1,true);
        }
    }
    
    new Particle(-20,0,20,0,1,14,true);
    new Line(-50,-10,50,-10,1,1);
    
    string outList[Particle::getParticleList().size()];
    
    for(int i=0; i<DURATION; i++) {
        int j=0;
        for(Particle* part : Particle::getParticleList()) {
            part->update();
            if(part->getDisplay()) {
            if(i==0) {
                outList[j] += "[";
                outList[j] += "[" + to_string(part->getRadius()) + ",0],";
            }
            

            outList[j] += part->disp();
            if(i < DURATION-1) {
                outList[j] += ",";
            } else {
                outList[j] += "]";
            }
            }
            j++;
        }
    }
    
    /*ifstream checkFile;
    checkFile.open("output.txt");
    
    if(checkFile.is_open()) {
        cout << "Deleting Duplicate File\n";
        checkFile.close();
        remove("output.txt");
    }*/
    
    ofstream file;
    file.open("output.txt", ios::trunc | ios::out);
    
    for(string i : outList) {
        if(!i.compare("")==0) {
            file << i << "\n";
        }
    }
    
    file.close();
    cout << "Finished";
}