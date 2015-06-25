struct PUnit;
struct Alphabet;

using Pattern = vector<PUnit>;
using Text = vector<Alphabet>;
using Integer = mpz_class;

vector<Text> docs;

bool preceq(Text&, Pattern&);
vector<Pattern> kmmg(int K, vector<Text>&_docs);
Integer language_size(Pattern&, int, bool);
