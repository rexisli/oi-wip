#include <bits/stdc++.h>
#define ONLINE(x) freopen(#x".in", "r", stdin), freopen(#x".out", "w", stdout)
#define mem(x,y) memset(x,y,sizeof(x))
#define int long long
using namespace std;

const int INF = 0x3f3f3f3f3f3f3f3f;

inline void solve() {
	int n, k, x, y;
	cin >> n >> k >> x >> y;
	vector<int> X(n + 1), Y(n + 1);
	for (int i = 1; i <= n; i++) cin >> X[i] >> Y[i];

	vector<int> preX(n + 1), preY(n + 1);
	for (int i = 1; i <= n; i++)
		preX[i] = preX[i - 1] + X[i], preY[i] = preY[i - 1] + Y[i];

	/*
	 * m = q*n + r;
	 *
	 * |x - preX[r] - q*preX[n]| + |y - preY[r] - q*preY[n]| <= k * (q*n + r)
	 *
	 * X = x - preX[r] - q*preX[n], Y = y - preY[r] - q*preY[n], Z = k * (q*n + r)
	 *
	 * |X| + |Y| = max(X + Y, X - Y, -X + Y, -X - Y)
	 *
	 * |X| + |Y| <= Z
	 *
	 * s1, s2 ∈ {-1, 1}
	 * s1 * X + s2 * Y <= Z
	 * s1 * (x - preX[r] - q*preX[n]) + s2 * (y - preY[r] - q*preY[n]) <= k * (q*n + r)
	 * s1 * (x - preX[r]) + s2 * (y - preY[r]) - (s1 * preX[n] + s2 * preY[n]) * q  <= k * n * q + k * r
	 * (s1 * preX[n] + s2 * preY[n] + k * n) * q >= s1 * (x - preX[r]) + s2 * (y - preY[r]) - k * r
	 *
	 * A = (s1 * preX[n] + s2 * preY[n] + k * n), B = s1 * (x - preX[r]) + s2 * (y - preY[r]) - k * r
	 *
	 * q >= B / A
	 */

	int ans = INF;

	auto get_q = [&](int r) -> int {
		int L = 0, R = INF;
		int s1 = 1, s2 = 1;
		do {
			do {
				int A = (s1 * preX[n] + s2 * preY[n] + k * n);
				int B = s1 * (x - preX[r]) + s2 * (y - preY[r]) - k * r;

				if (A > 0) L = max(L, (int) ceil((long double) B / A));
				else if (A < 0) R = min(R, (int) floor((long double) B / A));
				else if (B > 0) return INF;

				s2 = -s2;
			} while (s2 != 1);
			s1 = -s1;
		} while (s1 != 1);
		if (L > R) return INF;
		return L;
	};

	for (int r = 0; r < n; r++) {
		int q = get_q(r);
		if (q == INF) continue;
		ans = min(ans, q * n + r);
	}

	if (ans == INF) cout << -1 << "\n";
	else cout << ans << "\n";
}

signed main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr), cout.tie(nullptr);
	int T;
	cin >> T;
	while (T--) solve();
	return 0;
}
