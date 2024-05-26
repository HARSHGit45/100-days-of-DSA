void sortZeroesAndOne(int arr[], int size)
{
   int s=0;
   int e=size-1;


   while(s<e){
       if(arr[s]==0){
           s++;
       }


       else{
           int temp = arr[e];
           arr[e]=arr[s];
           arr[s]=temp;
           e--;
       }
       
   }




}