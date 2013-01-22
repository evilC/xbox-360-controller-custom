
//	Force Feedback Driver for Microsoft Xbox 360 Controller

#include	"effect.h"

//----------------------------------------------------------------------------------------------
//	CEffect
//----------------------------------------------------------------------------------------------
CEffect::CEffect()
{
	//	種類
	Type		= 0;
	//	DIEFFECT 構造体
	ZeroMemory( &DiEffect, sizeof( DIEFFECT ) );
	//	DIENVELOPE 構造体
	ZeroMemory( &DiEnvelope, sizeof( DIENVELOPE ) );
	//	DICONSTANTFORCE 構造体
	ZeroMemory( &DiConstantForce, sizeof( DICONSTANTFORCE ) );
	//	DIPERIODIC 構造体
	ZeroMemory( &DiPeriodic, sizeof( DIPERIODIC ) );
	//	DIRAMPFORCE 構造体
	ZeroMemory( &DiRampforce, sizeof( DIRAMPFORCE ) );

	//	ハンドル
	Handle		= NULL;

	//	状態
	Status		= 0;
	//	再生回数
	PlayCount	= 0;
	//	再生時刻
	StartTime	= 0;
}

//----------------------------------------------------------------------------------------------
//	Calc
//----------------------------------------------------------------------------------------------
VOID CEffect::Calc(
	 LONG *	LeftLevel
	,LONG *	RightLevel )
{
	//	エフェクトの再生時間、開始時刻、終了時刻、現在時刻を算出する
	ULONG	Duration	= max( 1, DiEffect.dwDuration / 1000 );
	ULONG	BeginTime	= StartTime + ( DiEffect.dwStartDelay / 1000 );
	ULONG	EndTime		= 0xFFFFFFFF;
	if( PlayCount != -1 )
	{
		EndTime	= BeginTime + Duration * PlayCount;
	}
	ULONG	CurrentTime	= GetTickCount();

	//	エフェクトは再生中？
	if( Status == DIEGES_PLAYING && BeginTime <= CurrentTime && CurrentTime <= EndTime )
	{
		//	エンベロープを計算する
		LONG	NormalRate;
		LONG	AttackLevel;
		LONG	FadeLevel;
		CalcEnvelope(
			 Duration
			,( CurrentTime - BeginTime ) % Duration
			,&NormalRate
			,&AttackLevel
			,&FadeLevel );

		//	フォースを計算する
		LONG	NormalLevel;
		LONG	WorkLeftLevel;
		LONG	WorkRightLevel;
		CalcForce(
			 Duration
			,( CurrentTime - BeginTime ) % Duration
			,NormalRate
			,AttackLevel
			,FadeLevel
			,&NormalLevel );

		//	フォースの正負を調整する
		WorkLeftLevel	= (NormalLevel > 0) ? NormalLevel : -NormalLevel;
		WorkRightLevel	= (NormalLevel > 0) ? NormalLevel : -NormalLevel;

		//	フォースの上限を調整する
		WorkLeftLevel	= min( 65535, WorkLeftLevel * 65535 / 10000 );
		WorkRightLevel	= min( 65535, WorkRightLevel * 65535 / 10000 );

		//	フォースを加算する
		*LeftLevel	= *LeftLevel + WorkLeftLevel;
		*RightLevel	= *RightLevel + WorkRightLevel;
	}
}

