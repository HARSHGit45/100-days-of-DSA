int FindSum(int *arr,int l)
{


    int sum=0;


    for(int i=0;i<l;i++)
    {
        if(arr[i]%2==0 || arr[i]%3==0){
            sum=sum+arr[i];
        }



    }

    return sum;
 
}