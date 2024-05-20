int findUnique(int *arr, int size)
{
  /*
  My approach

    for(int i=0;i<size;i++)
    {
         int cnt=0;

        for(int j=0;j<size;j++)
        {
          
           if(arr[i]==arr[j])
           {
               cnt++;
           }

          
        }
         if (cnt==1){
               return arr[i];
           }
    }

*/

// OPtimal Appoach
int rs=0;
for(int i=0;i<size;i++)
{
    rs=rs^arr[i];

}

return rs;


}
   