#include <iostream>


using namespace std;


int main(){
	int arr[3][3];
	
	cout<<"\n\nInsert the element : \n\n";
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			cin>>arr[i][j];
		}
		cout<<endl;
	}
	
	
	
	
	
		cout<<"\n\n Elements are: \n";
	
		for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			
			cout<<arr[i][j]<<" ";
		}
		
		cout<<endl;
		}
		
			cout<<"\n\n sums are: \n";
	int mx=0;
		for(int i=0;i<3;i++){
			
			int sum=0;
		for(int j=0;j<3;j++){
			sum =sum+arr[i][j];
			
			
		}
		cout<<"\nSum for "<<i<<"th row is :"<<sum;
		if(sum>mx){
			mx=sum;
		}
		cout<<endl;
		}
		
		cout<<"\n\n Max sum : "<<mx;

return 0;
		}
