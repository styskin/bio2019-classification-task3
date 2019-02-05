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

char decode(int c) {
    switch(c) {
        case 0: return 'A';
        case 1: return 'C';
        case 2: return 'T';
        case 3: return 'G';
    }
    return 0;

}

int codeD(char c) {
    switch(c) {
        case 'M': return 0;
        case 'X': return 1;
        case 'D': return 2;
        case 'I': return 3;
    }
    return 0;
}

char decodeD(int c) {
    switch(c) {
        case 0: return 'M';
        case 1: return 'X';
        case 2: return 'D';
        case 3: return 'I';
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

void maximize(pii& a, const pii& b) {
    if (a.first == -1) {
        a = b;
    } else if (b.first < a.first) {
        a = b;
    } else if (b.first == a.first && b.second > a.second) {
        a = b;
    }
}

int main(int argc, const char * argv[]) {
    for (int T = 3; T <= 3; ++T) {
        ifstream in;
        in.open(std::to_string(T) + ".txt");
        ofstream out;
        out.open(std::to_string(T) + ".out");
        int N, L, D;
        string s;
        in >> N >> L >> D;
        // N -- number of times
        // L - length
        // D - errors
        cout << N << " " << L << " " << D << endl;
        in >> s;

        bool found = false;
        for (int p = min(16, L - 1); p >= 2; --p) {
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
            unordered_map<string, vector<int> > candidates;

            for (auto y : m) {
                if (y.second >= N) {
                    cout << y.first << " = " << y.second << ", pos=" << pos[y.first][0] << endl;
                    for (auto a : pos[y.first]) {
                        candidates[s.substr(a, p)].push_back(a);
                    }
                }
            }
            for (const auto& cs : candidates) {
                if (cs.second.size() < N) {
                    continue;
                }
                vector<string> prev;
                vector<string> post;
                int x = L - p + D;
                for (int i = 0; i < cs.second.size(); ++i) {
                    int cst = cs.second[i];
                    string tp(s.substr(cst - x, x));
                    string tpost(s.substr(cst + p, x));
                    cout << cst << ": " << tp << " " << s.substr(cst, p) << " " << tpost << endl;
                    reverse(tp.begin(), tp.end());
                    prev.push_back(tp);
                    post.push_back(tpost);
                }
                int n = cs.second.size();


                // minimize max_edit
                // M [x, STATES, K,  pii (max_edit, prevstate)   ]

                int STATE_COUNT = 2;
                int STATES = 1 << STATE_COUNT;

                vector<vector< vector<pii> >> M(4, vector<vector<pii> >(x, vector<pii>(n, pii(-1, -1))));
                for (int i = 0; i < x; ++i) {
                    for (int j = 0; j < 4; ++j) {
                        for (int k = 0; k < n; ++k) {
                            if (i == 0) {
                                if (decode(j) == prev[k][i]) {
                                    M[j][i][k] = pii(0, 1);
                                } else {
                                    M[j][i][k] = pii(1, 0);
                                }
                            } else {
                                for (int pk = 0; pk < n; ++pk) {
                                    pii pr(M[j][i - 1][pk]);
                                    if (decode(j) == prev[k][pr.second]) { // M
                                        pr.second++;
                                        maximize(M[j][i][k], pr);
                                    } else if (decode(j) == prev[k][pr.second]) { // X
                                        pr.first ++;
                                        pr.second ++;
                                        maximize(M[j][i][k], pr);
                                    } else {    // I
                                    }           // D
                                }
                            }
                        }
                    }
                }
                // if GOOD then BREAK
                found = true;
                break;

            }
            if (found) {
                break;
            }
        }
        in.close();
        out.close();
    }
    return 0;
}
