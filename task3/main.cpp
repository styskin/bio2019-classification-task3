//
//  main.cpp
//  task3
//
//  Created by Andrey Styskin on 02.02.2019.
//  Copyright © 2019 Andrey Styskin. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <string>
#include <stdlib.h>
#include <math.h>
using namespace std;

typedef unordered_map<int, int> mii;
typedef vector<int> vi;
typedef pair<int, int> pii;
typedef vector<pii> vii;

int code(char c) {
    switch(c) {
        case 'A': return 0;
        case 'C': return 1;
        case 'U': return 2;
        case 'G': return 3;
    }
    return 0;
}

int codeA(const string& s) {
    int x = 0;
    for (int i = 0; i < s.length(); ++i) {
        x = ((x << 2) | code(s[i]));
    }
    return x;
}

int main(int argc, const char * argv[]) {
    ifstream in;
    in.open("/Users/styskin/bio2019/task3/task3/1.txt");
    ofstream out;
    out.open("/Users/styskin/bio2019/task3/task3/1.out");
    int N, L, D;
    string s;
    in >> N >> L >> D;
    // N -- number of times
    // L - length
    // D - errors
    in >> s;
    
    // Find best
    for (int p = L / 2; p >= 2; --p) {
        mii m;
        mii pos;
        vi scoded;
        for (int i = 0; i < s.length() - p; ++i) {
            int x = 0;
            for (int j = 0; j < p; ++j) {
                x = ((x << 2) | code(s[i + j]));
            }
            m[x] += 1;
            scoded.push_back(x);
            if (pos.find(x) == pos.end()) {
                pos[x] = i;
            }
        }
        
        vi pp;
        for (auto y : m) {
            if (y.second > N) {
                cout << y.first << " = " << y.second << ", pos=" << pos[y.first] << endl;
                pp.push_back(pos[y.first]);
            }
        }
        sort(pp.begin(), pp.end());
        // string: pp[0], length L
        string e;
        e = s.substr(pp[0], L);
        out << e << endl;
        
        // output places
        int o = 0;
        size_t f = s.find(e, pp[0]);
        while (f != string::npos && o < N) {
            out << (f+1) << " " << L << "M" << endl;
            f = s.find(e, f + L);
            ++o;
        }
//        for (int i = 0; i < pp.size(); ++i) {
//            cout << pp[i] << endl;
//        }
        if (pp.size() > 0) {
            break;
        }
    }
    
    
    
    
    in.close();
    out.close();
    return 0;
}
