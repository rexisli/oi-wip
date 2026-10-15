#include <bits/stdc++.h>
#define ONLINE(x) freopen(#x".in", "r", stdin), freopen(#x".out", "w", stdout)
#define mem(x,y) memset(x,y,sizeof(x))
//#define int long long
using namespace std;

inline void solve() {
	int n;
	cin >> n;
	string s1, s2, t1, t2;
	cin >> s1 >> s2 >> t1 >> t2;
	s1 = " " + s1, s2 = " " + s2;
	t1 = " " + t1, t2 = " " + t2;

	vector<int> mark(n + 1);
	for (int i = 1; i <= n; i++) {
		if (t1[i] == '1' && t2[i] == '0') mark[i] = 1;
		else if (t1[i] == '0' && t2[i] == '1') mark[i] = 2;
		else if (t1[i] == '1' && t2[i] == '1') mark[i] = 3;
	}

	int ans = 0;

	int cnt1_0 = 0, cnt1_1 = 0;
	int cnt2_0 = 0, cnt2_1 = 0;

	vector<int> rest1_0, rest1_1, rest2_0, rest2_1;
	vector<int> pos;

	auto calc = [&](int mk) -> void {
		int use0 = min(cnt1_0, cnt2_0);
		int use1 = min(cnt1_1, cnt2_1);
		cnt1_0 -= use0, cnt2_0 -= use0;
		cnt1_1 -= use1, cnt2_1 -= use1;
		ans += use0 + use1;
		rest1_0.push_back(cnt1_0);
		rest1_1.push_back(cnt1_1);
		rest2_0.push_back(cnt2_0);
		rest2_1.push_back(cnt2_1);
		pos.push_back(mk);
		cnt1_0 = cnt1_1 = cnt2_0 = cnt2_1 = 0;
	};

	// 先只处理完全可变的段落，有限制处记录两边多余个数
	for (int i = 1; i <= n; i++) {
		if (mark[i] == 3) {
			if (s1[i] == '0') cnt1_0++;
			else cnt1_1++;
			if (s2[i] == '0') cnt2_0++;
			else cnt2_1++;
		} else calc(mark[i]);
	}
	calc(0);

	// 有限制的端点可以使用两端的多余元素
	// 优先左端，然后使用右端

	for (int i = 0; i < (int) pos.size(); i++) {
		if (pos[i] == 0) continue;
		if (pos[i] == 1) {
			if (s2[i] == '0' && rest1_0[i]) ans++, rest1_0[i]--;
			else if (s2[i] == '1' && rest1_1[i]) ans++, rest1_1[i]--;
			else if (s2[i] == '0' && i < (int) pos.size() - 1 && rest1_0[i + 1]) ans++, rest1_0[i + 1]--;
			else if (s2[i] == '1' && i < (int) pos.size() - 1 && rest1_1[i + 1]) ans++, rest1_1[i + 1]--;
		} else {
			if (s1[i] == '0' && rest2_0[i]) ans++, rest2_0[i]--;
			else if (s1[i] == '1' && rest2_1[i]) ans++, rest2_1[i]--;
			else if (s1[i] == '0' && i < (int) pos.size() - 1 && rest2_0[i + 1]) ans++, rest2_0[i + 1]--;
			else if (s1[i] == '1' && i < (int) pos.size() - 1 && rest2_1[i + 1]) ans++, rest2_1[i + 1]--;
		}
	}

	cout << ans << "\n";

	// BUG:
	// 两限制端加中间段，中间段合并完全可能不优于一个向左一个向右
}

signed main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr), cout.tie(nullptr);
	int T;
	cin >> T;
	while (T--) solve();
	return 0;
}
