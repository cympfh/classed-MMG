#include "util.cc"
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


/*
 * non-erasing generalization system <=
 */

bool preceq(const Alphabet& a, const PUnit& u) {
  if (u.t == VAR) return true;
  if (u.t == POS) return a.pos == u.pos;
  return a.pos == u.pos and a.word == u.word;
}

bool preceq(const Text& s, const Pattern& p, bool DEBUG) {
  int n = s.size();
  int m = p.size();
  if (n < m) return false;

  // tail matching
  while (m > 0 and p[m-1].t != VAR) {
    if (preceq(s[n-1], p[m-1])) {
      --n; --m;
    } else {
      return false;
    }
  }
  if (n == 0 and m == 0) return true;
  if (m == 0) return false;

  // p should be "<>[.<>]*<>" or "[.<>]*<>"

  int __pos = 0; // of text
  int __begin = 0; // of pattern

  // head matching
  while (p[__begin].t != VAR) {
    if (preceq(s[__pos], p[__begin])) {
      ++__pos; ++__begin;
    } else {
      return false;
    }
    if (__pos >= n) return false;
  }

  // p should be "<>[.<>]*<>"
  
  int __end;
  for (int i = p.size();i > 0; --i) {
    while (__begin < m and p[__begin].t == VAR) {
      ++__pos; ++__begin;
    }
    if (__begin >= m and __pos <= n) return true;
    if (__pos >= n) return false;

    for (__end = __begin + 1; __end < m; ++__end)
      if (p[__end].t == VAR) break;

    if (DEBUG) {
      cerr << "pos, begin, end = "
        << __pos << ' '
        << __begin << ' '
        << __end << endl;
    }

    for (; __pos < n - (__end - __begin); ++__pos) {
      bool res = true;
      for (int i = 0; i < __end - __begin; ++i) {
        if (not preceq(s[__pos + i], p[__begin + i])) {
          res = false;
          break;
        }
      }
      if (res) {
        __pos += (__end - __begin);
        __begin = __end;
        break;
      }
    }
    if (__pos >= n - (__end - __begin)) return false;
  }
  return false;
}

