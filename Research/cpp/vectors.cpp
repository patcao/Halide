#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

 
class num{
	public :
	int val;
	num operator+ (const num& param);
	num operator- (const num& param);
	num operator/ (const num& param);
	num operator* (const num& param);

	static void print(num n){cout<< n.val << endl;}
};

num num::operator+ (const num& param){
	num temp;
	temp.val = val + param.val;
	return temp;
}

num num::operator- (const num& param){
	num temp;
	temp.val = val - param.val;
	return temp;
}

num num::operator/ (const num& param){
	num temp;
	temp.val = val / param.val;
	return temp;
}
num num::operator* (const num& param){
	num temp;
	temp.val = val * param.val;
	return temp;
}
/*
td::ostream& operator<<(std::ostream& out, num& instance) {
	  return out << "num" << endl;
}
*/


int main(){
	vector<num> vec;
	num temp;
	temp.val = 1;
	vec.push_back(temp);
	num::print(vec.back());
	
	int count = 0;
/*
	for(int i = 0; i < 5; ++i)
		vec.push_back(++count);
*/	
	
	//for(vector <int>::iterator it = vec.begin(); it != vec.end(); ++it)
	//	cout << *it  << endl;
	
	num n,n2;
	n.val = 5;
	n2.val = 6;

	num n3 = n2 * n;

	num::print(n3);
	//cout << n3.val << endl;


	return 0;
}

