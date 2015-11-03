#include <bits/stdc++.h>
using namespace std;

bool DEBUG = false;
int K = 1;
double limit = 1.0;

enum Mode { K_MULTIPLE, GAIN_LIMIT };
Mode mode = K_MULTIPLE;

// <> -> <A>
vector<string> priority_fill = {
  "VB",
  "IN"
};
vector<string> normal_fill = {};
vector<string> inpriority_fill = {
};

// <A> -> a
vector<string> priority_filler = {
};
vector<string> normal_filler = {};
vector<string> inpriority_filler = {
  "NN",
  "JJ",
  "CD"
};
