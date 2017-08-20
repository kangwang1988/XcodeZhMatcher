//
//  ZhMatcherUtil.hpp
//  ClangZhMatcherPlugin
//
//  Created by KyleWong on 14/10/2016.
//
//

#ifndef ZhMatcherUtil_hpp
#define ZhMatcherUtil_hpp
#include <string>
#include<algorithm>
#include <functional>
#include <string>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

static bool containsZh(string text){
    // 根据字符，来选择编码方式
    bool bHasChinese = false;
    for (int i=0; i<text.length(); i++)
    {
        if (text[i] < 0) // 0xBO 10110000 0xA1 10100000 第一位都是1，表示是负数
        {
            bHasChinese = true;
            break;
        }
    }
    return bHasChinese;
}

static vector<string> split(const string &s, char delim) {
    vector<string> elems;
    stringstream ss;
    ss.str(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

static inline string absolutePathFromRelative(string relativePath){
    vector<string> pathComs = split(relativePath,'/');
    vector<string> absoPathVec;
    ostringstream oss;
    for(vector<string>::iterator it = pathComs.begin();it!=pathComs.end();it++){
        string path = *it;
        if(!path.compare("") || !path.compare("."))
            continue;
        if(!path.compare("..")){
            absoPathVec.pop_back();
        }
        else{
            absoPathVec.push_back(string("/")+path);
        }
    }
    copy(absoPathVec.begin(), absoPathVec.end(), ostream_iterator<string>(oss));
    return oss.str();
}
#endif /* ZhMatcherUtil_hpp */
