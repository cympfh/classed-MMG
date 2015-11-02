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
  if (i <= 0) { return make_pair(s, s); }
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

void usage() {
  cerr << "Usage: mmg [options] < text_file" << endl;
  cerr << "  -K <int>       run k-mmg" << endl;
  cerr << "  -l <double>    run with gain limit" << endl;
  cerr << "  -D             debug mode" << endl;
  exit(0);
}

int main(int argc, char*argv[])
{
  // parse args
  {
    for (int i = 1; i < argc; ++i) {
      string s (argv[i]);
      if (s == "-D") {
        DEBUG = true;
      }
      else if (s == "-K") {
        K = str_to_int(string(argv[i+1]));
        mode = K_MULTIPLE;
        ++i;
      }
      else if (s == "-l") {
        limit = str_to_double(argv[i+1]);
        mode = GAIN_LIMIT;
        ++i;
      }
      else {
        usage();
      }
    }
  }

  if (DEBUG) {
    switch(mode) {
      case K_MULTIPLE:
        cerr << "  " << K << "-multiple" << endl;
        break;
      case GAIN_LIMIT:
        cerr << "  run with limit " << limit << endl;
        break;
      default:
        usage();
    }
  }

  // read documents
  vector<Text> docs;
  {
    string ln;
    while (getline(cin, ln)) {
      Text text = to_sentence(ln);
      docs.push_back(text);
    }
  }

  // dassee
  init(docs, DEBUG);

  // 行くぜ
  auto result = kmmg();
  for (auto& p: result) {
    cout << p << endl;
  }

  return 0;
}
