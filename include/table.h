// 3.1 Basic Definitions
//
//   A table has two parts:
//    - a schema
//    - a rectangular collection of cells.
//
//   A schema is an ordered sequence of column names and corresponding sorts.
//     – The column names must be distinct (no duplicates).
//     – The sorts can vary freely.
//
//   A header is a sequence of distinct column names (a schema without sorts).
//
//   A column name is a string-like first-class datatype.
//
//   A sort describes the kind of data that a cell may contain.
//
//   Common sorts are numbers and strings; uncommon sorts include images, sequences, and other tables.
//
//   The collection of cells has C ∗ R members, where:
//     – C is the length of the schema;
//     – R is an arbitrarily-large number of rows; and
//     – each cell has a unique index (c, r) for 0 ≤ c < C and 0 ≤ r < R.
//
//   The rectangular arrangement has four important consequences:
//     - the rows are ordered
//     - the columns are indexable by schema
//     - all columns contain exactly R cells
//     - all rows contain exactly C cells.
//
//   A row is an ordered sequence of cells.
//
//   A cell is a container for data.
//     – Cells may be empty.
//     – The data in cells of column c must match the sort of the c-th element of the schema.

#pragma once

#include "arena.h"
#include "range.h"
#include "slice.h"
#include "str.h"
#include <cstdio>

template <typename RowType> struct Table {
    Strs           cols;
    Slice<RowType> rows;

    Table(Strs cols_, isize cap_, Arena* a)
    {
        cols = cols_;
        rows = Slice<RowType>(a, cap_);
    };

    void Print()
    {
        BufArena(temp, buf, 1024);

        RANGE(i, rows.len)
        {
            printf("%ld: ", i);
            RANGE(j, cols.len)
            {
                //
                printf("%.*s, ", pstr(rows[i]->Print(j, &temp)));
            }
            printf("\n");
        }
    }
};
