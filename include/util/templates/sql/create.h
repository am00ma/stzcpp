/* Create SQL statement */
constexpr char SQL_CREATE_TABLE[] = "CREATE TABLE COMPANY(             \n"
                                    "  ID INT PRIMARY KEY     NOT NULL,\n"
                                    "  NAME           TEXT    NOT NULL,\n"
                                    "  AGE            INT     NOT NULL,\n"
                                    "  ADDRESS        CHAR(50),        \n"
                                    "  SALARY         REAL,            \n"
                                    "  IMAGE          BLOB             \n"
                                    ");                                \n";
