#include <bits/stdc++.h> 
vector<int> findArraySum(vector<int>&a, int n, vector<int>&b, int m) {
	  int ans1=0;

    int ans2=0;

    for(int i=0;i<n;i++){

        ans1=a[i]+10*ans1;

    }

    for(int i=0;i<m;i++){

        ans2=b[i]+10*ans2;

    }

    int sum=ans1+ans2;

    vector<int> ans;

    while(sum>0){

        ans.push_back(sum%10);

        sum/=10;

    }

    reverse(ans.begin(),ans.end());

    return ans;

}


