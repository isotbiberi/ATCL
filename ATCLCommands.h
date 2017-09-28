/*ENCODER*/
#define GetEncoderCountsPerRevX "!EGcx;"
#define GetEncoderCountsPerRevY "!EGcy;"
#define SetEncoderCountsPerRevX "!EScx;"
#define GetEncoderCountsPerRevY "!EGcy;"
#define GetEncodersAbsolute "!Egea;"

#define GetEncDiagPositionX "!EGex;"
#define ZeroEncDiagPositionX "!EZex;"
#define GetEncDiagPositionY "!EGey;"
#define ZeroEncDiagPositionY "!EZey;"

/*ENCODER*/


/*COORDINATES*/
#define GetRa "!CGra;" //Returns Hours(or N/A if not yet aligned)
#define GetDec "!CGde;" //Returns SIGNED_2DIGIT(or N/A if not yet aligned)
#define GetHourAngle "!CGha;"//Returns Hours(or N/A if not yet aligned)
#define GetAz "!CGaz;" //UNSIGNED 3 DIGIT(or N/A...)
#define GetAlt "!CGal;" //SIGNED_2DIGIT
#define GetAxialRealX "!CGax;"
#define GetAxialRealY "!CGay;"
#define GetAxialVelocityX "!CGvx;"
#define GetAxialVelocityY "!CGvy;"

#define  GetTargetAxialX "!CGtx;"
#define  GetTargetAxialY  "!CGty;"
#define GetScopeOrientation "!CGso;"
#define SetCoordFormat "!CScf;"
#define GetCoordFormat "!CGcf;"
#define GetAutomaticMoveStatus "!CGam;"
#define GetCoordinateAggregate1 "!CGa1;"
#define GetCoordinateAggregate2 "!CGa2;"


#define SetTargetDec "!CStd;"
#define GetTargetDec "!CGtd;"
#define SetTargetRA "!CStr;"
#define GetCoordFormat "!CGcf;"
#define SetTargetAlt "!CStl;"
#define GetTargetAlt "!CGtl;"
#define SetTargetAz "!CStz;"
#define GetTargetAz "!CGtz;"
/*COORDINATES*/

/*GOTO*/

#define GoToSelectedObject "!GOsl;"
#define GoToTargetRA_Dec "!GTrd;"
#define GoToTargetRA_DecEpochNow "!GTrn;"
#define GoToTargetAltAz "!GTlz;"
#define GoToLast "!GTol;"
#define GoToPark "!GTop;"
#define SetGoToHorizon "!GSgh;"
#define GetGoToHorizon  "!GGgh;"

#define SetGoToOrientation "!GSgo;"
#define GetGoToOrientation  "!GGgo;"
#define GetGoToRemainPercent "!GGgr;"
#define GetGoToProgressPercent "!GGgp;"
#define GetNumberOfGoTos  "!GGgn;"


/*GOTO*/

/*ALIGNMENT*/
#define GetAlignmentState "!AGas;"
#define AlignmentFromLastPosition "!AFlp;"
/*ALIGNMENT*/


/*TRACKING*/
#define SetTrackRate "!RStr;"
#define GetTrackRate "!RGtr;"
#define ToggleDrift "!RTdr;"
#define SetCustomTRateOffsetRA "!RSor;"
#define GetCustomTRateOffsetRA "!RGor;"
#define SetCustomTRateOffsetDec "!RSod;"
#define GetCustomTRateOffsetDec "!RGod;"
/*TRACKING*/


/*MOUNT SETTINGS*/

#define SetVelMaxX "!NSvx;"
#define GetVelMaxX "!NGvx;"
#define SetVelMaxY "!NSvy;"
#define GetVelMaxY "!NGvy;"
#define SetAccelMaxX "!NSax;"
#define GetAccelMaxX "!NGax;"
#define SetAccelMaxY "!NSay;"
#define GetAccelMaxY "!NGay;"

/* MOUNT SETTINGS */








