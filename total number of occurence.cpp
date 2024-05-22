#include <iostream>

using namespace std;


int frs(int arr[],int n,int s)
{
	int start=0;
	int end=n-1;
	int mid= start+ ((end-start)/2);
	int fs=-1;
	
	while(start<=end)
	{
		
		if(arr[mid]==s)
		{
		fs=mid;
		end=mid-1;	
			
			
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
	
	return fs;
}

int lst(int arr[],int n,int s)
{
	int start=0;
	int end=n-1;
	int mid= start+ ((end-start)/2);
	int ls=-1;
	
	while(start<=end)
	{
		
		if(arr[mid]==s)
		{
		ls=mid;
		start=mid+1;	
			
			
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
	
	return ls;
	
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
	
	int fs=frs(arr,n,s);
	int ls=lst(arr,n,s);
	
	cout<<"Occurance of number is : "<<(ls-fs)+1;
	
	return 0;
}