class Solution {
public:
    bool searchMatrix(vector<vector<int>>& m, int target) {
/*
my approach ok for time complx but large space complexity
        vector <int> s1;

        for(int i=0;i<m.size();i++){
            for(int j=0;j<m[i].size();j++){
                s1.push_back(m[i][j]);
            }
        }

         for(int i=0;i<s1.size();i++){
            cout<<s1[i]<<" ";
         }

        



        int s=0;
        int e=s1.size()-1;
        int mid = s+ (e-s)/2;

        while(s<=e){
            if(s1[mid]==target){
                return true;
            }
            else if(s1[mid]>target){
                e=mid-1;
            }
            else{
                s=mid+1;
            }

             mid = s+ (e-s)/2;



        }

        return false;
        */



        //optimal approach

        int row = m.size();
        int col = m[0].size();

        int s=0;
        int e = row*col-1;

        int mid = s+ (e-s)/2;

        while(s<=e){
            int el = m[mid/col][mid%col];

            if(el==target){
                return true;
            }
            else if(el>target){
                e=mid-1;
            }
            else{
                s=mid+1;

            }
            mid = s+ (e-s)/2;
        }


        return false;
        
    }
};