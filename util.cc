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

template<class S, class T>
ostream& operator<<(ostream& os, pair<S,T> p) {
  os << '(' << p.first << ", " << p.second << ')';
  return os;
}

vector<string> as_words(string s) {
  stringstream sin(s);
  vector<string> words;
  for (;;) {
    string s; sin >> s;
    if (!sin) break;
    words.push_back(s);
  }
  return words;
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

