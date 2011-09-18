/*
 *  Copyright (C) 2010 Alexander Chernykh 
 *
 *  This file is part of MyQuik: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  MyQuik is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *  mcapture.cpp
 */

#include "stdafx.h"
#include "mcapture.h"


namespace my {

	void MouseCapturer::Capture(HWND hWnd, UINT nFlags, CPoint point)
	{
	    m_capture = TRUE;
		// ��� ��������� �� ���� - � ������ ���� - ���������� �� ���������, 
		// ����� ���� �� ������� � ���� ��� ������� ��������
		m_hWnd = hWnd;
#if 0
		::SetCapture(hWnd);
#endif
		// ��������� ���������� ����
		::GetWindowRect(hWnd, m_rect);
		// ��������� ��������� ����� ������ ���� ���������
		::ClientToScreen(hWnd, &point);
		m_pos = point - m_rect.TopLeft();
	}

	void MouseCapturer::Release()
	{
		if (m_capture)
		{
			m_capture = FALSE;
#if 0
			// "���������" �����
			::ReleaseCapture();
#endif
			m_hWnd = NULL;
		}
	}

	void MouseCapturer::MoveWindow(HWND hWnd, CPoint point)
	{
		// ����������� ���������� ���� � ��������
		// ������ ��� ����� ����� ��� SetWindowPos()
		::ClientToScreen(hWnd, &point);
		// ������� ���� � ������������ � ������ ������������ ����
		::SetWindowPos(hWnd, NULL, point.x - m_pos.x, point.y - m_pos.y, 0, 0, SWP_NOSIZE|SWP_NOZORDER|SWP_SHOWWINDOW);
		// ��������� ���������� �� ��������� ��� ����� ����� ������������
		// �������������� ��������� ���������
		// �������� �������������� ScreenToClient(&point);
		// ����� �� ��������
	}

	void MouseCapturer::OnMouseMove(UINT nFlags, CPoint point)
	{
		if (nFlags & MK_LBUTTON)
		{// ���������� ����:
			if (m_capture && m_hWnd)
			{
				MoveWindow(m_hWnd, point);
			}
		}
	}

	BOOL MouseCapturer::m_capture = FALSE;
	CRect MouseCapturer::m_rect;
	CPoint MouseCapturer::m_pos;
	HWND MouseCapturer::m_hWnd = NULL;

} // namespace my
