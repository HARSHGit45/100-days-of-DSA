#include <iostream>

using namespace std;


void bin(int arr[],int n,int s)
{
	int start=0;
	int end=n-1;
	int mid= start+ ((end-start)/2);
	
	while(start<=end)
	{
		
		if(arr[mid]==s)
		{
			
			cout<<"\n\n FOund!!! at "<<mid+1<<" Location!!";
			return;
		}
		else if(arr[mid]<s)
		{
			start=mid+1;
			
		}
		else
		{
			end=mid-1;
		}
		
		mid= start+((end-start)/2);
	}
	
	
}




int main(int argc, char** argv)
{
	
	int arr[100];
	
	int n;
	
	cout<<"Enter the number of elements:";
	cin>>n;
	
	for(int i=0;i<n;i++)
	{
		cin>>arr[i];
	}
	
	int s;
	
	cout<<"Enter the element to be searched: ";
	cin>>s;
	
	
	bin(arr,n,s);
	
	return 0;
}