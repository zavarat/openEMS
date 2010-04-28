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

#include "engine_ext_mur_abc.h"
#include "operator_ext_mur_abc.h"
#include "engine.h"
#include "tools/array_ops.h"

Engine_Ext_Mur_ABC::Engine_Ext_Mur_ABC(Operator_Ext_Mur_ABC* op_ext) : Engine_Extension(op_ext)
{
	m_Op_mur = op_ext;
	m_numLines[0] = m_Op_mur->m_numLines[0];
	m_numLines[1] = m_Op_mur->m_numLines[1];
	m_ny = m_Op_mur->m_ny;
	m_nyP = m_Op_mur->m_nyP;
	m_nyPP = m_Op_mur->m_nyPP;
	m_LineNr = m_Op_mur->m_LineNr;
	m_LineNr_Shift = m_Op_mur->m_LineNr_Shift;

	m_Mur_Coeff = m_Op_mur->m_Mur_Coeff;

	m_volt_nyP = Create2DArray(m_numLines);
	m_volt_nyPP = Create2DArray(m_numLines);
}

Engine_Ext_Mur_ABC::~Engine_Ext_Mur_ABC()
{
	Delete2DArray(m_volt_nyP,m_numLines);
	m_volt_nyP = NULL;
	Delete2DArray(m_volt_nyPP,m_numLines);
	m_volt_nyPP = NULL;
}

void Engine_Ext_Mur_ABC::DoPreVoltageUpdates()
{
	if (m_Eng==NULL) return;
	if (m_Mur_Coeff==0) return;
	unsigned int pos[] = {0,0,0};
	unsigned int pos_shift[] = {0,0,0};
	pos[m_ny] = m_LineNr;
	pos_shift[m_ny] = m_LineNr_Shift;

	for (pos[m_nyP]=0;pos[m_nyP]<m_numLines[0];++pos[m_nyP])
	{
		pos_shift[m_nyP] = pos[m_nyP];
		for (pos[m_nyPP]=0;pos[m_nyPP]<m_numLines[1];++pos[m_nyPP])
		{
			pos_shift[m_nyPP] = pos[m_nyPP];
			m_volt_nyP[pos[m_nyP]][pos[m_nyPP]] = m_Eng->GetVolt(m_nyP,pos_shift) - m_Mur_Coeff * m_Eng->GetVolt(m_nyP,pos);
			m_volt_nyPP[pos[m_nyP]][pos[m_nyPP]] = m_Eng->GetVolt(m_nyPP,pos_shift) - m_Mur_Coeff * m_Eng->GetVolt(m_nyPP,pos);
		}
	}
}

void Engine_Ext_Mur_ABC::DoPostVoltageUpdates()
{
	if (m_Eng==NULL) return;
	if (m_Mur_Coeff==0) return;
	unsigned int pos[] = {0,0,0};
	unsigned int pos_shift[] = {0,0,0};
	pos[m_ny] = m_LineNr;
	pos_shift[m_ny] = m_LineNr_Shift;

	for (pos[m_nyP]=0;pos[m_nyP]<m_numLines[0];++pos[m_nyP])
	{
		pos_shift[m_nyP] = pos[m_nyP];
		for (pos[m_nyPP]=0;pos[m_nyPP]<m_numLines[1];++pos[m_nyPP])
		{
			pos_shift[m_nyPP] = pos[m_nyPP];
			m_volt_nyP[pos[m_nyP]][pos[m_nyPP]] += m_Mur_Coeff * m_Eng->GetVolt(m_nyP,pos_shift);
			m_volt_nyPP[pos[m_nyP]][pos[m_nyPP]] += m_Mur_Coeff * m_Eng->GetVolt(m_nyPP,pos_shift);
		}
	}
}

void Engine_Ext_Mur_ABC::Apply2Voltages()
{
	if (m_Eng==NULL) return;
	if (m_Mur_Coeff==0) return;
	unsigned int pos[] = {0,0,0};
	pos[m_ny] = m_LineNr;

	for (pos[m_nyP]=0;pos[m_nyP]<m_numLines[0];++pos[m_nyP])
	{
		for (pos[m_nyPP]=0;pos[m_nyPP]<m_numLines[1];++pos[m_nyPP])
		{
			m_Eng->GetVolt(m_nyP,pos) = m_volt_nyP[pos[m_nyP]][pos[m_nyPP]];
			m_Eng->GetVolt(m_nyPP,pos) = m_volt_nyPP[pos[m_nyP]][pos[m_nyPP]];
		}
	}
}
