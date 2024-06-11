class Solution {
public:
    int threeSumClosest(vector<int>& nums, int target) {

        int n =nums.size();
        sort(nums.begin(),nums.end());
        int ans= INT_MAX;

        int res=0;


        for(int i=0;i<n;i++){
            int j=i+1;
            int k=n-1;


            while(j<k){
                int sm=nums[i]+nums[j]+nums[k];

                if(sm==target){
                    return sm;
                }
                else if(abs(sm-target)<abs(ans)){
                    ans=sm-target;
                    res=sm;
                }
                if(sm>target){
                    k--;
                }
                else{
                    j++;
                }
            }
        }

        return res;




        


        
    }
};