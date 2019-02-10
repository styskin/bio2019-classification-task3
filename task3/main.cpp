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
typedef pair<int, int> pii;
typedef unordered_map<TLL, set<pii> > THash;

typedef unordered_map<string, int> msi;
typedef unordered_map<string, vi> msvi;
typedef vector<pii> vii;

const int TRIG = 4;

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
            --m;
            --n;
        } else if (D[m][n-1] + 1 == mn) {
            trace.push_back('I');
            --n;
        } else if (D[m-1][n] + 1 == mn) {
            trace.push_back('D');
            --m;
        }
    }
    
    while (m > 0) {
        trace.push_back('D');
        --m;
    }
    while (n > 0) {
        trace.push_back('I');
        --n;
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
    
    void combine(TLL s, pii start, int c, int l, int d, TLL mask) {
        if (d > D || c >= L) {
            return;
        }
        if (c == L - 1) {
            pii r(start);
            r.second = L - (l - start.second);
            // trim s to length
            TLL rs = s;
            for (int i = L; i < l; ++i) {
                rs[2*i] = 0;
                rs[2*i + 1] = 0;
            }
            H[rs].insert(r);
            return;
        }
        // Only append
        if (c >= l) {
            for (int i = 0; i < 4; ++i) {
                s[2*c] = i & 1;
                s[2*c+1] = i & 2;
                combine(s, start, c + 1, l, d + 1, mask);
            }
            return;
        }
        
        TLL cs = s;
        // skip symbol
        if (c == 0) {
            pii r(start);
            ++r.first;
            --r.second;
            s >>= 2;
            TLL mc = mask;
            mc >>= 1;
            combine(s, r, c, l - 1, d, mc);
        }
        // M
        s = cs;
        combine(s, start, c + 1, l, d, mask);
        if (mask[c - (l - start.second)] == 0) {
            // X
            for (int i = 0; i < 4; ++i) {
                s[2*c] = i & 1;
                s[2*c+1] = i & 2;
                if (s[2*c] != cs[2*c] || s[2*c+1] != cs[2*c]) {
                    combine(s, start, c + 1, l, d + 1, mask);
                }
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
                // Do not insert the same
                if (s[2*c] != cs[2*c] || s[2*c+1] != cs[2*c]) {
                    combine(s, start, c + 2, l + 1, d + 1, mask);
                }
            }
            // D
            if (c > 0) {
                s = cs;
                s >>= 2;
                for (int i = 0; i < 2*c; ++i) {
                    s[i] = cs[i];
                }
                combine(s, start, c, l - 1, d + 1, mask);
            }
        }
    }
    
    TLL check(vii& res) {
        for (const auto& c : H) {
            if (c.second.size() >= N-1) {
                pii prev(-100, 0);
                for (auto x : c.second) {
                    if (x.first >= prev.first + L) {
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
    
    for (int T = 9; T <= 9; ++T) {
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
        if (0) {
            TGenerate generate(N, L, D);
            
            for (int i = 0; i < s.length() - L; ++i) {
                // Check different length
                for (int j = L - D; j <= L + D; ++j) {
                    generate.combine(getTLL(s.substr(i, j)), pii(i, j), 0, j, 0, TLL());
                }
            }
            vii res;
            TLL xx = generate.check(res);
            if (res.size() >= N) {
                string e = fromTLL(xx, L);
                out << e << endl;
                for (auto start : res) {
                    vector<char> oo;
                    traceLD(s.substr(start.first, start.second), e, oo);
//                    out << start + 1 << " " << s.substr((start.first, start.second)) << " " << compact(oo) << endl;
                    out << start.first + 1  << " " << compact(oo) << endl;
                }
                return 0;
            }
            return 0;
        }
        
        // Test 7
        if (0) {
            string e = "AGGAACTGGGTTCGGGGAGCCCTGGGCGGGGCGGCTGTGAGG";
            out << e << endl;
            vii res;
            res.push_back(pii(4975, 42));
            res.push_back(pii(5072, 42));
            res.push_back(pii(5164, 42));
            for (auto start : res) {
                vector<char> oo;
                traceLD(s.substr(start.first, start.second), e, oo);
                out << start.first + 1 << " " << s.substr(start.first, start.second) << " " << compact(oo) << endl;
                //                        out << start.first + 1 << " " << compact(oo) << endl;
                
            }
            return 0;
        }

        if (0) {
            string e = "GCATTCTCTGTGCGGAGGA";
            out << e << endl;
            vii res;
            res.push_back(pii(24, 22));
            res.push_back(pii(63, 22));
            res.push_back(pii(130, 22));
            res.push_back(pii(211, 22));
            res.push_back(pii(1450, 22));
            for (auto start : res) {
                vector<char> oo;
                out << s.substr(start.first, start.second) << endl;
                int er = traceLD(s.substr(start.first, start.second), e, oo);
                out << start.first + 1 << " " << er << " " << s.substr(start.first, start.second) << " " << compact(oo) << endl;
                //                        out << start.first + 1 << " " << compact(oo) << endl;
                
            }
            return 0;
        }

        
        
        bool found = false;
        set<int> checked;
        // Should be L
//        L = L - 4;
        for (int p = L; p >= 2; --p) {
            cout << "Size " << p << endl;
            msvi pos;
            for (int i = 0; i < s.length() - p; ++i) {
                string x = s.substr(i, p);
                pos[x].push_back(i);
            }
            msvi candidates;
            for (const auto& y : pos) {
                if (y.second.size() >= N) {
                    cout << y.first << " = " << y.second.size() << ", pos=" << y.second[0] << endl;
                    int prev = -100;
                    for (auto a : y.second) {
                        if (a > prev + L) {
                            candidates[y.first].push_back(a);
                            prev = a;
                        }
                    }
                    if (candidates[y.first].size() < N) {
                        candidates.erase(y.first);
                    }
                }
            }
            for (const auto& cs : candidates) {
                if (cs.second.size() < N) {
                    continue;
                }
                TGenerate generate(N, L, D);

                int x = L - p + D;
                cout << "Calculate common trigrams" << endl;
                unordered_map<string, set<int> > ctrigrams;
                for (int i = 0; i < cs.second.size(); ++i) {
                    int cst = cs.second[i];
                    string ss = s.substr(max(cst - x, 0), L + 2 * D);
                    cout << ss << endl;

                    for (int j = 0; j < ss.length() - TRIG; ++j) {
                        string x = ss.substr(j, TRIG);
                        ctrigrams[x].insert(i);
                    }
                }
                vector<string> trigrams;
                for (const auto& y : ctrigrams) {
                    if (y.second.size() >= N / 2) {
                        trigrams.push_back(y.first);
                    }
                }
                
                for (int i = 0; i < cs.second.size(); ++i) {
                    int cst = cs.second[i];
                    cout << "Try " << cst << endl;
                }
                cout << "Let's try" << endl;
                
                for (int i = 0; i < cs.second.size(); ++i) {
                    int cst = cs.second[i];
                    int csts = max(cst - x, 0);
                    cout << "Try " << csts << endl;
//                    for (int j = max(cst - x, 0); j <= cst; ++j) {
                    {
                        // Check different length inside checker
                        // Calculate mask to skip
                        string can = s.substr(csts, L + 2 * D);
                        TLL mask;
                        for (const auto& t : trigrams) {
                            size_t tp = can.find(t);
                            if (tp != string::npos) {
                                for (int ti = 0; ti < TRIG; ++ti) {
                                    mask[tp + ti] = 1;
                                }
                            }
                        }
                        cout << csts << " ";
                        int zeros = 0;
                        for (int ti = can.length() - 1 ; ti >= 0; --ti) {
                            if (mask[ti] == 0) {
                                ++zeros;
                            } else {
                                break;
                            }
                        }
//                        int len = can.length() - (zeros - min(zeros, D));
                        int len = can.length();
                        for (int ti = 0; ti < len; ++ti) {
                            cout << mask[ti];
                        }
                        cout << endl;
                        generate.combine(getTLL(can), pii(csts, len), 0, len, 0, mask);
                    }
                }
                // Due to optimization let's not X, I, D in common 3grams
                
                
                vii res;
                cout << "CHECK" << endl;
                TLL xx = generate.check(res);
                if (res.size() >= N) {
                    string e = fromTLL(xx, L);
                    out << e << endl;
                    for (auto start : res) {
                        vector<char> oo;
                        traceLD(s.substr(start.first, start.second), e, oo);
                        out << start.first + 1 << " " << s.substr(start.first, start.second) << " " << compact(oo) << endl;
//                        out << start.first + 1 << " " << compact(oo) << endl;

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
