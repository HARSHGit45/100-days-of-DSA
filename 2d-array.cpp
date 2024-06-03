#include <iostream>


using namespace std;


int main(){
	int arr[2][2];
	
	for(int i=0;i<2;i++){
		for(int j=0;j<2;j++){
			cin>>arr[i][j];
		}
		cout<<endl;
	}
	
	
	
	
	cout<<"\n\n Elements are: \n";
	
		for(int i=0;i<2;i++){
		for(int j=0;j<2;j++){
			
			cout<<arr[i][j]<<" ";
		}
		
		cout<<endl;
		}
		
		
			for(int i=0;i<2;i++){
		for(int j=0;j<2;j++){
			
			if(arr[i][j]==5){
				cout<<"\n\nPresent at "<<i+1<<" row and "<<j+1<<" coulmn\n\n";
				break;
			}
		}
			}
			
			cout<<"not present";
		
		return 0;
}