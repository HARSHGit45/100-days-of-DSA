class Solution {
public:
    int searchInsert(vector<int>& nums, int target) {


        int s=0;
        int e=nums.size();
        int mid= s+ ((e-s)/2);


 if(target > nums[e-1])
            {
                return e;
            }
        while(s<=e)
        {
           
    


            if(nums[mid]==target){
                return mid;
            }
            else if(nums[mid]>target){
                e=mid-1;
            }
            else{
                s=mid+1;
            }

            mid= s+((e-s)/2);

        }

        return s;
        
    }
};