/* DP version is slow ??? */
bool preceq_table[2000][2000];
bool DP_preceq(const Text& s, const Pattern& p) {
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


// max |s| : s <- docs[i], i <- c
int upper_length(const vi& c) {
  int r = 0;
  for (int i: c) r = max<int>(r, docs[i].size());
  return r;
}


// find a minimal pattern from `p` respect to `c`
// precondition: S(c) subseteq L(p)
Pattern tighten(Pattern p, vi&c)
{
  const int n = p.size();
  assert(c.size() > 0);

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

  return p; // final
}


Pattern tighten(Pattern p, set<int> c) {
  vi v;
  for (int i: c) v.push_back(i);
  return tighten(p, v);
}


/*
 * 最小被覆を貪欲にするための優先度
 * 被覆サイズが大きいパターンに大きな値を与える
 * でも #L が大きい物には少し小さくさせたい
 */
inline
Integer priority(const Pattern& p, int ell, const set<int>& cover) {
  Integer r = cover.size();
  Integer c = language_size(p, ell);
  // cerr << "priority=" << (make_tuple(p, (r), make_pair(c, ell)))<<endl;
  return r;
}


vector<pair<Pattern, vi>> division(Pattern p, vi&c)
{
  const int
    n = p.size(),
    M = c.size();

  if (DEBUG) { cerr << "# division of " << p << ", " << c << endl; }

  map<string, vector<string>> dict; // dict[pos] = { word }
  {
    set<Alphabet> s = alphabets(c);
    for (auto&a: s) {
      dict[a.pos].push_back(a.word);
    }
  }

  vector<pair<Integer, pair<Pattern, set<int>>>> cspc; 
  int ell = upper_length(c);
  rep (i, n) {
    // <A> -> a/A
    if (p[i].t == POS) {
      string pos = p[i].pos;
      p[i].t = WORD;
      for (string&w: dict[pos]) {
        p[i].word = w;
        auto s = coverset(p, c);
        if (s.size() > 0) {
          Pattern r = tighten(p, s);
          Integer pr = priority(r, ell, s);
          cspc.push_back({ pr, { r, s }});
        }
      }
      p[i].t = WORD;
    }

    // <> -> <A>
    if (p[i].t == VAR) {
      p[i].t = POS;
      for (auto&pr: dict) {
        p[i].pos = pr.first;
        auto s = coverset(p, c);
        if (s.size() > 0) {
          Pattern r = tighten(p, s);
          Integer pr = priority(r, ell, s);
          cspc.push_back({ pr, { r, s }});
        }
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
      // cspc.push_back({ q, coverset(q, c) });
      auto s = coverset(q, c);
      if (s.size() > 0) {
        Pattern r = tighten(p, s);
        Integer pr = priority(r, ell, s);
        cspc.push_back({ pr, { r, s }});
      }
    }
  }

  if (DEBUG) {
    cerr << "--- cspc" << endl;
    for (auto& a: cspc) {
      cerr << a << endl;
    }
    cerr << "---" << endl;
  }

  vector<pair<Pattern, vi>> div;

  if (cspc.size() == 0) {
    if (DEBUG) { cerr << "cspc.size() == 0" << endl; }
    return div;
  }

  const
  auto sort_by_setsize = [](
      const pair<Integer, pair<Pattern, set<int>>>& x,
      const pair<Integer, pair<Pattern, set<int>>>& y) {
    return x.first > y.first;
  };

  { // 貪欲に近似の最小集合被覆
    int i = 0;
    set<int> m;
    while (m.size() < M) {
      sort(cspc.begin() + i, cspc.end(), sort_by_setsize);
      if (cspc[i].second.second.size() > 0) {
        vi v;
        for (int id: cspc[i].second.second) {
          v.push_back(id);
          if (m.count(id) == 0) {
            m.insert(id);
            for (int j = i + 1; j < cspc.size(); ++j) cspc[j].second.second.erase(id);
          }
        }
        Pattern& p = cspc[i].second.first;
        // cerr << "tighten of " << p << " where " << v << endl;
        p = tighten(p, v); // ここでついでに tightest にしておこう
        div.push_back({ p, v });
      }
      ++i;
    }
  }

  if (DEBUG) {
    cerr << "following " << div.size() << " patterns" << endl;
    for (auto& p: div) cerr << " -- " << p.first << " where " << p.second << endl;
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


void init(vector<Text>& _docs, bool DEBUG) {
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

  if (DEBUG) {
    trace(alphabet_size);
    for (auto&pr: class_size) trace(pr);
  }

}


vector<Pattern> kmmg(int K)
{
  const int n = docs.size();

  vi ids(n); rep(i, n) ids[i] = i;

  Pattern top { PUnit() };
  auto pc = tighten(top, ids);

  // いくつのパターンに被覆されているか
  vi cover_count(n, 1);

  // (priority, Pattern, cover)
  priority_queue<pair<Integer, pair<Pattern, vi>>> pcs;
  pcs.push({ 1, { pc, ids }});
  vector<Pattern> ret;

  /* division */
  while (not pcs.empty())
  {
    auto tpl = pcs.top(); pcs.pop();
    Pattern& p = tpl.second.first;
    vi& c = tpl.second.second;

    vector<pair<Pattern, vi>> dPC = division(p, c);

    // when not divisible
    if (dPC.size() < 2) {
      if (DEBUG) { cerr << "a pattern " << p << " is not divisible" << endl; }
      ret.push_back(p);
      continue;
    }

    // when |P| > K
    if (ret.size() + pcs.size() + dPC.size() > K) {
      if (DEBUG) {
        cerr << "#pattern is over K=" << K << endl;
        trace(ret.size()); trace(pcs.size()); trace(dPC.size());
      }
      ret.push_back(p);
      while (not pcs.empty()) {
        ret.push_back(pcs.top().second.first);
        pcs.pop();
      }
      return ret;
    }

    for (auto& qc: dPC) {
      Pattern& q = qc.first;
      if (RANDOM_PRIORITY) {
        pcs.push({ rand()%20, { q, qc.second }});
      } else {
        int ell = upper_length(qc.second);
        Integer pr = log(language_size(q, ell));
        pcs.push({ pr, { q, qc.second }});
      }
    }

    // update counter
    for (auto& qc: dPC) {
      for (int i: qc.second) ++cover_count[i];
    }

  }
  return ret;
}


/*
 * construct a NFA
 * s.t.  L(NFA) = L(p)
 *
 * NFA nfa = vector<>;
 * using state = int;
 * nfa[state] = map<PUnit, vector<state>>
 */
inline
vector<map<PUnit, vi>>
NFA(const Pattern& p, int ell, bool DEBUG)
{
  vector< map<PUnit, vi> > nfa(1); // nfa[state][alphabet] = { next states }
  int n = 0;
  for (const PUnit& a: p)
  {
    nfa[n][a].push_back( n+1 );
    nfa.push_back({});
    if (a.t == VAR) {
      nfa[n+1][a].push_back( n+1 );
    }
    n++;
  }
  if (DEBUG) {
    for (int i = 0; i < nfa.size(); ++i) {
      cout << "#" << i << " " << nfa[i] << endl;
    }
  }
  return nfa;
}


/*
 * construct DFA (via NFA)
 * s.t. L(DFA) = L(p)
 */
inline
pair<
  map<State, map<PUnit, State>>,
  int
>
DFA(const Pattern& p, int ell, bool DEBUG)
{
  auto nfa = NFA(p, ell, DEBUG);
  int n = nfa.size();
  if (DEBUG) { trace(nfa.size()); }

  // bit で NFA State の集合を表現します.
  // 初期状態は0bit目だけ立ってるので 1 です.
  assert (nfa.size() < 8*sizeof(State));

  map<State, map<PUnit, State>> dfa; // dfa[state][alphabet] = next state

  { // BFS
    queue<State> que; que.push(1);
    map<State, bool> visited;
    const PUnit var;

    while (not que.empty())
    {
      State P = que.front(); que.pop();
      if (visited[P]) continue;

      vector<int> states; // in NFA
      for (int p = 0; p < n; ++p) { // P -> {p}
        if ((1 << p) & P) states.push_back(p);
      }

      set<PUnit> readable_pos, readable_words;
      {
        for (int p: states) {
          for (auto& pr: nfa[p]) {
            const PUnit&a = pr.first;
            if (a.t == POS) { readable_pos.insert(a); }
            else if (a.t == WORD) { readable_words.insert(a); }
          }
        }
      }

      for (const PUnit& a: readable_words) {
        assert(a.t == WORD);
        PUnit pos_of_a(a.pos);
        State Q = 0;
        for (int p: states) {
          if (nfa[p].count(a) > 0) {
            for (int q: nfa[p][a])  Q |= 1 << q;
          }
          if (nfa[p].count(pos_of_a) > 0) {
            for (int q: nfa[p][pos_of_a]) Q |= 1 << q;
          }
          if (nfa[p].count(var) > 0) {
            for (int q: nfa[p][var]) Q |= 1 << q;
          }
        }
        que.push(Q);
        dfa[P][a] = Q;
      }

      for (const PUnit& a: readable_pos) { // reads POS - readble_words
        assert(a.t == POS);
        State Q = 0;
        for (int p: states) {
          if (nfa[p].count(a) > 0) {
            for (int q: nfa[p][a])  Q |= 1 << q;
          }
          if (nfa[p].count(var) > 0) {
            for (int q: nfa[p][var]) Q |= 1 << q;
          }
        }
        que.push(Q);
        dfa[P][a] = Q;
      }

      { // reads rest characters
        State Q = 0;
        for (int p: states) {
          if (nfa[p].count(var) > 0) {
            for (int q: nfa[p][var]) Q |= 1 << q;
          }
        }
        que.push(Q);
        dfa[P][var] = Q;
      }

      if (DEBUG) {
        cerr << "#" << d2b(P, n);
        for (auto& pr: dfa[P]) {
          cerr << ' ' << make_pair(pr.first, d2b(pr.second, n));
        } cerr << endl;
      }

      visited[P] = true;
    }
  }
  return make_pair(dfa, n);
}


inline
Integer language_size_sub(const Pattern& p, int ell, bool DEBUG)
{
  auto automata = DFA(p, ell, DEBUG);
  auto dfa = automata.first;
  const int n = automata.second;

  // adjacency matrix of the DFA
  int idx_init = -1,
      idx_final = -1;
  int id;
  vvi M;
  {
    map<State, int> state_id; // re-numbering State
    id = 0;
    for (auto&pr: dfa) {
      State P = pr.first;
      if (state_id.count(P) > 0) continue;
      if (P == 1) {
        idx_init = id++;
        state_id[P] = idx_init;
      }
      else if (P & (1 << (n-1))) {
        if (idx_final < 0) {
          idx_final = id++;
        }
        state_id[P] = idx_final;
      }
      else {
        state_id[P] = id++;
      }
    }

    if (DEBUG) {
      cerr << "re-numbering DFA states" << endl;
      for (auto&pr: state_id) {
        cerr << "# " << d2b(pr.first, n) << " => " << pr.second << endl;
      }
    }

    M.resize(id);
    rep (i, id) M[i] = vi(id, 0);

    { // count up alphabets
      for (auto&pr: dfa)
      {
        State P = pr.first;
        int state_of_P = state_id[P];
        map<PUnit, State>& m = pr.second;

        map<string, int> visited; // visited[POS]
        int num_of_else = alphabet_size;

        for (auto it = m.rbegin(); it != m.rend(); ++it) {
          State Q = it->second;
          int state_of_Q = state_id[Q];
          if (it->first.t == WORD) {
            M[state_of_P][state_of_Q] += 1;
            visited[it->first.pos]++;
            num_of_else--;
          }
          else if (it->first.t == POS) {
            string pos = it->first.pos;
            int x = max(0, class_size[pos] - visited[pos]);
            M[state_of_P][state_of_Q] += x;
            num_of_else -= x;
          }
          else { // <>
            M[state_of_P][state_of_Q] += num_of_else;
          }
        }
      }

      rep (i, id) {
        rep (j, id) {
          M[i][j] = min(M[i][j], alphabet_size);
        }
      }

      if (DEBUG) {
        cerr << "M =" << endl;
        rep (i, id) {
          rep (j, id) { cerr << ' ' << M[i][j]; }
          cerr << endl;
        }
      }
    }
  }

  // walking!
  vector<Integer> V(id, 0), U(id);
  V[idx_init] = 1;
  Integer sum = 0;

  rep (_, ell) {
    rep (i, id) {
      Integer ac = 0;
      rep (j, id) {
        ac += V[j] * M[j][i];
      }
      U[i] = ac;
    }
    V = U;
    sum += V[idx_final];
  }
  return sum;
}


/*
 * |L^{<= ell}(p)|
 */
Integer language_size(const Pattern& p, int ell, bool DEBUG) // about
{
  Integer ret = 1;
  for (auto&u: p) {
    if (u.t == WORD) --ell;
    else if (u.t == POS) {
      --ell;
      ret *= class_size[u.pos];
    }
  }
  while (ell > 0) {
    --ell;
    ret *= alphabet_size;
  }
  return ret;
}

/*
Integer language_size(const Pattern& p, int ell, bool DEBUG) // exact
{
  const int n = p.size();
  if (ell < n) return 0;
  Integer ac = 1;
  int i = 0,
      j = n - 1;
  // remove head (a, <A>)
  for (; i <= j; ++i) {
    if (p[i].t == VAR) break;
    if (p[i].t == POS) { ac *= class_size[p[i].pos]; }
    --ell;
  }
  // remove tail (a, <A>)
  for (; i <= j; --j) {
    if (p[j].t == VAR) break;
    if (p[j].t == POS) { ac *= class_size[p[j].pos]; }
    --ell;
  }
  Pattern r;
  for (; i <= j; ++i) r.push_back(p[i]);
  if (r.size() == 0) return ac;
  return ac * language_size_sub(r, ell, DEBUG);
}
*/

