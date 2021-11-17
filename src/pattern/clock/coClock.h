// Copyright(c) 2021 Jounayd Id Salah
#pragma once

class coClock
{
public:
	coClock();
	void Step();
	void SetFixedDt(coFloat fixedDt_) { fixedDt = fixedDt_; }
	coFloat GetDt() const { return dt; }
	coFloat GetRawDt() const { return rawDt; }
	coFloat64 GetTime() const { return time; }

private:
	coFloat fixedDt = 0.0f;
	coFloat maxDt = 1.0f / 10.0f;

	coFloat dt = 0.0f;
	coFloat64 time = 0.0f;
	coFloat rawDt = 0.0f;
	coUint64 lastTimeStamp = 0;
	coUint64 rootTimeStamp = 0;
};
