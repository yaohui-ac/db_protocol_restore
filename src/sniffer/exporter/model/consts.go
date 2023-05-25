package model

type SqlType int8

const (
	SELECT_TYPE      SqlType = 1
	INSERT_TYPE      SqlType = 2
	UPDATE_TYPE      SqlType = 3
	DELETE_TYPE      SqlType = 4
	TRUNCATE_TYPE    SqlType = 5
	DDL_TYPE         SqlType = 6
	SYS_COMMAND_TYPE SqlType = 7
)

func (s SqlType) String() string {
	switch s {
	case SELECT_TYPE:
		return "select"
	case INSERT_TYPE:
		return "insert"
	case UPDATE_TYPE:
		return "update"
	case DELETE_TYPE:
		return "delete"
	case TRUNCATE_TYPE:
		return "truncate"
	case DDL_TYPE:
		return "ddl"
	case SYS_COMMAND_TYPE:
		return "system command"
	default:
		panic("invalid sqltype")
	}
}
func (s SqlType) Int8() int8 {
	return (int8)(s)
}
