class Solution {
public:
    int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
        int n=gas.size();

        int path=0;
        int total=0;
        int ans=0;


        for(int i=0;i<n;i++){
            total = total + gas[i]-cost[i];
            path= path + gas[i] - cost[i];

            if(path<0){
                ans=i+1;
                path=0;
            }

        }

        if(total<0){
            return -1;
        }
        else{
            return ans;
        }
        
    }
};