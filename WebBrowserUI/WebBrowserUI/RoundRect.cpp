//
// CRoundRect.h : Version 1.0 - see article at CodeProject.com
//
// Author:  Darren Sessions
//          
//
// Description:
//     CRoundRect Draws or Fills rounded rectangles for GDI+.  It was implemented
//	   to overcome the asymmetric issues associated with GDI+ round rectangles
//
// History
//     Version 1.0 - 2008 June 24
//     - Initial public release
//
// License:
//     This software is released under the Code Project Open License (CPOL),
//     which may be found here:  http://www.codeproject.com/info/eula.aspx
//     You are free to use this software in any way you like, except that you 
//     may not sell this source code.
//
//     This software is provided "as is" with no expressed or implied warranty.
//     I accept no liability for any damage or loss of business that this 
//     software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "RoundRect.h"

CRoundRect::CRoundRect()
{
}

CRoundRect::~CRoundRect()
{
}

void CRoundRect::GetRoundRectPath(GraphicsPath* pPath, Rect r, int dia)
{
	// diameter can't exceed width or height
	if (dia > r.Width)	dia = r.Width;
	if (dia > r.Height)	dia = r.Height;

	// define a corner 
	Rect Corner(r.X, r.Y, dia, dia);

	// begin path
	pPath->Reset();

	// top left
	pPath->AddArc(Corner, 180, 90);

	// tweak needed for radius of 10 (dia of 20)
	if (dia == 20)
	{
		Corner.Width += 1;
		Corner.Height += 1;
		r.Width -= 1; r.Height -= 1;
	}

	// top right
	Corner.X += (r.Width - dia - 1);
	pPath->AddArc(Corner, 270, 90);

	// bottom right
	Corner.Y += (r.Height - dia - 1);
	pPath->AddArc(Corner, 0, 90);

	// bottom left
	Corner.X -= (r.Width - dia - 1);
	pPath->AddArc(Corner, 90, 90);

	// end path
	pPath->CloseFigure();
}

void CRoundRect::DrawRoundRect(Graphics* pGraphics, Rect r, Color color, int radius, int width)
{
	int dia = 2 * radius;

	// set to pixel mode
	int oldPageUnit = pGraphics->SetPageUnit(UnitPixel);

	// define the pen
	Pen pen(color, 1);
	pen.SetAlignment(PenAlignmentCenter);

	// get the corner path
	GraphicsPath path;

	// get path
	GetRoundRectPath(&path, r, dia);

	// draw the round rect
	pGraphics->DrawPath(&pen, &path);

	// if width > 1
	for (int i = 1; i < width; i++)
	{
		// left stroke
		r.Inflate(-1, 0);

		// get the path
		GetRoundRectPath(&path, r, dia);

		// draw the round rect
		pGraphics->DrawPath(&pen, &path);

		// up stroke
		r.Inflate(0, -1);

		// get the path
		GetRoundRectPath(&path, r, dia);

		// draw the round rect
		pGraphics->DrawPath(&pen, &path);
	}

	// restore page unit
	pGraphics->SetPageUnit((Unit)oldPageUnit);
}

void CRoundRect::FillRoundRect(Graphics* pGraphics, Rect r, Color color, int radius)
{
	SolidBrush sbr(color);
	FillRoundRect(pGraphics, &sbr, r, color, radius);
}

void CRoundRect::FillRoundRect(Graphics* pGraphics, Brush* pBrush, Rect r, Color border, int radius)
{
	int dia = 2 * radius;

	// set to pixel mode
	int oldPageUnit = pGraphics->SetPageUnit(UnitPixel);

	// define the pen
	Pen pen(border, 1);
	pen.SetAlignment(PenAlignmentCenter);

	// get the corner path
	GraphicsPath path;

	// get path
	GetRoundRectPath(&path, r, dia);

	// fill
	pGraphics->FillPath(pBrush, &path);

	// draw the border last so it will be on top in case the color is different
	pGraphics->DrawPath(&pen, &path);

	// restore page unit
	pGraphics->SetPageUnit((Unit)oldPageUnit);
}
