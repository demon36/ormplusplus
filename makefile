-include template.mk
ifneq (,$(findstring sqlite,$(DRIVERS)))
    $(shell touch src/sqlite_session.cpp)
    CFLAGS += -DSQLITE# -Ithirdparty/sqlite
	LIBS += -lsqlite3
#     DEP_SRC_DIRS := thirdparty/sqlite
else ifneq (,$(findstring mysql,$(DRIVERS)))
    $(shell touch src/mysql_session.cpp)
    CFLAGS += -DMYSQL
	LIBS += -lmysqlclient
endif