class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {


        map<int,int> mp1;

        for(int i=0;i<nums.size();i++){
            mp1[nums[i]]++;
        }

        multimap<int,int,greater<int>> mp2;

        for(auto it : mp1){
            mp2.insert({it.second,it.first});
        }


        vector<int> ans;

        int n=0;

        for(auto it : mp2 ){

            if(n>=k){
                break;
            }

            ans.push_back(it.second);
            n++;



        }

        return ans;
        

    }
};