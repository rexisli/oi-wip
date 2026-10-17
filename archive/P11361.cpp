#include <bits/stdc++.h>
#define ONLINE(x) freopen(#x".in", "r", stdin), freopen(#x".out", "w", stdout)
#define mem(x,y) memset(x,y,sizeof(x))
//#define int long long
using namespace std;

struct node {
	int cnt0 = 0, cnt1 = 0;
};

inline void solve() {
	int n;
	cin >> n;
	string s1, s2, t1, t2;
	cin >> s1 >> s2 >> t1 >> t2;
	s1 = " " + s1, s2 = " " + s2;
	t1 = " " + t1, t2 = " " + t2;

	auto add = [&](char c, int &pos0, int &pos1) -> void {
		if (c == '0') pos0++;
		else pos1++;
	};

	vector<int> head1(n + 1), head2(n + 1);
	vector<node> val1(n + 1), val2(n + 1);
	for (int i = 1; i <= n; i++) {
		if (t1[i] == '1' && t1[i - 1] == '1') head1[i] = head1[i - 1];
		else head1[i] = head1[i - 1] + 1;
		if (t2[i] == '1' && t2[i - 1] == '1') head2[i] = head2[i - 1];
		else head2[i] = head2[i - 1] + 1;
		add(s1[i], val1[head1[i]].cnt0, val1[head1[i]].cnt1);
		add(s2[i], val2[head2[i]].cnt0, val2[head2[i]].cnt1);
	}

	int ans = 0;

	for (int i = 1; i <= n; i++) {
		if (val1[head1[i]].cnt0 && val2[head2[i]].cnt0) {
			ans++;
			val1[head1[i]].cnt0--;
			val2[head2[i]].cnt0--;
		} else if (val1[head1[i]].cnt1 && val2[head2[i]].cnt1) {
			ans++;
			val1[head1[i]].cnt1--;
			val2[head2[i]].cnt1--;
		}
	}

	cout << ans << "\n";
}

signed main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr), cout.tie(nullptr);
	int T;
	cin >> T;
	while (T--) solve();
	return 0;
}
