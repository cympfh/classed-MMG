#include "util.cc"
#include <gmpxx.h>
#include "mmg.hh"
using namespace std;

enum PUnitType {
  VAR, POS, WORD
};

struct PUnit
{
  PUnitType t;
  string pos;
  string word;

  PUnit() : t(VAR) {}
  PUnit(string pos_) : t(POS), pos(pos_) {}
  PUnit(string pos_, string w) : t(WORD), pos(pos_), word(w) {}

  bool operator!=(PUnit&rh) {
    if (this-> t != rh.t) return true;
    switch (this->t) {
      case VAR:
        return false;
      case POS:
        return this->pos != rh.pos;
      case WORD:
        return this->word != rh.word;
    }
  }

  bool operator==(PUnit& rh) {
    return not (*this == rh);
  }

};

  bool operator<(const PUnit& rl, const PUnit& rh) {
    switch (rl.t) {
      case VAR:
        return (rh.t != VAR);
      case POS:
        if (rh.t == WORD) return true;
        if (rh.t == POS) return (rl.pos < rh.pos);
        return false;
      case WORD:
        if (rh.t == WORD) return (rl.word < rh.word);
        return false;
    }
  }

ostream& operator<<(ostream& os, PUnit r) {
  switch (r.t) {
    case VAR:
      return os << "<>";
    case POS:
      return os << '<' << r.pos << '>';
    case WORD:
      return os << r.word << '/' << r.pos;
  }
}

int num_of_fixed(Pattern& p) {
  int n = 0;
  for (auto&u: p)
    if (u.t != VAR) ++n;
}

struct Alphabet {
  string word;
  string pos;
  Alphabet(string p, string w) : pos(p), word(w) {}

  bool operator==(Alphabet& rh) {
    return this->word == rh.word and this->pos == rh.pos;
  }

  bool operator<(Alphabet& rh) {
    return this->pos < rh.pos or this->word < rh.word;
  }
};

  bool operator<(const Alphabet& lh, const Alphabet& rh) {
    return lh.pos < rh.pos or lh.word < rh.word;
  }

ostream& operator<<(ostream& os, const Alphabet& a) {
  return os << a.word << '/' << a.pos;
}

/* non-erasing generalization system <= */

bool preceq(Alphabet& a, PUnit& u) {
  if (u.t == VAR) return true;
  if (u.t == POS) return a.pos == u.pos;
  return a.pos == u.pos and a.word == u.word;
}

bool preceq_table[2000][2000];
bool preceq(Text& s, Pattern& p) {
  const int
    n = s.size(),
    m = p.size();

  if (n < m) return false; // non-erasing precondition

  assert(n < 2000);
  assert(m < 2000);

  rep (i, n) rep (j, m) preceq_table[i][j] = false;

  rep (i, n) {
    rep (j, m) {
      if (i == 0 and j == 0) {
        preceq_table[0][0] = preceq(s[0], p[0]);
        if (not preceq_table[0][0]) return false;
      }
      else if (i == 0) {
        continue;
      }
      else if (j == 0) {
        preceq_table[i][0] = p[0].t == VAR;
      }
      else {
        preceq_table[i][j] =
          (preceq_table[i-1][j-1] and preceq(s[i], p[j]))
          or (preceq_table[i-1][j] and p[j].t == VAR);
      }
    }
  }
  return preceq_table[n-1][m-1];
}

// S subseteq L(p); S = { docs[i] : i <- c }
bool language_include(vi&c, Pattern&p) {
  for (int i: c)
    if (not preceq(docs[i], p)) return false;
  return true;
}

// S cap L(p); S = { docs[i] : i <- c }
vi cover(Pattern&p, vi&c) {
  vi r;
  for (int i: c)
    if (preceq(docs[i], p)) r.push_back(i);
  return r;
}

set<int> coverset(Pattern&p, vi&c) {
  set<int> s;
  for (int i: c)
    if (preceq(docs[i], p)) s.insert(i);
  return s;
}

// { a : a <- docs[i], i <- c }
// gather all alphabets using in `c`
set<Alphabet> alphabets(vi&c) {
  set<Alphabet> s;
  for (int idx: c) {
    for (Alphabet&a: docs[idx]) {
      s.insert(a);
    }
  }
  return s;
}

