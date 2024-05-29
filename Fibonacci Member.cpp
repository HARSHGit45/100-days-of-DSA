
bool checkMember(int n){

  if(n==0 || n==1 ){
    return true;
  }


  int a =0 ;
   int b = 1 ;
  int c=1 ;

  while(c<n){

    c=a+b;
    a=b;
    b=c;

  }

  if(c==n){
    return true;
  }
  else{
    return false;
    
  }


}