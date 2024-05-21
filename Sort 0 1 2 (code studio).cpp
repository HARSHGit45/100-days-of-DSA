#include <bits/stdc++.h> 
void sort012(int *arr, int n)
{
 
 int mid=0;
 int low=0;
 int high=n-1;

 while(mid<=high)
 {
   if(arr[mid]==0)
   {
     int temp=arr[low];
     arr[low]=arr[mid];
     arr[mid]=temp;
     mid++;
     low++;
   }

   else if (arr[mid]==1)
   {
     mid++;
   }


   else{
     int temp=arr[high];
     arr[high]=arr[mid];
     arr[mid]=temp;
     
     high--;

   }



 }

}