// find a minimal pattern from `p` respect to `c`
// precondition: S(c) subseteq L(p)
Pattern tighten(Pattern p, vi&c, bool origin=false)
{
  const int n = p.size();

  if (DEBUG) {
    if (origin) {
      cerr << "tighten: ";
    } else {
      cerr << " -> ";
    }
    cerr << p << endl;;
  }

  map<string, vector<string>> dict; // dict[pos] = { word }
  {
    set<Alphabet> s = alphabets(c);
    for (auto&a: s) {
      dict[a.pos].push_back(a.word);
    }
  }

  rep (i, n) {
    // <A> -> a/A
    if (p[i].t == POS) {
      string pos = p[i].pos;
      p[i].t = WORD;
      for (string&w: dict[pos]) {
        p[i].word = w;
        if (language_include(c, p)) return tighten(p, c);
      }
      p[i].t = POS;
    }

    // <> -> <A>
    if (p[i].t == VAR) {
      p[i].t = POS;
      for (auto&pr: dict) {
        p[i].pos = pr.first;
        if (language_include(c, p)) return tighten(p, c);
      }
      p[i].t = VAR;
    }
  }

  Pattern q(n+1);

  // <> -> <> <>
  rep (i, n) {
    q[i] = p[i];
    if (p[i].t == VAR) {
      q[i+1] = PUnit();
      for (int j = i + 1; j < n; ++j) q[j+1] = p[j];
      if (language_include(c, q)) return tighten(q, c);
    }
  }

  if (DEBUG) {
    cerr << "tighten end" << endl;
  }
  return p; // final
}

vector<pair<Pattern, vi>> division(Pattern p, vi&c)
{
  const int
    n = p.size(),
    M = c.size();

  map<string, vector<string>> dict; // dict[pos] = { word }
  {
    set<Alphabet> s = alphabets(c);
    for (auto&a: s) {
      dict[a.pos].push_back(a.word);
    }
  }

  vector<pair<Pattern, set<int>>> cspc; 
  rep (i, n) {
    // <A> -> a/A
    if (p[i].t == POS) {
      string pos = p[i].pos;
      p[i].t = WORD;
      for (string&w: dict[pos]) {
        p[i].word = w;
        cspc.push_back({ p, coverset(p, c) });
      }
      p[i].t = WORD;
    }

    // <> -> <A>
    if (p[i].t == VAR) {
      p[i].t = POS;
      for (auto&pr: dict) {
        p[i].pos = pr.first;
        cspc.push_back({ p, coverset(p, c) });
      }
      p[i].t = VAR;
    }
  }

  Pattern q(n+1);

  // <> -> <> <>
  rep (i, n) {
    q[i] = p[i];
    if (p[i].t == VAR) {
      q[i+1] = PUnit();
      for (int j = i + 1; j < n; ++j) q[j+1] = p[j];
      cspc.push_back({ q, coverset(q, c) });
    }
  }

  vector<pair<Pattern, vi>> div;

  if (cspc.size() == 0) {
    return div;
  }

  auto sort_by_setsize = [](pair<Pattern, set<int>> x, pair<Pattern, set<int>> y) {
    return x.second.size() > y.second.size();
  };

  { // 貪欲に近似の最小集合被覆
    int i = 0;
    set<int> m;
    while (m.size() < M) {
      sort(cspc.begin() + i, cspc.end(), sort_by_setsize);
      vi v;
      for (int id: cspc[i].second) {
        v.push_back(id);
        if (m.count(id) == 0) {
          m.insert(id);
          for (int j = i + 1; j < cspc.size(); ++j) cspc[j].second.erase(id);
        }
      }
      div.push_back({ cspc[i].first, v });
      ++i;
    }
  }

  if (DEBUG) {
    cerr << "# division of " << p << endl;
    cerr << "following " << div.size() << " patterns" << endl;
    for (auto& p: div) cerr << " -- " << p.first << endl;
  }

  return div;
}

// <> <>* -> <>
Pattern var_simplify(Pattern&s) {
  Pattern r;
  bool b = false;
  for (auto&u: s) {
    if (u.t == VAR) {
      if (b) {
        continue;
      } else {
        r.push_back(u);
        b = true;
      }
    } else {
      r.push_back(u);
      b = false;
    }
  }
  return r;
}

