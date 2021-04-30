#include "stdafx.h"
#include "Math_Manager.h"

IMPLEMENT_SINGLETON(CMath_Manager)

CMath_Manager::CMath_Manager()
{
}

float CMath_Manager::Lerp(float _A, float _B, float _Alpha)
{
	return _A * (1 - _Alpha) + _B * _Alpha;
}
