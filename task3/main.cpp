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
#include <set>
#include <algorithm>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <math.h>
using namespace std;

typedef vector<int> vi;
typedef bitset<128> TLL;
typedef unordered_map<TLL, set<int> > THash;

typedef unordered_map<string, int> msi;
typedef unordered_map<string, vi> msvi;
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

int min(int a, int b, int c) {
    return min(a, min(b, c));
}

string compact(const vector<char>& out) {
    stringstream ss;
    for (int i = 0; i < out.size();) {
        int c = 0;
        char cur = out[i];
        while (i < out.size() && cur == out[i]) {
            ++i;
            ++c;
        }
        ss << c << cur;
    }
    return ss.str();
}

int traceLD(const std::string &s1, const std::string &s2, vector<char>& out) {
    int m = s1.length();
    int n = s2.length();
    int err = 0;
    vector< vector<int> > D(m + 1, vector<int>(n + 1));
    for (int j = 0; j <= n; ++j) {
        D[0][j] = j;
    }
    D[m][0] = m;
    for (int i = 0; i < m; ++i) {
        D[i+1][0] = i+1;
        for (int j = 0; j < n; ++j) {
            D[i+1][j+1] = min(D[i+1][j] + 1, D[i][j+1] + 1, D[i][j] + (s1[i] == s2[j] ? 0 : 1));
        }
    }
    err = D[m][n];
//    return D[m][n];
    vector<char> trace;
    while (m > 0 && n > 0) {
        int mn = min(D[m-1][n-1] + (s1[m-1] == s2[n-1] ? 0 : 1), D[m][n-1] + 1, D[m-1][n] + 1);
        if (D[m-1][n-1] + (s1[m-1] == s2[n-1] ? 0 : 1) == mn) {
            trace.push_back(s1[m-1] == s2[n-1] ? 'M':'X');
            if (s1[m-1] != s2[n-1]) {
                --err;
            }
            --m;
            --n;
        } else if (D[m][n-1] + 1 == mn) {
            trace.push_back('I');
            --n;
            --err;
        } else if (D[m-1][n] + 1 == mn) {
            trace.push_back('D');
            --m;
            --err;
        }
    }
    
    while (m > 0) {
        trace.push_back('D');
        --m;
        --err;
    }
    while (n > 0) {
        trace.push_back('I');
        --n;
        --err;
    }
    reverse(trace.begin(), trace.end());
    for (const auto x : trace) {
        out.push_back(x);
    }
    return err;
}


TLL getTLL(const string& s) {
    TLL x;
    for (int i = 0; i < s.length(); ++i) {
        int c = code(s[i]);
        x[2*i]     = c & 1;
        x[2*i + 1] = c & 2;
    }
    return x;
}

string fromTLL(TLL x, int L) {
    vector<char> ss;
    for (int i = 0; i < L; ++i) {
        ss.push_back(decode(x[2*i] + (x[2*i + 1] << 1)));
    }
    
    return string(ss.begin(), ss.end());
}


class TGenerate {
    
public:
    int N, L, D;
    THash H;
    
public:
    TGenerate(int n, int l, int d) {
        N = n;
        L = l;
        D = d;
    }
    
    void combine(TLL s, int start, int c, int l, int d) {
        if (d > D || c >= l) {
            return;
        }
        if (c == l - 1 && l == L) {
            H[s].insert(start);
            return;
        }
        TLL cs = s;
        // M
        combine(s, start, c + 1, l, d);
        // X
        for (int i = 0; i < 4; ++i) {
            s[c] = i & 1;
            s[c+1] = i & 2;
            combine(s, start, c + 1, l, d + 1);
        }
        // I
        s = cs;
        s <<= 2;
        for (int i = 0; i < 2*c; ++i) {
            s[i] = cs[i];
        }
        for (int i = 0; i < 4; ++i) {
            s[2*c] = i & 1;
            s[2*c + 1] = i & 2;
            combine(s, start, c + 2, l + 1, d + 1);
        }
        // D
        if (c > 0) {
            s = cs;
            s >>= 2;
            for (int i = 0; i < 2*c; ++i) {
                s[i] = cs[i];
            }
            combine(s, start, c, l - 1, d + 1);
        }
    }
    
