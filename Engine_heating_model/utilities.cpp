#include "utilities.h"

int gaussian(std::vector<std::vector<double>> a, std::vector<double>& ans) {
	int n = (int)a.size();
	int m = (int)a[0].size() - 1;
	double eps = 0.000001;

	cout << "in gaussian:" << endl;
	cout << a[0][0] << " " << a[0][1] << endl;
	cout << a[1][0] << " " << a[1][1] << endl;

	std::vector<int> where(m, -1);
	for (int col = 0, row = 0; col < m && row < n; ++col) {
		int sel = row;
		for (int i = row; i < n; ++i)
			if (abs(a[i][col]) > abs(a[sel][col]))
				sel = i;
		if (abs(a[sel][col]) < eps)
			continue;
		for (int i = col; i <= m; ++i)
			std::swap(a[sel][i], a[row][i]);
		where[col] = row;

		for (int i = 0; i < n; ++i)
			if (i != row) {
				double c = a[i][col] / a[row][col];
				for (int j = col; j <= m; ++j)
					a[i][j] -= a[row][j] * c;
			}
		++row;
	}

	ans.assign(m, 0);
	for (int i = 0; i < m; ++i)
		if (where[i] != -1)
			ans[i] = a[where[i]][m] / a[where[i]][i];
	for (int i = 0; i < n; ++i) {
		double sum = 0;
		for (int j = 0; j < m; ++j)
			sum += ans[j] * a[i][j];
		if (abs(sum - a[i][m]) > eps)
			return 0;
	}

	for (int i = 0; i < m; ++i)
		if (where[i] == -1)
			return -1;
	return EXIT_SUCCESS;
}

bool is_float_number(const std::string& string) {
	std::string::const_iterator it = string.begin();
	bool decimalPoint = false;
	int minSize = 0;
	if (string.size() > 0 && (string[0] == '-' || string[0] == '+')) {
		it++;
		minSize++;
	}
	while (it != string.end()) {
		if (*it == '.') {
			if (!decimalPoint) decimalPoint = true;
			else break;
		}
		else if (!isdigit(*it) && ((*it != 'f') || it + 1 != string.end() || !decimalPoint)) {
			break;
		}
		++it;
	}
	return string.size() > minSize && it == string.end();
}