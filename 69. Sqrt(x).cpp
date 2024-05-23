class Solution {
public:
    long long int mySqrt(int x) {

if(x==1)
{
    return 1;
    }
        
        int s=0;
        int e=x-1;
        long long int mid = s+((e-s)/2);
        long long int ans=0;

        while(s<=e)
        {
            if((mid*mid)==x){
                return mid;
            }
            else if((mid*mid)>x){
                e=mid-1;
            }
            else{
                
                ans=mid;
                s=mid+1;
            }

            mid = s+((e-s)/2);

        }

        return ans;


        
    }
};