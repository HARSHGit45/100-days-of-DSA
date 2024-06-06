class Solution {
public:
    int maxArea(vector<int>& arr) {


        int s=0;
        int e = arr.size()-1;
        int maxi=0;


        while(s<e){

            int area =min(arr[s],arr[e]) * (e-s);
            maxi=max(maxi,area);

            if(arr[s]<arr[e]){
                s++;
            }
            else{
                e--;
            }





        }

        return maxi;
        
    }
};