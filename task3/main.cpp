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
#include <sstream>
#include <stdlib.h>
#include <math.h>
using namespace std;

typedef unordered_map<int, int> mii;
typedef vector<int> vi;
typedef unordered_map<int, vi> mivi;
typedef pair<int, int> pii;
typedef vector<pii> vii;

int code(char c) {
    switch(c) {
        case 'A': return 0;
        case 'C': return 1;
        case 'T': return 2;
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

string diff(const string& s, const string& e, int p2, int l, int d) {
    std::stringstream ss;
    
    int M = 0;
    int X = 0;
    int I = 0;
    int D = 0;
    for (int i = 0, j = p2; i < e.length() && j < s.length(); ) {
        if (e[i] == s[j]) {
            ++M;
            ++i;
            ++j;
        } else if (e[i] == s[j+1]) {
            ++D;
            ++j;
        } else if (e[i+1] == s[j]) {
            ++I;
            ++i;
            
        } else if (e[i+1] == s[j+1]) {
            ++X;
            ++i;
            ++j;
        } else {
            ++i;
            ++j;
            cout << "XXXXXX" << endl;
        }
        if ((X + I + D) > 0) {
            if (M > 0) {
                ss << M << "M";
            }
            if (X > 0) {
                ss << X << "X";
            }
            if (D > 0) {
                ss << D << "D";
            }
            if (I > 0) {
                ss << I << "I";
            }
            M = 0;
            X = 0;
            D = 0;
            I = 0;
        }
    }
    if (M > 0) {
        ss << M << "M";
    }

    return ss.str();
}


int main(int argc, const char * argv[]) {
    for (int T = 5; T <= 5; ++T) {
        ifstream in;
        in.open(string("/Users/styskin/bio2019/task3/task3/") + std::to_string(T) + ".txt");
        ofstream out;
        out.open(string("/Users/styskin/bio2019/task3/task3/") + std::to_string(T) + ".out");
        int N, L, D;
        string s;
        in >> N >> L >> D;
        // N -- number of times
        // L - length
        // D - errors
        in >> s;
        
        // Find best
//        L = 8;
//        for (int p = min(16, L - 1); p >= 2; --p) {
        for (int p = 3; p >= 2; --p) {
            cout << "Size " << p << endl;
            mii m;
            mivi pos;
            for (int i = 0; i < s.length() - p; ++i) {
                int x = 0;
                for (int j = 0; j < p; ++j) {
                    x = ((x << 2) | code(s[i + j]));
                }
                m[x] += 1;
                pos[x].push_back(i);
            }
            vi pp;
            for (auto y : m) {
                if (y.second >= N) {
                    cout << y.first << " = " << y.second << ", pos=" << pos[y.first][0] << endl;
                    for (auto a : pos[y.first]) {
                        pp.push_back(a);
                    }
                }
            }
            if (pp.size() == 0) {
                continue;
            }
            sort(pp.begin(), pp.end());
            
            int x = codeA(s.substr(pp[0], p));
            vi& starts = pos[x];
            for (auto c : pp) {
                cout << c << " " << s.substr(c, p) << endl;
            }
            // string: pp[0], length L
            string e;
            e = s.substr(pp[0], L);
//            e = "GCGTAACAGCTGTGAA";
            out << e << endl;
            // count distance
            for (int ii = 0; ii < N; ++ii) {
                out << (starts[ii] + 1) << " " << diff(s, e, starts[ii], L, D) << endl;
            }
            break;
        }
        in.close();
        out.close();
    }
    return 0;
}
