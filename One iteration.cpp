#include <bits/stdc++.h> 
int oneIteration(vector<int> A)
{
    int n = A.size();
   sort(A.begin(),A.end());


   return (A[n-1]+A[n-2]);  
}