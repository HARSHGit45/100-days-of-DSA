bool ispossible(vector<int> &arr,int n,int k,long long mid){

int painter=1;
long long tot=0;

for(long long i=0;i<arr.size();i++)
{
    if(tot+arr[i]<=mid){
        tot=tot+arr[i];
    }
    else{
        painter++;
        if(painter>k || arr[i]>mid){
            return false;
        }
       

        tot=arr[i];
    }



}

return true;


}




int findLargestMinDistance(vector<int> &boards, int k)
{
    int n = boards.size();
    int s= *max_element(boards.begin(),boards.end());
    long long sum=0;
    for(long long i=0;i<n;i++)
    {
        sum=sum+boards[i];
    }
    long long e=sum;

    long long mid= s+((e-s)/2);
    long long ans=-1;


    while(s<=e)
    {
        if(ispossible(boards,n,k,mid)){
            ans=mid;
            e=mid-1;
        }
        else{
            s=mid+1;
        }

        mid= s+((e-s)/2);


    }


return ans;



}