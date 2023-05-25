package model

import (
	"fmt"

	"github.com/blastrain/vitess-sqlparser/sqlparser"
)

func GetTableName(sql_text string, query_type SqlType) string {
	stmt, err := sqlparser.Parse(sql_text)
	if err != nil {
		fmt.Printf("%+v\n", err)
	}
	switch query_type {
	case SELECT_TYPE:
		from_expr := stmt.(*sqlparser.Select).From[0].(*sqlparser.AliasedTableExpr).Expr.(sqlparser.TableName).Name.String()
		return from_expr
	case INSERT_TYPE:
		expr := stmt.(*sqlparser.Insert).Table.Name.String()
		return expr
	case UPDATE_TYPE:
		expr := stmt.(*sqlparser.Update).TableExprs[0].(*sqlparser.AliasedTableExpr).Expr.(sqlparser.TableName).Name.String()
		return expr
	case DELETE_TYPE:
		expr := stmt.(*sqlparser.Delete).TableExprs[0].(*sqlparser.AliasedTableExpr).Expr.(sqlparser.TableName).Name.String()
		return expr
	case TRUNCATE_TYPE:
		return stmt.(*sqlparser.TruncateTable).Table.Name.String()
	case DROP_TYPE:
		return stmt.(*sqlparser.DDL).Table.Name.String()
	case SYS_COMMAND_TYPE:
		return ""
	default:
		panic("invalid sqltype")
	}
}
