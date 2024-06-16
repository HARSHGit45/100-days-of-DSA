class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int n = prices.size();
        int prof=0;
        int buy=prices[0];
        int mx=0;


        for(int i=0;i<n;i++){

            if(prices[i]<buy){
                buy=prices[i];
            }
            else{
                prof=prices[i]-buy;
                buy=prices[i];
                mx=mx+prof;
            }
        }

        return mx;

    }
};