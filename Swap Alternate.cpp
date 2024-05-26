void swapAlternate(int *arr, int size)
{
   for(int i=0;i<size;i=i+2){
       
       if(i+1<size){
           int temp=arr[i+1];
           arr[i+1]=arr[i];
           arr[i]=temp;
       }

   }
   
}