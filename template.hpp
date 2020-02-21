#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <string>
#include <utility>
#include <algorithm>
#include <set>
#include <map>
#include <queue>

template <typename T>
using PriorityQueue = std::priority_queue<T, std::vector<T>, std::greater<T>>;

#define TRACE (std::cout << "[line " << __LINE__ << "] ")
#define trace(what)(TRACE << #what " = " << (what) << std::endl)
#define lintf(fmt, ...)(printf("[line %d] " fmt, __LINE__, __VA_ARGS__))

#define FOR(i, to) for (int i = 0; i < static_cast<int>(to); ++i)

template <typename T0, typename T1>
static inline std::ostream &operator<<(std::ostream &o, const std::pair<T0, T1> &rhs)
{
	o << '{' << rhs.first << ", " << rhs.second << '}';
	return o;
}

template <typename T>
static inline std::ostream &operator<<(std::ostream &o, const std::vector<T> &rhs)
{
	o << '{';
	for (auto item : rhs) {
		o << item << "; ";
	}
	o << '}';
	return o;
}

