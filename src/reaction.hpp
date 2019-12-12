#include "ofMain.h"

class reaction : public ofNode {
    struct fluid {
        double a;
        double b;
    };
    
    int size;
    int w_n;
    int h_n;
    
    double d_a;
    double d_b;
    double feed;
    double k;
    
    public:
        reaction();
        void init();
        void customDraw();
        void diffuse();
        void addSubstance(int x, int y);
    
    protected:
        int ix(int i, int j);
        double convolute(int i, int j, char s);
        vector<fluid> state;
};