//----------------------------------------------------------------------------------------------
//	CalcEnvelope
//----------------------------------------------------------------------------------------------
VOID CEffect::CalcEnvelope(
	 ULONG	Duration
	,ULONG	CurrentPos
	,LONG *	NormalRate
	,LONG *	AttackLevel
	,LONG *	FadeLevel )
{
	//	エンベロープを計算する
	if( ( DiEffect.dwFlags & DIEP_ENVELOPE ) && DiEffect.lpEnvelope != NULL )
	{
		//	アタックの割合を算出する
		LONG	AttackRate	= 0;
		ULONG	AttackTime	= max( 1, DiEnvelope.dwAttackTime / 1000 );
		if( CurrentPos < AttackTime )
		{
			AttackRate	= ( AttackTime - CurrentPos ) * 100 / AttackTime;
		}
		//	フェードの割合を算出する
		LONG	FadeRate	= 0;
		ULONG	FadeTime	= max( 1, DiEnvelope.dwFadeTime / 1000 );
		ULONG	FadePos		= Duration - FadeTime;
		if( FadePos < CurrentPos )
		{
			FadeRate	= ( CurrentPos - FadePos ) * 100 / FadeTime;
		}
		//	算出した値を返す
		*NormalRate		= 100 - AttackRate - FadeRate;
		*AttackLevel	= DiEnvelope.dwAttackLevel * AttackRate;
		*FadeLevel		= DiEnvelope.dwFadeLevel * FadeRate;
	} else {
		*NormalRate		= 100;
		*AttackLevel	= 0;
		*FadeLevel		= 0;
	}
}

