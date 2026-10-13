#include <bits/stdc++.h>
#define ONLINE(x) freopen(#x".in", "r", stdin), freopen(#x".out", "w", stdout)
#define mem(x,y) memset(x,y,sizeof(x))
//#define int long long
using namespace std;

const int mod = 998244353;

int pow2[10005];
int C[10005][10005];

inline void init() {
	pow2[0] = 1;
	for (int i = 1; i <= 10000; i++) pow2[i] = pow2[i - 1] * 2 % mod;
	C[0][0] = 1;
	for (int i = 1; i <= 10000; i++) {
		C[i][0] = 1;
		for (int j = 1; j <= i; j++)
			C[i][j] = (C[i - 1][j - 1] + C[i - 1][j]) % mod;
	}
}

inline void solve() {
	int n, m;
	cin >> n >> m;
	vector<int> a(n + 1);
	for (int i = 1; i <= n; i++) cin >> a[i];

	sort(a.begin() + 1, a.end(), greater<int>());

	int ans = pow2[n];

	for (int y = 1; y <= n; y++) {
		// y：第一个没买到的、定价为 2 的糖果（原价较大）
		int ptr = n + 1;
		for (int z = y + 1; z <= n; z++) {
			// z：最后一个买到的、定价为 1 的糖果（原价较小）
			if (a[y] == a[z] || m - y - 1 < 0) continue;
			if (a[z] * 2 <= a[y]) break;

			while (ptr > 1 && a[ptr - 1] + a[z] < a[y]) ptr--;
			int cnt = n - ptr + 1; // 满足 a[w]+a[z] < a[y] 的 w 的个数

			ans = (ans - 1LL * C[z - 2][m - y - 1] * pow2[cnt] % mod + mod) % mod;
		}
	}

	cout << ans << "\n";
}

/*
正难则反：

原价降序排序后（下标越小原价越大）：

y < x < z
x：在 z 之前买到的、定价为 1 的糖果中
y：第一个没买到的、定价为 2 的糖果
z：最后一个买到的、定价为 1 的糖果

不可行条件：
	a[y] / 2 < a[z] 且  a[y] != a[z]
	所以小 R 会买 z 之前的一些定价 1、2，z，
	导致后面遇到 y 时钱不够 2 元，于是跳过 y。

z 之后（下标更大）的原价极小糖果 w，满足 a[w] + a[z] < a[y]。
它们无论定价 1 还是 2，都排在 y 后面，不影响购买过程，
所以可以任意定价，贡献 2^cnt。

组合数 C[z-2][m-y-1]：
	在 z 之前、除 y 外的糖果中，选恰好 m-y-1 个定价为 1，
	其余定价为 2。这些定价 1 包括 x。
	这样能保证 y 确实是第一个没买到的定价 2。
*/

signed main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr), cout.tie(nullptr);
	init();
	int c, t;
	cin >> c >> t;
	while (t--) solve();
	return 0;
}
