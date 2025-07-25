#pragma once

#include "slice.h"
#include "str.h"
#include <sqlite3.h>

// Early defs to prevent error later
typedef struct Table  Table;
typedef struct Column Column;
typedef void*         Cell;
typedef Slice<Cell>   Row;

typedef enum {
    DB_SUCCESS = 0,
    DB_FAIL,

} DbError;

typedef struct Db {
    bool valid;

    Str   path;
    Arena mem;

    sqlite3* db;

    // Default constructor
    Db() = default;

    // Connect
    Db(Str path, isize cap = 16 * 1024 * 1024);

    // Disconnect
    ~Db();

    // Methods
    Slice<Table>  ListTables();
    Slice<Column> ListColumns(Str table);
    Slice<Row>    ListRows(Str table, Slice<Column> columns);

    // No rows returned
    DbError ExecVoid(Str sql);

    // Debug
    void Print();

} Db;

typedef enum CellType {

    CELL_NULL    = 0,
    CELL_INTEGER = 1,
    CELL_REAL    = 2,
    CELL_TEXT    = 3,
    CELL_BLOB    = 4,

} CellType;

constexpr const char* CellTypeStr[] = {
    "NULL",    // = 0
    "INTEGER", // = 1
    "REAL",    // = 2
    "TEXT",    // = 3
    "BLOB",    // = 4
    "NUMERIC", // = 5
};

typedef struct Column {
    i32      idx;
    Str      name;
    CellType type;
    bool     notnull;
    Str      defaultvalue;
    bool     primarykey;

} Column;

typedef struct Table {
    Str  schema;
    Str  name;
    Str  type;
    i32  ncol;
    bool wr;
    bool strict;

} Table;

void PrintTables(Slice<Table> tables);
void PrintColumns(Slice<Column> columns);
void PrintRows(Slice<Row> rows, Slice<Column> columns);
