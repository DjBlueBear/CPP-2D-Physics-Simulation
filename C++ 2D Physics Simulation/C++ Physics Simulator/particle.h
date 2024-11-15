#include <iostream>
#include <cmath>
#include <list>
#include "line.h"

using namespace std;

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

float magnitude(float foo[2]) {
    return sqrt(pow(foo[0],2)+pow(foo[1],2));
}

void normalize(float in[2], float out[2]) {
    float mag = magnitude(in);
    out[0] = in[0] / mag;
    out[1] = in[1] / mag;
}

float distance(float a[2], float b[2]) {
    float temp[2];
    temp[0] = a[0]-b[0];
    temp[1] = a[1]-b[1];
    return magnitude(temp);
}
/*
float dot(float a[2], float b[2]) {
    return a[0]*b[0]+a[1]*b[1];
}*/



class Particle {
    private:
        inline static float coefficientOfRest = 1.0;
        inline static float timeStep = 0.01;
        inline static float gravity = 0;
        inline static float drag = 0.01;//fluid density * coeficcient of drag
        float pos[2];
        float vel[2];
        float radius;
        float mass;
        bool display;
        

        
        void collision(Particle* other) {
            if(this->vel[0]-other->vel[0] == 0 && this->vel[1]-other->vel[1] == 0) {
                return;
            }
            
            float normal[2] = {other->pos[0] - this->pos[0], other->pos[1] - this->pos[1]};
            float distance = magnitude(normal);
            normal[0] /= distance;
            normal[1] /= distance;
            float thisNormalVel = dot(this->vel,normal);
            float otherNormalVel = dot(other->vel,normal);

            float thisAccel = (this->mass*thisNormalVel+other->mass*otherNormalVel-other->mass*Particle::coefficientOfRest*(thisNormalVel-otherNormalVel))/(this->mass+other->mass)-thisNormalVel;
            float otherAccel = this->mass/other->mass*thisAccel;
            this->vel[0] += thisAccel*normal[0];
            this->vel[1] += thisAccel*normal[1];
            other->vel[0] -= otherAccel*normal[0];
            other->vel[1] -= otherAccel*normal[1];
            float move = this->radius + other->radius - distance;
            this->pos[0] -= normal[0] * move / 2;
            this->pos[1] -= normal[1] * move / 2;
            other->pos[0] += normal[0] * move / 2;
            other->pos[1] += normal[1] * move / 2;
        }
        
        void collisionLine(Line* other, float distance) {
            float normal[2];//NORMALS ARE ONE SIDED !!! FIX LATER
            other->getNormal(normal);//NORMALS ARE ONE SIDED !!! FIX LATER
            float thisNormalVel = dot(this->vel,normal);
            float silly = -thisNormalVel*Particle::coefficientOfRest*other->getCoefficientOfRest() - thisNormalVel;
            this->vel[0] += silly*normal[0];
            this->vel[1] += silly*normal[1];
            float move = this->radius - distance;
            this->pos[0] -= normal[0] * move;
            this->pos[1] -= normal[1] * move;
        }
        
    public:
        inline static list<Particle*> particleList = {};
        
        Particle(float x, float y, float xVelocity, float yVelocity, float rad, float mass, bool display) {
            if(mass <= 0) {
                cout << "Error Mass Cannot Be Negative\n";
            }
            if(rad < 0) {
                cout << "Error Radius Cannot Be Negative\n";
            }
            this->pos[0] = x;
            this->pos[1] = y;
            this->vel[0] = xVelocity;
            this->vel[1] = yVelocity;
            this->radius = rad;
            this->mass = mass;
            this->display = display;
            Particle::particleList.push_back(this);
        }
        
        bool getDisplay() {
            return this->display;
        }
        
        float getRadius() {
            return this->radius;
        }
        
        static list<Particle*> getParticleList() {
            return Particle::particleList;
        }

        string disp() {
            if(isinf(this->pos[0]) || isinf(this->pos[1])) {
                cout << "Infinite Detected\n";
            }
            if(isnan(this->pos[0]) || isnan(this->pos[1])) {
                cout << "NaN Detected\n";
            }
            
            string x = to_string(this->pos[0]);
            string y = to_string(this->pos[1]);
            x = x.substr(0,x.find(".",0)+3);
            y = y.substr(0,y.find(".",0)+3);
            
            if(x.substr(x.find(".",0),x.length()).compare(".00") == 0) {
                x = x.substr(0,x.find(".",0));
            }
            
            if(y.substr(y.find(".",0),y.length()).compare(".00") == 0) {
                y = y.substr(0,y.find(".",0));
            }
            
             return "[" + x + "," + y + "]";
            //cout << "Pos: (" << this -> pos[0] << ", " << this -> pos[1] << ") Vel: (" << this -> pos[2] << ", " << this -> pos[3] << ")\n";
        }//Have to remove zeros at end of  numbers to reduce filesize of output.txt (CodeHS wont let you run the program if any of the files are too big)
        
        void update() {
            this->vel[1] -= Particle::gravity * Particle::timeStep;

            this->vel[0] -= sgn(this->vel[0])*pow(this->vel[0],2)/2*Particle::drag * Particle::timeStep;
            this->vel[1] -= sgn(this->vel[1])*pow(this->vel[1],2)/2*Particle::drag * Particle::timeStep;
            
            this->pos[0] += this->vel[0] * Particle::timeStep;
            this->pos[1] += this->vel[1] * Particle::timeStep;
            
            for(Particle* i : Particle::particleList) {
                if(i == this) {
                    continue;
                }
                
                float distance = sqrt(pow(this->pos[0]-i->pos[0],2)+pow(this->pos[1]-i->pos[1],2));
                
                if(distance == 0) {
                    cout << "Error Distance Is 0\n";
                    continue;
                }
                
                if(distance < this->radius + i->radius) {
                    this->collision(i);
                }
            }
            
            for(Line* i : Line::getLineList()) {
                float distance = i->getSignedDistance(this->pos) - this->radius;
                
                if(distance < 0) {
                    this->collisionLine(i,distance);
                }
            }
        }
};