#include <bits/stdc++.h>

using namespace std;

template<typename T>
class cht {
private:
  static constexpr T inf = numeric_limits<T>::max() / 2;
public:
  struct line {
    T k, m, p;

    bool operator<(const line &b) const {
      return k < b.k;
    }

    bool operator<(T x) const {
      return p < x;
    }

    T eval(T x) const {
      return k * x + m;
    }
  };

  deque<line> hull;

  T floor_div(T a, T b) {
    return a / b - ((a ^ b) < 0 && a % b);
  }

  bool intersection(line &x, const line &y) {
    if (x.k == y.k) {
      x.p = (x.m > y.m) ? inf : -inf;
    } else {
      x.p = floor_div(y.m - x.m, x.k - y.k);
    }
    return x.p >= y.p;
  }

  void insert(T k, T m) {
    line l = {k, m, 0};
    while (int(hull.size()) >= 2 &&
           (intersection(l, hull.back()), intersection(hull.back(), hull[int(hull.size()) - 2]), l.p < hull.back().p)) {
      hull.pop_back();
    }
    hull.push_back(l);
  }

  T query(T x) {
    while (int(hull.size()) >= 2 && hull[0].eval(x) >= hull[1].eval(x)) {
      hull.pop_front();
    }
    return hull[0].eval(x);
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  long long c;
  cin >> n >> c;
  vector<long long> h(n), dp(n);
  for (int i = 0; i < n; ++i) {
    cin >> h[i];
  }
  cht<long long> cht;
  auto insert_line = [&](int i) {
    cht.insert(-h[i] * 2, dp[i] + (h[i] * h[i]));
  };
  dp[0] = 0;
  insert_line(0);
  for (int i = 1; i < n; ++i) {
    long long x = cht.query(h[i]);
    dp[i] = c + (h[i] * h[i]) + x;
    insert_line(i);
  }
  cout << dp[n - 1] << '\n';
  return 0;
}

// dp[i] : the min cost to travel from stone 1 to stone i
// dp[i] = max(dp[j], (h_i + h_j)^2 + C)
// dp[i] = C + h_i^2 + max(dp[j] + -2*h_i*h_j + h_j^2)
// dp[j] + -2*h_i*h_j + h_j^2 = (-2*h_j)x + (dp[j] + h_j^2) where x = h_i
