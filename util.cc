#include "./global.cc"
#include <bits/stdc++.h>
#include <gmpxx.h>
using namespace std;

#define rep(i,n) for(int i=0;i<(n);++i)
#define loop for(;;)
#define trace(var) cerr<<">>> "<<#var<<" = "<<var<<endl;

using Integer = mpz_class;

template<class T>
inline
ostream& operator<<(ostream& os, vector<T> v) {
  if (v.size() == 0) { return os << "(empty vector)"; }
  os << v[0];
  for (int i = 1, len = v.size(); i < len; ++i) {
    os << " " << v[i];
  }
  return os;
}

template<class T>
inline
ostream& operator<<(ostream& os, set<T> v) {
  for (T t: v) os << t << ' ';
  return os;
}

template<class S, class T>
inline
ostream& operator<<(ostream& os, pair<S,T> p) {
  return os << '(' << p.first << ", " << p.second << ')';
}

template<class S, class T, class U>
inline
ostream& operator<<(ostream& os, tuple<S,T,U> t) {
  return os << '('
    << get<0>(t) << ", "
    << get<1>(t) << ", "
    << get<2>(t) << ", " << ')';
}

template<class S, class T>
inline
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
using vvi = vector<vi>;

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

Integer log(Integer x) {
  if (x <= 1) return 0;
  Integer r = 1;
  while (r < x) r *= 2;
  return r;
}

