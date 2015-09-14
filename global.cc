#include <bits/stdc++.h>
using namespace std;

bool DEBUG = false;
int K = 1;
double rho = 0.3;
enum Mode { KMULTIPLE, ABSTRACTION };
Mode mode = KMULTIPLE;
bool all_output= false;
bool RANDOM_PRIORITY = false;

vector<string> priority_pos = {
  "VB", "VBD", "VBG", "VBN", "VBP", "VBZ",
  "NN",
  "IN"
};

vector<string> inpriority_pos;

set<string> stoppos = {
  "CD",
  "DT",
  "JJ", "JJR", "JJS"
};

