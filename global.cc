#include <bits/stdc++.h>
using namespace std;

bool DEBUG = false;
int K = 1;
double limit = 1.0;

enum Mode { K_MULTIPLE, GAIN_LIMIT };
Mode mode = K_MULTIPLE;

vector<string> priority_pos = {
  "VB", "VBD", "VBG", "VBN", "VBP", "VBZ",
  "NN",
  "IN"
};

vector<string> inpriority_pos;

set<string> stoppos = {
//  "CD",
//  "DT",
//  "JJ", "JJR", "JJS"
};

