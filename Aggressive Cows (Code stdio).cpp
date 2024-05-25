bool ispossible(vector<int> &arr,int n,int mid,int k){

int cow=1;
int lastcount=arr[0];

for(int i=0;i<n;i++)
{
    if(arr[i]-lastcount>=mid)
    {
        cow++;
        if(cow==k){
            return true;
        }
        lastcount=arr[i];
    }
}

return false;





}



int aggressiveCows(vector<int> &stalls, int k)
{

    sort(stalls.begin(),stalls.end());
    int n= stalls.size();

    int s=0;

    int e= *max_element(stalls.begin(),stalls.end());

    int mid = s+((e-s)/2);
    int ans=-1;

    while(s<=e){

        if(ispossible(stalls,n,mid,k)){
            ans=mid;
            s=mid+1;
        }

        else{
            e=mid-1;
        }

mid = s+((e-s)/2);


    }


    return ans;
}