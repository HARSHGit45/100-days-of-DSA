#include <iostream>

using namespace std;


bool lnr(int arr[],int n,int s)
{
	
	for(int i=0;i<n;i++)
	{
		if(arr[i]==s)
		{
			
			return true;
		}
		
	}
	return false;
	
	
	
	
}


int main(int argc, char** argv)
{
	int n;
	int s;
	
	cout<<"Enter the  number of element:"<<endl;
	cin>>n;
	
	int arr[100];
	
	cout<<"Enter the element of array: "<<endl;
	for(int i=0;i<n;i++)
	{
		cin>>arr[i];
		
	}
	
	
	cout<<"Enter the element to be found: ";
	cin>>s;
	
	if(	lnr(arr,n,s)){
		cout<<"\n\n Element is present!!";
		
	}
	else
	{
		cout<<"\n\n Not present!!";
	}
		
}