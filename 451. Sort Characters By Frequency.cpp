class Solution {
public:
 static bool compare(const pair<int, char>& a, const pair<int, char>& b) {
        return a.first > b.first;
    }
    string frequencySort(string s) {

        unordered_map<char,int> mp1;


        for(int i=0;i<s.length();i++){
            mp1[s[i]]++;
        }


        vector<pair<int,char>> mp2;

        for(auto it : mp1){
            mp2.push_back({it.second,it.first});
        }

        sort(mp2.begin(),mp2.end(),compare);


string ans="";

        for(auto it : mp2){

            ans.append(it.first,it.second);


        }
        return ans;
      
        
    }
};