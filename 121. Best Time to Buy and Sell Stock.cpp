class Solution {
public:
    int maxProfit(vector<int>& prices) {

     /*   int n = prices.size();
        int mc=0;
        for(int i=0;i<n;i++){
            for(int j=i+1;j<n;j++){

                if(prices[j]-prices[i]>mc){
                    mc=prices[j]-prices[i];
                }



            }
        }

        return mc;*/

        int n = prices.size();
        int mc=0;
        int buy=prices[0];
        int i=1;


        while(i<n){

            if(prices[i]-buy>mc){
                mc=prices[i]-buy;

            }
            if(prices[i]<buy){
                buy=prices[i];
            }

            i++;
        }
        

        return mc;
        
        
        
        
        
        }
};