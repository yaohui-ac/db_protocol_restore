package model

import (
	"fmt"
	"strings"

	"github.com/blastrain/vitess-sqlparser/sqlparser"
)

func GetDBDetailQueryType(sql_text string) SqlType {
	// words := strings.Fields(sql_text)
	// query_type_str := strings.ToLower(words[0])
	//fmt.Println(sql_text)
	sql_text = strings.Trim(sql_text, " ")
	if strings.HasPrefix(sql_text, "show") {
		return SYS_COMMAND_TYPE
	}

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
	case (*sqlparser.DDL), (*sqlparser.CreateTable):
		return DDL_TYPE
	default:
		return SYS_COMMAND_TYPE
	}

}
func GetTableName(sql_text string, query_type SqlType) string {
	// fmt.Println(sql_text)
	sql_text = strings.Trim(sql_text, " ")
	if strings.HasPrefix(sql_text, "show") || strings.HasPrefix(sql_text, "SHOW") {
		return ""
	}
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
		if _, ok := stmt.(*sqlparser.DDL); ok {
			return stmt.(*sqlparser.DDL).Table.Name.String()
		}
		if _, ok := stmt.(*sqlparser.CreateTable); ok {
			return stmt.(*sqlparser.CreateTable).Table.Name.String()
		}
	case SYS_COMMAND_TYPE:
		return ""
	default:
		panic("invalid sql type")
	}
	return ""
}
