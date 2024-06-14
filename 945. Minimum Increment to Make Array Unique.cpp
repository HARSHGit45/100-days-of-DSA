class Solution {
public:
    int minIncrementForUnique(vector<int>& nums) {

        int n =nums.size();

        sort(nums.begin(),nums.end());

        int count=0;

        for(int i=1;i<n;i++){
           
           if(nums[i]<=nums[i-1]){

            count=count + ((nums[i-1]+1)-nums[i]);

            nums[i]=nums[i-1]+1;
           }

        }
return count;

        
    }
};