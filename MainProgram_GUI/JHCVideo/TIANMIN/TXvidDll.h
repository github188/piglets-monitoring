
//////////////////////////////////////////////////////////////////////////

//	*****QQQ*******	W******W******W	EEEEEEEEEEEEEEE	RRRRRRRRRRRRR**
//	***QQ***QQ*****	WW****WWW****WW	EE*************	RR********RRRRR
//	*QQ*******QQ***	*WW**WW*WW**WW*	EE*************	RR******RRRRR**
//	QQ****Q****QQ**	*WW**WW*WW**WW*	EEEEEEEEEEEEEEE	RR****RRRRR****
//	*QQ****QQ*QQ***	*WW**WW*WW**WW*	EEEEEEEEEEEEEEE	RR**RRRR*******
//	**QQ*****QQ****	*WW**WW*WW**WW*	EE*************	RRRR***********
//	***QQ***QQ*QQ**	**W**W***W**W**	EE*************	RR**RRRR*******
//	*****QQQ*****QQ	***WW*****WW***	EEEEEEEEEEEEEEE	RR****RRRRRRRRR

//	Author	qwer4821
//	Company	10moons
//	City	shenzhen
//  Date	2004-2-9
//  File	TXvidDll.h

//////////////////////////////////////////////////////////////////////////
#ifndef TXvidDll_qwer_
#define TXvidDll_qwer_
//////////////////////////////////////////////////////////////////////////
void XD_SetDefaultValue();
UINT XD_GetQuantizer();
BOOL XD_SetQuantizer(UINT nQuant);
UINT XD_GetQuality();
BOOL XD_SetQuality(UINT nQuality);
DWORD XD_GetBitrate();
BOOL XD_SetBitrate(DWORD dwBitrate);
UINT XD_GetEncodingMode();
BOOL XD_SetEncodingMode(UINT nMode);
BOOL XD_ShowVCMDialog(HWND hWndParent);
UINT XD_GetFourCC();
BOOL XD_SetFourCC(UINT nFCC);
//////////////////////////////////////////////////////////////////////////

#endif