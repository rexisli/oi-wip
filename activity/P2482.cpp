#include <bits/stdc++.h>
#define mem(x,y) memset(x,y,sizeof(x))
//#define int long long
#define endl "\n"
using namespace std;

int n; // 猪数
int m; // 牌数

bool MP_drop = false;

struct Pig {
	string identity; // 身份
	vector<char> card; // 手牌

	int HP = 4; // 体力
	bool is_dead = false;
	bool has_equiped_Z = false; // 诸葛连弩
	string showed_identity = "NA";
	map<int, int> recognization; // 0不认识 1友方 2敌方

	inline bool if_full_HP() {
		return HP == 4;
	}
};

vector<Pig> pig;

int cur_card;
vector<char> cards;

inline char get_nxt_card() {
	if (++cur_card <= m) return cards[cur_card];
	else return cards[m];
}

inline void read() {
	cin >> n >> m;

	pig.resize(n + 1);
	for (int i = 1; i <= n; i++) {
		cin >> pig[i].identity;
		for (int j = 1; j <= 4; j++) {
			char c;
			cin >> c;
			pig[i].card.push_back(c);
		}
	}

	cards.resize(m + 1);
	for (int i = 1; i <= m; i++) {
		cin >> cards[i];
	}
}

inline int get_nxt_player(int i) {
	if (++i > n) i = 1;
	if (pig[i].is_dead) return get_nxt_player(i);
	return i;
}

inline void catch_card(int idx) {
	pig[idx].card.push_back(get_nxt_card());
}

inline bool Duo(int idx) {
	for (int i = 0; i < pig[idx].card.size(); i++) {
		if (pig[idx].card.at(i) == 'D') {
			pig[idx].card.erase(pig[idx].card.begin() + i);
			return true;
		}
	}
	return false;
}

inline bool use_P_to_survive(int idx) {
	for (int i = 0; i < pig[idx].card.size(); i++) {
		if (pig[idx].card.at(i) == 'P') {
			pig[idx].card.erase(pig[idx].card.begin() + i);
			return true;
		}
	}
	return false;
}

inline string get_winner() {
	for (int i = 1; i <= n; i++) {
		if (!pig[i].is_dead && pig[i].identity == "MP") return "MP";
		if (!pig[i].is_dead && pig[i].identity == "FP") return "FP";
	}
	return "NA";
}

inline bool check_end() {
	int cnt_F = 0;
	for (int i = 1; i <= n; i++) {
		if (pig[i].identity == "MP" && pig[i].is_dead) return true;
		if (pig[i].identity == "FP" && !pig[i].is_dead) cnt_F++;
	}
	return cnt_F == 0;
}

inline bool check_dead(int idx) {
	if (pig[idx].HP == 0) {
		if (!use_P_to_survive(idx)) {
			pig[idx].is_dead = true;
			return true;
		}
	}
	return false;
}

inline void get_reward(int u, int v) {
	if (pig[v].identity == "FP") {
		catch_card(u);
		catch_card(u);
		catch_card(u);
	} else if (pig[v].identity == "ZP" && pig[u].identity == "MP") {
		MP_drop = true;
	}
}

inline void Kill(int u, int v) {
	pig[u].recognization[v] = 2;
	pig[v].recognization[u] = 2;
	if (!Duo(v)) {
		pig[v].HP--;
		if (check_dead(v)) {
			get_reward(u, v);
		}
	}
}

inline bool use_Kill_to_avoid_Fight(int idx) {
	for (int i = 0; i < pig[idx].card.size(); i++) {
		if (pig[idx].card.at(i) == 'K') {
			pig[idx].card.erase(pig[idx].card.begin() + i);
			return true;
		}
	}
	return false;
}

inline void Fight(int u, int v) {
	if (pig[u].identity == "MP" && pig[v].identity == "ZP") {
		pig[v].HP--;
		if (check_dead(v)) {
			get_reward(u, v);
		}
		return;
	}
	pig[u].recognization[v] = 2;
	pig[v].recognization[u] = 2;
	int cur = 1;
	while (true) {
		if (cur == 1) {
			if (!use_Kill_to_avoid_Fight(u)) {
				pig[v].HP--;
				if (check_dead(v)) {
					get_reward(u, v);
				}
				break;
			}
		} else {
			if (!use_Kill_to_avoid_Fight(u)) {
				pig[u].HP--;
				if (check_dead(u)) {
					get_reward(u, v);
				}
				break;
			}
		}
		cur = (cur + 1) % 2;
	}
}

inline void use_card(int idx) {
	int card_idx = 0;
	bool used_kill = false;
	vector<int> used;
	while (true) {
		if (card_idx == (int) pig[idx].card.size()) break;

		bool used_card = false;

		if (pig[idx].card.at(card_idx) == 'P') {
			if (pig[idx].if_full_HP()) continue;
			pig[idx].HP++;
			used_card = true;
		} else if (pig[idx].card.at(card_idx) == 'K') {
			if (used_kill && !pig[idx].has_equiped_Z) continue;
			int target = get_nxt_player(idx);
			if (target == idx) continue;
			if (pig[idx].recognization[target] == 2) {
				Kill(idx, target);
				used_kill = true;
				used_card = true;
				if (MP_drop) break;
			}
		} else if (pig[idx].card.at(card_idx) == 'F') {
			int target = get_nxt_player(idx);
			while (target != idx) {
				if (pig[idx].recognization[target] == 2) {
					Fight(idx, target);
					used_card = true;
					break;
				}
				target = get_nxt_player(target);
			}
		}

		if (used_card) {
			used.push_back(card_idx);
			card_idx++;
		} else break;
	}

	if (MP_drop) {
		pig[idx].card.clear();
		pig[idx].has_equiped_Z = false;
		MP_drop = false;
		return;
	}

	for (int i = used.size() - 1; i >= 0; i--) {
		pig[idx].card.erase(pig[idx].card.begin() + used[i]);
	}
}

inline void solve() {
	int st = 1;
	for (; st <= n; st++) if (pig[st].identity == "MP") break;

	pig[st].showed_identity = "MP";
	for (int i = 1; i <= n; i++) {
		if (pig[i].identity == "FP") {
			pig[i].recognization[st] = 2;
		}
		if (pig[i].identity == "ZP") {
			pig[i].recognization[st] = 1;
		}
	}

	int cur = st;
	while (!check_end()) {
		catch_card(cur);
		catch_card(cur);
		use_card(cur);

		cur = get_nxt_player(cur);
	}

	cout << get_winner() << endl;
	for (int i = 1; i <= n; i++) {
		if (pig[i].is_dead) cout << "DEAD" << endl;
		else {
			for (auto c: pig[i].card) cout << c << " ";
			cout << endl;
		}
	}
}

signed main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr), cout.tie(nullptr);
	read();
	solve();
	return 0;
}
