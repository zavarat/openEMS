/*
*	Copyright (C) 2010 Thorsten Liebig (Thorsten.Liebig@gmx.de)
*
*	This program is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ENGINE_H
#define ENGINE_H

#include <fstream>
#include "operator.h"

class Engine_Extension;

class Engine
{
public:
	enum EngineType
	{
		BASIC, SSE, UNKNOWN
	};

	static Engine* New(const Operator* op);
	virtual ~Engine();

	virtual void Init();
	virtual void Reset();

	//!Iterate a number of timesteps
	virtual bool IterateTS(unsigned int iterTS);

	virtual unsigned int GetNumberOfTimesteps() {return numTS;};

	//this access functions muss be overloaded by any new engine using a different storage model
	inline virtual FDTD_FLOAT& GetVolt( unsigned int n, unsigned int x, unsigned int y, unsigned int z ) { return volt[n][x][y][z]; }
	inline virtual FDTD_FLOAT& GetVolt( unsigned int n, unsigned int pos[3] ) { return volt[n][pos[0]][pos[1]][pos[2]]; }
	inline virtual FDTD_FLOAT& GetCurr( unsigned int n, unsigned int x, unsigned int y, unsigned int z ) { return curr[n][x][y][z]; }
	inline virtual FDTD_FLOAT& GetCurr( unsigned int n, unsigned int pos[3] ) { return curr[n][pos[0]][pos[1]][pos[2]]; }

	virtual void UpdateVoltages(unsigned int startX, unsigned int numX);
	virtual void ApplyVoltageExcite();
	virtual void UpdateCurrents(unsigned int startX, unsigned int numX);
	virtual void ApplyCurrentExcite();

	inline size_t GetExtensionCount() {return m_Eng_exts.size();}
	inline Engine_Extension* GetExtension(size_t nr) {return m_Eng_exts.at(nr);}

	EngineType GetType() const {return m_type;}

protected:
	EngineType m_type;

	Engine(const Operator* op);
	const Operator* Op;

	unsigned int numLines[3];

	FDTD_FLOAT**** volt;
	FDTD_FLOAT**** curr;
	unsigned int numTS;

	virtual void InitExtensions();
	vector<Engine_Extension*> m_Eng_exts;

	ofstream file_et; //excite signal dump file
};

#endif // ENGINE_H
