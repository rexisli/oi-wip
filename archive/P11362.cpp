#include <bits/stdc++.h>
#define ONLINE(x) freopen(#x".in", "r", stdin), freopen(#x".out", "w", stdout)
#define mem(x,y) memset(x,y,sizeof(x))
#define int long long
using namespace std;

const int mod = 1e9 + 7;

inline int qpow(int base, int idx, int p) {
	int res = 1;
	while (idx) {
		if (idx & 1) res = res * base % p;
		base = base * base % p;
		idx >>= 1;
	}
	return res;
}

inline void solve() {
	/*
	一元限制位置排序去重为 p_1 < ... < p_m（同位置不同 d => 0）。
	
	设 R_i 为 x_i 的可行取值集合，只会是 [1,v]、单点、或空：
	
	只有“从受限位置出发、一路 a_i 都等于当前强制值”的链会传播，
	撞进下一个受限位置且值不同时无解。
	
	失败仅在相邻受限位置 p < q 之间；各段用的 (a_i,b_i) 互不相同，独立相乘。
	
	令 L = q - p（段内 L 条边）：总方案 v^(2L)，
	致败 a_p=d_p、之后各 a_i=b_{i-1}、中间 b 自由(L-1 个)、最后 b_{q-1}!=d_q，
	bad = v^(L-1) * (v-1)
	所以每段贡献 v^(2L) - bad。
	
	c_min 之前、c_max 之后的边永远自由，每条 v^2。
	
	ans = v^(2*(n-1-(c_max-c_min))) * Π( v^(2L) - v^(L-1)*(v-1) )  (mod 1e9+7)
	*/
	
	int n, m, v;
	cin >> n >> m >> v;
	
	map<int, int> w;
	for (int i = 1; i <= m; i++) {
		int c, d;
		cin >> c >> d;
		if (w.count(c) && w[c] != d) {
			cout << 0 << "\n";
			return;
		}
		w[c] = d;
	}
	
	vector<int> lim;
	for (auto &i : w) lim.push_back(i.first);
	
	int ans = qpow(v, 2 * (n - 1 - (lim.back() - lim.front())), mod);
	
	for (int i = 0; i < (int)lim.size() - 1; i++) {
		int L = lim[i + 1] - lim[i];
		(ans *= (qpow(v, 2 * L, mod) - qpow(v, L - 1, mod) * (v - 1) % mod + mod) % mod) %= mod;
	}
	
	cout << ans << "\n";
}

signed main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr), cout.tie(nullptr);
//	ONLINE(assign2);
	int T;
	cin >> T;
	while (T--) solve();
	return 0;
}
