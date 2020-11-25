#pragma once
#include "define.h"


class CTimer
{
	SINGLE(CTimer);

private:
	LARGE_INTEGER		m_OldTime;
	LARGE_INTEGER		m_CurTime;
	LARGE_INTEGER		m_Frequency;

	float				m_fDeltaTime;
	float				m_fFPS;
	double			m_dAccTime;

public:
	void init();
	void update();

	void SetFPS(float _fFPS) {};

	float GetDeltaTime() const { return m_fDeltaTime; }
	float GetFPS() const { return m_fFPS; }
};

