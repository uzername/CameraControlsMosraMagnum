#ifndef GLOBALBEACON_GUARD
#define GLOBALBEACON_GUARD

enum SomeName_HowTheCameraShouldMoveAround {
	SMNM_CameraUp,
	SMNM_CameraDown,
	SMNM_CameraLeft,
	SMNM_CameraRight,
	SMNM_CameraForward,
	SMNM_CameraBackward
};


#ifdef _DEBUG 
#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 
#endif 
#endif  // _DEBUG  

#define USEORBITINGCAMERA

#endif