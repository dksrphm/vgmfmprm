VGMFMPRM
==
Extract FM-chip tone parameters(AttackRate, DecayRate, Feedback, Connection...) for mml2vgm from VGM file.  

How to use
--
vgmfmprm.exe [-dr] VGMFile  
option:  
+ -d: debug info output  
+ -r: print register status

Example
--
&gt; vgmfmprm.exe MagicalSoundShower.vgm  
  
	YM2151[4] samples:321 @0		// YM2151 Ch4 uses tone id @0 at VGMsample#: 321
	'@ M 0					// FM tone parameters of @0 in mml2vgm format.
	    AR  DR  SR  RR  SL  TL  KS  ML DT1 DT2 AME
	'@ 031,018,007,008,012,043,000,010,004,000,000,
	'@ 031,017,000,008,015,042,001,001,007,000,000,
	'@ 031,009,000,008,015,025,001,000,004,000,000,
	'@ 031,009,000,008,015,000,001,000,004,000,000,
	   ALG  FB
	'@ 001,006　　

	YM2151[5] samples:402 @1		// YM2151 Ch5 uses tone id @1 at VGMsample#: 402
	'@ M 1					// FM tone parameters of @1 in mml2vgm format.
	    AR  DR  SR  RR  SL  TL  KS  ML DT1 DT2 AME
	'@ 029,006,005,004,005,045,001,005,000,000,000,
	'@ 029,007,006,005,004,047,001,003,002,000,000,
	'@ 030,007,006,006,004,033,001,001,003,000,000,
	'@ 029,006,008,007,003,014,001,001,007,000,000,
	   ALG  FB
	'@ 003,006
	:
	YM2151[6] samples:483 @1		// YM2151 Ch6 also uses tone id @1 at VGMsample#: 483
	:

These parameters can use in mml2vgm's *.gwi files.  
　　

How to build
--
Extract source zip.   
&gt; cd Default    
&gt; make all  

You need MinGW(gcc, make, ...) to build.  
I use Eclipse with CDT, but you can make this.
