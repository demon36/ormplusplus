#ifndef INCLUDE_DBH_
#define INCLUDE_DBH_

#include <string>
#include <memory>
#include <type_traits>

#include "TableColumn.h"
#include "DBSessionBase.h"

namespace ORMPlusPlus{

template<class UserModel, class TableName>
class Model;
class ModelBase;

class DB{
private:
	//to overcome adding a separate source file
	static std::shared_ptr<DBSessionBase>& getDefaultSessionPtrRef(){
		static std::shared_ptr<DBSessionBase> defaultDBSession;
		return defaultDBSession;
	}

public:
	//make sure template parameter is a child of Model<UserModel>
	template <class UserModel>
	static void assertClassIsUserModel(){
		if(!std::is_base_of<ModelBase, UserModel>::value){
			throw std::runtime_error("template parameter is not a legitimate child of ModelBase");
		}
	}

	static void setDefaultSession(std::shared_ptr<DBSessionBase> DBSession){
		getDefaultSessionPtrRef() = DBSession;
	}

	static DBSessionBase& getDefaultSession(){
		if(getDefaultSessionPtrRef()){
			return *getDefaultSessionPtrRef();
		}else{
			throw std::runtime_error("no default database session is set");
		}
	}

	template <class UserModel>
	static void createOrReplaceTable(){

	}

	// CREATE TABLE 'client_info' ('age' INT, 'height' INT, 'name' VARCHAR(1024));
	template <class UserModel>
	static void createTable(){
		assertClassIsUserModel<UserModel>();
		getDefaultSession().createTable(UserModel::getTableName(), UserModel::getDBSchema());
	}

	template <class UserModel>
	static void dropTable(){
		getDefaultSession().dropTable(UserModel::getTableName());
	}

	template <class UserModel>
	static bool tableExists(bool checkSchema = false){
		assertClassIsUserModel<UserModel>();
		TableSchema schema = UserModel::getDBSchema();
		if(checkSchema){
			return getDefaultSession().tableExists(UserModel::getTableName(), schema);
		}else{
			return getDefaultSession().tableExists(UserModel::getTableName());
		}
	}

};

}



#endif /* INCLUDE_DBH_ */
