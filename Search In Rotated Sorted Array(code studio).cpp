int pivot(vector<int>& nums,int n)
{
    int s=0;
    int e=n-1;
    int mid= s+((e-s)/2);

    while(s<e)
    {
        if(nums[mid]>=nums[0])
        {
            s=mid+1;
        }
        else{
            e=mid;
        }

        mid= s+((e-s)/2);
    }
return s;



}


int bin(vector<int>& arr , int s, int e, int target)
{
    int mid=s+((e-s)/2);

    while(s<=e)
    {
        if(arr[mid]==target)
        {
            return mid;
        }
        else if(arr[mid]>target)
        {
            e=mid-1;
        }
        else{
            s=mid+1;
        }

        mid=s+((e-s)/2);

    }

    return -1;
}




int search(vector<int>& nums, int n, int target)
{

           
        int piv=pivot(nums,n);

        if(target>=nums[piv] && target<=nums[n-1]){

           return bin(nums,piv,n-1,target);

        }
        else
        {
            return bin(nums,0,piv-1,target);
        }
        

    



}
