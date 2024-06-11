class Solution {
public:
    vector<vector<int>> fourSum(vector<int>& nums, int target) {

        sort(nums.begin(),nums.end());

        vector<vector<int>> ans;
        int n = nums.size();

        for(int i=0;i<n;i++)
        {
            for(int j=i+1;j<n;j++){

                int a=j+1;
                int b=n-1;


                while(a<b){
                    long long sm=(long long)nums[i]+nums[j]+nums[a]+nums[b];


                    if(sm<target){
                        a++;
                    }
                    else if(sm>target){
                        b--;
                    }

                  
                  
                  
                  
                    else{
                        vector<int> an;
                        an.push_back(nums[i]);
                        an.push_back(nums[j]);
                        an.push_back(nums[a]);
                        an.push_back(nums[b]);
                    

                    if(find(ans.begin(),ans.end(),an)==ans.end()){
                        ans.push_back(an);
                    }

                    b--;
                    a++;
                    }
                    
                }
            }
        }

        return ans;
        
    }
};