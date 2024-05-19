class Solution {
public:
    int subtractProductAndSum(int n) {

        int s=0;
        int p=1;
    
        while(n>0)
        {
            int dg=n%10;
            s=s+dg;
            p=p*dg;

            n/=10;


        }

        return (p-s);


        
        
    }
};