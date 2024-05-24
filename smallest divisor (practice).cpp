#include <iostream>
#include <cmath>

using namespace std;


int sm(int arr[],int dn,int n)
{
	int sum=0;
	for(int i=0;i<n;i++)
	{
		int d = ceil(arr[i]/dn);
		sum=sum+d;
		
		
	}
	
	return sum;
	
}


int main(){
	
	
	
	int arr[5]={1,11,22,33,44};
	int k=6;
	int n=5;
	
	int s=0;
	int e=arr[n];
	int mid=s+((e-s)/2);
	int ans=0;
	
	while(s<=e)
	{
		int sn=sm(arr,mid,n);
		
		cout<<"   "<<arr[mid]<<"    "<<sn<<endl;
		
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
	
	cout<<ans;
	
	
	
	
}
