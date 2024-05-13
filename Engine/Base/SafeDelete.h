#pragma once
//消すやつ

template<class T> inline void SafeDelete(T*& p) {
	delete p;
	p = nullptr;
}

