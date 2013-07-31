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

#include "cbase.h"
#ifdef MOD_CURL
#include "curl/curl.h"

namespace curl
{
	bool HttpPost( const char *url, char *post )
	{
		CURL *curl = curl_easy_init();
		if( curl )
		{
			curl_easy_setopt( curl, CURLOPT_URL, url );
			curl_easy_setopt( curl, CURLOPT_POSTFIELDS, post );
			curl_easy_setopt( curl, CURLOPT_POSTFIELDSIZE, ( long )Q_strlen( post ) );
		
			CURLcode res = curl_easy_perform( curl );

			curl_easy_cleanup( curl );

			return true;
		}
		else
		{
			return false;
		}
	}

	/**
	 * Returns 0 on success
	 *
	 */
	int DownloadFromGoogle( const char *url )
	{
		return 0;
	}
};

#endif // MOD_CURL
