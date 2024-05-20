#include <iostream>

using namespace std;



void swap(int arr[],int i, int j){
	
	int temp=0;
	
	temp=arr[i];
	arr[i]=arr[j];
	arr[j]=temp;
	
	
	
}

int main(int argc, char** argv)
{
	int arr[100];
	int n;
	
	cout<<"Enter the number of Element: ";
	cin>>n;
	
	for(int i=0;i<n;i++)
	{
		cin>>arr[i];
	}
	
	
	for(int i=0;i<n;i+=2)
	{
		if(i+1<n){
			swap(arr,i,i+1);
		}
	}
	
	
	for(int i=0;i<n;i++)
	{
		cout<<arr[i];
	}
	
}