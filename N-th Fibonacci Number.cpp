#include <bits/stdc++.h>

using namespace std;

const int mod = 1e9 + 7;

vector<vector<long long>> multiply(const vector<vector<long long>>& a, const vector<vector<long long>>& b) {

    int n = a.size();

    vector<vector<long long>> c(n, vector<long long>(n));

    for (int i = 0; i < n; ++i) {

        for (int j = 0; j < n; ++j) {

            for (int k = 0; k < n; ++k) {

                c[i][j] = (c[i][j] + a[i][k] * b[k][j]) % mod;

            }

        }

    }

    return c;

}

vector<vector<long long>> matrixExpo(vector<vector<long long>>& base, int exp) {

    int n = base.size();

    vector<vector<long long>> result(n, vector<long long>(n));

    for (int i = 0; i < n; ++i) {

        result[i][i] = 1;  // Identity matrix

    }

    while (exp > 0) {

        if (exp % 2 == 1) {

            result = multiply(result, base);

        }

        base = multiply(base, base);

        exp /= 2;

    }

    return result;

}

int fibonacciNumber(int n) {

    if (n == 1 || n == 2) {

        return 1;

    }

    vector<vector<long long>> base = {

        {1, 1},

        {1, 0}

    };

    vector<vector<long long>> result = matrixExpo(base, n - 1);

    return result[0][0];  // F(n)

}