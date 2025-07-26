#pragma once

#include "slice.h"
#include "str.h"
#include <sqlite3.h>

// Early defs to prevent error later
typedef struct DbTable  DbTable;
typedef struct DbColumn DbColumn;
typedef void*           DbCell;
typedef Slice<DbCell>   DbRow;

typedef enum {
    DB_SUCCESS = 0,
    DB_FAIL,

} DbError;

// Callbacks
typedef int DbCallback(void* NotUsed, int argc, char** argv, char** azColName);

int PrintCallback(void* NotUsed, int argc, char** argv, char** azColName);

// Main Struct
typedef struct Db {
    bool valid;

    Str   path;
    Arena mem;

    sqlite3* db;

    // Default constructor
    Db() = default;

    // Connect
    Db(Str path, int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, isize cap = 16 * 1024 * 1024);

    // Disconnect
    ~Db();

    // Methods
    Slice<DbTable>  ListTables();
    Slice<DbColumn> ListColumns(Str tablename);
    Slice<DbRow>    ListRows(Str tablename, Slice<DbColumn> columns);

    // Actual interaction

    // No rows returned, only print
    DbError ExecVoid(Str sql, DbCallback callback = PrintCallback);

    // Debug
    void Print();

} Db;

typedef enum DbCellType {

    CELL_NULL    = 0,
    CELL_INTEGER = 1,
    CELL_REAL    = 2,
    CELL_TEXT    = 3,
    CELL_BLOB    = 4,

} DbCellType;

constexpr const char* DbCellTypeStr[] = {
    "NULL",    // = 0
    "INT",     // = 1
    "REAL",    // = 2
    "TEXT",    // = 3
    "BLOB",    // = 4
    "NUMERIC", // = 5
};

typedef struct DbColumn {

    i32        idx          = 0;
    Str        name         = "column";
    DbCellType type         = CELL_TEXT;
    bool       notnull      = false;
    Str        defaultvalue = "none";
    bool       primarykey   = false;

    DbColumn(i32 idx_ = 0, Str name_ = "column", DbCellType type_ = CELL_TEXT, bool notnull_ = false,
             Str defaultvalue_ = "none", bool primarykey_ = false)
    {
        idx          = idx_;
        name         = name_;
        type         = type_;
        notnull      = notnull_;
        defaultvalue = defaultvalue_;
        primarykey   = primarykey_;
    }

} DbColumn;

typedef struct DbTable {
    Str  schema;
    Str  name;
    Str  type;
    i32  ncol;
    bool wr;
    bool strict;

} DbTable;

Str StmtCreateTable(Str tablename, Slice<DbColumn> columns, Arena* a);

void PrintTables(Slice<DbTable> tables);
void PrintColumns(Slice<DbColumn> columns);
void PrintRows(Slice<DbRow> rows, Slice<DbColumn> columns);
