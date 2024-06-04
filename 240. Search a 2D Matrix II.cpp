class Solution {
public:
    bool searchMatrix(vector<vector<int>>& m, int target) {


        int row=m.size();
        int col=m[0].size();


        int r=0;
        int c = col-1;


        while(r<row && c>=0)
        {

            int el=m[r][c];

            if(el==target)
            {
                return true;
            }
            else if(el<target)
            {
                r++;

            }
            else
            {
                c--;
            }

        }
return false;

    }

        
    
};