//----------------------------------------------------------------------------------------------
//	CalcForce
//----------------------------------------------------------------------------------------------
VOID CEffect::CalcForce(
	 ULONG		Duration
	,ULONG		CurrentPos
	,LONG		NormalRate
	,LONG		AttackLevel
	,LONG		FadeLevel
	,LONG *		NormalLevel )
{
	//	変数宣言
	LONG	Magnitude	= 0;
	LONG	Period;
	LONG	R;
	LONG	Rate;

	//	エフェクトの種類によって処理を振り分ける
	switch( Type )
	{
		//	条件
		case SPRING:
		case DAMPER:
		case INERTIA:
		case FRICTION:
			;
			break;

		//	コンスタント フォース
		case CONSTANT_FORCE:
			//	エンベロープを考慮したマグニチュードを求める
			Magnitude	= DiConstantForce.lMagnitude;
			Magnitude	= ( Magnitude * NormalRate + AttackLevel + FadeLevel ) / 100;
			break;

		//	カスタム フォース
		case CUSTOM_FORCE:
			;
			break;

		//	周期的エフェクト
		case SQUARE:
			//	１周期の時間（ミリ秒）と経過時間から 0 ～ 359 度のどこかを求める
			Period	= max( 1, ( DiPeriodic.dwPeriod / 1000 ) );
			R		= ( CurrentPos % Period ) * 360 / Period;
			//	フェーズを計算する
			R	= ( R + ( DiPeriodic.dwPhase / 100 ) ) % 360;
			//	エンベロープを考慮したマグニチュードを求める
			Magnitude	= DiPeriodic.dwMagnitude;
			Magnitude	= ( Magnitude * NormalRate + AttackLevel + FadeLevel ) / 100;
			//	正方形を考慮したマグニチュードを求める
			if( 180 <= R )
			{
				Magnitude	= Magnitude * -1;
			}
			//	オフセットを計算する
			Magnitude	= Magnitude + DiPeriodic.lOffset;
			break;

		case SINE:
			//	１周期の時間（ミリ秒）と経過時間から 0 ～ 359 度のどこかを求める
			Period	= max( 1, ( DiPeriodic.dwPeriod / 1000 ) );
			R		= ( CurrentPos % Period ) * 360 / Period;
			//	フェーズを計算する
			R		= ( R + ( DiPeriodic.dwPhase / 100 ) ) % 360;
			//	エンベロープを考慮したマグニチュードを求める
			Magnitude	= DiPeriodic.dwMagnitude;
			Magnitude	= ( Magnitude * NormalRate + AttackLevel + FadeLevel ) / 100;
			//	正弦波を考慮したマグニチュードを求める
			Magnitude	= (LONG)( Magnitude * sin( R * 3.1415 / 180.0 ) );
			//	オフセットを計算する
			Magnitude	= Magnitude + DiPeriodic.lOffset;
			break;

		case TRIANGLE:
			//	１周期の時間（ミリ秒）と経過時間から 0 ～ 359 度のどこかを求める
			Period	= max( 1, ( DiPeriodic.dwPeriod / 1000 ) );
			R		= ( CurrentPos % Period ) * 360 / Period;
			//	フェーズを計算する
			R		= ( R + ( DiPeriodic.dwPhase / 100 ) ) % 360;
			//	エンベロープを考慮したマグニチュードを求める
			Magnitude	= DiPeriodic.dwMagnitude;
			Magnitude	= ( Magnitude * NormalRate + AttackLevel + FadeLevel ) / 100;
			//	三角波を考慮したマグニチュードを求める
			if( 0 <= R && R < 90 )
			{
				Magnitude	= -Magnitude * ( 90 - R ) / 90;
			}
			if( 90 <= R && R < 180 )
			{
				Magnitude	= Magnitude * ( R - 90 ) / 90;
			}
			if( 180 <= R && R < 270 )
			{
				Magnitude	= Magnitude * ( 90 - ( R - 180 ) ) / 90;
			}
			if( 270 <= R && R < 360 )
			{
				Magnitude	= -Magnitude * ( R - 270 ) / 90;
			}
			//	オフセットを計算する
			Magnitude	= Magnitude + DiPeriodic.lOffset;
			break;

		case SAWTOOTH_UP:
			//	１周期の時間（ミリ秒）と経過時間から 0 ～ 359 度のどこかを求める
			Period	= max( 1, ( DiPeriodic.dwPeriod / 1000 ) );
			R		= ( CurrentPos % Period ) * 360 / Period;
			//	フェーズを計算する
			R		= ( R + ( DiPeriodic.dwPhase / 100 ) ) % 360;
			//	エンベロープを考慮したマグニチュードを求める
			Magnitude	= DiPeriodic.dwMagnitude;
			Magnitude	= ( Magnitude * NormalRate + AttackLevel + FadeLevel ) / 100;
			//	アップ鋸歯を考慮したマグニチュードを求める
			if( 0 <= R && R < 180 )
			{
				Magnitude	= -Magnitude * ( 180 - R ) / 180;
			}
			if( 180 <= R && R < 360 )
			{
				Magnitude	= Magnitude * ( R - 180 ) / 180;
			}
			//	オフセットを計算する
			Magnitude	= Magnitude + DiPeriodic.lOffset;
			break;

		case SAWTOOTH_DOWN:
			//	１周期の時間（ミリ秒）と経過時間から 0 ～ 359 度のどこかを求める
			Period	= max( 1, ( DiPeriodic.dwPeriod / 1000 ) );
			R		= ( CurrentPos % Period ) * 360 / Period;
			//	フェーズを計算する
			R		= ( R + ( DiPeriodic.dwPhase / 100 ) ) % 360;
			//	エンベロープを考慮したマグニチュードを求める
			Magnitude	= DiPeriodic.dwMagnitude;
			Magnitude	= ( Magnitude * NormalRate + AttackLevel + FadeLevel ) / 100;
			//	ダウン鋸歯を考慮したマグニチュードを求める
			if( 0 <= R && R < 180 )
			{
				Magnitude	= Magnitude * ( 180 - R ) / 180;
			}
			if( 180 <= R && R < 360 )
			{
				Magnitude	= -Magnitude * ( R - 180 ) / 180;
			}
			//	オフセットを計算する
			Magnitude	= Magnitude + DiPeriodic.lOffset;
			break;

		//	傾斜フォース
		case RAMP_FORCE:
			//	始点、終点の割合を算出する
			Rate		= ( Duration - CurrentPos ) * 100
						/ max( 1,  DiEffect.dwDuration / 1000 );
			//	エンベロープを考慮したマグニチュードを求める
			Magnitude	= ( DiRampforce.lStart * Rate
							+ DiRampforce.lEnd * ( 100 - Rate ) ) / 100;
			Magnitude	= ( Magnitude * NormalRate + AttackLevel + FadeLevel ) / 100;
			break;
	}

	//	ゲインを考慮したフォースを返す
	*NormalLevel	= Magnitude * (LONG)DiEffect.dwGain / 10000;
}
