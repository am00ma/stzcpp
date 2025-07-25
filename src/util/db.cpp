#include "util/db.h"
#include "log.h"

static inline bool DbOpen(sqlite3** db, Str path, Arena* mem)
{
    Arena temp = *mem;
    int   err  = sqlite3_open(path.Cstr(&temp), db);
    if (err != SQLITE_OK)
    {
        error("sqlite3_open: %s", sqlite3_errmsg(*db));
        return false;
    }

    // debug("Opened database successfully: %.*s", pstr(path));
    return true;
}

Db::Db(Str path_, isize cap_)
{
    path  = path_;
    mem   = Arena(cap_);
    valid = DbOpen(&db, path, &mem);
};

Db::~Db()
{
    int err = sqlite3_close(db);
    if (err != SQLITE_OK)
    {
        error("sqlite3_close: %d", err);
        valid = false;
        return;
    }
    // debug("Db closed");
}

static int callback(void* NotUsed, int argc, char** argv, char** azColName)
{
    RANGE(i, argc) { printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL"); }
    printf("\n");
    return 0;
}

DbError Db::ExecVoid(Str sql)
{
    Arena temp = mem;
    // debug("Executing:\n%.*s", pstr(sql));

    char* zErrMsg = 0;
    int   rc      = sqlite3_exec(db, sql.Cstr(&temp), callback, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        error("SQL error: %s", zErrMsg);
        sqlite3_free(zErrMsg); // Continues so we can close
        sqlite3_close(db);
        return DB_FAIL;
    }
    // debug("Statement executed successfully.\n");

    return DB_SUCCESS;
}

Slice<Table> Db::ListTables()
{
    Arena temp  = mem;
    Str   query = "PRAGMA table_list";
    // debug("query : %.*s", pstr(query));

    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, query.Cstr(&temp), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        error("sqlite3_prepare_v2: %.*s", pstr(query));
        return Slice<Table>();
    }
    // debug("Successfully prepared statement.");

    // Strings are copied into database's arena
    Slice<Table> tables = Slice<Table>(&mem, 1024);
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        Table table  = {};
        table.schema = Str(&mem, 64, "%s", (const char*)sqlite3_column_text(stmt, 0));
        table.name   = Str(&mem, 1024, "%s", (const char*)sqlite3_column_text(stmt, 1));
        table.type   = Str(&mem, 64, "%s", (const char*)sqlite3_column_text(stmt, 2));
        table.ncol   = sqlite3_column_int(stmt, 3);
        table.wr     = sqlite3_column_int(stmt, 4) != 0;
        table.strict = sqlite3_column_int(stmt, 5) != 0;
        tables.Append(table);
    }
    if (rc != SQLITE_DONE) { error("Error executing statement: %s", sqlite3_errmsg(db)); }
    // debug("Finished collecting tables: %ld", tables.len);

    rc = sqlite3_finalize(stmt);
    if (rc != SQLITE_OK)
    {
        error("sqlite3_prepare_v2: %.*s", pstr(query));
        return Slice<Table>();
    }
    // debug("Successfully finalized.\n");

    return tables;
}

