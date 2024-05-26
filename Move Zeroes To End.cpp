void pushZerosAtEnd(vector<int> &arr) 
{
	int s=0;

	for(int i=0;i<arr.size();i++){

		if(arr[i]!=0){
			swap(arr[i],arr[s]);
			s++;

		}
	}
	

	



	
}