    TLL check(vi& res) {
        for (const auto& c : H) {
            if (c.second.size() >= N) {
                int prev = -100;
                for (auto x : c.second) {
                    if (x >= prev + L - 1) {
                        res.push_back(x);
                        prev = x;
                    }
                }
                if (res.size() >= N) {
                    return c.first;
                } else {
                    res.clear();
                }
            }
        }
        return TLL(0);
    }
};

int main(int argc, const char * argv[]) {
    if (0) {
        vector<char> oo;
        cout << traceLD("TTCCATACCA", "CCCCTAACCA", oo) << endl;
//        cout << traceLD("TTCCAT", "CCCCTA", oo) << endl;
//        cout << traceLD("TTCC", "CCCC", oo) << endl;
        for (char c : oo ) {
            cout << c;
        }
        cout << endl;
        return 0;
    }
    
    
    for (int T = 3; T <= 3; ++T) {
        ifstream in;
        in.open("/Users/styskin/bio2019/task3/task3/" + std::to_string(T) + ".txt");
        ofstream out;
        out.open("/Users/styskin/bio2019/task3/task3/" + std::to_string(T) + ".out2");
        int N, L, D;
        string s;
        in >> N >> L >> D;
        // N -- number of times
        // L - length
        // D - errors
        cout << N << " " << L << " " << D << endl;
        in >> s;
        if (1) {
            TGenerate generate(N, L, D);
            
            for (int i = 0; i < s.length() - L; ++i) {
                generate.combine(getTLL(s.substr(i, L)), i, 0, L, 0);
            }
            vi res;
            TLL xx = generate.check(res);
            if (res.size() >= N) {
                string e = fromTLL(xx, L);
                out << e << endl;
                for (auto start : res) {
                    vector<char> oo;
                    traceLD(s.substr(start, L), e, oo);
//                    out << start + 1 << " " << s.substr(start, L) << " " << compact(oo) << endl;
                    out << start + 1  << " " << compact(oo) << endl;
                }
                return 0;
            }
            return 0;
        }
        bool found = false;
        for (int p = L; p >= 2; --p) {
            cout << "Size " << p << endl;
            msi m;
            msvi pos;
            for (int i = 0; i < s.length() - p; ++i) {
                string x = s.substr(i, p);
                m[x] += 1;
                pos[x].push_back(i);
            }
            msvi candidates;
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
                TGenerate generate(N, L, D);
                for (int i = 0; i < cs.second.size(); ++i) {
                    int cst = cs.second[i];
                    for (int j = max(cst - x, 0); j <= cst; ++j) {
                        generate.combine(getTLL(s.substr(j, L)), j, 0, L, 0);
                    }
                }
                vi res;
                cout << "CHECK" << endl;
                TLL xx = generate.check(res);
                if (res.size() >= N) {
                    string e = fromTLL(xx, L);
                    out << e << endl;
                    for (auto start : res) {
                        vector<char> oo;
                        traceLD(s.substr(start, L), e, oo);
                        out << start + 1 << " " << s.substr(start, L) << " " << compact(oo) << endl;
                    }
                    found = true;
                    break;
                }
                
                // Test 5

                // Test 6
                // TCGGG GAGCCCTGGGCGG GGCGG
                //string eprev = "TCGGG";
                //string epost = "GGCGG";
                // Test 8
                //string eprev = "TGCGGGTGCCAGGGAACCAGGG";
                //string epost = "AAGTGAT";

                // Test 9
                //string eprev = "AACGCATT";
                //string epost = "GGGAGGG";


//                string e = eprev + cs.first + epost;
//                out << e << endl;
//                for (int i = 0; i < cs.second.size(); ++i) {
//                    int cst = cs.second[i];
//                    vector<char> oo;
//                    traceLD(s.substr(cst - eprev.length(), eprev.length()), eprev, oo);
//                    for (int i = 0; i < p; ++i) {
//                        oo.push_back('M');
//                    }
//                    traceLD(s.substr(cst + p, epost.length()), epost, oo);
//                    out << (cst - eprev.length() + 1) << " " << compact(oo) << endl;
//                }
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
