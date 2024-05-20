int findDuplicate(vector<int> &arr) 
{
    
    /* My approach
   for(int i=0;i<arr.size();i++)
   {
       int cnt=0;
       for(int j=0;j<arr.size();j++)
       {
           if(arr[i]==arr[j]){
               cnt++;
           }

       } 
       if(cnt>1)
       {
           return arr[i];
       }

   }
   return 0;
	
    */


int s=0;
    for(int i=0;i<arr.size();i++)
    {
        s=s^(arr[i]^i);
    }

    return s;



}
