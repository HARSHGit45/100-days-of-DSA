int findpiv(int* arr, int n){

    int s=0;
    int e=n-1;

    int mid = s+ (e-s)/2;

    while(s<e){



if(arr[mid]>=arr[0]){
    s=mid+1;
} 


else {
    e = mid;
}


mid = s+ (e-s)/2;

    }
    

    return s;
}




int bin(int* arr, int n,int s,int e,int key){


int mid =  s + (e-s)/2;



while(s<=e){
    if(arr[mid]==key){
        return mid;
    }

    else if(arr[mid]>key){
        e=mid-1;
    }
    else{
        s=mid+1;
    }

    mid =  s + (e-s)/2;
}

return -1;


}



int search(int* arr, int n, int key) {



    int piv= findpiv(arr,n);

    if(key>=arr[piv] && key<=arr[n-1]){
       return bin(arr,n,piv,n-1,key);
    }
    else{
        return bin(arr,n,0,piv-1,key);
    }
   
   


}