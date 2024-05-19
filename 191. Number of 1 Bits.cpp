#include <vector>
using namespace std;

class Solution {
public:
    int hammingWeight(int n) {
        vector<int> binaryNum;
        int count = 0;

        while (n > 0) {
            binaryNum.push_back(n % 2);
            n = n / 2;
        }

        for (int i = 0; i < binaryNum.size(); i++) {
            if (binaryNum[i] == 1) {
                count++;
            }
        }

        return count;
    }
};
