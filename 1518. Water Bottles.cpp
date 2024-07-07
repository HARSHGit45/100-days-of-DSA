class Solution {
public:
    int numWaterBottles(int numBottles, int numExchange) {

        int tot=numBottles;

        while(numBottles>=numExchange)
        {
            tot+=numBottles/numExchange;

            numBottles= (numBottles/numExchange) + (numBottles%numExchange);
        }

        return tot;
        
    }
};