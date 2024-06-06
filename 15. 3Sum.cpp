class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& arr) {

        int t=0;

        sort(arr.begin(),arr.end());

        set<vector<int>> s;


        for(int i=0;i<arr.size();i++)
        {
            int j = i+1;
            int k = arr.size()-1;

            while(j<k){

            int sum = arr[i]+arr[j]+arr[k];


            if(sum==t){
                s.insert({arr[i],arr[j],arr[k]});
                j++;
                k--;
            }
            else if(sum<t){
                j++;
            }
            else{
                k--;
            }

            }

            
        }

        vector<vector<int>> ans;

        for(auto i: s){
            ans.push_back(i);
        }

        return ans;
        
        
    }
};