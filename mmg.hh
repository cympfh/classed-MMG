struct PUnit;
struct Alphabet;

using Pattern = vector<PUnit>;
using Text = vector<Alphabet>;
using Integer = mpz_class;

// initial with `init`
vector<Text> docs;
Integer alphabet_size = -1;
map<string, Integer> class_size;
set<Alphabet> vocabulary;
// set<string> pos_vocabulary;

bool preceq(Text&, Pattern&);
void init(vector<Text>&_docs);
vector<Pattern> kmmg(int K);
Integer language_size(Pattern&, int, bool);
