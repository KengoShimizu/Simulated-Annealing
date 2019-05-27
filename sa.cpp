#include <iostream>
#include <cmath>
#include <stdio.h>
#include <random>
#include <stdlib.h>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <string.h>
#include <stdarg.h>
#include <string>
#include <list>
#include <sstream>
#include <cctype>
#include <algorithm>
using namespace std;
#define SIZE 255

const double costf(vector<string> strList, vector<string> tmpList, int *a, int num, ...){ //目的関数
	
	//可変長引数
	double vec[num];
	va_list list;
	va_start(list, num);
    for(int i = 0; i < num; i++) {
        vec[i] = va_arg(list, double);
        //cout << i << " " << vec[i];
    }
    va_end(list);

	//ここで変数には乱数(double)を，数字はstringからdoubleに変換している．
	double b[strList.size()];
	int count=0,i;
    for(i=0; i<strList.size(); i++){
    	for(int j=0; j<tmpList.size(); j++){
    		if(strList[i]==tmpList[j]) b[i] = vec[j];
    		if(a[i]==2) b[i] = stoi(strList[i]);
    		if(a[i]==0 || a[i]==1) {
    			count++;
    			b[i] = 0;
    		}
    	}
	}

	//ここで目的関数を計算し，目的関数値を得る．
	double cost[count+1];
	for(i=0; i<count+1; i++) cost[i] = 1; 
	int c=0;
	for(i=0; i<strList.size(); i++){
		if(a[i]==0 || a[i]==1) c++;
		else cost[c] *= b[i];
	}
	c=0;
	for(i=0; i<strList.size(); i++){
		if(a[i]==0){
			c++;
			cost[0] += cost[c];
		}
		if(a[i]==1){
			c++;
			cost[0] -= cost[c];
		}
	}

	return cost[0];
}

const double annealingoptimize(double T, double cool, double step, int num, vector<string> strList, vector<string> tmpList, int *a){
	//T:温度 cool:冷却 step:近傍解の範囲 num:変数の数

	double vec[num];
	for(int i=0; i<num; i++) vec[i] = ((double)rand() / ((double)RAND_MAX + 1)) * 4 - 2;
	//((double)rand() / ((double)RAND_MAX + 1)) * M   は[0,M]の乱数　この時[-2,2]
	
	while (T > 0.0001){

		//変数の値を増加させるか、減少させるかを決定する。
		double dir[num];
		for(int i=0; i<num; i++) {
			dir[i] = ((double)rand() / ((double)RAND_MAX + 1)) * 1;
			dir[i] = (dir[i] - 0.5) * step;
		}

		//変数の値を変更する。
		double newvec[num];
		for(int i=0; i<num; i++) newvec[i] = vec[i] + dir[i];

		//変更前と変更後のコストを計算する。
		double newcost = costf(strList, tmpList, a, num, *newvec);
		double cost = costf(strList, tmpList, a, num, *vec);

		//温度から確率を定義する。
		double p = exp(-abs(newcost - cost) / T);
		cout << "p=" << p << "\n";

		//変更後のコストが小さければ採用する。
		//コストが大きい場合は確率的に採用する。
		if(newcost < cost or ((double)rand() / ((double)RAND_MAX + 1)) * 1 < p) {
			for(int i=0; i<num; i++) vec[i] = newvec[i];
		}

		//温度を下げる
		T = T * cool;
 	 }
    
    //これがSA適用後の変数の値．
 	for(int i=0; i<num; i++) cout << tmpList[i] << "：" << vec[i] << endl;
	//return *vec;

	}


vector<string> split(const string& input, char delimiter){
	//splitを定義（理解は不要）

    istringstream stream(input);

    string field;
    vector<string> result;
    while (getline(stream, field, delimiter)) {
        result.push_back(field);
    }
    return result;
}

bool check_int(string str) 
//stringの中身が数字ならば1，それ以外は0を返す関数
{
    try {
        int n = stoi(str);
    }
    catch (...) {
        return false;
    }
    return true;
}

const void make_mokuteki(string str){
	int *a;
	//int count = 0; //変数の個数をカウント(この数字はdir生成の際に使う) 
	vector<string> strList, tmpList, y;
	for (const string& s : split(str, ' ')) strList.push_back(s);
	//読み込んだ文字列をスペースで分割してリストに追加

	a = new int[strList.size()];

	for(int i=0; i<strList.size(); i++){
		a[i] = 0;
		if(check_int(strList[i])) a[i] = 2; //2は数字を表す
		else {
			a[i] = 3; //3は変数を表す．
			//count++; 
			tmpList.push_back(strList[i]);
		}
		if(strList[i] == "+") {
			a[i] = 0; //0は+を表す
			//count--;
			tmpList.pop_back();
		}
		if(strList[i] == "-") {
			a[i] = 1; //1は-を表す
			//count--;
			tmpList.pop_back();
		}
	}

	sort(tmpList.begin(), tmpList.end());
	tmpList.erase(unique(tmpList.begin(), tmpList.end()), tmpList.end());
	//存在する変数をtmpListに格納　この時tmpList.size()がSAにかけられる変数の数

	double vec[tmpList.size()];
	annealingoptimize(10000, 0.99, 1, tmpList.size(), strList, tmpList, a);
	

}





int main()
{
	string str;
	srand((unsigned) time(NULL));
	ifstream fin("sample.dat"); //ファイルを開く
	while(getline(fin,str)){
        str = str;
        //一行ごとに読み込み
    }
    make_mokuteki(str);
	fin.close();
    return 0;
}