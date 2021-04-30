#pragma once
class CMath_Manager
{
	DECLARE_SINGLETON(CMath_Manager)
private:
	CMath_Manager();
	~CMath_Manager() = default; 

public:
	float Lerp(float _A, float _B, float _Alpha);

};

