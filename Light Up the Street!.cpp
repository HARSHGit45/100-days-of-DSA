#include <bits/stdc++.h> 

int LightUptheStreet(int n, int m , int k , vector <int> Checkpoints){

      

      int startPos = 0;

      int target = 0;

      int count = 0;

      int i = 0;

      bool ft = 0;

 

       while(i < m)

      {

           if((Checkpoints[i] - k) <= (startPos+1))

           {

                target = Checkpoints[i] + k; 

                if(Checkpoints[i] + k >= n)

                {

                    count++;

                    return count;

                }

                i++;

                ft = 1;

                continue;

           }

           else

           {

                startPos = target;

                count++;

                if(ft)

                {

                    ft = 0;

                    continue;

                }

           }

 

          i++;

      }

     

     return -1;

}