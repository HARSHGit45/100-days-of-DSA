class Solution {
public:
    int eraseOverlapIntervals(vector<vector<int>>& arr) {

        sort(arr.begin(),arr.end());

        
        
        int cnt=0;

        int end = arr[0][1];


        for(int i=1;i<arr.size();i++)
        {
            if(arr[i][0]<end){
                cnt++;
                end=min(end,arr[i][1]);
            }
            else{
                end=arr[i][1];
            }
        }

        return cnt;
        
    }
};