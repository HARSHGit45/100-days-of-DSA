int fst(vector<int>& arr,int k){
    int n= arr.size();

    int s=0;
    int e=n-1;
    int  mid= s+((e-s)/2);
    int fs=-1;

    while(s<=e){
        if(arr[mid]==k){
            fs=mid;
            e=mid-1;
        }
        else if(arr[mid]<k){
            s=mid+1;
        }
        else{
            e=mid-1;

        }

        mid=s+((e-s)/2);

    }

    return fs;

}


int lst(vector<int>& arr,int k){
    int n= arr.size();

    int s=0;
    int e=n-1;
    int  mid= s+((e-s)/2);
    int ls=-1;

    while(s<=e){
        if(arr[mid]==k){
            ls=mid;
            s=mid+1;
        }
        else if(arr[mid]<k){
            s=mid+1;
        }
        else{
            e=mid-1;

        }

        mid=s+((e-s)/2);

    }

    return ls;

}



class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {

        int fs=fst(nums,target);
        int ls=lst(nums,target);

        vector<int> rs;
        rs.push_back(fs);
        rs.push_back(ls);

        return rs;

        
    }
};