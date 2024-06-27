class Solution {
public:
       vector<int> goodDaysToRobBank(vector<int>& security, int t) {
        int n=security.size();
        vector<int>prefix(n),suffix(n);
        prefix[0]=0;
        suffix[n-1]=0;
        for(int i=1;i<n;i++)
        {
            if(security[i]<=security[i-1])
            {
                prefix[i]=prefix[i-1]+1; 
            }
            else{
                prefix[i]=0;
            }
        }
        for(int i=n-2;i>=0;i--)
        {
            if(security[i]<=security[i+1]){
                suffix[i]=suffix[i+1]+1;
            }
            else{
                suffix[i]=0;
            }
        }
        vector<int>ans;
        for(int i=0;i<n;i++){
            if(suffix[i]>=t && prefix[i]>=t){
                ans.push_back(i);
            }
        }
        return ans;
    }
};