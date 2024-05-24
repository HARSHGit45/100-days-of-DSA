
int sm(vector<int>&  arr,int dn,int n)
{
	int sum=0;
	for(int i=0;i<n;i++)
	{
		int d = ceil((double)arr[i]/(double)dn);
		sum=sum+d;
		
		
	}
	
	return sum;
	
}



int smallestDivisor(vector<int>& arr, int k) {

int n=arr.size();


    int s=0;
	int e =*max_element(arr.begin(),arr.end());
	int mid=s+((e-s)/2);
	int ans=0;
	
	while(s<=e)
	{
		int sn=sm(arr,mid,n);
		
		
		
		if(sn<=k){
			ans=mid;
			
			e=mid-1;
		}
		else
		{
			s=mid+1;
			
		}
		
		mid=s+((e-s)/2);
		
	}
	
	return ans;
	
	
        
    }
