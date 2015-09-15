#include "./mmg.cc"
using namespace std;

pair<string, string> split_word_pos(string&s)
{
  string w, p;
  const int n = s.size();
  int i;
  for (i = n - 2; i >= 0; --i) {
    if (s[i] == '_') break;
  }
  if (i <= 0) {
    for (i = n - 2; i >= 0; --i) {
      if (s[i] == '/') break;
    }
  }
  assert(i > 0);
  w = s.substr(0, i);
  p = s.substr(i + 1);
  return make_pair(w, p);
}

Text to_sentence(string s)
{
  stringstream sin(s);
  Text t;
  for (;;) {
    string s; sin >> s;
    if (!sin) break;
    auto pr = split_word_pos(s);
    t.push_back( Alphabet(pr.second, pr.first) );
  }
  return t;
}

int main(int argc, char*argv[])
{
  // parse args
  {
    for (int i = 1; i < argc; ++i) {
      string s (argv[i]);
      if (s == "-D") {
        DEBUG = true;
        continue;
      }
      if (s == "-K") {
        K = str_to_int(string(argv[i+1]));
        mode = KMULTIPLE;
      }
      if (s == "-r") {
        stringstream sin(argv[i+1]);
        sin >> rho;
        mode = ABSTRACTION;
      }
      if (s == "-A") {
        all_output = true;
      }
      if (s == "-U") {
        UNWEIGHTED_COVERING = true;
      }
      if (s == "-h") {
        cerr << "Usage: mmg [options] < text_file" << endl;
        cerr << "  -D             debug mode" << endl;
        cerr << "  -K <int>       set k-mutliple" << endl;
        cerr << "  -r <double>    unset k-mutliple and set rho" << endl;
        cerr << "  -A             set all output mode" << endl;
        cerr << "  -R             select randomly in division" << endl;
        return 0;
      }
    }
  }

  if (DEBUG) {
    if (mode == KMULTIPLE) {
      cerr << "mode: k-multiple; k=" << K << endl;
    }
    else {
      cerr << "mode: abstraction; rho=" << rho << endl;
    }
  }

  // read documents
  vector<Text> docs;
  {
    string ln;
    loop {
      getline(cin, ln);
      if (not cin) break;
      Text text = to_sentence(ln);
      docs.push_back(text);
    }
  }

  // dassee
  init(docs, DEBUG);

  // 行くぜ
  auto result = kmmg(mode, K, rho);
  for (auto& p: result) {
    cout << p << endl;
  }

  return 0;
}
