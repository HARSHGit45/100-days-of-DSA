class Solution {
public:

    int maximumGap(vector<int>& arr) {


        if(arr.size()<2){
            return 0;
        }

      sort(arr.begin(),arr.end());

      int e=arr.size()-1;
      int mc=0;



      for(int i=e;i>0;i--){
        if((arr[i]-arr[i-1])>mc){
            mc=arr[i]-arr[i-1];
        }
      }

      return mc;


       

        
        
    }
};