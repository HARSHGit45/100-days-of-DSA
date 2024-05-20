#include <iostream>

using namespace std;

int max(int a[],int n)
{
	int mx=a[0];
	for(int i=0;i<n;i++)
	{
		if(mx<a[i])
		{
			mx=a[i];
		}
	}
	
	return mx;
	
	
}

int min(int a[],int n)
{
		int mn=a[0];
	for(int i=0;i<n;i++)
	{
		if(mn>a[i])
		{
			mn=a[i];
		}
	}
	
	return mn;
	
}

int sum(int arr[],int n){
	
	
	int sm=0;
	
	for(int i=0;i<n;i++){
		sm=sm+arr[i];
	}
	
	return sm;
}

int main(int argc, char** argv)
{
	int n;
	
	cout<<"Enter the  number of element:"<<endl;
	cin>>n;
	
	int arr[100];
	
	cout<<"Enter the element of array: "<<endl;
	for(int i=0;i<n;i++)
	{
		cin>>arr[i];
	}
	
	int mx=max(arr,n);
	int mn=min(arr,n);
	int sm=sum(arr,n);
	
	cout<<"Max nmuber: "<<mx<<endl;
	cout<<"Min number: "<<mn<<endl;
	cout<<"Sum: "<<sm<<endl;
	
	
	
	
	return 0;
}