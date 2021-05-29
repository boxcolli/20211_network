#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>

#define M_DEFAULT_HOST      "127.0.0.1" // ip address
#define M_DEFAULT_USER      "omok_main"
#define M_DEFAULT_PASSWD    "1q2w#E$R"
#define M_DEFAULT_DB        "user"
#define M_DEFAULT_PORT      3306 // host should bind this
#define M_DEFAULT_USOCK     NULL
#define M_DEFAULT_CFLAG     0

#define M_BUFSIZ            1024
#define M_DEFAULT_TABLE     "user_priavte"
#define M_DEFAULT_ID_LEN    12 // ?
#define M_DEFAULT_ELO       1000

#define M_RET_ERROR         -1
#define M_RET_SUCCESS       0
#define M_RET_EXISTS        1
#define M_RET_NOTEXISTS     2

void finish_with_error(MYSQL *con)
{
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}



    // host         where is host; name or NULL or "localhost"
    // user         MySQL login ID; if NULL or empty "", current user assumed
    // passwd       password for user
    // db           If NULL, connection sets the default database
    // port         If not NULL, will be used for TCP/IP connection.
    // unix_socket  If not NULL, it should specify the socket or named pipe
    // clientflag   usually 0, but can be set to combination of:
    //      CAN_HANDLE_EXPIRED_PASSWORDS
    //      CLIENT_COMPRESS
    //      CLIENT_FOUND_ROWS....
MYSQL* mysql_default_start()
{
    char buf[M_BUFSIZ];
    MYSQL* con = mysql_init(NULL);
    if (con == NULL)
        finish_with_error(con);

    if (mysql_real_connect(con,
            M_DEFAULT_HOST,
            M_DEFAULT_USER,
            M_DEFAULT_PASSWD,
            M_DEFAULT_DB,
            M_DEFAULT_PORT,
            M_DEFAULT_USOCK,
            M_DEFAULT_CFLAG) == NULL) {
        finish_with_error(con);
    }

/*
    sprintf(buf, "\n\
        CREATE TABLE IF NOT EXISTS %s (\n\
            userid INT NOT NULL AUTO_INCREMENT,\n\
            id VARCHAR(%d) NOT NULL UNIQUE,\n\
            salt VARCHAR(%d) NOT NULL,\n\
            hash VARCHAR(%d) NOT NULL,\n\
            date DATE NOT NULL,\n\
            elo INT NOT NULL,\n\
            PRIMARY KEY (userid),\n\
            )",
        M_DEFAULT_TABLE, M_DEFAULT_ID_LEN, M_DEFAULT_SALT_LEN, M_DEFAULT_HASH_LEN);
    if ((mysql_query(con, buf)) != 0) {
        finish_with_error(con);
    }
*/
    sprintf(buf, "\n\
        USE %s",
        M_DEFAULT_DB);
    if (mysql_query(con, buf) != 0) {
        finish_with_error(con);
    }

    return con;
}


int mysql_default_update_elo(MYSQL *con, const char *id, int elo)
{
    char buf[M_BUFSIZ];

    sprintf(buf, "\n\
        UPDATE %s SET elo = %d WHERE id='%s'",
        M_DEFAULT_TABLE, elo, id);
    if (mysql_query(con, buf) != 0) {
        return M_RET_ERROR;
    }

    return M_RET_SUCCESS;
}