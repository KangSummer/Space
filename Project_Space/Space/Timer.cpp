#include "Timer.h"

CTimer::CTimer() :
	m_OldTime{} 
	, m_CurTime{}
	, m_Frequency{}
	, m_fDeltaTime(0.f)
	, m_fFPS(0.f)
	, m_dAccTime(0.f)
{
}

CTimer::~CTimer()
{
}


void CTimer::init()
{
	QueryPerformanceFrequency(&m_Frequency);
	QueryPerformanceCounter(&m_OldTime);
}

void CTimer::update()
{
	QueryPerformanceCounter(&m_CurTime);

	m_fDeltaTime = (float)m_CurTime.QuadPart - (float)m_OldTime.QuadPart / (float)m_Frequency.QuadPart;
	m_OldTime = m_CurTime;

	m_dAccTime += m_fDeltaTime;

	if (m_fDeltaTime < 0.f)
	{
		m_fDeltaTime = 0.0f;
	}

	if (m_dAccTime > 1.f)
	{
		QueryPerformanceFrequency(&m_Frequency);
		m_dAccTime = 0.f;
		m_fFPS = 1 / (float)m_fDeltaTime;
	}
}