#pragma once
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>
#include <list>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <limits>
#include <stdexcept>
#include "enum.h"
using namespace std;

void dbg_out() { cerr << endl; } 
// ejmplo de uso: dbg(cosa1, cosa2, cosa3, ...);
template<typename Head, typename... Tail> 
void dbg_out(Head H, Tail... T) { cerr << ' ' << H; dbg_out(T...); }
#define dbg(...) cerr << "(" << #__VA_ARGS__ << "): ", dbg_out(__VA_ARGS__)