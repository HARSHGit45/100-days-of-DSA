class Solution {
public:
    void rotate(vector<int>& nums, int k) {
       /* int n= nums.size();
        vector<int> temp(n);


        nums=temp;

        
    }*/

    k=k%nums.size();


    reverse(nums.begin(),nums.end());

    reverse(nums.begin(),nums.begin()+k);

    reverse(nums.begin()+k,nums.end());

	}  
};