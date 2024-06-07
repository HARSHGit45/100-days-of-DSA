class Solution {
public:
    int removeDuplicates(vector<int>& arr) {

        int n = arr.size();

        vector<int> ans;

        ans.push_back(arr[0]);

        for(int i=1;i<n;i++){
            if(arr[i-1]!=arr[i]){
                ans.push_back(arr[i]);
            }
        }

        arr=ans;

        return ans.size();

        
    }
};