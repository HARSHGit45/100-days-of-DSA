vector<int> rotateArray(vector<int>arr, int k) {
   
  
   int count=0;
   int n = arr.size();


   while(count!=k){

       int temp = arr[0];

       for(int i=0;i<arr.size();i++)
   {
       arr[i]=arr[i+1];
   }

   arr[n-1]=temp;
   count++;
        
  
   }

   return arr;




}
