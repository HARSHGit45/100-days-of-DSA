class Solution {
public:
    int maximumElementAfterDecrementingAndRearranging(vector<int>& arr) {


        sort(arr.begin(),arr.end());



        int temp=0;

        int n = arr.size();

    
        for(int i=0;i<n;i++){

            arr[i]=min(temp+1,arr[i]);
            temp=arr[i];

        }
        


        return arr[n-1];

        
    }
};