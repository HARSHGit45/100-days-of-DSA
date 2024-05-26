bool ispossible(vector<int>& arr, int n, int m,int mid){

    int std=1;
    int sm=0;
    for(int i=0;i<n;i++){
        if(sm+arr[i]<=mid){
            sm=sm+arr[i];
        }
        else{
            std++;
            if(std>m || arr[i]>mid){
                return false;
            }
            sm=arr[i];
        }
    }
    return true;
}


int findPages(vector<int>& arr, int n, int m) {
  
if(m>n)
{
    return -1;
}

  int s= 0;
  int sum=0;
  for(int i=0;i<n;i++)
  {
      sum=sum+arr[i];
  }
  int e=sum;
int ans=0;

  int mid = s + (e-s)/2;


  while(s<=e){

      if(ispossible(arr,n,m,mid)){
          ans=mid;
          e=mid-1;

      }
      else{
          s=mid+1;
      }


      mid = s + (e-s)/2;
  }

  return s;
}