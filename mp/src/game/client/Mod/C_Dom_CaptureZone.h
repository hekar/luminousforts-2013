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

#ifndef C_DOM_CAPTUREZONE_H
#define C_DOM_CAPTUREZONE_H

#ifdef WIN32
#pragma once
#endif

class C_DomCaptureZone : public C_BaseEntity
{
public:
	DECLARE_CLASS( C_DomCaptureZone, C_BaseEntity )
	DECLARE_CLIENTCLASS()

	C_DomCaptureZone();
	~C_DomCaptureZone();

	void UpdatePos( const Vector& Origin );

	void OnDataChanged( DataUpdateType_t updateType );

	const bool GetLocked() const { return m_Locked; }
	const int GetZoneId() const { return m_ZoneId; }
	const int GetCapState() const { return m_CapState; }
	const int GetCapturePercentage() const { return m_iCapturePercentage; }
	const int GetCapturingCount() const { return m_iCapturingCount; }
	const int GetCapturingTeam() const { return m_iCapturingTeam; }

private:
	CNetworkVar( bool, m_Locked );
	CNetworkVar( int, m_ZoneId );
	CNetworkVar( int, m_CapState );
	CNetworkVar( int, m_iCapturePercentage );
	CNetworkVar( int, m_iCapturingCount );
	CNetworkVar( int, m_iCapturingTeam );

	Vector m_vecLastOrigin;
};

#endif // C_DOM_CAPTUREZONE_H