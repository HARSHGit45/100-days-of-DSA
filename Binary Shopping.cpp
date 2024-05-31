string binaryShopping(string S, int P)

{

int count =0;

// #000011 --> 000000

// #111111 --> 111100

// 000011 --> 000000

for(int i=S.size()-1;i>=0;i--){

if(count<2){

 

if (S[i]== '1') {

count++;

S[i] = '0';

 

}

}

 

}

// #000001 --> 100000

if(count==1){

S[0]='1';

}

return S;

}