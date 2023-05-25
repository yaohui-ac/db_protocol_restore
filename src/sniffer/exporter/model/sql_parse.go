package model

import (
	"fmt"

	"github.com/blastrain/vitess-sqlparser/sqlparser"
)

func GetDBDetailQueryType(sql_text string) SqlType {
	// words := strings.Fields(sql_text)
	// query_type_str := strings.ToLower(words[0])
	stmt, err := sqlparser.Parse(sql_text)
	if err != nil {
		fmt.Printf("%+v\n", err)
	}
	switch stmt.(type) {
	case (*sqlparser.Select):
		return SELECT_TYPE
	case (*sqlparser.Update):
		return UPDATE_TYPE
	case (*sqlparser.Insert):
		return INSERT_TYPE
	case (*sqlparser.Delete):
		return DELETE_TYPE
	case (*sqlparser.TruncateTable):
		return TRUNCATE_TYPE
	case (*sqlparser.DDL):
		return DDL_TYPE
	case (*sqlparser.Use):
		return SYS_COMMAND_TYPE
	default:
		panic("Invalid Sql Text")
	}

	// switch query_type_str {
	// case "select":
	// 	return SELECT_TYPE
	// case "insert":
	// 	return INSERT_TYPE
	// case "update":
	// 	return UPDATE_TYPE
	// case "delete":
	// 	return DELETE_TYPE
	// case "truncate":
	// 	return TRUNCATE_TYPE
	// case "use":
	// 	return SYS_COMMAND_TYPE
	// default:
	// 	return DDL_TYPE
	// }

}
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
	case DDL_TYPE:
		return stmt.(*sqlparser.DDL).Table.Name.String()
	case SYS_COMMAND_TYPE:
		return ""
	default:
		panic("invalid sql type")
	}
}
