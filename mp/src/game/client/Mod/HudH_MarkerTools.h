/* ***** BEGIN LICENSE BLOCK *****
Version: MPL 1.1/LGPL 2.1/GPL 2.0

The contents of this file are subject to the Mozilla Public License Version 
1.1 (the "License"); you may not use this file except in compliance with
...
for the specific language governing rights and limitations under the
License.

The Original Code is for LuminousForts.

The Initial Developer of the Original Code is Hekar Khani.
Portions created by the Hekar Khani are Copyright (C) 2010
Hekar Khani. All Rights Reserved.

Contributor(s):
  Hekar Khani <hekark@gmail.com>

Alternatively, the contents of this file may be used under the terms of
either of the GNU General Public License Version 2 or later (the "GPL"),
...
the terms of any one of the MPL, the GPL or the LGPL.

***** END LICENSE BLOCK ***** */

#ifndef HUDH_MARKERTOOLS_H
#define HUDH_MARKERTOOLS_H

class DrawMarker
{
public:
	DrawMarker();
	~DrawMarker();
	void Draw();

	void SetTexture( CHudTexture *texture )
	{
		m_Texture = texture;
	}

	void SetDivider( float divider )
	{
		m_Divider = divider;
	}

	void SetWidthDim( float width_min, float width_max )
	{
		m_WidthMin = width_min;
		m_WidthMax = width_max;
	}

	void SetHeightDim( float height_min, float height_max )
	{
		m_HeightMin = height_min;
		m_HeightMax = height_max;
	}

	void SetColor( Color color )
	{
		m_Color = color;
	}

	void SetOrigin( Vector origin )
	{
		m_Origin = origin;
	}

private:
	CHudTexture *m_Texture;
	Color m_Color;
	Vector m_Origin;

	float m_Divider;
	float m_WidthMin;
	float m_WidthMax;
	float m_HeightMin;
	float m_HeightMax;
};


#endif // HUDH_MARKERTOOLS_H