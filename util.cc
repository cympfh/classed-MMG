#include "./global.cc"
#include <bits/stdc++.h>
using namespace std;

#define rep(i,n) for(int i=0;i<(n);++i)
#define loop for(;;)
#define trace(var) cerr<<">>> "<<#var<<" = "<<var<<endl;

template<class T>
ostream& operator<<(ostream& os, vector<T> v) {
  if (v.size() == 0) {
    os << "(empty vector)";
    return os;
  }
  os << v[0];
  for (int i = 1, len = v.size(); i < len; ++i) {
    os << " " << v[i];
  }
  return os;
}

template<class T>
ostream& operator<<(ostream& os, set<T> v) {
  for (T t: v) os << t << ' ';
  return os;
}

template<class S, class T>
ostream& operator<<(ostream& os, pair<S,T> p) {
  os << '(' << p.first << ", " << p.second << ')';
  return os;
}

template<class S, class T>
ostream& operator<<(ostream& os, map<S,T>& m) {
  for (const pair<S,T>& pr: m) {
    os << pr << ' ';
  }
  return os;
}

double str_to_double(string s) {
  stringstream a(s);
  double r; a >> r;
  return r;
}

int str_to_int(string s) {
  stringstream a(s);
  int r; a >> r;
  return r;
}

using vi = vector<int>;

template<typename T>
string d2b(T s, int n) {
  stringstream sout;
  vi xs;
  rep (i, n) {
    if (s & (1 << i)) xs.push_back(i);
  }
  if (xs.size() == 0) {
    return "(empty bit-set)";
  }
  sout << xs[0];
  rep (i, xs.size()-1) sout << ',' << xs[i+1];
  return sout.str();
}
