class Solution {
public:
    vector<vector<int>> merge(vector<vector<int>>& arr) {


        sort(arr.begin(),arr.end());


        vector<vector<int>> ans;


        for(int i=0;i<arr.size();i++){


            vector<int> temp = arr[i];


            while(i<arr.size() && temp[1]>=arr[i][0]){
                temp[1]=max(arr[i][1],temp[1]);
                i++;
            }


            if(i<arr.size() && temp!=arr[i] )
            {
                i--;
               
            }


             ans.push_back(temp);
        }

        return ans;


       


    }
};