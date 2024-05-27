long long appleAndCoupon(int n, int m, vector<int> arr){
    

    sort(arr.begin(),arr.end());

    int sm=0;

    for(int i=0;i<n;i++){
        sm=sm+arr[i];
    }



    sm=sm-arr[n-m];

    return sm;
}