Slice<Column> Db::ListColumns(Str table)
{
    Arena temp  = mem;
    Str   query = Str(&temp, 1024, "PRAGMA table_info(%.*s);", pstr(table));
    // debug("query : %.*s", pstr(query));

    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, query.Cstr(&temp), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        error("sqlite3_prepare_v2: %.*s", pstr(query));
        return Slice<Column>();
    }
    // debug("Successfully prepared statement.");

    // Strings are copied into database's arena
    Slice<Column> columns = Slice<Column>(&mem, 1024);
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        Column col = {};
        col.idx    = sqlite3_column_int(stmt, 0);
        col.name   = Str(&mem, 1024, "%s", (const char*)sqlite3_column_text(stmt, 1));

        Str type = Str(&mem, 1024, "%s", (const char*)sqlite3_column_text(stmt, 2));
        if (type[0, 3] == "INT") col.type = CELL_INTEGER;
        else if (type == "TEXT") col.type = CELL_TEXT;
        else if (type[0, 4] == "CHAR") col.type = CELL_TEXT;
        else if (type[0, 7] == "VARCHAR") col.type = CELL_TEXT;
        else if (type == "REAL") col.type = CELL_REAL;
        else if (type == "BLOB") col.type = CELL_BLOB;
        else assert(false);

        col.notnull      = sqlite3_column_int(stmt, 3) != 0;
        col.defaultvalue = Str(&mem, 1024, "%s", (const char*)sqlite3_column_text(stmt, 4));
        col.primarykey   = sqlite3_column_int(stmt, 5) != 0;
        columns.Append(col);
    }
    if (rc != SQLITE_DONE) { error("Error executing statement: %s", sqlite3_errmsg(db)); }
    // debug("Finished collecting columns: %ld", columns.len);

    rc = sqlite3_finalize(stmt);
    if (rc != SQLITE_OK)
    {
        error("sqlite3_prepare_v2: %.*s", pstr(query));
        return Slice<Column>();
    }
    // debug("Successfully finalized.\n");

    return columns;
}

Slice<Row> Db::ListRows(Str table, Slice<Column> columns)
{
    Arena temp  = mem;
    Str   query = Str(&temp, 1024, "SELECT * FROM \"%.*s\";", pstr(table));
    // debug("query : %.*s", pstr(query));

    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, query.Cstr(&temp), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        error("sqlite3_prepare_v2: %.*s", pstr(query));
        return Slice<Row>();
    }
    // debug("Successfully prepared statement.");

    // Strings are copied into database's arena (1MB allotted)
    Slice<Row> rows  = Slice<Row>(&mem, 1024 * 1024);
    isize      count = 0;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        Row row = {};
        rows.Append(row);
        count++;
    }
    if (rc != SQLITE_DONE) { error("Error executing statement: %s", sqlite3_errmsg(db)); }
    // debug("Finished collecting columns: %ld", columns.len);

    rc = sqlite3_finalize(stmt);
    if (rc != SQLITE_OK)
    {
        error("sqlite3_prepare_v2: %.*s", pstr(query));
        return Slice<Row>();
    }
    // debug("Successfully finalized.\n");

    return rows;
}

void Db::Print() { printf("Path: %.*s (%s)\n", pstr(path), valid ? "valid" : "invalid"); }

void PrintTables(Slice<Table> tables)
{
    printf(COLOR_BLUE_BOLD);
    printf(" %10s |", "schema");
    printf(" %30s |", "name");
    printf(" %10s |", "type");
    printf(" %5s |", "ncol");
    printf(" %6s |", "wr");
    printf(" %6s |", "strict");
    printf(COLOR_RESET);
    printf("\n");

    RANGE(i, tables.len)
    {
        printf(" %10.*s |", pstr(tables.data[i].schema));
        printf(" %30.*s |", pstr(tables.data[i].name));
        printf(" %10.*s |", pstr(tables.data[i].type));
        printf(" %5d |", tables.data[i].ncol);
        printf(" %6s |", tables.data[i].wr ? "true" : "false");
        printf(" %6s |", tables.data[i].strict ? "true" : "false");
        printf("\n");
    }
}

void PrintColumns(Slice<Column> columns)
{
    printf(COLOR_BLUE_BOLD);
    printf(" %5s |", "idx");
    printf(" %30s |", "name");
    printf(" %10s |", "type");
    printf(" %10s |", "notnull");
    printf(" %10s |", "primarykey");
    printf(" %-12s |", "defaultvalue");
    printf(COLOR_RESET);
    printf("\n");

    RANGE(i, columns.len)
    {
        printf(" %5d |", columns.data[i].idx);
        printf(" %30.*s |", pstr(columns.data[i].name));
        printf(" %10s |", CellTypeStr[columns.data[i].type]);
        printf(" %10s |", columns.data[i].notnull ? "true" : "false");
        printf(" %10s |", columns.data[i].primarykey ? "true" : "false");
        printf(" %-12.*s |", pstr(columns.data[i].defaultvalue));
        printf("\n");
    }
}