void init(vector<Text>& _docs) {
  docs = _docs;

  class_size.clear();
  vocabulary.clear();

  for (const Text& doc: docs) {
    for (const Alphabet& a: doc) {
      vocabulary.insert(a);
    }
  }
  alphabet_size = vocabulary.size();

  for (const Alphabet& a: vocabulary) {
    class_size[a.pos]++;
  }
}

vector<Pattern> kmmg(int K)
{
  const int n = docs.size();

  vi ids(n); rep(i, n) ids[i] = i;

  Pattern top { PUnit() };
  auto pc = tighten(top, ids, true);

  // いくつのパターンに被覆されているか
  vi cover_count(n, 1);

  // (num_of_fixed, Pattern, cover)
  priority_queue<pair<int, pair<Pattern, vi>>> pcs;
  pcs.push({ -num_of_fixed(pc), { pc, ids }});
  vector<Pattern> ret;

  /* division */
  while (not pcs.empty())
  {
    auto tpl = pcs.top(); pcs.pop();
    auto& p = tpl.second.first;
    auto& c = tpl.second.second;

    // S = Doc - L(Pi - p)
    vi S;
    for (int i: c) {
      if (cover_count[i] == 1) S.push_back(i);
      --cover_count[i];
    }
    if (S.size() == 0) continue;

    vector<pair<Pattern, vi>> pcs_next = division(p, S);
    if (pcs_next.size() < 2) { // not divisible
      if (DEBUG) {
        cerr << "a pattern " << p << " is not divisible" << endl;
      }
      ret.push_back(p);
      continue;
    }

    if (ret.size() + pcs.size() + pcs_next.size() > K) { // over
      if (DEBUG) {
        cerr << "#pattern is over K=" << K << endl;
        trace(ret.size());
        trace(pcs.size());
        trace(pcs_next.size());
      }
      ret.push_back(p);
      while (not pcs.empty()) {
        ret.push_back(pcs.top().second.first);
        pcs.pop();
      }
      return ret;
    }

    for (auto&pc_next: pcs_next) {
      for (int i: pc_next.second) ++cover_count[i];
    }
    for (auto&pc_next: pcs_next) {
      // Doc - (Pi - pc_next)
      vi S;
      for (int i: pc_next.second) {
        if (cover_count[i] == 1) S.push_back(i);
        else --cover_count[i];
      }
      auto p_next = tighten(pc_next.first, S, true);
      pcs.push({ -num_of_fixed(p_next), { p_next, S }});
    }
  }
  return ret;
}

Integer language_size_table[200][200];

Integer language_size(Pattern&p, int ell, bool DEBUG=false)
{

  // DP
  assert(p.size() < 200);
  assert(ell < 200);

  const int n = p.size();
  rep (i, ell) rep (j, n) language_size_table[i][j] = 0;

  rep (i, ell) {
    rep (j, n) {
      if (i == 0 and j == 0) {
        Integer r;
        {
          if (p[0].t == VAR) r = alphabet_size;
          else if (p[0].t == POS) r = class_size[p[0].pos];
          else r = 1;
        }
        language_size_table[0][0] = r;
      }
      else if (i == 0) {
        language_size_table[0][j] = 0;
      }
      else if (j == 0) {
        if (p[0].t == VAR) {
          language_size_table[i][j] = language_size_table[i-1][j] * alphabet_size;
        } else {
          language_size_table[i][j] = 0;
        }
      }
      else {
        Integer r;
        {
          r = language_size_table[i-1][j-1];
          if (p[j].t == VAR) r *= alphabet_size;
          else if (p[j].t == POS) r *= class_size[p[j].pos];
          if (i == 1 and j == 1) trace(p[j]);
        }

        Integer s = 0;
        {
          if (p[j].t == VAR) s = language_size_table[i-1][j] * alphabet_size;
        }

        language_size_table[i][j] = r + s;
      }
    }
  }

  if (DEBUG) {
    rep (i, ell) {
      rep (j, n) {
        cout << language_size_table[i][j] << ' ';
      } cout << endl;
    }
  }

  return language_size_table[ell-1][n-1];
}

