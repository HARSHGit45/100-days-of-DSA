class Solution {
public:
    bool check(vector<int>& arr) {
        int count=0;

        int n=arr.size();


        for(int i=1;i<n;i++){


            if(arr[i-1]>arr[i]){
                count++;
            }
        }


        if(arr[n-1]>arr[0]){
            count++;
        }



        if(count<=1){
            return true;
        }

        else{
            return false;
        }




        
    }
};