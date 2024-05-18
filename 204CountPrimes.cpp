class Solution {
public:
    int countPrimes(int n) {

        int p=n;

        int cnt=0;

        vector<int> prm(n+1,0);

        for(long i=2;i<n;i++)
        {
            if(prm[i]==0)
            {
                for(long j=i*i;j<=n;j=j+i)
                {
                    prm[j]=1;
                }
            }
        }


        for(long i=2;i<n;i++)
        {
            if(prm[i]==0)
            {
                cnt=cnt+1;
            }
        }

        return cnt;





    }






};