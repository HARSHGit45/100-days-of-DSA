class Solution {
public:
   class Solution {
public:

bool isvalid(char ch){
    if( (ch>='a' && ch<='z')  || (ch>='A' && ch<='Z') || (ch>='0' && ch<='9')  )
    {
        return true;
    }
    else{
        return false;
    }
}

char lower(char ch){
    if( (ch>='a' && ch<='z')  || (ch>='0' && ch<='9') ){
        return ch;
    }
    else{
        char temp;

        temp = ch-'A'+'a';

        return temp;
    }
}

bool check(string s){

    int st=0;
    int e=s.length()-1;

    while(st<e){
        if(s[st]!=s[e]){
            return false;
        }
        else{
            st++;
            e--;
        }
    }

    return true;
}






    bool isPalindrome(string s) {


        string temp="";

        for(int i=0;i<s.length();i++){
            
            if(isvalid(s[i])){
                temp.push_back(s[i]);
            }
        }

        for(int i=0;i<temp.length();i++){

            temp[i]=lower(temp[i]);
        }


        return check(temp);




       





    }
};
    }
};