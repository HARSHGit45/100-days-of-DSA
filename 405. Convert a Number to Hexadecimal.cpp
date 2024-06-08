class Solution {
public:
    string toHex(int num) {

        if(num==0){
            return "0";
        }




        string dig="0123456789abcdef";

        unsigned int n= num;
        string res;

        while(n>0){
            int d=n%16;
            res=res+dig[d];
            n=n/16;

        }


        reverse(res.begin(),res.end());

        return res;
        
    }
};