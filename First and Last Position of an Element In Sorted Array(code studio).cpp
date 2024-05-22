
int frst(vector<int>& arr, int n, int k){

    int st=0;
    int e=n-1;
    int mid=st+((e-st)/2);
    int fs=-1;


    while(st<=e){

        if(arr[mid]==k){
            fs=mid;

            e=mid-1;
        }
        else if(arr[mid]<k){
            st=mid+1;

        }
        else{
            e=mid-1;
        }


        mid=st+((e-st)/2);
    }

    return fs;

}

int lst(vector<int>& arr, int n, int k){
    
    int st=0;
    int e=n-1;
    int mid=st+((e-st)/2);
    int ls=-1;


    while(st<=e){

        if(arr[mid]==k){
            ls=mid;

            st=mid+1;
        }
        else if(arr[mid]<k){
            st=mid+1;

        }
        else{
            e=mid-1;
        }


        mid=st+((e-st)/2);
    }

    return ls;

}




pair<int, int> firstAndLastPosition(vector<int>& arr, int n, int k)
{
    pair<int,int> p;

    p.first =frst(arr,n,k);
  p.second =lst(arr,n,k);

    
        return p;

   
   

}
