#include <list>
#include <cmath>

using namespace std;

float dot(float a[2], float b[2]) {
    return a[0]*b[0]+a[1]*b[1];
}

float clamp(float n, float lower, float upper) {
  return std::max(lower, std::min(n, upper));
}

class Line{
    private:
        inline static std::list<Line*> lineList = {};
        float pos_0[2];
        float pos_1[2];
        float coefficientOfRest;
        float normal[2];
        float radius;
    public:
        Line(float x0, float y0, float x1, float y1,float radius, float rest) {
            this->pos_0[0] = x0;
            this->pos_0[1] = y0;
            this->pos_1[0] = x1;
            this->pos_1[1] = y1;
            this->radius = radius;
            
            this->normal[0] = y0-y1;
            this->normal[1] = x1-x0;
            
            float mag = sqrt(normal[0]*normal[0]+normal[1]*normal[1]);
            
            this->normal[0] /= mag;
            this->normal[1] /= mag;//NORMALS ARE ONE SIDED !!! FIX LATER
            
            this->coefficientOfRest = rest;
            Line::lineList.push_back(this);
        }
        
        static list<Line*> getLineList() {
            return Line::lineList;
        }
        
        void getNormal(float out[2]) {//NORMALS ARE ONE SIDED !!! FIX LATER
            out[0] = this->normal[0];
            out[1] = this->normal[1];
        }
        
        float getCoefficientOfRest() {
            return this->coefficientOfRest;
        }
        
        float getSignedDistance(float p[2]) {
                float pa[2];
                pa[0] = p[0] -this->pos_0[0];
                pa[1] = p[1] -this->pos_0[1];
                float ba[2];
                ba[0] =this->pos_1[0] -this->pos_0[0];
                ba[1] =this->pos_1[1] -this->pos_0[1];
                float h = clamp( dot(pa,ba)/dot(ba,ba),0,1);
                float out[2];
                out[0] = pa[0] - ba[0]*h;
                out[1] = pa[1] - ba[1]*h;
                return sqrt(out[0]*out[0]+out[1]*out[1]) - this->radius;
            }//code adapted from GLSL to c++ (original from Inigo Quilez)
};