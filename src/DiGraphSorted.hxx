#pragma once
#include <vector>
#include <ostream>
#include <iostream>
#include "_main.hxx"

using std::vector;
using std::ostream;
using std::cout;




// DI-GRAPH SORTED
// ---------------

template <class V=NONE, class E=NONE>
class DiGraphSorted {
  public:
  using TVertex = V;
  using TEdge   = E;

  private:
  vector<int>  none;
  vector<bool> vex;
  vector2d<int> vto;
  vector2d<E> edata;
  vector<V>         vdata;
  int N = 0, M = 0;

  // Cute helpers
  private:
  int s() const { return vto.size(); }
  int ei(int u, int v) const {
    int i = lowerBoundIndex(vto[u], v);
    return i<0 || vto[u][i]!=v? -1 : i;
  }

  // Read operations
  public:
  int span()  const { return s(); }
  int order() const { return N; }
  int size()  const { return M; }

  bool hasVertex(int u)      const { return u < s() && vex[u]; }
  bool hasEdge(int u, int v) const { return u < s() && ei(u, v) >= 0; }
  auto edges(int u)          const { return u < s()? iterable(vto[u]) : iterable(none); }
  int degree(int u)          const { return u < s()? vto[u].size()    : 0; }
  auto vertices()      const { return filter(range(s()), [&](int u) { return  vex[u]; }); }
  auto nonVertices()   const { return filter(range(s()), [&](int u) { return !vex[u]; }); }
  auto inEdges(int v)  const { return filter(range(s()), [&](int u) { return ei(u, v) >= 0; }); }
  int inDegree(int v) const { return countIf(range(s()), [&](int u) { return ei(u, v) >= 0; }); }

  V vertexData(int u)   const { return hasVertex(u)? vdata[u] : V(); }
  void setVertexData(int u, V d) { if (hasVertex(u)) vdata[u] = d; }
  E edgeData(int u, int v)   const { return hasEdge(u, v)? edata[u][ei(u, v)] : E(); }
  void setEdgeData(int u, int v, E d) { if (hasEdge(u, v)) edata[u][ei(u, v)] = d; }

  // Write operations
  public:
  void addVertex(int u, V d=V()) {
    if (hasVertex(u)) return;
    if (u >= s()) {
      vex.resize(u+1);
      vto.resize(u+1);
      edata.resize(u+1);
      vdata.resize(u+1);
    }
    vex[u] = true;
    vdata[u] = d;
    N++;
  }

  void addEdge(int u, int v, E d=E()) {
    if (hasEdge(u, v)) return;
    addVertex(u);
    addVertex(v);
    int i = lowerBound(vto[u], v)-vb;
    insertIndex(vto[u], i, v);
    insertIndex(edata[u], i, d);
    M++;
  }

  void removeEdge(int u, int v) {
    if (!hasEdge(u, v)) return;
    int o = ei(u, v);
    eraseIndex(vto[u], o);
    eraseIndex(edata[u], o);
    M--;
  }

  void removeEdges(int u) {
    if (!hasVertex(u)) return;
    M -= degree(u);
    vto[u].clear();
    edata[u].clear();
  }

  void removeInEdges(int v) {
    if (!hasVertex(v)) return;
    for (int u : inEdges(v))
      removeEdge(u, v);
  }

  void removeVertex(int u) {
    if (!hasVertex(u)) return;
    removeEdges(u);
    removeInEdges(u);
    vex[u] = false;
    N--;
  }
};




// DI-GRAPH PRINT
// --------------

template <class V, class E>
void write(ostream& a, const DiGraphSorted<V, E>& x, bool all=false) {
  a << "order: " << x.order() << " size: " << x.size();
  if (!all) { a << " {}"; return; }
  a << " {\n";
  for (int u : x.vertices()) {
    a << "  " << u << " ->";
    for (int v : x.edges(u))
      a << " " << v;
    a << "\n";
  }
  a << "}";
}

template <class V, class E>
ostream& operator<<(ostream& a, const DiGraphSorted<V, E>& x) {
  write(a, x);
  return a;
}
