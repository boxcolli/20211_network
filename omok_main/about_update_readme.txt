메인게이트는 해시함수가 필요없네요 생각해보니..

<mysql_ready_m.h>
	MYSQL* mysql_default_start()
		리턴값이 전체적으로 MySQL을 관리하는 구조체라고합니다. 리턴값을 저장해놔야합니다.
	int mysql_default_update_elo(MYSQL *con, const char *id, int elo)
		위에 구조체 레퍼런스를 이용해 해당 id의 유저의 elo(레이팅점수)를 업데이트합니다.
		M_RET_SUCCESS 성공
		M_RET_ERROR 실패
