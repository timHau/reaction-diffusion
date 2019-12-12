//
//  reaction.cpp
//  react_diffuse
//
//  Created by tim on 04.10.19.
//

#include "reaction.hpp"

reaction::reaction() {
    d_a = 1.1;
    d_b = 0.3;
    feed = 0.067;
    k = 0.066;
}

void reaction::init() {
    size = 3;
    w_n = int(ofGetWidth() / size);
    h_n = int(ofGetHeight() / size);
    
    for (unsigned int i = 0; i < w_n * h_n; ++i) {
        fluid f;
        f.a = 0.0;
        f.b = 0.0;
        state.push_back(f);
    }
    
    int startDrops = 0;
    int r = 10;
    for (unsigned int k = 0; k < startDrops; ++k) {
        int x = ofRandom(w_n);
        int y = ofRandom(h_n);
        
        for (unsigned int i = x - r; i < x + r; ++i) {
            for (unsigned int j = y - r; j < y + r; ++j) {
                state[ix(i, j)].b = 1.0;
            }
        }
    }
   
}

void reaction::addSubstance(int x, int y) {
    int i = int(x / size);
    int j = int(y / size);
    state[ix(i,j)].b = 1.0;
}

void reaction::customDraw() {
    for (unsigned int i = 0; i < w_n; ++i) {
        for (unsigned int j = 0; j < h_n; ++j) {
            fluid current = state[ix(i,j)];
            int c = std::floor((current.a - current.b) * 255);
            c = std::max(0, std::min(255, c));
            ofSetColor(c, c, c);
            ofDrawRectangle(i * size, j * size, size, size);
        }
    }
}

void reaction::diffuse() {
    int dt = 1.0;
    for (unsigned int i = 0; i < w_n; ++i) {
        for (unsigned int j = 0; j < h_n; ++j) {
            double a = state[ix(i,j)].a;
            double b = state[ix(i,j)].b;
            state[ix(i,j)].a +=
                ((d_a * convolute(i, j, 'a')) - (a * b * b) + (feed * (1 - a))) * dt;
            state[ix(i,j)].b +=
                ((d_b * convolute(i, j, 'b')) + (a * b * b) - ((k + feed) * b)) * dt;
            
            state[ix(i,j)].a = std::fmaxf(0.0, std::fminf(1.0, state[ix(i,j)].a));
            state[ix(i,j)].b = std::fmaxf(0.0, std::fminf(1.0, state[ix(i,j)].b));
        }
    }
}

double reaction::convolute(int i, int j, char s) {
    double sum = 0.0;
    
    fluid center = state[ix(i, j)];
    if (s == 'a') sum += -1.0 * center.a;
    if (s == 'b') sum += -1.0 * center.b;
    
    fluid direct[] = {
        state[ix(i, j - 1)],
        state[ix(i + 1, j)],
        state[ix(i, j + 1)],
        state[ix(i - 1, j)]
    };
    for (const fluid &neighbor : direct) {
        if (s == 'a') sum += neighbor.a * 0.2;
        if (s == 'b') sum += neighbor.b * 0.2;
    }
    
    fluid diagonal[] = {
        state[ix(i + 1, j - 1)],
        state[ix(i + 1, j + 1)],
        state[ix(i - 1, j + 1)],
        state[ix(i - 1, j - 1)]
    };
    for (const fluid &neighbor : diagonal) {
        if (s == 'a') sum += neighbor.a * 0.05;
        if (s == 'b') sum += neighbor.b * 0.05;
    }
    
    return sum;
}


int reaction::ix(int i, int j) {
    int xIndex = std::max(0, std::min(w_n-1, i));
    int yIndex = std::max(0, std::min(h_n-1, j));
    return xIndex + yIndex * w_n;
}
