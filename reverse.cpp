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
	
	
	
	int i=0;
	int j=n-1;
	
	while(i<=j)
	{
		swap(arr,i,j);
		
		
		i++;
		j--;
	}
	
	
	for(int i=0;i<n;i++)
	{
		
		cout<<arr[i]<<endl;
	}
	
	
	
	
	
	
	return 0;
}